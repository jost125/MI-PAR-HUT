/* 
 * File:   TokenPlacer.cpp
 * Author: honza
 * 
 * Created on October 15, 2011, 7:33 PM
 */

#include "TokenPlacer.h"

TokenPlacer::TokenPlacer(const Matrix & matrix, const int maxTokens, const int pricePerToken) : matrix(matrix) {
	this->maxTokens = maxTokens;
	this->pricePerToken = pricePerToken;
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

static Coordinate mapIndexOnCoordinate(const int index) {
	
}

static int mapCoordinateOnIndex(const Coordinate & coordinate) {

}

Configuration TokenPlacer::findBestConfiguration() {
	// While stack is not empty
	while (!this->stack.empty()) {

		// Pop configuration from stack.
		Configuration currentConfiguration = stack.back();
		stack.pop_back();

		// Count price.
		int currentPrice = this->countPrice(currentConfiguration);

		// Compare with best solution.
		if (currentPrice > this->bestPrice) {
			// It's better, so delete previous configuration.
			if (this->bestPrice != TokenPlacer::UNDEFINED_PRICE) {
				delete this->bestConfiguration;
			}

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
