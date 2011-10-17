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
	
	printLineDelimiter(this->matrix->getWidth(), cellSize, MatrixRenderer::FIRST_LINE);
	
	// Print matrix.
	for (int y = 0; y < this->matrix->getHeight(); y++) {
		for (int x = 0; x < this->matrix->getWidth(); x++) {
			cout << "║";
			Coordinate c = Coordinate(x, y);
			if (config->contains(c)) {
				cout << "●" << setw(2) << this->matrix->getValue(c) << " ";
			} else {
				cout << setw(3) << this->matrix->getValue(c) << " ";
			}
		}
		cout << "║";
		// Print bottom border.
		if (y == this->matrix->getHeight() - 1) {
			printLineDelimiter(this->matrix->getWidth(), cellSize, MatrixRenderer::BOTTOM_LINE);
		} else {
			printLineDelimiter(this->matrix->getWidth(), cellSize, MatrixRenderer::NORMAL_LINE);
		}
	}	
	

}

void MatrixRenderer::printLineDelimiter(int numberOfCells, int cellSize, int flag) const {
	cout << endl;
	for (int i = 0; i <= numberOfCells; i++) {
		// Edge point.
		if (i == 0) {
			switch (flag) {
				case FIRST_LINE: cout << "╔"; break;
				case BOTTOM_LINE: cout << "╚"; break;
				default: case NORMAL_LINE: cout << "╠"; break;
			}
		} else if (i == numberOfCells) {
			switch (flag) {
				case FIRST_LINE: cout << "╗"; break;
				case BOTTOM_LINE: cout << "╝"; break;
				default: case NORMAL_LINE: cout << "╣"; break;
			}
		} else {
			switch (flag) {
				case FIRST_LINE: cout << "╦"; break;
				case BOTTOM_LINE: cout << "╩"; break;
				default: case NORMAL_LINE: cout << "╬"; break;
			}
		}
		
		// Edge line.
		if (i < numberOfCells) {
			for (int j = 0; j < cellSize; j++) {
				cout << "═";
			}
		}
		
		// Insert new line at each end of row.
		if (i == numberOfCells) {
			cout << endl;
		}
	}
}
