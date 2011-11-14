/* 
 * File:   ConfigurationFactory.h
 * Author: honza
 *
 * Created on November 13, 2011, 2:44 PM
 */

#ifndef CONFIGURATIONFACTORY_H
#define	CONFIGURATIONFACTORY_H

#include "Configuration.h"

class ConfigurationFactory {
public:
    ConfigurationFactory(const int & matrixWidth, const int & matrixHeight);
    ConfigurationFactory(const ConfigurationFactory& orig);
    virtual ~ConfigurationFactory();
    Configuration createFirstConfiguration(const int & numberOfTokens) const;
    Configuration createLastConfiguration(const int & numberOfTokens) const;
    Configuration getNextConfiguration(const Configuration & configuration) const;
    bool isLast(const Configuration & configuration) const;
    Configuration createFromIndexes(int * indexes, const int size) const;

    int getMatrixWidth() const;
    int getMatrixHeight() const;
private:
    int matrixWidth;
    int matrixHeight;

    int getLastMatrixPosition() const;
};


#endif	/* CONFIGURATIONFACTORY_H */

