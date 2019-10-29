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


template<typename T>
OULinkedList<T>::OULinkedList(Comparator<T>* comparator)
{
	// Creates new LinkedList object and sets the comparator to order the list.
	this->comparator = comparator;
}

template<typename T>
OULinkedList<T>::~OULinkedList()
{

	delete first;
	first = NULL;
}

// if an equivalent item is not already present, insert item in order and return true
// if an equivalent item is already present, leave list unchanged and return false
template<typename T>
bool OULinkedList<T>::insert(T item)
{
	OULink<T>* previousElement = first;
	OULink<T>* currentElement = first;

	if (this->append(item)) return true;

	while (currentElement != NULL)
	{
		int resultOfCompare = comparator->compare(item, currentElement->data);
		if (resultOfCompare == 0)
		{
			return false;
		}
		else if (resultOfCompare == -1)
		{
			OULink<T>* linkToInsert = new OULink<T>(item);
			if (currentElement == first)
			{
				linkToInsert->next = first;
				first = linkToInsert;
			}
			else
			{
				previousElement->next = linkToInsert;
				linkToInsert->next = currentElement;
			}
			size++;
			return true;
		}
		else
		{
			previousElement = currentElement;
			currentElement = currentElement->next;
		}
	}
	return false;
}

// if item is greater than item at last, append item at end and return true
// if item is less than or equal to item at last, leave list unchanged and return false
template<typename T>
bool OULinkedList<T>::append(T item)
{
	if (last != NULL && comparator->compare(item, last->data) <= 0) return false;

	OULink<T>* linkToAppend = new OULink<T>(item);

	if (first == NULL)
	{
		first = linkToAppend;
	}
	else
	{
		last->next = linkToAppend;
	}

	last = linkToAppend;
	size++;
	return true;
}

// if an equivalent item is already present, replace item and return true
// if an equivalent item is not already present, leave list unchanged and return false
template<typename T>
bool OULinkedList<T>::replace(T item)
{
	// Check of the list is empty.
	if (first == NULL) return false;

	//If the list does not contain the item, return false.
	if (!this->contains(item)) return false;

	OULink<T>* current = first->next;

	while (current != NULL)
	{
		if (comparator->compare(item, current->data) == 0)
		{
			current->data = item;
			return true;
		}

		current = current->next;
	}
	return false;
}

// if an equivalent item is already present, remove item and return true
// if an equivalent item is not already present, leave list unchanged and return false
template<typename T>
bool OULinkedList<T>::remove(T item)
{
	OULink<T>* previousElement = first;
	OULink<T>* currentElement = first;

	// Check first element.
	if (first == NULL) return false;
	if (comparator->compare(first->data, item) == 0)
	{
		OULink<T>* tmp = first;
		first = first->next;
		tmp->next = NULL;
		delete tmp;

		size--;
		return true;
	}

	// Check elements after first
	currentElement = previousElement->next;
	while (currentElement != NULL)
	{
		if (comparator->compare(currentElement->data, item) == 0)
		{
			// Assign next to the matching link's next.
			previousElement->next = currentElement->next;

			// Set the next of the current element to NULL in order to delete properly.
			currentElement->next = NULL;
			delete currentElement;
			currentElement = NULL;
			return true;
		}
		else
		{
			previousElement = currentElement;
			currentElement = currentElement->next;
		}
	}
	return false;
}

// if any items are present, return a copy of the first item
// if no items are present, throw new ExceptionLinkedListAccess
template<typename T>
T OULinkedList<T>::getFirst() const
{
	if (first != NULL) return first->data;
	else throw new ExceptionLinkedListAccess();
}

// if any items are present, return a copy of the first item, remove it from list
// if no items are present, throw new ExceptionLinkedListAccess
template<typename T>
T OULinkedList<T>::pullFirst()
{
	// Check if element exists
	if (first != NULL)
	{
		// Keep track of next element
		OULink<T>* nextElement = first->next;
		T firstData = first->data;

		first = nextElement;

		return firstData;
	}
	else throw new ExceptionLinkedListAccess();
}

// if any items are present, remove the first item and return true
// if no items are present, leave list unchanged and return false
template<typename T>
bool OULinkedList<T>::removeFirst()
{
	if (first == NULL) return false;
	else
	{
		// Set pointer for next element after first.
		OULink<T>* nextElement = first->next;

		// Delete first.
		delete first;
		first = NULL;

		// Set first to the next Element.
		first = nextElement;
		return true;
	}
}

// if an equivalent item is present, return true
// if an equivalent item is not present, false
template<typename T>
bool OULinkedList<T>::contains(T item) const
{
	// Keep track element
	OULink<T>* currentElement = first;

	// Loop through all elements until found of end it reached
	while (currentElement != NULL)
	{
		if (comparator->compare(currentElement->data, item) == 0)
		{
			return true;
		}
		currentElement = currentElement->next;
	}
	return false;
}

// if an equivalent item is present, return a copy of that item
// if an equivalent item is not present, throw a new ExceptionLinkedListAccess
template<typename T>
T OULinkedList<T>::find(T item) const
{
	// Keep track element
	OULink<T>* currentElement = first;

	// Loop through all elements until found of end it reached
	while (currentElement != NULL)
	{
		if (comparator->compare(currentElement->data, item) == 0)
		{
			return currentElement->data;
		}
		currentElement = currentElement->next;
	}
	throw new ExceptionLinkedListAccess();
}

// Deletes all links in the list and resets size to 0
template<typename T>
void OULinkedList<T>::clear()
{
	// Deleting first element calls delete on all subsequent elements.

	delete first;
	first = NULL;

	// Reset size to 0.
	size = 0;
}

// returns the current number of items in the list
template<typename T>
unsigned long OULinkedList<T>::getSize() const
{
	return size;
}

// Creates and returns an enumerator for this linked list
template<typename T>
OULinkedListEnumerator<T> OULinkedList<T>::enumerator() const
{
	return OULinkedListEnumerator<T>(first);
}


#endif // !OU_LINKED_LIST
