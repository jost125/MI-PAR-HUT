/* 
 * File:   ConfigurationInterval.cpp
 * Author: honza
 * 
 * Created on November 13, 2011, 2:26 PM
 */

#include "ConfigurationInterval.h"
#include "ConfigurationFactory.h"
#include "Debug.h"
#include <math.h>

ConfigurationInterval::ConfigurationInterval(const Configuration & start, const Configuration & end) {
	this->start = new Configuration(start);
	this->end = new Configuration(end);
}

ConfigurationInterval::ConfigurationInterval(const ConfigurationInterval& orig) {
	this->start = new Configuration(*orig.start);
	this->end = new Configuration(*orig.end);
}

Configuration ConfigurationInterval::getStart() const {
	return *this->start;
}

Configuration ConfigurationInterval::getEnd() const {
	return *this->end;
}

bool ConfigurationInterval::isEmpty() const {
	return this->start == NULL;
}

void ConfigurationInterval::shiftFirst(const ConfigurationFactory & factory) {
	if (this->start != NULL) {


		if (this->start->equals(*this->end)) {
			delete this->start;
			delete this->end;

			this->start = NULL;
			this->end = NULL;
		} else {
			delete this->start;

			this->start = new Configuration(factory.getNextConfiguration(*this->start));
		}
	}
}

ConfigurationInterval::~ConfigurationInterval() {
	if (this->start != NULL) {
		delete this->start;
	}
	if (this->end != NULL) {
		delete this->end;
	}
}

ConfigurationInterval ConfigurationInterval::split(const ConfigurationFactory & factory) {
	Configuration secondEnd = Configuration(*this->end);
	
	delete this->end;
	this->end = new Configuration(this->findCenter(factory));

	return ConfigurationInterval(factory.getNextConfiguration(*this->end), secondEnd);
}

bool ConfigurationInterval::isSplitable(const ConfigurationFactory & factory) {
	if (this->isEmpty()) {
		return false;
	}

	Configuration current = Configuration(*this->start);

	for (int i = 0; i < 2; i++) {
		current = factory.getNextConfiguration(current);
		if (current.equals(*this->end)) {
			return false;
		}
	}
	return true;
}

Configuration ConfigurationInterval::findCenter(const ConfigurationFactory & factory) const {
	int endSize = end->getCoordinates().size();
	int startSize = start->getCoordinates().size();
	int sizeDiff = endSize - startSize;

	if (sizeDiff == 0) {
		for (int i = 0; i < endSize; i++) {
			Coordinate currEnd = end->getCoordinates().at(i);
			Coordinate currStart = start->getCoordinates().at(i);

			int currEndIndex = currEnd.toIndex(factory.getMatrixWidth());
			int currStartIndex = currStart.toIndex(factory.getMatrixWidth());

			int diff = currEndIndex - currStartIndex;

			if (diff >= 2) {
				Configuration configuration = Configuration(*end);
				int coordinateIndex = currStartIndex + diff / 2;
				
				for (int j = 0; j < end->getCoordinates().size() - i; j++) {
					Coordinate coordinate = Coordinate::createCoordinateFromIndex(
						coordinateIndex + j,
						factory.getMatrixWidth()
					);
					configuration.setCoordinate(j, coordinate);
				}

				return configuration;
			} else if (diff == 1) {
				int previous = currEndIndex;
				int current = currEndIndex;
				bool lastIteration = false;
				for (int j = 0; j < endSize - i; j++) {

					lastIteration = j == endSize - i - 1;
					
					if (current <= previous - (!lastIteration ? 2 : 3)) {
						Configuration configuration = Configuration(*end);

						for (int k = 0; k < endSize - i; k++) {
							Coordinate coordinate = Coordinate::createCoordinateFromIndex(
								currStartIndex + k + 1,
								factory.getMatrixWidth()
							);
							configuration.setCoordinate(k, coordinate);
						}

						return configuration;
					}
					previous = end->getCoordinates().at(j).toIndex(factory.getMatrixWidth());
				}

				Configuration configuration = Configuration(*start);

				int coordinateIndex = start->getCoordinates().at(i + 1).toIndex(factory.getMatrixWidth());

				for (int j = 1; j < start->getCoordinates().size() - i; j++) {
					Coordinate coordinate = Coordinate::createCoordinateFromIndex(
						coordinateIndex + j,
						factory.getMatrixWidth()
					);
					configuration.setCoordinate(j, coordinate);
				}

				return configuration;
			}
		}
	} else if (sizeDiff == 1) {
		int newCoordinatesSize = endSize;

		int endFrontIndex = end->getCoordinates().front().toIndex(factory.getMatrixWidth());

		if (endFrontIndex == 0) {
			for (int i = 1; i < newCoordinatesSize; i++) {
				int endCurrentIndex = end->getCoordinates().at(i).toIndex(factory.getMatrixWidth());
				int indexDifference = (int)(endCurrentIndex + i);
				if (indexDifference / 2 > 0) {

					Configuration configuration = Configuration(*end);
					configuration.setCoordinate(i, Coordinate::createCoordinateFromIndex(i - indexDifference, factory.getMatrixWidth()));

					return configuration;
				}
			}

			Configuration configuration = Configuration(*start);
			int startFrontIndex = start->getCoordinates().front().toIndex(factory.getMatrixWidth());

			configuration.setCoordinate(0, Coordinate::createCoordinateFromIndex(startFrontIndex + 1, factory.getMatrixWidth()));

			return configuration;
		}

		int index = (int)(endFrontIndex / 2);

		std::vector<Coordinate> coordinates = std::vector<Coordinate>();
		for (int i = 0; i < newCoordinatesSize; i++) {
			coordinates.push_back(Coordinate::createCoordinateFromIndex(index + i, factory.getMatrixWidth()));
		}

		return Configuration(coordinates);
	} else {
		int index = (int)(factory.getMatrixHeight() * factory.getMatrixWidth() / 2);

		int newCoordinatesSize = startSize + ceil((float)sizeDiff / 2);

		std::vector<Coordinate> coordinates = std::vector<Coordinate>();
		for (int i = 0; i < newCoordinatesSize; i++) {
			coordinates.push_back(Coordinate::createCoordinateFromIndex(index + i, factory.getMatrixWidth()));
		}

		return Configuration(coordinates);
	}
}

std::ostream & operator << (std::ostream &os, const ConfigurationInterval & interval) {
	os << '(' << interval.getStart() << '-' << interval.getEnd() << ')';
	return os;
}