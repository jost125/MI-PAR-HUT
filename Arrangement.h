/* 
 * File:   Arangement.h
 * Author: honza
 *
 * Created on October 4, 2011, 12:17 PM
 */

#include "Coordinate.h"

#ifndef ARRANGEMENT_H
#define	ARRANGEMENT_H

class Arrangement {
    private:
        int length;
        Coordinate * coordiantes;
        Coordinate();
    public:
        Coordinate(const Coordinate * coordinates, const int length);
};

#endif	/* ARRANGEMENT_H */

