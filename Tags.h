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
private:

};

#endif	/* TAGS_H */

