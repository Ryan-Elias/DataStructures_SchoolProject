#ifndef HASH_TABLE
#define HASH_TABLE

#include <math.h>
#include "Exceptions.h"
#include "Comparator.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "Hasher.h"

const unsigned int SCHEDULE_SIZE = 25;
const unsigned int SCHEDULE[SCHEDULE_SIZE] = { 1, 2, 5, 11, 23, 53, 107, 223, 449, 907, 1823, 3659, 7309, 14621, 29243, 58511, 117023, 234067, 468157, 936319, 1872667, 3745283, 7490573, 14981147, 29962343 };
const unsigned int DEFAULT_SCHEDULE_INDEX = 3;
const unsigned long DEFAULT_BASE_CAPACITY = SCHEDULE[DEFAULT_SCHEDULE_INDEX];
const float DEFAULT_MAX_LOAD_FACTOR = 0.8f;
const float DEFAULT_MIN_LOAD_FACTOR = 0.3f;

template <typename T>
class HashTable {
	template <typename U>
	friend class HashTableEnumerator;
private:
	Comparator<T>* comparator;                                // used to determine item equality
	Hasher<T>* hasher;                                        // used to compute hash value
	unsigned long size = 0;                                    // actual number of items currently in hash table
	float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR;
	float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR;
	unsigned int scheduleIndex = DEFAULT_SCHEDULE_INDEX;
	unsigned long baseCapacity = DEFAULT_BASE_CAPACITY;        // the size of the array
	unsigned long totalCapacity = baseCapacity;                // the size of the array plus chains of more than one link
	OULinkedList<T>** table = NULL; // table will be an array of pointers to OULinkedLists of type T
	// you may add additional member variables and functions here to support the operation of your code


	//added functions
	void TableResize(unsigned int scheduleIndex);

	//added members
	unsigned long overflowChains = 0;


public:
	HashTable(Comparator<T>* comparator, Hasher<T>* hasher);            // creates an empty table of DEFAULT_BASE_CAPACITY
	HashTable(Comparator<T>* comparator, Hasher<T>* hasher,
		// if size given, creates empty table with size from schedule of sufficient capacity (considering maxLoadFactor)
		unsigned long size,
		float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR,
		float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR);
	virtual ~HashTable();

	// if an equivalent item is not already present, insert item at proper location and return true
	// if an equivalent item is already present, leave table unchanged and return false
	bool insert(T item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave table unchanged and return false
	bool replace(T item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave table unchanged and return false
	bool remove(T item);

	// if an equivalent item is present, return a copy of the item
	// if an equivalent item is not present, throw a new ExceptionHashTableAccess
	T find(T item) const;

	unsigned long getSize() const;                            // returns the current number of items in the table
	unsigned long getBaseCapacity() const;                    // returns the current base capacity of the table
	unsigned long getTotalCapacity() const;                    // returns the current total capacity of the table
	float getLoadFactor() const;                            // returns the current load factor of the table
	unsigned long getBucketNumber(T item) const;            // returns the current bucket number for an item
};

// Add your implementation below this line.

#endif // !HASH_TABLE

template<typename T>
inline void HashTable<T>::TableResize(unsigned int scheduleIndex)
{
	overflowChains = 0;
	//create new list
	OULinkedList<T>** newTable = new OULinkedList<T> * [SCHEDULE[scheduleIndex]];

	unsigned long newCapacity = SCHEDULE[scheduleIndex];

	//initialize the new table with new LinkedLists
	for (unsigned long i = 0; i < newCapacity; ++i) {
		newTable[i] = new OULinkedList<T>(comparator);
	}

	//insert all items in the first list into the second list
	for (unsigned long i = 0; i < baseCapacity; ++i) {
		if (table[i]->getSize() == 0) {
			continue; // if the table in the old list is empty, then nothing needs to be done.
		}
		else {
			OULinkedListEnumerator<T> tempEnum = table[i]->enumerator();
			while (tempEnum.hasNext()) {

				unsigned long newTableIndex = hasher->hash(tempEnum.peek());
				if (newTableIndex > newCapacity) {
					newTableIndex = newTableIndex % newCapacity;
				}

				newTable[newTableIndex]->insert(tempEnum.next());
				if (newTable[newTableIndex]->getSize() > 1) {
					overflowChains++;
				}
			}
		}

		delete table[i];
	}

	delete table;

	table = newTable;

	baseCapacity = newCapacity;
	totalCapacity = baseCapacity + overflowChains;
}

//table will be an array of pointers to OULinkedLists of type T
template<typename T>
inline HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher)
{
	this->hasher = hasher;
	this->comparator = comparator;

	table = new OULinkedList<T>*[baseCapacity];
	for (unsigned long i = 0; i < baseCapacity; i++) {
		table[i] = new OULinkedList<T>(comparator);
	}
}

template<typename T>
inline HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher, unsigned long size, float maxLoadFactor, float minLoadFactor)
{
	this->hasher = hasher;
	this->comparator = comparator;

	this->size = size;
	this->maxLoadFactor = maxLoadFactor;
	this->minLoadFactor = minLoadFactor;

	//will lower until the load factor is within the range
	while (this->getLoadFactor() > maxLoadFactor) {
		scheduleIndex++;
		baseCapacity = SCHEDULE[scheduleIndex];
		totalCapacity = baseCapacity;
	}
	//will raise until the load factor is within the range
	while (this->getLoadFactor() < minLoadFactor) {
		scheduleIndex--;
		baseCapacity = SCHEDULE[scheduleIndex];
		totalCapacity = baseCapacity;
	}

	table = new OULinkedList<T>*[baseCapacity];
	for (unsigned long i = 0; i < baseCapacity; i++) {
		table[i] = new OULinkedList<T>(comparator);
	}

	this->size = 0;
}


