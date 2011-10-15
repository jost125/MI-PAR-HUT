/* 
 * File:   Token.h
 * Author: honza
 *
 * Created on October 15, 2011, 4:58 PM
 */

#include "Coordinate.h"

#ifndef TOKEN_H
#define	TOKEN_H

class Token {
public:
    Token(const Coordinate & coordinate, const int price);
    Coordinate getCoordinate() const;
    int getPrice() const;
    ~Token();
private:
    Coordinate coordinate;
    int price;
};

#endif	/* TOKEN_H */

