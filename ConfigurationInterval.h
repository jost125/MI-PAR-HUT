/* 
 * File:   ConfigurationInterval.h
 * Author: honza
 *
 * Created on November 13, 2011, 2:26 PM
 */

#include "Configuration.h"

#ifndef CONFIGURATIONINTERVAL_H
#define	CONFIGURATIONINTERVAL_H

class ConfigurationInterval {
public:
    ConfigurationInterval(const Configuration & start, const Configuration & end);
    ConfigurationInterval(const ConfigurationInterval& orig);
    virtual ~ConfigurationInterval();
private:
    Configuration start;
    Configuration end;
};

#endif	/* CONFIGURATIONINTERVAL_H */

