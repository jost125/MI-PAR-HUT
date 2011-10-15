/* 
 * File:   Stack.cpp
 * Author: honza
 * 
 * Created on October 15, 2011, 4:44 PM
 */

#include "Stack.h"

template <class ItemType> Stack<ItemType>::Stack() {
}

template <class ItemType> ItemType Stack<ItemType>::pop() const {
	return items.back();
}

template <class ItemType> void Stack<ItemType>::push(const ItemType item) {
	items.push_back(item);
}

template <class ItemType> Stack<ItemType>::~Stack() {
}
