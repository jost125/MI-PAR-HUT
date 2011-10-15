/* 
 * File:   TokenPlacer.cpp
 * Author: honza
 * 
 * Created on October 15, 2011, 7:33 PM
 */

#include "TokenPlacer.h"

TokenPlacer::TokenPlacer(const Matrix & matrix) : matrix(matrix) {
	this->bestPrice = -1;
}

TokenPlacer::TokenPlacer(const TokenPlacer& orig) : matrix(orig.matrix) {
}

TokenPlacer::~TokenPlacer() {
}

Configuration TokenPlacer::findBestConfiguration() {
	
}

Configuration TokenPlacer::getNextConfiguration() {
	
}