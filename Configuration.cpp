/* 
 * File:   Configuration.cpp
 * Author: honza
 * 
 * Created on October 15, 2011, 7:24 PM
 */

#include "Configuration.h"

Configuration::Configuration(std::vector<Coordinate> coordinates) {
	this->coordinates = coordinates;
}

Configuration::Configuration(const Configuration & orig) {
	this->coordinates = orig.coordinates;
}

Configuration::~Configuration() {
	
}

bool Configuration::contains(const Coordinate & coordinate) const {
	for (int i = 0; i < this->coordinates.size(); i++) {
		if (coordinate.getX() == this->coordinates.at(i).getX() &&
				  coordinate.getY() == this->coordinates.at(i).getY()) {
			return true;
		}
	}
	return false;
}

std::vector<Coordinate> Configuration::getCoordinates() const {
	return this->coordinates;
}

bool Configuration::equals(const Configuration & compared) const {
	for (int i = 0; i < coordinates.size(); i++) {
		if (!coordinates.at(i).equals(compared.coordinates.at(i))) {
			return false;
		}
	}
	return true;
}

std::ostream & operator << (std::ostream & os, Configuration & configuration) {
	std::vector<Coordinate> coordinates = configuration.getCoordinates();
	for (int i = 0; i < coordinates.size(); i++) {
		os << coordinates.at(i);
	}
	return os;
}
