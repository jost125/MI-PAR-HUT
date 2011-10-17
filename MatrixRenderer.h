/* 
 * File:   MatrixRenderer.h
 * Author: honza
 *
 * Created on September 22, 2011, 11:50 PM
 */

#include "Matrix.h"
#include "Configuration.h"

#ifndef MATRIXRENDERER_H
#define	MATRIXRENDERER_H

class MatrixRenderer {
    public:
        MatrixRenderer(Matrix * matrix);
        void render();
        void render(Configuration * config);
    private:
        Matrix * matrix;
        void printLineDelimiter(int numberOfCells, int cellSize, int flag) const;
        static const int FIRST_LINE = 1;
        static const int NORMAL_LINE = 2;
        static const int BOTTOM_LINE = 3;
};

#endif	/* MATRIXRENDERER_H */

