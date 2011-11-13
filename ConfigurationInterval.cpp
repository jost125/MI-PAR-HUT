/* 
 * File:   ConfigurationInterval.cpp
 * Author: honza
 * 
 * Created on November 13, 2011, 2:26 PM
 */

#include "ConfigurationInterval.h"

ConfigurationInterval::ConfigurationInterval(const Configuration & start, const Configuration & end) : start(start), end(end) {
	this->start = start;
	this->end = end;
}

ConfigurationInterval::ConfigurationInterval(const ConfigurationInterval& orig) : start(orig.start), end(orig.end) {
	this->start = orig.start;
	this->end = orig.end;
}

ConfigurationInterval::~ConfigurationInterval() {
}
