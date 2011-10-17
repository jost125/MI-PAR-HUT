#include "TokenPlacer.h"
#include <stdexcept>
#include <iostream>

TokenPlacer::TokenPlacer(const Matrix & matrix, const int maxTokens, const int pricePerToken) : matrix(matrix) {
	this->matrix = matrix;
	this->maxTokens = maxTokens;
	this->pricePerToken = pricePerToken;
	std::vector<Coordinate> emptyList = std::vector<Coordinate>();
	this->bestConfiguration = new Configuration(emptyList, matrix.getWidth(), matrix.getHeight());
}

TokenPlacer::TokenPlacer(const TokenPlacer& orig) : matrix(orig.matrix) {
}

TokenPlacer::~TokenPlacer() {
	delete this->bestConfiguration;
}

void TokenPlacer::constructStack() {
	for (int numberOfTokens = 1; numberOfTokens <= this->maxTokens; numberOfTokens++) {
		this->generateEachCombination(numberOfTokens);
	}
}

void TokenPlacer::generateEachCombination(const int numberOfTokens) {
	std::vector<Coordinate> coordinateList = std::vector<Coordinate>();
	for (int index = 0; index < numberOfTokens; index++) {
		coordinateList.push_back(Coordinate::createCoordinateFromIndex(index, this->matrix.getWidth()));
	}
	Configuration currentConfiguration = Configuration(coordinateList, this->matrix.getWidth(), this->matrix.getHeight());
	this->stack.push_back(currentConfiguration);

	do {
		currentConfiguration = this->getNextConfiguration(currentConfiguration, numberOfTokens);
		this->stack.push_back(currentConfiguration);
	} while (this->existsNextConfiguration(currentConfiguration, numberOfTokens));
}

bool TokenPlacer::existsNextConfiguration(const Configuration & configuration, const int & numberOfTokens) const {
	int position = this->matrix.getHeight() * this->matrix.getWidth() - 1;
	for (int i = 0; i < numberOfTokens; i++) {
		Coordinate c = Coordinate::createCoordinateFromIndex(position, this->matrix.getWidth());
		if (!configuration.contains(c)) {
			return true;
		}
		position--;
	}
	return false;
}

Configuration TokenPlacer::getNextConfiguration(const Configuration & configuration, const unsigned int & numberOfTokens) const {
	int position = this->matrix.getHeight() * this->matrix.getWidth() - 1;
	std::vector<Coordinate> coordinates = configuration.getCoordinates();

	Coordinate * coordinate;
	for (unsigned int i = 0; i < numberOfTokens; i++) {
		coordinate = &coordinates.back();
		if (position == coordinate->toIndex(this->matrix.getWidth())) {
			// Look further.
			position--;
			coordinates.pop_back();
		} else {
			// Create new configuration.
			break;
		}
	}
	int coordinateIndex = coordinate->toIndex(this->matrix.getWidth());
	Coordinate movedCoordinate = Coordinate::createCoordinateFromIndex(++coordinateIndex, this->matrix.getWidth());

	coordinates.pop_back();
	coordinates.push_back(movedCoordinate);

	while (coordinates.size() < numberOfTokens) {
		coordinates.push_back(Coordinate::createCoordinateFromIndex(++coordinateIndex, this->matrix.getWidth()));
	}

	return Configuration(coordinates, this->matrix.getWidth(), this->matrix.getHeight());
}

Configuration TokenPlacer::findBestConfiguration() {
	this->constructStack();
	// While stack is not empty
	while (!this->stack.empty()) {

		// Pop configuration from stack.
		Configuration currentConfiguration = stack.back();
		stack.pop_back();

		// Count price.
		double currentPrice = this->countPrice(currentConfiguration);

		// Compare with best solution.
		if (currentPrice > this->bestPrice) {
			// Delete previous configuration.
			delete this->bestConfiguration;

			// Save current solution as best solution.
			this->bestPrice = currentPrice;
			this->bestConfiguration = new Configuration(currentConfiguration);
		}
	}
	std::cout << "Max price: " << this->bestPrice << std::endl;
	
	return *this->bestConfiguration;
}
	
double TokenPlacer::countPrice(const Configuration & configuration) const {
	double totalPrice = 0;
	
	// Count price for each field.
	for (int x = 0; x < this->matrix.getWidth(); x++) {
		for (int y = 0; y < this->matrix.getHeight(); y++) {
			// Find the shortest way from field to token.
			int minDistance = -1;
			for (int k = 0; k < configuration.getCoordinates().size(); k++) {
				int distance = configuration.getCoordinates().at(k).manhattanDistance(Coordinate(x, y));
				if (distance == 0) {
					minDistance = distance;
					break;
				}
				
				if (distance < minDistance || minDistance == -1) {
					minDistance = distance;
				}
			}
			// We should have minimal distance, so count the price.
			if (minDistance != -1) {
				Coordinate c = Coordinate(x, y);
				totalPrice += (double)this->matrix.getValue(c) / (double)(1 + minDistance);
			}
		}
	}
	// Subtract tokens * pricePerToken.
	totalPrice -= configuration.getCoordinates().size() * this->pricePerToken;
	return totalPrice;
}
