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
        Configuration(std::vector<Coordinate> coordinates, int matrixWidth, int matrixHeight);
        Configuration(const Configuration & orig);
        virtual ~Configuration();
        bool contains(const Coordinate & coordinate) const;
        std::vector<Coordinate> getCoordinates() const;
    private:
        unsigned long long * stash;
        int matrixWidth;
        int matrixHeight;
        int stashSize;
        unsigned short numberOfBits;

        void insertCoordinatesToStash(std::vector<Coordinate> coordinates);
        void enlargeStash(const int newStashSize);
        int getSlot(const int index) const;
        void addIndexToStash(const int index, const int slot);
};

std::ostream & operator << (std::ostream & os, Configuration & configuration);

#endif	/* CONFIGURATION_H */
