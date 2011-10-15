/* 
 * File:   Stack.h
 * Author: honza
 *
 * Created on October 15, 2011, 4:44 PM
 */

#include <vector>

#ifndef STACK_H
#define	STACK_H

template <class ItemType> class Stack {
    public:
        Stack();
        void push(const ItemType item);
        ItemType pop() const;
        ~Stack();
    private:
        std::vector<ItemType> items;
};

#endif	/* STACK_H */

