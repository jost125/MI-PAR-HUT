/* 
 * File:   ConfigurationInterval.h
 * Author: honza
 *
 * Created on November 13, 2011, 2:26 PM
 */

#include "Configuration.h"
#include "ConfigurationFactory.h"

#ifndef CONFIGURATIONINTERVAL_H
#define	CONFIGURATIONINTERVAL_H

class ConfigurationInterval {
public:
    ConfigurationInterval(const Configuration & start, const Configuration & end);
    ConfigurationInterval(const ConfigurationInterval& orig);
    virtual ~ConfigurationInterval();
    ConfigurationInterval split(const ConfigurationFactory & factory);
    bool isSplitable(const ConfigurationFactory & factory);

    Configuration getStart() const;
    Configuration getEnd() const;
private:
    Configuration start;
    Configuration end;

    Configuration findCenter(const ConfigurationFactory & factory) const;
};

std::ostream & operator << (std::ostream &os, const ConfigurationInterval & interval);

#endif	/* CONFIGURATIONINTERVAL_H */

