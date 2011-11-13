/* 
 * File:   InputReader.h
 * Author: honza
 *
 * Created on November 13, 2011, 12:39 PM
 */

#ifndef INPUTREADER_H
#define	INPUTREADER_H

class InputReader {
public:
    InputReader();
    InputReader(const InputReader& orig);
    virtual ~InputReader();
    int readHeight();
    int readWidth();
    int readMaxTokens();
    int readPricePerToken();
private:
    int width;
    int height;
};

#endif	/* INPUTREADER_H */

