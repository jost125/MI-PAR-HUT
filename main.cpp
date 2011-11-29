/* 
 * File:   main.cpp
 * Author: honza
 *
 * Created on September 20, 2011, 7:54 PM
 */

#include "mpi.h"
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
	
	if (argc != 5) {
		cerr << "Usage: " << (argv)[0] << " <width> <height> <maxTokens> <pricePerToken>" << endl;
		exit(-1);
	}

	// Inputs.
	// Width.
	if ((matrixWidth = atoi((argv)[1])) < 3) {
		cerr << "Error: Width of matrix must not be less than 3." << endl;
		exit(-1);
	}

	// Height.
	if ((matrixHeight = atoi((argv)[2])) < 3) {
		cerr << "Error: Height of matrix must not be less than 3." << endl;
		exit(-1);
	}

	// Max tokens
	maxTokens = atoi((argv)[3]);
	int maxAllowedTokens = (matrixHeight * matrixWidth) / 2;
	if (maxTokens < 1 || maxTokens > maxAllowedTokens) {
		cerr << "Error: Maximum of tokens must be between 1 and " << maxAllowedTokens << "." << endl;
		exit(-1);
	}

	// Price per token
	pricePerToken = atoi((argv)[4]);
	if (pricePerToken < 1 || pricePerToken > 100) {
		cerr << "Error: Price for token must be between 1 and 100." << endl;
		exit(-1);
	}
	
	Matrix matrix = Matrix(matrixWidth, matrixHeight);
	MatrixRandomGenerator(&matrix).fillRandom(1, 100);


	TokenPlacer tp = TokenPlacer(matrix, maxTokens, pricePerToken);

	int my_rank;
	int p;
	double tStart, tEnd;
		
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* find out number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	tStart = MPI_Wtime();
	Configuration bestConfiguration = tp.findBestConfiguration();
	tEnd = MPI_Wtime();
	
	cout << "------------------------------------" << endl;
	cout << "Start at: " << tStart << endl;
	cout << "End   at: " << tEnd << endl;
	cout << "Elapsed time was: " << tEnd - tStart << endl;
	cout << "------------------------------------" << endl;
	
	// Debug output
	MatrixRenderer(&matrix).render(& bestConfiguration);
	
	MPI_Finalize();
	
	return 0;
}
