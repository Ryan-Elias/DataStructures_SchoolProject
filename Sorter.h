// Signature must be retained. Implementation must be added.

#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"

template <typename T>
class Sorter {
private:
	// additional member functions (methods) and variables (fields) may be added

	inline static void Quicksort(ResizableArray<T>& array, long low, long high, const Comparator<T>& comparator);
	inline static long Partition(ResizableArray<T>& array, const Comparator<T>& comparator, long low, long high);
	inline static void Swap(long indexOne, long indexTwo, ResizableArray<T>& array);

public:
	static void sort(ResizableArray<T>& array, const Comparator<T>& comparator);
};


//code below done with the assistance of fellow student Christian Hackelman.

template<typename T>
void Sorter<T>::Swap(long indexOne, long indexTwo, ResizableArray<T>& array) {
	T temp = array[indexOne];
	array.replaceAt(array[indexTwo], indexOne);
	array.replaceAt(temp, indexTwo);
}

template<typename T>
void Sorter<T>::Quicksort(ResizableArray<T>& array, long low, long high, const Comparator<T>& comparator) {
	long index;
	if (low < high) {
		index = Partition(array, comparator, low, high);

		Quicksort(array, low, index - 1, comparator);
		Quicksort(array, index + 1, high, comparator);
	}
}

template<typename T>
long Sorter<T>::Partition(ResizableArray<T>& array, const Comparator<T>& comparator, long low, long high) {
	long index = low;
	long pivot = high;

	for (long i = index; i < high; ++i) {
		if (comparator.compare(array[i], array[pivot]) < 0) {
			Swap(i, index, array);
			index++;
		}
	}
	Swap(pivot, index, array);
	return index;
}


template<typename T>
void Sorter<T>::sort(ResizableArray<T>& array, const Comparator<T>& comparator) {

	//calls quicksort function, which uses recursion. 
	Sorter<T>::Quicksort(array, 0, array.getSize() - 1, comparator);
}
#endif
