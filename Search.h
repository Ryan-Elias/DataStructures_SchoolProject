// Signature must be retained. Implementation must be added.

#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "ResizableArray.h"
#include "Comparator.h"

	/* returns FIRST array location matching the given item (based on the comparator)
	if not found, returns -(location to insert + 1)
	*/


	//findFirst was implemented with help from geeksforgeeks.org
template <typename T>
long long findFirst(const ResizableArray<T>& array, long low, long high, const Comparator<T>& comparator, const T& item) {

	long midPoint = low + (high - low) / 2;
	if (high >= low) {
		if ((midPoint == 0 || comparator.compare(array[midPoint - 1], item) < 0) && (comparator.compare(array[midPoint], item) == 0)) {
			return midPoint;
		}
		else if (comparator.compare(array[midPoint], item) < 0) {
			return findFirst(array, midPoint + 1, high, comparator, item);
		}
		else {
			return findFirst(array, low, midPoint - 1, comparator, item);
		}
	}
	return 0 - (midPoint);
}

template <typename T>
long long binarySearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator) {

	return findFirst(array, 0, array.getSize() - 1, comparator, item);

}



#endif