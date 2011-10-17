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

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) { 
	int matrixWidth;
	int matrixHeight;
	int maxTokens;
	int pricePerToken;
	
	/**
		Inputs.
	 */
	
	cout << "Enter height of matrix:" << endl;
	cin >> matrixHeight;
	
	if (matrixHeight < 3) {
		cerr << "Error: Height of matrix must not be less than 3." << endl;
		return -1;
	}
		
	cout << "Enter width of matrix:" << endl;
	cin >> matrixWidth;
	
	if (matrixWidth < 3) {
		cerr << "Error: Width of matrix must not be less than 3." << endl;
		return -2;
	}
	
	cout << "Enter number of maximal tokens:" << endl;
	cin >> maxTokens;
	
	if (maxTokens < 1 || maxTokens > ((matrixHeight * matrixWidth ) / 2)) {
		cerr << "Error: Maximum of tokens must be between 1 and " << (matrixHeight * matrixWidth ) / 2 << "." << endl;
		return -3;
	}
	
	cout << "Enter price for token:" << endl;
	cin >> pricePerToken;
	
	if (pricePerToken < 1 || pricePerToken > 100) {
		cerr << "Error: Price for token must be between 1 and 100." << endl;
		return -4;
	}
	
	Matrix matrix = Matrix(matrixWidth, matrixHeight);
	MatrixRandomGenerator(&matrix).fillRandom(1, 100);


	TokenPlacer tp = TokenPlacer(matrix, maxTokens, pricePerToken);
	Configuration bestConfiguration = tp.findBestConfiguration();

	// Debug output
	MatrixRenderer(&matrix).render(& bestConfiguration);
	
	return 0;
}


