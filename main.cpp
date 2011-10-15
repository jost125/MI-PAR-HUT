/* 
 * File:   main.cpp
 * Author: honza
 *
 * Created on September 20, 2011, 7:54 PM
 */

#include <cstdlib>
#include "Matrix.h"
#include "MatrixRenderer.h"
#include "MatrixRandomGenerator.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
	Matrix matrix = Matrix(5, 8);
	MatrixRandomGenerator(&matrix).fillRandom(1, 100);
	MatrixRenderer(&matrix).render();
	
	return 0;
}
