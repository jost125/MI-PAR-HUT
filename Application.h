/* 
 * File:   App.h
 * Author: honza
 *
 * Created on November 14, 2011, 11:27 PM
 */

#include "mpi.h"
#include "Matrix.h"

#ifndef APP_H
#define	APP_H

class Application {
public:
    Application(int * argc, char *** argv);
    Application(const Application& orig);
    virtual ~Application();
    void run();
private:
    void init(int * argc, char *** argv);
    void readInputs();
    void sendInputs();
    void receiveInputs();
    
    void generateMatrix();
    void createMatrix();

    void reallocateBuffer(int size);

    MPI_Request request;
    MPI_Status status;

    char * buffer;
    int bufferSize;

    int rank;
    int processNumber;

    int matrixWidth;
    int matrixHeight;
    int maxTokens;
    int pricePerToken;

    Matrix * matrix;
};

#endif	/* APP_H */

