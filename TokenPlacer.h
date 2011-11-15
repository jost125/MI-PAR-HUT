/* 
 * File:   TokenPlacer.h
 * Author: honza
 *
 * Created on October 15, 2011, 7:33 PM
 */

#include "Matrix.h"
#include "Configuration.h"
#include "ConfigurationFactory.h"
#include "ConfigurationInterval.h"
#include <vector>

#ifndef TOKENPLACER_H
#define	TOKENPLACER_H

class TokenPlacer {
public:
    TokenPlacer(Matrix * matrix, const ConfigurationFactory & configurationFactory, const int maxTokens, const int pricePerToken);
    TokenPlacer(const TokenPlacer & orig);
    virtual ~TokenPlacer();
    void findBestConfiguration(ConfigurationInterval * interval);
    Configuration getBestConfiguration() const;
    double getBestPrice() const;
    double countPrice(const Configuration & configuration) const;
private:
    static const int UNDEFINED_PRICE = -1;
    double bestPrice;
    int maxTokens;
    int pricePerToken;
    Matrix * matrix;
    ConfigurationFactory factory;
    Configuration * bestConfiguration;
    std::vector<Configuration> stack;
    
    void compareAndSaveSolution(double price, Configuration & currentConfiguration);
};

#endif	/* TOKENPLACER_H */

