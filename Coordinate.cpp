/* 
 * File:   Coordinate.cpp
 * Author: honza
 * 
 * Created on September 22, 2011, 11:33 PM
 */

#include "Coordinate.h"

Coordinate::Coordinate(int const x, int const y) {
	this->x = x;
	this->y = y;
}

Coordinate::~Coordinate() {
}

int Coordinate::getX() const {
	return this->x;
}

int Coordinate::getY() const {
	return this->y;
}