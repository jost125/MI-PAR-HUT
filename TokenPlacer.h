/* 
 * File:   TokenPlacer.h
 * Author: honza
 *
 * Created on October 15, 2011, 7:33 PM
 */

#include "Matrix.h"
#include "Configuration.h"
#include "ConfigurationFactory.h"
#include <vector>

#ifndef TOKENPLACER_H
#define	TOKENPLACER_H

class TokenPlacer {
public:
    TokenPlacer(const Matrix & matrix, const ConfigurationFactory & configurationFactory, const int maxTokens, const int pricePerToken);
    TokenPlacer(const TokenPlacer & orig);
    virtual ~TokenPlacer();
    Configuration findBestConfiguration(const Configuration & start, const Configuration & end);
    double countPrice(const Configuration & configuration) const;
private:
    static const int UNDEFINED_PRICE = -1;
    double bestPrice;
    int maxTokens;
    int pricePerToken;
    Matrix matrix;
    ConfigurationFactory configurationFactory;
    Configuration * bestConfiguration;
    std::vector<Configuration> stack;
    
    void generateEachCombination(const Configuration & start, const Configuration & end);
    void compareAndSaveSolution(double price, Configuration & currentConfiguration);
};

#endif	/* TOKENPLACER_H */

