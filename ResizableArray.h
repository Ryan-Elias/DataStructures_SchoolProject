#ifndef RESIZABLE_ARRAY_H
#define RESIZABLE_ARRAY_H

#include "Exceptions.h"

const unsigned long DEFAULT_ARRAY_CAPACITY = 10;        // capacity used in no arg constructor

template <typename T>
class ResizableArray {
private:
	unsigned long capacity = DEFAULT_ARRAY_CAPACITY;    // maximum capacity, in items
	unsigned long size = 0;                             // actual number of items currently in array
	T* data = NULL;                                     // pointer to array of any type
	void doubleCapacity();                              // method to double array capacity
	void halveCapacity();                               // method to halve array capacity
public:
	ResizableArray();                                   // constructs array with default capacity
	ResizableArray(unsigned long capacity);             // constructs array with specified capacity
	virtual ~ResizableArray();                          // frees array space as object is deleted
	void add(T item);                                   // adds item, increments size, doubles capacity as necessary
	void addAt(T item, unsigned long index);            // adds item at index, shifts following, doubles capacity as necessary
	void replaceAt(T item, unsigned long index);        // replaces item at index, otherwise unchanged
	void removeAt(unsigned long index);                 // removes item at index, shifts following back
	T get(unsigned long index) const;                   // returns (copy of) item at index
	unsigned long getSize() const;                      // returns number of items currently in array
	unsigned long getCapacity() const;                  // returns the current capacity of the array
	T operator[](unsigned long index) const;            // returns (copy of) item at index
	void clear();                                       // removes all items from array, resets to default capacity, set size to 0
};

// Add your implementation below this line. Do not add or modify anything above this line.


//Constructors & Destructor
template <typename T>
ResizableArray<T>::ResizableArray() {
	data = new T[DEFAULT_ARRAY_CAPACITY];
}

template <typename T>
ResizableArray<T>::ResizableArray(unsigned long capacity) {
	data = new T[capacity];
	this->capacity = capacity;
}

template <typename T>
ResizableArray<T>::~ResizableArray() {
	delete[] data;
}


//double, halving, and add
template <typename T>
void ResizableArray<T>::doubleCapacity() {
	T* temp = new T[capacity * 2];
	for (unsigned long i = 0; i < size; ++i) {
		temp[i] = data[i];
	}
	capacity = capacity * 2;
	delete[] data;
	data = temp;
}

template <typename T>
void ResizableArray<T>::halveCapacity() {
	T* temp = new T[capacity / 2];
	for (unsigned long i = 0; i < size; ++i) {
		temp[i] = data[i];
	}
	capacity = capacity / 2;
	delete[] data;
	data = temp;
}

template <typename T>
void ResizableArray<T>::add(T item) {
	if (size == capacity) {
		doubleCapacity();
	}
	data[size] = item;
	++size;
}


//Methods that throw IndexOutOfRange Exceptions
//addAt, replaceAt, removeAt, get, operator[]
template<typename T>
void ResizableArray<T>::addAt(T item, unsigned long index) {
	if (index > size) {
		throw new ExceptionIndexOutOfRange();
	}
	if (size == capacity) {
		doubleCapacity();
	}
	for (unsigned long i = size; i > index; --i) {
		data[i] = data[i - 1];
	}
	data[index] = item;
	++size;
}
template <typename T>
void ResizableArray<T>::replaceAt(T item, unsigned long index) {
	if (index >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	data[index] = item;
}

template <typename T>
void ResizableArray<T>::removeAt(unsigned long index) {
	if (index >= size) {
		throw new ExceptionIndexOutOfRange();
	}

	for (unsigned long i = index; i < size - 1; ++i) {
		data[i] = data[i + 1];
	}
	data[size - 1] = T(); // I might not need this, need to test
	size--;

	if (size == capacity / 2) {
		halveCapacity();
	}
}

template <typename T>
T ResizableArray<T>::get(unsigned long index) const {
	if (index >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	return data[index]; // threw an exception
}

template <typename T>
T ResizableArray<T>::operator[](unsigned long index) const {
	if (index >= size) {
		throw new ExceptionIndexOutOfRange();
	}
	return data[index];
}

//getSize, getCapacity, clear
template <typename T>
unsigned long ResizableArray<T>::getSize() const {
	return size;
}

template <typename T>
unsigned long ResizableArray<T>::getCapacity() const {
	return capacity;
}

template <typename T>
void ResizableArray<T>::clear() {
	size = 0;
	capacity = DEFAULT_ARRAY_CAPACITY;
	delete[] data;
}
#endif