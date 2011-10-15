/* 
 * File:   Matrix.h
 * Author: honza
 *
 * Created on September 22, 2011, 7:27 PM
 */

#include "Coordinate.h"

#ifndef MATRIX_H
#define	MATRIX_H

class Matrix {
        private:
            int height;
            int width;
            int ** fields;
        public:
            Matrix(int const width, int const height);
            ~Matrix();
            void setValue(const Coordinate & coordinate, int const value);
            int getValue(const Coordinate & coordinate) const;
            int getWidth() const;
            int getHeight() const;
            bool isCoordinateInside(const Coordinate & coordinate) const;
};

#endif	/* MATRIX_H */

