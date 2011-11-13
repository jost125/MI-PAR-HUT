/* 
 * File:   InputReader.cpp
 * Author: honza
 * 
 * Created on November 13, 2011, 12:39 PM
 */

#include <iosfwd>
#include <iostream>

#include "InputReader.h"

InputReader::InputReader() {
}

InputReader::InputReader(const InputReader& orig) {
}

InputReader::~InputReader() {
}

int InputReader::readHeight() {
	int height;

	std::cout << "Enter height of matrix:" << std::endl;
	std::cin >> height;

	return height;
}

int InputReader::readWidth() {

	int width;

	std::cout << "Enter width of matrix:" << std::endl;
	std::cin >> width;

	return width;
}

int InputReader::readMaxTokens() {

	int maxTokens;

	std::cout << "Enter number of maximal tokens:" << std::endl;
	std::cin >> maxTokens;

	return maxTokens;
}

int InputReader::readPricePerToken() {

	int pricePerToken;

	std::cout << "Enter price for token:" << std::endl;
	std::cin >> pricePerToken;

	return pricePerToken;
}
