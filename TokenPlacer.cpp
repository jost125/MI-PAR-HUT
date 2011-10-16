/* 
 * File:   TokenPlacer.cpp
 * Author: honza
 * 
 * Created on October 15, 2011, 7:33 PM
 */

#include "TokenPlacer.h"
#include <stdexcept>

TokenPlacer::TokenPlacer(const Matrix & matrix, const int maxTokens, const int pricePerToken) : matrix(matrix) {
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
	
}

Coordinate TokenPlacer::mapIndexOnCoordinate(const int index) {
	if (index >= this->matrix.getHeight() * this->matrix.getWidth()) {
		throw std::runtime_error("Index is out of range.");
	}
	int x = index % this->matrix.getWidth();
	int y = index / this->matrix.getWidth();
	return Coordinate(x,y);
}

int TokenPlacer::mapCoordinateOnIndex(const Coordinate & coordinate) {
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
//		int currentPrice = this->countPrice(currentConfiguration);
		int currentPrice = 0;

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

Configuration TokenPlacer::getNextConfiguration(const Configuration & configuration) const {
	
}

Configuration TokenPlacer::getInitialConfiguration() const {
	std::vector<Coordinate> set;
	set.push_back(Coordinate(1, 1));
	return Configuration(set);
}
