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
            int * fields;
        public:
            Matrix(int const width, int const height);
            Matrix(const Matrix & orig);
            Matrix & operator= (const Matrix & other);
            virtual ~Matrix();
            void setValue(const Coordinate & coordinate, int const value);
            int getValue(const Coordinate & coordinate) const;
            int getWidth() const;
            int getHeight() const;
            bool isCoordinateInside(const Coordinate & coordinate) const;
            void initFields();
            int * getFields() const;
            void setFields(int * fields) const;
};

#endif	/* MATRIX_H */

