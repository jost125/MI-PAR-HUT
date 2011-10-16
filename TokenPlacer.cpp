#include "TokenPlacer.h"
#include <stdexcept>

#include <iostream>
using namespace std;

TokenPlacer::TokenPlacer(const Matrix & matrix, const int maxTokens, const int pricePerToken) : matrix(matrix) {
	this->matrix = matrix;
	this->maxTokens = maxTokens;
	this->pricePerToken = pricePerToken;
	std::vector<Coordinate> emptyList = std::vector<Coordinate>();
	this->bestConfiguration = new Configuration(emptyList);
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
		coordinateList.push_back(Coordinate(this->mapIndexOnCoordinate(index)));
	}
	Configuration currentConfiguration = Configuration(coordinateList);
	this->stack.push_back(currentConfiguration);

	while (this->existsNextConfiguration(currentConfiguration, numberOfTokens)) {
		currentConfiguration = this->getNextConfiguration(currentConfiguration, numberOfTokens);
		this->stack.push_back(currentConfiguration);
	}
}

bool TokenPlacer::existsNextConfiguration(const Configuration & configuration, const int & numberOfTokens) const {
	int position = this->matrix.getHeight() * this->matrix.getWidth() - 1;
	for (int i = 0; i < numberOfTokens; i++) {
		Coordinate c = this->mapIndexOnCoordinate(position);
		if (!configuration.contains(c)) {
			return true;
		}
		position--;
	}
	return false;
}

Configuration TokenPlacer::getNextConfiguration(const Configuration & configuration, const int & numberOfTokens) const {
	int position = this->matrix.getHeight() * this->matrix.getWidth() - 1;
	std::vector<Coordinate> coordinates = configuration.getCoordinates();

	Coordinate * coordinate;
	for (int i = 0; i < numberOfTokens; i++) {
		coordinate = &coordinates.back();
		if (position == this->mapCoordinateOnIndex(*coordinate)) {
			// Look further.
			position--;
			coordinates.pop_back();
		} else {
			// Create new configuration.
			break;
		}
	}

	int coordinateIndex = this->mapCoordinateOnIndex(Coordinate(*coordinate));
	Coordinate movedCoordinate = this->mapIndexOnCoordinate(++coordinateIndex);

	coordinates.pop_back();
	coordinates.push_back(movedCoordinate);

	while (coordinates.size() == numberOfTokens) {
		coordinates.push_back(this->mapIndexOnCoordinate(++coordinateIndex));
	}

	return Configuration(coordinates);
}

Coordinate TokenPlacer::mapIndexOnCoordinate(const int index) const {
	if (index >= this->matrix.getHeight() * this->matrix.getWidth()) {
		throw std::runtime_error("Index is out of range.");
	}
	int x = index % this->matrix.getWidth();
	int y = index / this->matrix.getWidth();
	return Coordinate(x, y);
}

int TokenPlacer::mapCoordinateOnIndex(const Coordinate & coordinate) const {
	if (!this->matrix.isCoordinateInside(coordinate)) {
		throw std::runtime_error("Coordinates are out of matrix.");
	}
	return (coordinate.getY() * this->matrix.getWidth()) + coordinate.getX();
}

Configuration TokenPlacer::findBestConfiguration() {
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
}
	
double TokenPlacer::countPrice(const Configuration & configuration) const {
	double totalPrice = 0;
	
	// Count price for each field.
	for (int x = 0; x < this->matrix.getWidth(); x++) {
		for (int y = 0; y < this->matrix.getHeight(); y++) {
			// Find the shortest way from field to token.
			cout << "-----------------------" << endl;
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
			cout << "MINIMAL DISTANCE: " << minDistance << endl;
			// We should have minimal distance, so count the price.
			if (minDistance != -1) {
				Coordinate c = Coordinate(x, y);
				totalPrice += (double)this->matrix.getValue(c) / (double)(1 + minDistance);
				cout << "VALUE:" << this->matrix.getValue(c) << " | += " << (double)this->matrix.getValue(c) / (double)(1 + minDistance) << endl;
			}
		}
	}
	// Subtract tokens * pricePerToken.
	totalPrice -= configuration.getCoordinates().size() * this->pricePerToken;
	return totalPrice;
}
