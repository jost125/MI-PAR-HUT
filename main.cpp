/* 
 * File:   main.cpp
 * Author: honza
 *
 * Created on September 20, 2011, 7:54 PM
 */

#include <cstdlib>
#include <iostream>
#include "Matrix.h"
#include "MatrixRenderer.h"
#include "MatrixRandomGenerator.h"
#include "TokenPlacer.h"
#include "InputReader.h"
#include "mpi.h"
#include "ConfigurationFactory.h"
#include "Debug.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

	InputReader input = InputReader();

	int matrixWidth, matrixHeight, maxTokens, pricePerToken, maxAllowedTokens;

	// Inputs.
	// Width.
	if ((matrixWidth = input.readWidth()) < 3) {
		cerr << "Error: Width of matrix must not be less than 3.";
		return -1;
	}

	// Height.
	if ((matrixHeight = input.readHeight()) < 3) {
		cerr << "Error: Height of matrix must not be less than 3.";
		return -1;
	}

	// Max tokens
	maxTokens = input.readMaxTokens();
	maxAllowedTokens = (matrixHeight * matrixWidth) / 2;
	if (maxTokens < 1 || maxTokens > maxAllowedTokens) {
		cerr << "Error: Maximum of tokens must be between 1 and " << maxAllowedTokens << ".";
		return -1;
	}

	// Price per token
	pricePerToken = input.readPricePerToken();
	if (pricePerToken < 1 || pricePerToken > 100) {
		std::cerr << "Error: Price for token must be between 1 and 100." << std::endl;
		return -1;
	}
	
	Matrix matrix = Matrix(matrixWidth, matrixHeight);
	MatrixRandomGenerator(&matrix).fillRandom(1, 100);

	ConfigurationFactory factory = ConfigurationFactory(matrix.getWidth(), matrix.getHeight());

	TokenPlacer tp = TokenPlacer(matrix, factory, maxTokens, pricePerToken);
	
	Configuration bestConfiguration = tp.findBestConfiguration(
		factory.createFirstConfiguration(1),
		factory.createLastConfiguration(maxTokens)
	);

	// Debug output
	MatrixRenderer(&matrix).render(& bestConfiguration);
	
	return 0;
}
