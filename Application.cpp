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
	if (this->placer != NULL) {
		delete this->placer;
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
	this->request = NULL;
	this->placer = NULL;

	this->color = this->WHITE;
	this->finished = false;
	this->end = false;

	MPI_Init(argc, argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &this->rank);
	MPI_Comm_size(MPI_COMM_WORLD, &this->processNumber);

	if (this->rank == 0) {
		this->token = this->WHITE;
	} else {
		this->token = this->NOTOKEN;
	}
	this->rankCounter = -1;
}

int Application::getNextRank() {
	this->rankCounter = ++this->rankCounter % this->processNumber;
	if (this->rankCounter == this->rank) {
		return this->getNextRank();
	}

	return this->rankCounter;
}

bool Application::hasToken() {
	return this->token != this->NOTOKEN;
}

void Application::sendToken() {
	if (this->rank == 0) {
		this->token = this->WHITE;
		this->color = this->WHITE;
	} else if (this->color == this->BLACK) {
		this->token = this->BLACK;
		this->color = this->WHITE;
	}

	this->waitForSend();

	int desitination = (this->rank + 1) % this->processNumber;

	MPI_Isend(&this->token, 1, MPI_SHORT, desitination, Tags::TOKEN, MPI_COMM_WORLD, &request);
	COM_PRINTLN(this->rank << " " << (this->token == this->WHITE ? "white" : "black") << " Send token to " << desitination);
	this->token = this->NOTOKEN;
}

void Application::receiveToken() {
	MPI_Recv(&this->token, 1, MPI_SHORT, MPI_ANY_SOURCE, Tags::TOKEN, MPI_COMM_WORLD, &status);
	COM_PRINTLN(this->rank << " " << (this->token == this->WHITE ? "white" : "black") << " Received token");
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
	
	COM_PRINTLN(this->rank << " Received inputs");
}

