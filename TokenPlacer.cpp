#include "TokenPlacer.h"
#include <stdexcept>
#include <iostream>
#include "Debug.h"

TokenPlacer::TokenPlacer(const Matrix & matrix, const int maxTokens, const int pricePerToken) : matrix(matrix) {
	this->matrix = matrix;
	this->maxTokens = maxTokens;
	this->pricePerToken = pricePerToken;
	std::vector<Coordinate> emptyList = std::vector<Coordinate>();
	this->bestConfiguration = new Configuration(emptyList);
}

TokenPlacer::TokenPlacer(const TokenPlacer& orig) : matrix(orig.matrix) {
	this->matrix = orig.matrix;
	this->maxTokens = orig.maxTokens;
	this->pricePerToken = orig.pricePerToken;
	this->bestConfiguration = new Configuration(*orig.bestConfiguration);
}

TokenPlacer::~TokenPlacer() {
	delete this->bestConfiguration;
}

void TokenPlacer::generateEachCombination(const Configuration & start, const Configuration & end) {
	// Current is starting configuration
	Configuration currentConfiguration = Configuration(start);

	// Count price.
	double currentPrice = this->countPrice(currentConfiguration);

	// Compare with best solution.
	this->compareAndSaveSolution(currentPrice, currentConfiguration);

	do {
		currentConfiguration = this->getNextConfiguration(currentConfiguration);

		// Count price.
		double currentPrice = this->countPrice(currentConfiguration);

		// Compare with best solution.
		this->compareAndSaveSolution(currentPrice, currentConfiguration);
	} while (!currentConfiguration.equals(end));
}

void TokenPlacer::compareAndSaveSolution(double price, Configuration & currentConfiguration) {
	if (price > this->bestPrice) {
		// Delete previous configuration.
		delete this->bestConfiguration;

		// Save current solution as best solution.
		this->bestPrice = price;
		this->bestConfiguration = new Configuration(currentConfiguration);
	}
}

bool TokenPlacer::isLast(const Configuration & configuration, const int & numberOfTokens) const {
	int position = this->matrix.getHeight() * this->matrix.getWidth() - 1;
	for (int i = 0; i < numberOfTokens; i++) {
		Coordinate c = Coordinate::createCoordinateFromIndex(position, this->matrix.getWidth());
		if (!configuration.contains(c)) {
			return false;
		}
		position--;
	}
	return true;
}

Configuration TokenPlacer::createFirstConfiguration(const int & numberOfTokens) const {
	std::vector<Coordinate> coordinates = std::vector<Coordinate>();
	for (int i = 0; i < numberOfTokens; i++) {
		coordinates.push_back(Coordinate::createCoordinateFromIndex(i, this->matrix.getWidth()));
	}

	return Configuration(coordinates);
}

Configuration TokenPlacer::createLastConfiguration(const int & numberOfTokens) const {
	std::vector<Coordinate> coordinates = std::vector<Coordinate>();
	int lastPosition = this->matrix.getWidth() * this->matrix.getHeight() - 1;
	for (int i = numberOfTokens - 1; i >= 0; i--) {
		coordinates.push_back(Coordinate::createCoordinateFromIndex(lastPosition - i, this->matrix.getWidth()));
	}

	return Configuration(coordinates);
}

Configuration TokenPlacer::getNextConfiguration(const Configuration & configuration) const {
	std::vector<Coordinate> coordinates = configuration.getCoordinates();
	int numberOfTokens = coordinates.size();

	if (this->isLast(configuration, numberOfTokens)) {
		return this->createFirstConfiguration(numberOfTokens + 1);
	}

	int position = this->matrix.getHeight() * this->matrix.getWidth() - 1;
	
	Coordinate * coordinate;
	for (int i = 0; i < numberOfTokens; i++) {
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

	return Configuration(coordinates);
}

Configuration TokenPlacer::findBestConfiguration(const Configuration & start, const Configuration & end) {
	this->generateEachCombination(start, end);
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
