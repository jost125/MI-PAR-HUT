/* 
 * File:   Queue.h
 * Author: honza
 *
 * Created on October 4, 2011, 12:10 PM
 */

#include "Arrangement.h"

#ifndef QUEUE_H
#define	QUEUE_H

class Queue {
    public:
        void push(Arrangement arrangement);
        Arrangement pop();
        Arrangement head() const;
};

#endif	/* QUEUE_H */
