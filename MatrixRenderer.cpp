/* 
 * File:   MatrixRenderer.cpp
 * Author: honza
 * 
 * Created on September 22, 2011, 11:50 PM
 */

#include "MatrixRenderer.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>

using namespace std;

MatrixRenderer::MatrixRenderer(Matrix * matrix) {
	this->matrix = matrix;
}

void MatrixRenderer::render() {
	for (int y = 0; y < this->matrix->getHeight(); y++) {
		for (int x = 0; x < this->matrix->getWidth(); x++) {
			Coordinate c = Coordinate(x, y);
			cout << this->matrix->getValue(c) << ' ';
		}
		cout << endl;
	}
}

void MatrixRenderer::render(Configuration * config) {
	int cellSize = 4;
	// Print top border.
	printLineDelimiter(this->matrix->getWidth(), cellSize);
	
	// Print matrix.
	for (int y = 0; y < this->matrix->getHeight(); y++) {
		for (int x = 0; x < this->matrix->getWidth(); x++) {
			cout << "║";
			Coordinate c = Coordinate(x, y);
			if (config->contains(c)) {
				cout << "●" << setw(2) << this->matrix->getValue(c) << " ";
//				printCellDelimiter(4, cellSize);
			} else {
				cout << setw(3) << this->matrix->getValue(c) << " ";
//				printCellDelimiter(2, cellSize);
			}
		}
		cout << "║";
		// Print bottom border.
		printLineDelimiter(this->matrix->getWidth(), cellSize);
	}	
	

}

void MatrixRenderer::printLineDelimiter(int numberOfCells, int cellSize) {
	cout << endl;
	for (int i = 0; i < numberOfCells; i++) {
		cout << "╬";
		for (int j = 0; j < cellSize; j++) {
			cout << "═";
		}
		if (i == numberOfCells - 1) {
			cout << "╬" << endl;
		}
	}
}

void MatrixRenderer::printCellDelimiter(int alreadyWritten, int cellSize) {
	for (int i = 0; i <= cellSize - alreadyWritten; i++) {
		cout << " ";
	}
	cout << "|";
}
