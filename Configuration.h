/* 
 * File:   Configuration.h
 * Author: honza
 *
 * Created on October 15, 2011, 7:24 PM
 */

#include <vector>
#include <ostream>
#include "Coordinate.h"

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

class Configuration {
    public:
        Configuration(std::vector<Coordinate> coordinates);
        Configuration(const Configuration & orig);
        virtual ~Configuration();
        bool contains(const Coordinate & coordinate) const;
        std::vector<Coordinate> getCoordinates() const;
        bool equals(const Configuration & compared) const;
    private:
        std::vector<Coordinate> coordinates;
};

std::ostream & operator << (std::ostream & os, Configuration & configuration);

#endif	/* CONFIGURATION_H */
