#ifndef OU_LINKED_LIST_ENUMERATOR
#define OU_LINKED_LIST_ENUMERATOR

#include "Enumerator.h"
#include "OULink.h"
#include "Exceptions.h"

template <typename T>
class OULinkedListEnumerator : public Enumerator<T>
{
private:
	OULink<T>* current;
public:
	OULinkedListEnumerator(OULink<T>* first);
	bool hasNext() const;
	T next();			// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;		// throws ExceptionEnumerationBeyondEnd if no next item is available
};

// Add your implementation below this line. Do not add or modify anything above this line.


template<typename T>
OULinkedListEnumerator<T>::OULinkedListEnumerator(OULink<T>* first) {
	current = first;
}

template<typename T>
 bool OULinkedListEnumerator<T>::hasNext() const
{
	if (current == NULL) {
		return false;
	}
	else {
		return true;
	}
}

template<typename T>
 T OULinkedListEnumerator<T>::next()
{
	if (current == NULL) {
		throw new ExceptionEnumerationBeyondEnd();
	}

	else {
		T info = current->data;
		current = current->next;
		return info;
	}
}

template<typename T>
 T OULinkedListEnumerator<T>::peek() const
{
	if (current == NULL) {
		throw new ExceptionEnumerationBeyondEnd();
	}

	else {
		return current->data;
	}

}


#endif // !OU_LINKED_LIST_ENUMERATOR





