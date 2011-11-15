/* 
 * File:   App.cpp
 * Author: honza
 * 
 * Created on November 14, 2011, 11:27 PM
 */

#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include "MatrixRenderer.h"
#include "MatrixRandomGenerator.h"
#include "TokenPlacer.h"
#include "InputReader.h"
#include "mpi.h"
#include "ConfigurationFactory.h"
#include "Debug.h"
#include "ConfigurationInterval.h"
#include "Application.h"
#include "Tags.h"

using namespace std;

Application::Application(int * argc, char *** argv) {
	this->init(argc, argv);
}

Application::Application(const Application& orig) {
}

Application::~Application() {
	MPI_Finalize();

	if (this->sendBuffer != NULL) {
		delete [] this->sendBuffer;
	}
	if (this->receiveBuffer != NULL) {
		delete [] this->receiveBuffer;
	}
	if (this->matrix != NULL) {
		delete this->matrix;
	}
	if (this->interval != NULL) {
		delete this->interval;
	}
	if (this->factory != NULL) {
		delete this->factory;
	}
}

void Application::reallocateReceiveBuffer(int size) {
	delete [] this->receiveBuffer;
	this->receiveBufferSize = size;
	this->receiveBuffer = new char[size];
}

void Application::reallocateSendBuffer(int size) {
	delete [] this->sendBuffer;
	this->sendBufferSize = size;
	this->sendBuffer = new char[size];
}

void Application::init(int * argc, char *** argv) {
	this->sendBuffer = NULL;
	this->receiveBuffer = NULL;
	this->matrix = NULL;
	this->interval = NULL;

	MPI_Init(argc, argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &this->rank);
	MPI_Comm_size(MPI_COMM_WORLD, &this->processNumber);
}

void Application::readInputs() {
	InputReader input = InputReader();

	// Inputs.
	// Width.
	if ((matrixWidth = input.readWidth()) < 3) {
		cerr << "Error: Width of matrix must not be less than 3." << endl;
		exit(-1);
	}

	// Height.
	if ((matrixHeight = input.readHeight()) < 3) {
		cerr << "Error: Height of matrix must not be less than 3." << endl;
		exit(-1);
	}

	// Max tokens
	maxTokens = input.readMaxTokens();
	int maxAllowedTokens = (matrixHeight * matrixWidth) / 2;
	if (maxTokens < 1 || maxTokens > maxAllowedTokens) {
		cerr << "Error: Maximum of tokens must be between 1 and " << maxAllowedTokens << "." << endl;
		exit(-1);
	}

	// Price per token
	pricePerToken = input.readPricePerToken();
	if (pricePerToken < 1 || pricePerToken > 100) {
		cerr << "Error: Price for token must be between 1 and 100." << endl;
		exit(-1);
	}
}

void Application::createMatrix() {
	matrix = new Matrix(matrixWidth, matrixHeight);
}

void Application::generateMatrix() {
	matrix = new Matrix(matrixWidth, matrixHeight);
	MatrixRandomGenerator(matrix).fillRandom(1, 100);
}

void Application::receiveInputs() {

	int position = 0;
	this->reallocateReceiveBuffer(4 * sizeof(int));

	MPI_Recv(receiveBuffer, receiveBufferSize, MPI_PACKED, 0, Tags::INPUTS, MPI_COMM_WORLD, &status);
	MPI_Unpack(receiveBuffer, receiveBufferSize, &position, &this->matrixWidth, 1, MPI_INT, MPI_COMM_WORLD);
	MPI_Unpack(receiveBuffer, receiveBufferSize, &position, &this->matrixHeight, 1, MPI_INT, MPI_COMM_WORLD);
	MPI_Unpack(receiveBuffer, receiveBufferSize, &position, &this->maxTokens, 1, MPI_INT, MPI_COMM_WORLD);
	MPI_Unpack(receiveBuffer, receiveBufferSize, &position, &this->pricePerToken, 1, MPI_INT, MPI_COMM_WORLD);
}

void Application::receiveMatrix() {
	this->createMatrix();

	int position = 0;
	this->reallocateReceiveBuffer(this->matrixWidth * this->matrixHeight * sizeof(int));

	MPI_Recv(receiveBuffer, receiveBufferSize, MPI_PACKED, 0, Tags::MATRIX_VALUES, MPI_COMM_WORLD, &status);
	MPI_Unpack(receiveBuffer, receiveBufferSize, &position, this->matrix->getFieldsPointer(), this->matrixWidth * this->matrixHeight, MPI_INT, MPI_COMM_WORLD);
}

void Application::sendMatrix() {
	this->waitForSend();
	
	int position = 0;
	this->reallocateSendBuffer(this->matrixWidth * this->matrixHeight * sizeof(int));

	MPI_Pack(this->matrix->getFieldsPointer(), this->matrixWidth * this->matrixHeight, MPI_INT, sendBuffer, sendBufferSize, &position, MPI_COMM_WORLD);
	for (int i = 1; i < this->processNumber; i++) {
		MPI_Isend(sendBuffer, position, MPI_PACKED, i, Tags::MATRIX_VALUES, MPI_COMM_WORLD, &request);
	}
}

