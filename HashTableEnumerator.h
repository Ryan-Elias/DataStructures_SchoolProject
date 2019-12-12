#ifndef HASH_TABLE_ENUMERATOR
#define HASH_TABLE_ENUMERATOR

#include "Enumerator.h"
#include "Exceptions.h"
#include "HashTable.h"

template <typename T>
class HashTableEnumerator : public Enumerator<T>
{
private:
	unsigned long bucket = 0;
	OULinkedListEnumerator<T>* chainEnumerator = NULL;
	HashTable<T>* hashTable;
public:
	HashTableEnumerator(HashTable<T>* hashTable);
	virtual ~HashTableEnumerator();
	bool hasNext() const;
	T next();              // throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;        // throws ExceptionEnumerationBeyondEnd if no next item is available
};

// Add your implementation below this line.


template<typename T>
inline HashTableEnumerator<T>::HashTableEnumerator(HashTable<T>* hashTable)
{
	this->hashTable = hashTable;

	if (hashTable->getSize() == 0) {
		throw new ExceptionHashTableAccess();
	}

	OULinkedListEnumerator<T> tempEnum = hashTable->table[bucket]->enumerator();

	chainEnumerator = new OULinkedListEnumerator<T>(tempEnum);
}

template<typename T>
inline HashTableEnumerator<T>::~HashTableEnumerator()
{
}

template<typename T>
inline bool HashTableEnumerator<T>::hasNext() const
{
	if (hashTable->getSize() == 0) {
		throw ExceptionHashTableAccess();
	}


	if (chainEnumerator->hasNext()) {
		return true;
	}
	

	for (unsigned long i = bucket + 1; i < hashTable->getBaseCapacity(); ++i) {

		OULinkedListEnumerator<T> tempEnum = hashTable->table[i]->enumerator();

		if (tempEnum.hasNext()) {
			return true;
		}
	}


	return false;

}

template<typename T>
inline T HashTableEnumerator<T>::next()
{
	if (hashTable->getSize() == 0) {
		throw ExceptionHashTableAccess();
	}


	if (chainEnumerator->hasNext()) {
		return chainEnumerator->next();
	}
	else {
		bucket++;
		while (bucket < hashTable->getBaseCapacity()) {
			chainEnumerator = new OULinkedListEnumerator<T>(hashTable->table[bucket]->enumerator());

			if (chainEnumerator->hasNext()) {
				return chainEnumerator->next();
			}
			else {
				bucket++;
			}
		}
	}

	//if this point is reached, thorws exception.

	throw ExceptionEnumerationBeyondEnd();


}

template<typename T>
inline T HashTableEnumerator<T>::peek() const
{
	if (hashTable->getSize() == 0) {
		throw ExceptionHashTableAccess();
	}


	if (chainEnumerator->hasNext()) {
		return chainEnumerator->peek();
	}
	else {
		for (unsigned long i = bucket + 1; i < hashTable->getBaseCapacity(); ++i) {

			OULinkedListEnumerator<T> tempEnum = hashTable->table[i]->enumerator();

			if (tempEnum.hasNext()) {
				return tempEnum.peek();
			}



		}
	}


	throw ExceptionEnumerationBeyondEnd();
}


#endif // !HASH_TABLE_ENUMERATOR