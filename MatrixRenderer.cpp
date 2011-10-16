/* 
 * File:   MatrixRenderer.cpp
 * Author: honza
 * 
 * Created on September 22, 2011, 11:50 PM
 */

#include "MatrixRenderer.h"
#include <iostream>

MatrixRenderer::MatrixRenderer(Matrix * matrix) {
	this->matrix = matrix;
}

void MatrixRenderer::render() {
	for (int y = 0; y < this->matrix->getHeight(); y++) {
		for (int x = 0; x < this->matrix->getWidth(); x++) {
			Coordinate c = Coordinate(x, y);
			std::cout << this->matrix->getValue(c) << ' ';
		}
		std::cout << std::endl;
	}
}