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

