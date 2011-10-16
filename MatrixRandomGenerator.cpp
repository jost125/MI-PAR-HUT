/* 
 * File:   RandomGenerator.cpp
 * Author: honza
 * 
 * Created on September 23, 2011, 12:59 AM
 */

#include <stdlib.h>
#include <time.h>

#include "MatrixRandomGenerator.h"

MatrixRandomGenerator::MatrixRandomGenerator(Matrix * matrix) {
	this->matrix = matrix;
}

void MatrixRandomGenerator::fillRandom(const int min, const int max) {
	srand(time(NULL));
	for (int x = 0; x < this->matrix->getWidth(); x++) {
		for (int y = 0; y < this->matrix->getHeight(); y++) {
			Coordinate c = Coordinate(x, y);
			this->matrix->setValue(c, rand() % (max - min) + min);
		}
	}
}