void Application::sendInputs() {
	int position = 0;
	this->reallocateSendBuffer(4 * sizeof(int));

	MPI_Pack(&this->matrixWidth, 1, MPI_INT, sendBuffer, sendBufferSize, &position, MPI_COMM_WORLD);
	MPI_Pack(&this->matrixHeight, 1, MPI_INT, sendBuffer, sendBufferSize, &position, MPI_COMM_WORLD);
	MPI_Pack(&this->maxTokens, 1, MPI_INT, sendBuffer, sendBufferSize, &position, MPI_COMM_WORLD);
	MPI_Pack(&this->pricePerToken, 1, MPI_INT, sendBuffer, sendBufferSize, &position, MPI_COMM_WORLD);
	for (int i = 1; i < this->processNumber; i++) {
		MPI_Isend(sendBuffer, position, MPI_PACKED, i, Tags::INPUTS, MPI_COMM_WORLD, &request);
	}
}

ConfigurationFactory * Application::getFactory() {
	if (this->factory == NULL) {
		this->factory = new ConfigurationFactory(this->matrixWidth, this->matrixHeight);
	}
	return this->factory;
}

void Application::setInterval(ConfigurationInterval * interval) {
	if (this->interval != NULL) {
		delete interval;
	}
	this->interval = interval;
}

ConfigurationInterval * Application::getInterval() const {
	return this->interval;
}

void Application::sendInitIntervals() {
	this->setInterval(new ConfigurationInterval(
		this->getFactory()->createFirstConfiguration(1),
		this->getFactory()->createLastConfiguration(this->maxTokens)
	));

	ConfigurationInterval ** intervals = new ConfigurationInterval * [this->processNumber];
	intervals[0] = new ConfigurationInterval(*this->getInterval());

	int from = 0;
	int jump = 2;
	int served;

	for (served = 1; served < this->processNumber; served++) {
		if (intervals[from]->isSplitable(*this->getFactory())) {
			intervals[served] = new ConfigurationInterval(intervals[from]->split(*this->getFactory()));
			from++;
			if (jump == served) {
				jump *= 2;
				from = 0;
			}
		} else {
			break;
		}
	}

	for (int i = 0; i < served; i++) {
		this->sendInterval(*intervals[i], i);
		delete intervals[i];
	}
	delete [] intervals;
}

bool Application::isSent() {
	int flag = 0;
	MPI_Test(&request, &flag, &status);
	return flag == 1;
}

void Application::waitForSend() {
	while (!isSent()) {
		usleep(100);
	}
}

Configuration Application::receiveConfiguration() {
	int bufferSize = 0;

	MPI_Recv(&bufferSize, 1, MPI_INT, MPI_ANY_SOURCE, Tags::BUFFER_SIZE, MPI_COMM_WORLD, &status);

	int position = 0;
	this->reallocateReceiveBuffer(bufferSize);

	int size = (bufferSize) / sizeof(int);


	int * data = new int[size];

	MPI_Recv(receiveBuffer, receiveBufferSize, MPI_INT, MPI_ANY_SOURCE, Tags::CONFIGURATION, MPI_COMM_WORLD, &status);
	MPI_Unpack(receiveBuffer, receiveBufferSize, &position, data, size, MPI_INT, MPI_COMM_WORLD);

	std::vector<Coordinate> coordinates = std::vector<Coordinate>();
	for (int i = 0; i < size / 2; i++) {
		coordinates.push_back(Coordinate(data[i * 2], data[i * 2 + 1]));
	}

	delete [] data;

	return Configuration(coordinates);
}

void Application::receiveInterval() {
	this->setInterval(new ConfigurationInterval(
		this->receiveConfiguration(),
		this->receiveConfiguration()
	));
	TRACELN("Received interval" << *this->getInterval());
}

void Application::sendConfiguration(const Configuration & configuration, int rank) {
	this->waitForSend();

	std::vector<Coordinate> coordinates = configuration.getCoordinates();

	int position = 0;
	this->reallocateSendBuffer(coordinates.size() * 2 * sizeof(int));

	MPI_Isend(&sendBufferSize, 1, MPI_INT, rank, Tags::BUFFER_SIZE, MPI_COMM_WORLD, &request);

	for (int i = 0; i < coordinates.size(); i++) {
		int x = coordinates[i].getX();
		int y = coordinates[i].getY();
		MPI_Pack(&x, 1, MPI_INT, sendBuffer, sendBufferSize, &position, MPI_COMM_WORLD);
		MPI_Pack(&y, 1, MPI_INT, sendBuffer, sendBufferSize, &position, MPI_COMM_WORLD);
	}

	this->waitForSend();

	MPI_Isend(sendBuffer, position, MPI_PACKED, rank, Tags::CONFIGURATION, MPI_COMM_WORLD, &request);
}

void Application::sendInterval(ConfigurationInterval interval, int rank) {
	TRACELN("Sending interval" << interval);
	this->sendConfiguration(interval.getStart(), rank);
	this->sendConfiguration(interval.getEnd(), rank);
}

void Application::run() {

	if (this->rank == 0) {
		this->readInputs();
		this->generateMatrix();
		this->sendInputs();
		this->sendMatrix();
		this->sendInitIntervals();
	} else {
		this->receiveInputs();
		this->receiveMatrix();
		this->receiveInterval();
	}

	TRACELN("Process " << this->rank << " done.");
	
//	TokenPlacer tp = TokenPlacer(matrix, factory, maxTokens, pricePerToken);

//	Configuration bestConfiguration = tp.findBestConfiguration(
//		factory.createFirstConfiguration(1),
//		factory.createLastConfiguration(maxTokens)
//	);
//
//	MatrixRenderer(matrix).render(&bestConfiguration);
}