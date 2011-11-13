#include "TokenPlacer.h"
#include <stdexcept>
#include <iostream>
#include "Debug.h"
#include "ConfigurationFactory.h"

TokenPlacer::TokenPlacer(const Matrix & matrix, const ConfigurationFactory & configurationFactory, const int maxTokens, const int pricePerToken) :
		matrix(matrix), configurationFactory(configurationFactory) {
	
	this->matrix = matrix;
	this->configurationFactory = configurationFactory;
	this->maxTokens = maxTokens;
	this->pricePerToken = pricePerToken;
	std::vector<Coordinate> emptyList = std::vector<Coordinate>();
	this->bestConfiguration = new Configuration(emptyList);
}

TokenPlacer::TokenPlacer(const TokenPlacer& orig) : matrix(orig.matrix), configurationFactory(orig.configurationFactory) {
	this->configurationFactory = orig.configurationFactory;
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
	
	bool hasNext = true;
	for (Configuration currentConfiguration = Configuration(start); 
			hasNext;
			currentConfiguration = this->configurationFactory.getNextConfiguration(currentConfiguration)) {

		TRACELN(currentConfiguration);
		// Compare with best solution.
		this->compareAndSaveSolution(this->countPrice(currentConfiguration), currentConfiguration);
		hasNext = !currentConfiguration.equals(end);
	}
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
