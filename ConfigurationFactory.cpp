/* 
 * File:   ConfigurationFactory.cpp
 * Author: honza
 * 
 * Created on November 13, 2011, 2:44 PM
 */

#include "ConfigurationFactory.h"
#include "Configuration.h"
#include "Debug.h"

ConfigurationFactory::ConfigurationFactory(const int & matrixWidth, const int & matrixHeight) {
	this->matrixHeight = matrixHeight;
	this->matrixWidth = matrixWidth;
}

ConfigurationFactory::ConfigurationFactory(const ConfigurationFactory& orig) {
	this->matrixHeight = orig.matrixHeight;
	this->matrixWidth = orig.matrixWidth;
}

ConfigurationFactory::~ConfigurationFactory() {
}

Configuration ConfigurationFactory::createFirstConfiguration(const int & numberOfTokens) const {
	std::vector<Coordinate> coordinates = std::vector<Coordinate>();
	for (int i = 0; i < numberOfTokens; i++) {
		coordinates.push_back(Coordinate::createCoordinateFromIndex(i, this->matrixWidth));
	}

	return Configuration(coordinates);
}

Configuration ConfigurationFactory::createLastConfiguration(const int & numberOfTokens) const {
	std::vector<Coordinate> coordinates = std::vector<Coordinate>();
	int lastPosition = this->getLastMatrixPosition();
	for (int i = numberOfTokens - 1; i >= 0; i--) {
		coordinates.push_back(Coordinate::createCoordinateFromIndex(lastPosition - i, this->matrixWidth));
	}

	return Configuration(coordinates);
}

Configuration ConfigurationFactory::getNextConfiguration(Configuration * configuration) const {
	Configuration copiedConfiguration = Configuration(*configuration);

	std::vector<Coordinate> coordinates = copiedConfiguration.getCoordinates();
	int numberOfTokens = coordinates.size();

	if (this->isLast(&copiedConfiguration)) {
		return this->createFirstConfiguration(numberOfTokens + 1);
	}

	int position = this->getLastMatrixPosition();

	Coordinate * coordinate;
	for (int i = 0; i < numberOfTokens; i++) {
		coordinate = &coordinates.back();
		if (position == coordinate->toIndex(this->matrixWidth)) {
			// Look further.
			position--;
			coordinates.pop_back();
		} else {
			// Create new configuration.
			break;
		}
	}

	int coordinateIndex = coordinate->toIndex(this->matrixWidth);
	Coordinate movedCoordinate = Coordinate::createCoordinateFromIndex(++coordinateIndex, this->matrixWidth);

	coordinates.pop_back();
	coordinates.push_back(movedCoordinate);

	while (coordinates.size() < numberOfTokens) {
		coordinates.push_back(Coordinate::createCoordinateFromIndex(++coordinateIndex, this->matrixWidth));
	}

	return Configuration(coordinates);
}

bool ConfigurationFactory::isLast(Configuration * configuration) const {
	int position = this->getLastMatrixPosition();
	int numberOfTokens = configuration->getCoordinates().size();
	for (int i = 0; i < numberOfTokens; i++) {
		Coordinate c = Coordinate::createCoordinateFromIndex(position, this->matrixWidth);
		if (!configuration->contains(c)) {
			return false;
		}
		position--;
	}
	return true;
}

int ConfigurationFactory::getLastMatrixPosition() const {
	return this->matrixWidth * this->matrixHeight - 1;
}

int ConfigurationFactory::getMatrixWidth() const {
	return this->matrixWidth;
}

int ConfigurationFactory::getMatrixHeight() const {
	return this->matrixHeight;
}

Configuration ConfigurationFactory::createFromIndexes(int * indexes, const int size) const {

	std::vector<Coordinate> coordinates = std::vector<Coordinate>();

	for (int i = 0; i < size; i++) {
		coordinates.push_back(Coordinate::createCoordinateFromIndex(indexes[i], matrixWidth));
	}

	return Configuration(coordinates);
}