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

void ConfigurationInterval::shiftFirst(ConfigurationFactory * factory) {
	if (this->start != NULL) {


		if (this->start->equals(*this->end)) {
			if (this->start != NULL) {
				delete this->start;
				this->start = NULL;

				delete this->end;
				this->end = NULL;
			}
		} else {

			Configuration * tmp = new Configuration(factory->getNextConfiguration(this->start));

			if (this->start != NULL) {
				delete this->start;
				this->start = NULL;
			}

			this->start = tmp;
		}
	}
}

ConfigurationInterval::~ConfigurationInterval() {
	if (this->start != NULL) {
		delete this->start;
		this->start = NULL;
	}
	if (this->end != NULL) {
		delete this->end;
	}
}

ConfigurationInterval ConfigurationInterval::split(ConfigurationFactory * factory) {
	Configuration secondEnd = Configuration(*this->end);
	
	delete this->end;
	this->end = new Configuration(this->findCenter(factory));

	return ConfigurationInterval(factory->getNextConfiguration(this->end), secondEnd);
}

bool ConfigurationInterval::isSplitable(ConfigurationFactory * factory) {
	if (this->isEmpty()) {
		return false;
	}

	for (int i = 0; i < 3; i++) {
		Configuration current = factory->getNextConfiguration(this->start);
		if (current.equals(*this->end)) {
			return false;
		}
	}
	return true;
}

Configuration ConfigurationInterval::findCenter(ConfigurationFactory * factory) const {
	int endSize = end->getCoordinates().size();
	int startSize = start->getCoordinates().size();
	int sizeDiff = endSize - startSize;

	if (sizeDiff == 0) {
		for (int i = 0; i < endSize; i++) {
			// Current coordinate of end configuration
			Coordinate currEnd = end->getCoordinates().at(i);
			// Current coordinate of start configuration
			Coordinate currStart = start->getCoordinates().at(i);

			// Their index representation
			int currEndIndex = currEnd.toIndex(factory->getMatrixWidth());
			int currStartIndex = currStart.toIndex(factory->getMatrixWidth());

			// Difference between indexes
			int diff = currEndIndex - currStartIndex;

			if (diff >= 2) { // If the indexes differ more than 2 from each other

				// Copy start configuration.
				Configuration configuration = Configuration(*start);

				// Find center of indexes.
				int coordinateIndex = currStartIndex + diff / 2; 

				// Go through rest coordinates and set them greater then founded new coordinate.
				for (int j = 0; j < startSize - i; j++) {
					Coordinate coordinate = Coordinate::createCoordinateFromIndex(
						coordinateIndex + j,
						factory->getMatrixWidth()
					);
					configuration.setCoordinate(j + i, coordinate);
				}

				return configuration;
			} else if (diff == 1) { // If the indexes differ about 1 from each other
				
				// Remember previous coordinate index.
				int previous = currStartIndex;

				// Remember current iterated index.
				int current = currStartIndex;

				// Do we have last iteration?
				bool lastIteration = false;

				// Go through rest of coordinates.
				for (int j = 0; j < startSize - i; j++) {
					// Currently iterated coordinate index
					current = start->getCoordinates().at(j + i).toIndex(factory->getMatrixWidth());

					// is this last iteration?
					lastIteration = j >= endSize - i;

					// Previously iterated coordinate from current more then 2 respectively 3?
					if (current - previous >= (!lastIteration ? 2 : 3)) {
						Configuration configuration = Configuration(*start);

						// Go through rest of coordinates.
						for (int k = 0; k < endSize - (i + j); k++) {
							Coordinate coordinate = Coordinate::createCoordinateFromIndex(
								current + k + 1,
								factory->getMatrixWidth()
							);
							configuration.setCoordinate(k + j + i, coordinate);
						}

						return configuration;
					}
					previous = current;
				}

				Configuration configuration = Configuration(*start);

				int coordinateIndex = start->getCoordinates().at(i + 1).toIndex(factory->getMatrixWidth());

				for (int j = 1; j < start->getCoordinates().size() - i; j++) {
					Coordinate coordinate = Coordinate::createCoordinateFromIndex(
						coordinateIndex + j,
						factory->getMatrixWidth()
					);
					configuration.setCoordinate(j, coordinate);
				}

				return configuration;
			}
		}
	} else if (sizeDiff == 1) {
		// Number of coordinates in new configuration
		int newCoordinatesSize = endSize;

		// Index representation of first coordinate from last configuration
		int endFrontIndex = end->getCoordinates().front().toIndex(factory->getMatrixWidth());

		if (endFrontIndex == 0) { // There is possibility of having first configuration from set

			for (int i = 1; i < newCoordinatesSize; i++) {
				int endCurrentIndex = end->getCoordinates().at(i).toIndex(factory->getMatrixWidth());
				int indexDifference = (int)(endCurrentIndex - i);
				if ((int)(indexDifference / 2) > 0) {

					int newCoordinateIndex = i - indexDifference / 2;

					Configuration configuration = Configuration(*end);

					for (int j = 0; j < newCoordinatesSize - i; j++) {
						configuration.setCoordinate(i + j, Coordinate::createCoordinateFromIndex(newCoordinateIndex + j, factory->getMatrixWidth()));
					}

					return configuration;
				}
			}

			// If we are here, the end configuration is the very first for the number of coordinates.
			// In that case, we will just increment first coordinate of start configuration
			// and update the rest to be consists.

			// Copy start
			Configuration configuration = Configuration(*start);
			int startFrontIndex = start->getCoordinates().front().toIndex(factory->getMatrixWidth());

			// Go through all and update.
			for (int i = 0; i < startSize; i++) {
				configuration.setCoordinate(i, Coordinate::createCoordinateFromIndex(startFrontIndex + i + 1, factory->getMatrixWidth()));
			}

			return configuration;
		} else {

			// It safe to create first configuration for set
			int index = (int)(endFrontIndex / 2);

			std::vector<Coordinate> coordinates = std::vector<Coordinate>();
			for (int i = 0; i < newCoordinatesSize; i++) {
				coordinates.push_back(Coordinate::createCoordinateFromIndex(index + i, factory->getMatrixWidth()));
			}

			return Configuration(coordinates);
			
		}
	} else {
		int index = (int)(factory->getMatrixHeight() * factory->getMatrixWidth() / 2);

		int newCoordinatesSize = startSize + ceil((float)sizeDiff / 2);

		std::vector<Coordinate> coordinates = std::vector<Coordinate>();
		for (int i = 0; i < newCoordinatesSize; i++) {
			coordinates.push_back(Coordinate::createCoordinateFromIndex(index + i, factory->getMatrixWidth()));
		}

		return Configuration(coordinates);
	}
}

std::ostream & operator << (std::ostream &os, const ConfigurationInterval & interval) {
	os << '(' << interval.getStart() << '-' << interval.getEnd() << ')';
	return os;
}