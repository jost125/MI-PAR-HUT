/* 
 * File:   Matrix.h
 * Author: honza
 *
 * Created on September 22, 2011, 7:27 PM
 */

#include "Coordinate.h"
#include <vector>

#ifndef MATRIX_H
#define	MATRIX_H

class Matrix {
        private:
            int height;
            int width;
            std::vector<std::vector<int> > fields;
        public:
            Matrix(int const width, int const height);
            Matrix(const Matrix & orig);
            virtual ~Matrix();
            void setValue(const Coordinate & coordinate, int const value);
            int getValue(const Coordinate & coordinate) const;
            int getWidth() const;
            int getHeight() const;
            bool isCoordinateInside(const Coordinate & coordinate) const;
            void initFields();
};

#endif	/* MATRIX_H */

