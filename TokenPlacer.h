/* 
 * File:   TokenPlacer.h
 * Author: honza
 *
 * Created on October 15, 2011, 7:33 PM
 */

#include "Matrix.h"
#include "Configuration.h"
#include <vector>

#ifndef TOKENPLACER_H
#define	TOKENPLACER_H

class TokenPlacer {
public:
    TokenPlacer(const Matrix & matrix, const int maxTokens, const int pricePerToken);
    TokenPlacer(const TokenPlacer & orig);
    virtual ~TokenPlacer();
    Configuration findBestConfiguration();
private:
    static const int UNDEFINED_PRICE = -1;
    int bestPrice;
    int maxTokens;
    int pricePerToken;
    Matrix matrix;
    Configuration * bestConfiguration;
    std::vector<Configuration> stack;

    void constructStack();
    bool existsNextConfiguration(const Configuration & configuration, const int & numberOfTokens) const;
    Configuration getNextConfiguration(const Configuration & configuration) const;
    int countPrice(const Configuration & configuration) const;
    void generateEachCombination(const int numberOfTokens);
    int mapCoordinateOnIndex(const Coordinate & coordinate) const;
    Coordinate mapIndexOnCoordinate(const int index) const;
};

#endif	/* TOKENPLACER_H */

