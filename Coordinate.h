/* 
 * File:   Coordinate.h
 * Author: honza
 *
 * Created on September 22, 2011, 11:33 PM
 */

#include <ostream>

#ifndef COORDINATE_H
#define	COORDINATE_H

class Coordinate {
    public:
        Coordinate(int const x, int const y);
        Coordinate(const Coordinate & coordinate);
        virtual ~Coordinate();
        int getX() const;
        int getY() const;
        int manhattanDistance(Coordinate coordinate) const;
        bool equals(const Coordinate & compared);
    private:
        int x;
        int y;
};

std::ostream & operator << (std::ostream &os, const Coordinate & coordinate);

#endif	/* COORDINATE_H */

