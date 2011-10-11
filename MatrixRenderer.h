/* 
 * File:   MatrixRenderer.h
 * Author: honza
 *
 * Created on September 22, 2011, 11:50 PM
 */

#include "Matrix.h"

#ifndef MATRIXRENDERER_H
#define	MATRIXRENDERER_H

class MatrixRenderer {
    public:
        MatrixRenderer(Matrix * matrix);
        void render();
    private:
        Matrix * matrix;
};

#endif	/* MATRIXRENDERER_H */

