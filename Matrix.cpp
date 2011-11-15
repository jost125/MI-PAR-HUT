
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
	this->fields = new int [this->width * this->height];
}

int * Matrix::getFieldsPointer() const {
	return this->fields;
}

void Matrix::setFields(int* fields) const {
	for (int i = 0; i < this->width * this->height; i++) {
		this->fields[i] = fields[i];
	}
}

Matrix::Matrix(const Matrix & orig) {
	this->height = orig.height;
	this->width = orig.width;

	this->fields = new int [this->width * this->height];
	for (int i = 0; i < this->width * this->height; i++) {
		this->fields[i] = orig.fields[i];
	}
}

Matrix & Matrix::operator= (const Matrix & other) {
	if (this != &other) {
		this->height = other.height;
		this->width = other.width;
		
		this->fields = new int [this->width * this->height];
		for (int i = 0; i < this->width * this->height; i++) {
			this->fields[i] = other.fields[i];
		}
	}
	
	return * this;
}

Matrix::~Matrix() {
	delete [] this->fields;
	this->fields = NULL;
}

int Matrix::getValue(const Coordinate & coordinate) const {
	if (!this->isCoordinateInside(coordinate)) {
		throw std::runtime_error("Coordinate is out of matrix");
	}
	return this->fields[this->width * coordinate.getY() + coordinate.getX()];
}

void Matrix::setValue(const Coordinate & coordinate, int const value) {
	if (!this->isCoordinateInside(coordinate)) {
		throw std::runtime_error("Coordinate is out of matrix");
	}
	this->fields[this->width * coordinate.getY() + coordinate.getX()] = value;
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
