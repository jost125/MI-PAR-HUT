
#include "Matrix.h"
#include "Coordinate.h"
#include <stdexcept>

Matrix::Matrix(const int width, const int height) {
	this->width = width;
	this->height = height;
	this->initFields();
}

void Matrix::initFields() {
	this->fields = std::vector<std::vector<int> >();
	for (int x = 0; x < this->width; x++) {
		std::vector<int> row = std::vector<int>();
		for (int y = 0; y < this->height; y++) {
			row.push_back(0);
		}
		this->fields.push_back(row);
	}
}

Matrix::Matrix(const Matrix & orig) {
	this->height = orig.height;
	this->width = orig.width;
	this->fields = orig.fields;
}

Matrix::~Matrix() {
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
