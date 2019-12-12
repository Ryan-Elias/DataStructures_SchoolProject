#ifndef OU_LINKED_LIST
#define OU_LINKED_LIST

#include "OULink.h"
#include "Comparator.h"
#include "OULinkedListEnumerator.h"

// OULinkedList stands for Ordered, Unique Linked List. It is a linked list that is always maintained in
// order (based on the comparator provided to it when the list is created) and that only contains unique
// items (that is, duplicates are not allowed)
template <typename T>
class OULinkedList {
	//template <typename T>
	//friend class OULinkedListEnumerator;
private:
	Comparator<T>* comparator = NULL;               // used to determine list order and item equality
	unsigned long size = 0;                         // actual number of items currently in list
	OULink<T>* first = NULL;                        // pointer to first link in list
	OULink<T>* last = NULL;                         // pointer to last link in list
public:
	OULinkedList(Comparator<T>* comparator);        // creates empty linked list with comparator
	virtual ~OULinkedList();                        // deletes all links and their data items

	// if an equivalent item is not already present, insert item in order and return true
	// if an equivalent item is already present, leave list unchanged and return false
	bool insert(T item);

	// if item is greater than item at last, append item at end and return true
	// if item is less than or equal to item at last, leave list unchanged and return false
	bool append(T item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave list unchanged and return false
	bool replace(T item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave list unchanged and return false
	bool remove(T item);

	// if any items are present, return a copy of the first item
	// if no items are present, throw new ExceptionLinkedListAccess
	T getFirst() const;

	// if any items are present, return a copy of the first item, remove it from list
	// if no items are present, throw new ExceptionLinkedListAccess
	T pullFirst();

	// if any items are present, remove the first item and return true
	// if no items are present, leave list unchanged and return false
	bool removeFirst();

	// if an equivalent item is present, return true
	// if an equivalent item is not present, false
	bool contains(T item) const;

	// if an equivalent item is present, return a copy of that item
	// if an equivalent item is not present, throw a new ExceptionLinkedListAccess
	T find(T item) const;

	void clear();                                     // deletes all links in the list, resets size to 0

	unsigned long getSize() const;                    // returns the current number of items in the list

	OULinkedListEnumerator<T> enumerator() const;     // create an enumerator for this linked list
};

// Add your implementation below this line. Do not add or modify anything above this line.


#endif // !OU_LINKED_LIST

template<typename T>
OULinkedList<T>::OULinkedList(Comparator<T>* comparator) {
	this->comparator = comparator;
}

template<typename T>
OULinkedList<T>::~OULinkedList() {
	if (size > 0) {
		delete first;
	}
}

template<typename T>
bool OULinkedList<T>::insert(T item) {
	OULink<T>* curLink = this->first;
	OULink<T>* nextLink;
	OULink<T>* lastLink = this->last;

	//if the first item is empty (the list is empty) set first to curLink, then set last = first.
	if (curLink == NULL) {
		this->first = new OULink<T>(item);
		this->last = first;
		++size;
		return true;
	}

	//compare to first item.
	// if first item and the inserted item are equal, set local variables to null and then return false.
	if (comparator->compare(curLink->data, item) == 0) {
		curLink = NULL;
		lastLink = NULL;
		return false;
	}
	//if first item is greater than inserted item, then the new item becomes the first item.
	if (comparator->compare(curLink->data, item) > 0) {
		this->first = new OULink<T>(item);
		first->next = curLink;
		curLink = NULL;
		lastLink = NULL;
		++size;
		return true;
	}

	//compare to last item
	//if the last item in the list is less than the item to be inserted, then everything can be set to null and the new item can be appended.
	if (comparator->compare(lastLink->data, item) < 0) {
		lastLink = new OULink<T>(item);
		this->last->next = lastLink;
		this->last = lastLink;
		lastLink = NULL;
		curLink = NULL;
		nextLink = NULL;
		++size;
		return true;
	}
	//if last item and current item are equal, return false.
	if (comparator->compare(lastLink->data, item) == 0) {
		curLink = NULL;
		lastLink = NULL;
		return false;
	}

	//if the item is greater than the first item and less than the last item, then it must be somewhere inside of the list.

	while (curLink->next != NULL) {
		nextLink = curLink->next;
		if (comparator->compare(nextLink->data, item) == 0) { //if at any point, a link's data is equal to the item, return false, since we already checked current, we can check next
			curLink = NULL;
			nextLink = NULL;
			lastLink = NULL;
			return false;
		}

		//now that border cases have been handled, we need to find where curLink.data is less than item and nextLink.data is greater than item.
		if (comparator->compare(curLink->data, item) < 0 && comparator->compare(nextLink->data, item) > 0) {

			curLink->next = new OULink<T>(item);
			curLink->next->next = nextLink;

			curLink = NULL;
			nextLink = NULL;
			lastLink = NULL;
			size++;
			return true;
		}
		curLink = nextLink;
	}

	std::cout << "Insert reached end of method, needs to be reevaluated." << std::endl;

	curLink = NULL;
	nextLink = NULL;
	lastLink = NULL;

	return false;
}

template<typename T>
bool OULinkedList<T>::append(T item) {
	OULink<T>* lastLink = new OULink<T>(item);

	if (this->last == NULL) {
		this->last = lastLink;
		this->first = lastLink;
		lastLink = NULL;

		++size;
		return true;

	}

	if (comparator->compare(last->data, item) >= 0) {
		return false;
	}
	this->last->next = lastLink;
	this->last = lastLink;
	lastLink = NULL;

	++size;
	return true;



}

template<typename T>
bool OULinkedList<T>::replace(T item) {


	OULink<T>* temp = first;
	while (temp != NULL) {
		if (comparator->compare(temp->data, item) == 0) {
			temp->data = item;
			return true;
		}
		temp = temp->next;
	}

	return false;

	//test last (if neccessary)
}

template<typename T>
bool OULinkedList<T>::remove(T item) {
	//will work like replace, except instead of replacing that thing, will delete the item at temp.next

	OULink<T>* curLink = first;
	OULink<T>* nextLink;


	//test first
	//if the first item is equal to the removed item, delete and return true.
	if (comparator->compare(curLink->data, item) == 0) {
		this->first = first->next;
		curLink->next = NULL;
		--size;
		return true; // because curLink is local, it should call the destructor for the matched item when it goes out of scope.
	}
	//iterate the rest of the list

	while (curLink->next != NULL) {
		nextLink = curLink->next;

		if (comparator->compare(nextLink->data, item) == 0) {
			curLink->next = nextLink->next;
			nextLink->next = NULL;
			curLink = NULL;
			--size;
			return true;
		}
		curLink = nextLink;
	}

	curLink = NULL;
	nextLink = NULL;
	//if this point is reached, then the item is not there.
	return false;

}

template<typename T>
T OULinkedList<T>::find(T item) const {

	if (first == NULL) {
		throw new ExceptionLinkedListAccess();
	}

	OULinkedListEnumerator<T> e = enumerator();
	//test the rest of list
	while (e.hasNext()) {
		if (comparator->compare(e.peek(), item) == 0) { //should check the item and iterate enumerator.
			return e.peek();
		}
		e.next();
	}

	//test last

	if (comparator->compare(last->data, item) == 0) {
		return last->data;
	}

	//if this point is reached, then an exception should be thrown.
	throw new ExceptionLinkedListAccess();


}

template<typename T>
void OULinkedList<T>::clear() {
	if (this->first != NULL) {
		delete first; //should create a chain that deletes every link
	}
	size = 0;
}

template<typename T>
unsigned long OULinkedList<T>::getSize() const
{
	return this->size;
}

template<typename T>
 OULinkedListEnumerator<T> OULinkedList<T>::enumerator() const {
	return OULinkedListEnumerator<T>(this->first);
}

template<typename T>
 T OULinkedList<T>::getFirst() const
{
	if (size < 1) {
		throw new ExceptionLinkedListAccess();
	}

	return first->data;
}

template<typename T>
 T OULinkedList<T>::pullFirst()
{
	if (size < 1) {
		throw new ExceptionLinkedListAccess();
	}

	OULink<T>* temp = first;
	if (first->next != NULL) {
		first = first->next;
	}
	--size;
	return temp->data;

}

template<typename T>
 bool OULinkedList<T>::removeFirst()
{
	if (size < 1) {
		return false;
	}

	OULink<T>* temp = first->next;
	first->next = NULL;
	delete first;
	first = temp;
	--size;
	return true;

}

template<typename T>
bool OULinkedList<T>::contains(T item) const {

	OULinkedListEnumerator<T> e = enumerator();
	while (e.hasNext()) {
		if (comparator->compare(e.next(), item) == 0) {
			return true;
		}
	}
	//test last item
	if (this->last != NULL) {
		if (comparator->compare(this->last->data, item) == 0) {
			return true;
		}
	}
	//if this point is reached, the item is not in the list
	return false;
}
