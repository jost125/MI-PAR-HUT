/* 
 * File:   Token.cpp
 * Author: honza
 * 
 * Created on October 15, 2011, 4:58 PM
 */

#include "Token.h"

Token::Token(const Coordinate & coordinate, const int price) : coordinate(coordinate) {
	this->coordinate = coordinate;
	this->price = price;
}

Token::~Token() {
}

Coordinate Token::getCoordinate() const {
	return this->coordinate;
}

int Token::getPrice() const {
	return this->price;
}
