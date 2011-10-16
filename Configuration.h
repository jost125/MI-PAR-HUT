/* 
 * File:   Configuration.h
 * Author: honza
 *
 * Created on October 15, 2011, 7:24 PM
 */

#include <vector>
#include "Coordinate.h"

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

class Configuration {
    public:
        Configuration(std::vector<Coordinate> coordinates);
        Configuration(const Configuration & orig);
        virtual ~Configuration();
    private:
        std::vector<Coordinate> coordinates;
};

#endif	/* CONFIGURATION_H */
