/* 
 * File:   RandomGenerator.h
 * Author: honza
 *
 * Created on September 23, 2011, 12:59 AM
 */

#include "Matrix.h"

#ifndef MATRIXRANDOMGENERATOR_H
#define	MATRIXRANDOMGENERATOR_H

class MatrixRandomGenerator {
    public:
        MatrixRandomGenerator(Matrix * matrix);
        void fillRandom(const int min, const int max);
    private:
        Matrix * matrix;
};

#endif	/* MATRIXRANDOMGENERATOR_H */

