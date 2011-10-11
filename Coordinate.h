/* 
 * File:   Coordinate.h
 * Author: honza
 *
 * Created on September 22, 2011, 11:33 PM
 */

#ifndef COORDINATE_H
#define	COORDINATE_H

class Coordinate {
    public:
        Coordinate(int const x, int const y);
        ~Coordinate();
        int getX() const;
        int getY() const;
    private:
        int x;
        int y;
};

#endif	/* COORDINATE_H */