void Application::receiveMatrix() {
	this->createMatrix();

	int position = 0;
	this->reallocateReceiveBuffer(this->matrixWidth * this->matrixHeight * sizeof(int));

	MPI_Recv(receiveBuffer, receiveBufferSize, MPI_PACKED, 0, Tags::MATRIX_VALUES, MPI_COMM_WORLD, &status);
	MPI_Unpack(receiveBuffer, receiveBufferSize, &position, this->matrix->getFieldsPointer(), this->matrixWidth * this->matrixHeight, MPI_INT, MPI_COMM_WORLD);

	COM_PRINTLN(this->rank << " Received matrix");
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

TokenPlacer * Application::getPlacer() {
	if (this->placer == NULL) {
		this->placer = new TokenPlacer(matrix, *this->getFactory(), maxTokens, pricePerToken);
	}
	return this->placer;
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

	for (int i = 1; i < served; i++) {
		this->sendJob(*intervals[i], i);
		delete intervals[i];
	}

	for (int i = served; i < this->processNumber; i++) {
		this->sendNoJob(i);
	}
	delete [] intervals;
}

bool Application::isSent() {
	int flag = 0;
	MPI_Test(&request, &flag, &status);
	return flag == 1;
}

void Application::waitForSend() {
	while (this->request != NULL && !isSent()) {
		usleep(100); // really?
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

void Application::receiveJob() {
	int rank;

	MPI_Recv(&rank, 1, MPI_INT, MPI_ANY_SOURCE, Tags::JOB_SENT, MPI_COMM_WORLD, &status);

	this->setInterval(new ConfigurationInterval(
		this->receiveConfiguration(),
		this->receiveConfiguration()
	));
	COM_PRINTLN(this->rank << " Received interval from " << rank << " : " << *this->getInterval());
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

void Application::sendJob(ConfigurationInterval interval, int rank) {
	this->waitForSend();

	COM_PRINTLN(this->rank << " Sending job to " << rank);

	MPI_Isend(&this->rank, 1, MPI_INT, rank, Tags::JOB_SENT, MPI_COMM_WORLD, &request);

	this->sendConfiguration(interval.getStart(), rank);
	this->sendConfiguration(interval.getEnd(), rank);

	if (this->rank < rank) {
		this->waitForSend();

		this->color = BLACK;
	}
}

void Application::requestForJob() {
	this->waitForSend();
	int requestedRank = this->getNextRank();
	COM_PRINTLN(this->rank << " Requesting job from process " << requestedRank);

	MPI_Isend(&this->rank, 1, MPI_INT, requestedRank, Tags::JOB_REQUEST, MPI_COMM_WORLD, &request);
}

void Application::receiveNoJob() {
	int rank;

	MPI_Recv(&rank, 1, MPI_INT, MPI_ANY_SOURCE, Tags::JOB_NOJOB, MPI_COMM_WORLD, &status);
}

int Application::receiveJobRequest() {
	int rank;

	MPI_Recv(&rank, 1, MPI_INT, MPI_ANY_SOURCE, Tags::JOB_REQUEST, MPI_COMM_WORLD, &status);

	COM_PRINTLN(this->rank << " Received job request from " << rank);

	return rank;
}

void Application::sendNoJob(const int & rank) {
	this->waitForSend();
	
	MPI_Isend(&this->rank, 1, MPI_INT, rank, Tags::JOB_NOJOB, MPI_COMM_WORLD, &request);
}

void Application::checkMessages() {
	int flag;

	COM_PRINTLN(this->rank << " Checking messages");

	int receivedRank;

	MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
	if (flag) {
		switch (status.MPI_TAG) {
			case Tags::JOB_REQUEST:
				receivedRank = this->receiveJobRequest();
				if (this->hasJob() && this->getInterval()->isSplitable(*this->getFactory())) {
					this->sendJob(this->getInterval()->split(*this->getFactory()), receivedRank);
				} else {
					this->sendNoJob(receivedRank);
				}
				break;
			case Tags::JOB_SENT:
				if (!this->hasJob()) {
					this->receiveJob();
				} else {
					// todo refuse
				}
				break;
			case Tags::JOB_NOJOB:
				this->receiveNoJob();
				break;
			case Tags::TOKEN:
				this->receiveToken();
				if (this->rank == 0 && this->token == this->WHITE) {
					this->sendFinish();
					this->finish();
				}
				break;
			case Tags::FINISH:
				this->receiveFinish();
				this->finish();
				break;
			case Tags::WHICH_PRICE:
				if (this->receivePrice() == this->getPlacer()->getBestPrice()) {
					this->sendConfiguration(this->getPlacer()->getBestConfiguration(), 0);
				}
				break;
			case Tags::END:
				this->end = true;
				break;
		}
	}
}

double Application::receivePrice() {
	double price;

	MPI_Recv(&price, 1, MPI_DOUBLE, 0, Tags::WHICH_PRICE, MPI_COMM_WORLD, &status);

	return price;
}

void Application::finish() {

	COM_PRINTLN(this->rank << " is finishing");

	this->finished = true;

	double localBestPrice = this->getPlacer()->getBestPrice();
	double bestPrice;

	MPI_Reduce(&localBestPrice, &bestPrice, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	if (this->rank == 0) {
		this->sendWhichPrice(bestPrice);

		Configuration bestConfiguration = this->receiveConfiguration();

		this->sendEnd();

		MatrixRenderer(matrix).render(&bestConfiguration);
	} else {
		while (!this->end) {
			this->checkMessages();
		}
	}
}

void Application::sendFinish() {
	this->waitForSend();

	for (int i = 1; i < this->processNumber; i++) {
		MPI_Isend(&this->rank, 1, MPI_INT, i, Tags::FINISH, MPI_COMM_WORLD, &request);
	}
}

void Application::sendEnd() {
	this->waitForSend();

	for (int i = 1; i < this->processNumber; i++) {
		MPI_Isend(&this->rank, 1, MPI_INT, i, Tags::END, MPI_COMM_WORLD, &request);
	}
}

void Application::sendWhichPrice(double price) {
	this->waitForSend();

	for (int i = 1; i < this->processNumber; i++) {
		MPI_Isend(&price, 1, MPI_DOUBLE, i, Tags::WHICH_PRICE, MPI_COMM_WORLD, &request);
	}
}

void Application::receiveFinish() {
	int rank;

	MPI_Recv(&rank, 1, MPI_INT, MPI_ANY_SOURCE, Tags::FINISH, MPI_COMM_WORLD, &status);

	COM_PRINTLN(this->rank << " finish received from " << rank);
}

bool Application::hasJob() const {
	return this->getInterval() != NULL;
}

bool Application::isFinished() {
	return this->finished;
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
//		this->receiveInterval();
	}

	while (!this->isFinished()) {
		// periodically check messages
		this->checkMessages();

		// request job if not having
		if (!this->hasJob()) {
			this->requestForJob();
			usleep(100); // really?
		} else {
			// do job

			COM_PRINTLN(this->rank << " Do job");

			this->getPlacer()->findBestConfiguration(this->interval);
			delete this->interval;
			this->interval = NULL;
		}

		COM_PRINTLN(this->rank << " Is idle");

		// idle
		if (this->hasToken()) {
			this->sendToken();
		}
	}
	
	COM_PRINTLN("Process " << this->rank << " done.");
}