template<typename T>
inline HashTable<T>::~HashTable()
{

	delete table;

}

// if an equivalent item is not already present, insert item at proper location and return true
// if an equivalent item is already present, leave table unchanged and return false
template<typename T>
inline bool HashTable<T>::insert(T item)
{
	//cases
	//1. The list at that item key's index location is empty
	//2. Inserting the item into that list causes a collision
	//3. The item is already present.
	//4. The index is greater than number of lists in array

	//effects
	//1. increment size
	//2. number of overflow chains
	//3. test load factor against maxloadfactor, resize as neccessary


	unsigned long tableIndex = hasher->hash(item); //get the index of the array that the item needs to be inserted.


	//4. 

	tableIndex = tableIndex % baseCapacity;


	//1.
	if (table[tableIndex]->getSize() == 0) {
		table[tableIndex]->insert(item);
		++size; //1

		if (this->getLoadFactor() >= this->maxLoadFactor) {
			this->scheduleIndex++;
			TableResize(scheduleIndex); // 3
		}

		return true;
	}

	//2 & 3

	//if insert returns true
	if (table[tableIndex]->insert(item)) {
		size++; //1
		//only if the size is 2 (meaning the first collision that causes that chain to become an overflow chain)
		if (table[tableIndex]->getSize() > 1) { //2
			totalCapacity++;
		}
		if (getLoadFactor() > this->maxLoadFactor) {
			this->scheduleIndex++;
			TableResize(scheduleIndex);
		}

		return true;
	}
	//if insert is false (item is already present)
	else {
		return false;
	}
}



// if an equivalent item is already present, replace item and return true
// if an equivalent item is not already present, leave table unchanged and return false
template<typename T>
inline bool HashTable<T>::replace(T item)
{

	//for each chain in table
	for (unsigned long i = 0; i < baseCapacity; ++i) {
		//if the chain is empty
		if (table[i]->getSize() == 0) {
			continue;
		}
		//either the item will be replaced, and true will be returned, or the item will not be found and false will be returned.
		if (table[i]->replace(item)) {
			return true;
		}
	}
	return false;
}


// if an equivalent item is already present, remove item and return true
// if an equivalent item is not already present, leave table unchanged and return false
template<typename T>
inline bool HashTable<T>::remove(T item)
{

	//for each chain in table
	for (unsigned long i = 0; i < baseCapacity; ++i) {
		//if the chain is empty
		if (table[i]->getSize() == 0) {
			continue;
		}
		//either the item will be removed, and true will be returned, or the item will not be found and false will be returned.
		if (table[i]->remove(item)) {
			
			//if the size of the linked list after the remove is 1, then it used to be an overflow chain and is no longer an overflow chain.
			--size;
			if (table[i]->getSize() == 1) {
				overflowChains--;
			}
			//test to see if remove function will trigger a resize
			if (getLoadFactor() < this->minLoadFactor) {
				this->scheduleIndex--;
				TableResize(scheduleIndex);
			}

			return true;
		}
	}
	return false;
}



// if an equivalent item is present, return a copy of the item
// if an equivalent item is not present, throw a new ExceptionHashTableAccess
template<typename T>
inline T HashTable<T>::find(T item) const
{

	//search through the table
	for (unsigned long i = 0; i < this->baseCapacity; ++i) {
		if (table[i]->getSize() == 0) {
			continue;
		}

		OULinkedListEnumerator<T> tempEnum = table[i]->enumerator();

		//because the enumerator will return false while the next item is null as well as the current item.
		while (tempEnum.hasNext()) {
			if (comparator->compare(tempEnum.peek(), item) == 0) {
				return tempEnum.next();
			}
			tempEnum.next();
		}
	}


	//if this point is reached, the item has not been found and will throw an exception.

	throw new ExceptionHashTableAccess();

}

template<typename T>
inline unsigned long HashTable<T>::getSize() const
{
	return size;
}

template<typename T>
inline unsigned long HashTable<T>::getBaseCapacity() const
{
	return this->baseCapacity;
}

template<typename T>
inline unsigned long HashTable<T>::getTotalCapacity() const
{
	return this->totalCapacity;
}

template<typename T>
inline float HashTable<T>::getLoadFactor() const
{

	float fSize = (float)this->size; // done to eliminate integer (or long) division.
	float fCap = (float)this->totalCapacity;
	float LF = fSize / fCap;
	return LF;
}

template<typename T>
inline unsigned long HashTable<T>::getBucketNumber(T item) const
{
	//if the item's time stamps ASCII value is less than the size, then this should return that value mod table size (base capacity)

	unsigned long bucketNum = hasher->hash(item);

	if (bucketNum > this->baseCapacity) {
		bucketNum = bucketNum % baseCapacity;
	}

	return bucketNum;

}
