/* 
 * File:   ConfigurationInterval.cpp
 * Author: honza
 * 
 * Created on November 13, 2011, 2:26 PM
 */

#include "ConfigurationInterval.h"
#include "ConfigurationFactory.h"
#include "Debug.h"
#include <math.h>

ConfigurationInterval::ConfigurationInterval(const Configuration & start, const Configuration & end) : start(start), end(end) {
	this->start = start;
	this->end = end;
}

ConfigurationInterval::ConfigurationInterval(const ConfigurationInterval& orig) : start(orig.start), end(orig.end) {
	this->start = orig.start;
	this->end = orig.end;
}

Configuration ConfigurationInterval::getStart() const {
	return this->start;
}

Configuration ConfigurationInterval::getEnd() const {
	return this->end;
}

ConfigurationInterval::~ConfigurationInterval() {
}

std::ostream & operator << (std::ostream &os, const ConfigurationInterval & interval) {
	os << '(' << interval.getStart() << '-' << interval.getEnd() << ')';
	return os;
}