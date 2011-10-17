/* 
 * File:   Configuration.cpp
 * Author: honza
 * 
 * Created on October 15, 2011, 7:24 PM
 */

#include "Configuration.h"
#include <string.h>
#include <math.h>
#include <iostream>

Configuration::Configuration(std::vector<Coordinate> coordinates, int matrixWidth, int matrixHeight) {
	this->stashSize = 1;
	this->stash = new unsigned long long[this->stashSize];
	this->matrixHeight = matrixHeight;
	this->matrixWidth = matrixWidth;
	this->insertCoordinatesToStash(coordinates);
	this->numberOfBits = sizeof (unsigned long long) * 8;
}

void Configuration::insertCoordinatesToStash(std::vector<Coordinate> coordinates) {
	
	for (long unsigned int i = 0; i < coordinates.size(); i++) {
		Coordinate c = coordinates.at(i);
		int index = c.toIndex(this->matrixWidth) + 1;
		int slot = this->getSlot(index);
		if (this->stashSize < slot + 1) {
			this->enlargeStash(slot + 1);
		}
		this->addIndexToStash(index, slot);
	}
}

void Configuration::addIndexToStash(const int index, const int slot) {
	this->stash[slot] = 1ULL << index % this->numberOfBits;
}

int Configuration::getSlot(const int index) const {
	return ceil((float)index / (float)this->numberOfBits) - 1;
}

void Configuration::enlargeStash(const int newStashSize) {
	unsigned long long* newStash = new unsigned long long[newStashSize];

	memcpy(newStash, this->stash, newStashSize * sizeof (unsigned long long));

	this->stashSize = newStashSize;
	delete [] this->stash;
	this->stash = newStash;
}

Configuration::Configuration(const Configuration & orig) {
	this->stash = new unsigned long long[orig.stashSize];

	memcpy(this->stash, orig.stash, orig.stashSize * sizeof (unsigned long long));
	
	this->stashSize = orig.stashSize;
	this->matrixHeight = orig.matrixHeight;
	this->matrixWidth = orig.matrixWidth;
}

Configuration::~Configuration() {
	delete this->stash;
}

bool Configuration::contains(const Coordinate & coordinate) const {
	int index = coordinate.toIndex(this->matrixWidth) + 1;
	int slot = this->getSlot(index);
	return (bool)(this->stash[slot] & index);
}

std::vector<Coordinate> Configuration::getCoordinates() const {
	std::vector<Coordinate> coordinates = std::vector<Coordinate>();
	for (int slot = 0; slot < this->stashSize; slot++) {
		unsigned long long stash = this->stash[slot];
		while (stash != 0) {
			unsigned long long item = (~stash + 1) & stash;
			stash = item ^ stash;
			int index = log2(item) + slot * this->numberOfBits;
			coordinates.push_back(Coordinate::createCoordinateFromIndex(index, this->matrixWidth));
		}
	}
	
	return coordinates;
}

std::ostream & operator << (std::ostream & os, Configuration & configuration) {
	std::vector<Coordinate> coordinates = configuration.getCoordinates();
	for (unsigned int i = 0; i < coordinates.size(); i++) {
		os << coordinates.at(i);
	}
	return os;
}
