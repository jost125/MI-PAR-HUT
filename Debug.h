/* 
 * File:   Debug.h
 * Author: honza
 *
 * Created on November 12, 2011, 4:33 PM
 */

#include <iostream>

#ifndef DEBUG_H
#define	DEBUG_H

#ifdef DEBUG
#define TRACE(arg) (std::cout << arg)
#define TRACELN(arg) (std::cout << arg << std::endl)
#else
#define TRACE(arg)
#define TRACELN(arg)
#endif

#endif	/* DEBUG_H */

