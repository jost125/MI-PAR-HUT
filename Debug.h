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
#define COM_PRINT(arg) (std::cout << arg)
#define COM_PRINTLN(arg) (std::cout << arg << std::endl)
#else
#define COM_PRINT(arg)
#define COM_PRINTLN(arg)
#endif

#endif	/* DEBUG_H */

