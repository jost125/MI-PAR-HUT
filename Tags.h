/* 
 * File:   Tags.h
 * Author: honza
 *
 * Created on November 15, 2011, 12:01 AM
 */

#ifndef TAGS_H
#define	TAGS_H

class Tags {
public:
    Tags();
    Tags(const Tags& orig);
    virtual ~Tags();
    static short const INPUTS = 0;
    static short const MATRIX_VALUES = 1;
    static short const BUFFER_SIZE = 2;
    static short const CONFIGURATION = 3;
private:

};

#endif	/* TAGS_H */

