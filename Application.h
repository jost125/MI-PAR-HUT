/* 
 * File:   App.h
 * Author: honza
 *
 * Created on November 14, 2011, 11:27 PM
 */

#include "mpi.h"
#include "Matrix.h"
#include "ConfigurationFactory.h"
#include "ConfigurationInterval.h"
#include "TokenPlacer.h"

#ifndef APP_H
#define	APP_H

class Application {
public:
    static const short NOTOKEN = -1;
    static const short WHITE = 0;
    static const short BLACK = 1;

    Application(int * argc, char *** argv);
    Application(const Application& orig);
    virtual ~Application();
    void run();
private:
    void init(int * argc, char *** argv);
    void processArguments(int * argc, char *** argv);

    void initWholeInterval();
    bool isSingleProcess();

    void sendMatrix();
    void receiveMatrix();

    void sendInitIntervals();
    void sendJob(ConfigurationInterval interval, int rank);
    void receiveJob();
    
    bool hasToken();
    void sendToken();
    void receiveToken();

    void sendFinish();
    void receiveFinish();

    void finish();

    void doJob();

    void sendConfiguration(const Configuration & configuration, int rank);
    Configuration receiveConfiguration();

    void sendNoJob(const int & rank);
    void receiveNoJob();
    void requestForJob();
    int receiveJobRequest();
    void sendWhichPrice(double price);
    void sendEnd();
    void receiveEnd();
    double receivePrice();
    bool hasJob() const;
    bool isFinished();

    void checkMessages();

    int getNextRank();

    bool isSent();
    void waitForSend();
    
    void generateMatrix();
    void createMatrix();

    void reallocateSendBuffer(int size);
    void reallocateReceiveBuffer(int size);

    ConfigurationFactory * getFactory();
    TokenPlacer * getPlacer();
    ConfigurationInterval * getInterval() const;
    void setInterval(ConfigurationInterval * interval);

    double countPrice(const Configuration & configuration) const;

    MPI_Request request;
    MPI_Status status;

    short color;
    short token;

    char * sendBuffer;
    int sendBufferSize;

    char * receiveBuffer;
    int receiveBufferSize;

    bool finished;
    bool end;

    int rank;
    int processNumber;

    int matrixWidth;
    int matrixHeight;
    int maxTokens;
    int pricePerToken;

    bool jobRequested;

    int rankCounter;

    double bestPrice;
    Configuration * bestConfiguration;

    Matrix * matrix;

    TokenPlacer * placer;

    ConfigurationInterval * interval;
    ConfigurationFactory * factory;
};

#endif	/* APP_H */

