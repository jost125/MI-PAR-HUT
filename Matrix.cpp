
#include "Matrix.h"
#include "Coordinate.h"
#include <stdexcept>

Matrix::Matrix(const int width, const int height) {
	this->width = width;
	this->height = height;
	this->initFields();
}

void Matrix::initFields() {
	this->fields = new int*[height];

	for (int y = 0; y < height; y++) {
		this->fields[y] = new int[width];
		for (int x = 0; x < width; x++) {
			this->fields[y][x] = 0;
		}
	}
}

Matrix::Matrix(const Matrix & matrix) {
	this->height = matrix.height;
	this->width = matrix.width;
	this->initFields();
}

Matrix::~Matrix() {
	for (int y = 0; y < this->height; y++) {
		delete [] this->fields[y];
	}
}

int Matrix::getValue(const Coordinate & coordinate) const {
	if (!this->isCoordinateInside(coordinate)) {
		throw std::runtime_error("Coordinate is out of matrix");
	}
	return this->fields[coordinate.getY()][coordinate.getX()];
}

void Matrix::setValue(const Coordinate & coordinate, int const value) {
	if (!this->isCoordinateInside(coordinate)) {
		throw std::runtime_error("Coordinate is out of matrix");
	}
	this->fields[coordinate.getY()][coordinate.getX()] = value;
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
