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

	delete [] this->buffer;
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

void Application::reallocateBuffer(int size) {
	delete [] this->buffer;
	this->bufferSize = size;
	this->buffer = new char[size];
}

void Application::init(int * argc, char *** argv) {
	this->bufferSize = 1;
	this->buffer = new char[1];
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
	this->reallocateBuffer(4 * sizeof(int));

	MPI_Recv(buffer, bufferSize, MPI_PACKED, 0, Tags::INPUTS, MPI_COMM_WORLD, &status);
	MPI_Unpack(buffer, bufferSize, &position, &this->matrixWidth, 1, MPI_INT, MPI_COMM_WORLD);
	MPI_Unpack(buffer, bufferSize, &position, &this->matrixHeight, 1, MPI_INT, MPI_COMM_WORLD);
	MPI_Unpack(buffer, bufferSize, &position, &this->maxTokens, 1, MPI_INT, MPI_COMM_WORLD);
	MPI_Unpack(buffer, bufferSize, &position, &this->pricePerToken, 1, MPI_INT, MPI_COMM_WORLD);

	this->createMatrix();

	position = 0;
	this->reallocateBuffer(this->matrixWidth * this->matrixHeight * sizeof(int));

	MPI_Recv(buffer, bufferSize, MPI_PACKED, 0, Tags::MATRIX_VALUES, MPI_COMM_WORLD, &status);
	MPI_Unpack(buffer, bufferSize, &position, this->matrix->getFields(), this->matrixWidth * this->matrixHeight, MPI_INT, MPI_COMM_WORLD);
}

void Application::sendInputs() {
	for (int i = 1; i < this->processNumber; i++) {
		int position = 0;
		this->reallocateBuffer(4 * sizeof(int));

		MPI_Pack(&this->matrixWidth, 1, MPI_INT, buffer, bufferSize, &position, MPI_COMM_WORLD);
		MPI_Pack(&this->matrixHeight, 1, MPI_INT, buffer, bufferSize, &position, MPI_COMM_WORLD);
		MPI_Pack(&this->maxTokens, 1, MPI_INT, buffer, bufferSize, &position, MPI_COMM_WORLD);
		MPI_Pack(&this->pricePerToken, 1, MPI_INT, buffer, bufferSize, &position, MPI_COMM_WORLD);
		MPI_Send(buffer, position, MPI_PACKED, i, Tags::INPUTS, MPI_COMM_WORLD);

		position = 0;
		this->reallocateBuffer(this->matrixWidth * this->matrixHeight * sizeof(int));

		MPI_Pack(this->matrix->getFields(), this->matrixWidth * this->matrixHeight, MPI_INT, buffer, bufferSize, &position, MPI_COMM_WORLD);
		MPI_Isend(buffer, position, MPI_PACKED, i, Tags::MATRIX_VALUES, MPI_COMM_WORLD, &request);
	}
}

void Application::run() {

//	if (this->rank == 0) {
		this->readInputs();
		this->generateMatrix();
//		this->sendInputs();
//		MatrixRenderer(matrix).render();
//	} else {
//		this->receiveInputs();
//		MatrixRenderer(matrix).render();
//	}

	ConfigurationFactory factory = ConfigurationFactory(matrix->getWidth(), matrix->getHeight());

	TokenPlacer tp = TokenPlacer(*matrix, factory, maxTokens, pricePerToken);

	Configuration bestConfiguration = tp.findBestConfiguration(
		factory.createFirstConfiguration(1),
		factory.createLastConfiguration(maxTokens)
	);

	MatrixRenderer(matrix).render(&bestConfiguration);
}