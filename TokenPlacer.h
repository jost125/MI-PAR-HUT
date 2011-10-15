/* 
 * File:   TokenPlacer.h
 * Author: honza
 *
 * Created on October 15, 2011, 7:33 PM
 */

#include "Matrix.h"
#include "Configuration.h"

#ifndef TOKENPLACER_H
#define	TOKENPLACER_H

class TokenPlacer {
public:
    TokenPlacer(const Matrix & matrix);
    TokenPlacer(const TokenPlacer& orig);
    virtual ~TokenPlacer();
    Configuration findBestConfiguration();
private:
    int bestPrice;
    Matrix matrix;
    Configuration * bestConfiguration;
    Configuration * actualConfiguration;
    
    Configuration getNextConfiguration();
};

#endif	/* TOKENPLACER_H */

