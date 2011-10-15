/* 
 * File:   Configuration.h
 * Author: honza
 *
 * Created on October 15, 2011, 7:24 PM
 */

#include "Coordinate.h"

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

class Configuration {
    public:
        Configuration(Coordinate * coordinates, const int length);
        Configuration(const Configuration& orig);
        virtual ~Configuration();
        int countPrice() const;
    private:
};

#endif	/* CONFIGURATION_H */
