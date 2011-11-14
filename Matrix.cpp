
#include "Matrix.h"
#include "Coordinate.h"
#include "Debug.h"
#include <stdexcept>
#include <iostream>

Matrix::Matrix(const int width, const int height) {
	this->width = width;
	this->height = height;
	this->initFields();
}

void Matrix::initFields() {
	this->fields = new int * [this->width];
	for (int x = 0; x < this->width; x++) {
		this->fields[x] = new int[this->height];
	}
}

int ** Matrix::getFields() const {
	return this->fields;
}

void Matrix::setFields(int** fields) const {
	for (int x = 0; x < this->width; x++) {
		for (int y = 0; y < this->height; y++) {
			this->fields[x][y] = fields[x][y];
		}
	}
}

Matrix::Matrix(const Matrix & orig) {
	this->height = orig.height;
	this->width = orig.width;
	
	this->fields = new int * [this->width];
	for (int x = 0; x < this->width; x++) {
		this->fields[x] = new int[this->height];
		for (int y = 0; y < this->height; y++) {
			this->fields[x][y] = orig.fields[x][y];
		}
	}
}

Matrix & Matrix::operator= (const Matrix & other) {
	if (this != &other) {
		this->height = other.height;
		this->width = other.width;
		
		this->fields = new int * [this->width];
		for (int x = 0; x < this->width; x++) {
			this->fields[x] = new int[this->height];
			for (int y = 0; y < this->height; y++) {
				this->fields[x][y] = other.fields[x][y];
			}
		}
	}
	
	return * this;
}

Matrix::~Matrix() {
	for (int x = 0; x < this->width; x++) {
		delete [] this->fields[x];
	}	
	delete [] this->fields;
	this->fields = NULL;
}

int Matrix::getValue(const Coordinate & coordinate) const {
	if (!this->isCoordinateInside(coordinate)) {
		throw std::runtime_error("Coordinate is out of matrix");
	}
	return this->fields[coordinate.getX()][coordinate.getY()];
}

void Matrix::setValue(const Coordinate & coordinate, int const value) {
	if (!this->isCoordinateInside(coordinate)) {
		throw std::runtime_error("Coordinate is out of matrix");
	}
	this->fields[coordinate.getX()][coordinate.getY()] = value;
}

bool Matrix::isCoordinateInside(const Coordinate & coordinate) const {
	return coordinate.getY() >= 0 && coordinate.getX() >= 0 &&
		coordinate.getY() < this->height && coordinate.getX() < this->width;
}

int Matrix::getHeight() const {
	return this->height;
}

int Matrix::getWidth() const {
	return this->width;
}
