/*
* This file is part of XTKLib project.
*
* Copyright(C) 2003-2006 Mario Casciaro xshadow[AT]email(DOT)it
*
* Licensed under:
*   - Apache License, Version 2.0 or
*   - GNU General Public License (GPL)
* You should have received at least one copy of them along with this program.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED.SEE YOUR CHOOSEN LICENSE FOR MORE DETAILS.
*/

/**
* @file tdatastructures.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#ifndef XTK_TDATASTRUCTURES_H
#define XTK_TDATASTRUCTURES_H

#include "../globals.h"
#include "array.h"
#include "exception.h"
#include "interfaces.h"
#include <math.h>

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/**
 * An template iterator over a collection.
 *
 * @warning Iterators in xtklib are not guaranteed to be coherent with the underling data
 * structure if concurrent modifications occurs none exceptions are risen. This control
 * is left to user.
 */
template <class TClass>
class XTKAPI xTIterator : public virtual xObject
{
public:
	virtual ~xTIterator()
	{}

	/**
	 * Returns true if the iteration has more elements.
	 */
	virtual bool hasNext() = 0;


	/**
	 * Returns the next element in the iteration.
	 * @exception xNoSuchElementException
	 */
	virtual TClass& next() throw(xNoSuchElementException) = 0;

	/**
	 * Removes from the underlying collection the last element returned
	 * by the iterator.
	 *
	 * @exception xIllegalStateException
	 */
	virtual void remove() throw(xIllegalStateException) = 0;
};
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/**
 * An interface for such objects that can return an iterator over a generic group of elements.
 */
template <class TClass>
class xTIterable : public virtual xObject
{
public:
	virtual ~xTIterable()
	{}

	virtual xTIterator<TClass>* iterator() = 0;
};

/**
 * Call delete for every element of the xTIterable collection.
 */
template<class TClass>
void xtkDeleteAll(xTIterable<TClass>& iterable)
{
	xTIterator<TClass>* iter = iterable.iterator();
	while(iter->hasNext())
		delete iter->next();
	delete iter;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/**
 * An template iterator for lists that allows the programmer to traverse the list in
 * either direction and modify the list during iteration.
 *
 * @warning Iterators in xtklib are not guaranteed to be coerent with the underling data
 * structure if concurrent modifications occurs none exceptions are risen. This control
 * is left to user.
 */
template <class TClass>
class XTKAPI xTListIterator : public xTIterator<TClass>
{
public:
	virtual ~xTListIterator()
	{}

	/**
	 * Returns true if this list iterator has more elements when traversing the
	 * list in the reverse direction
	 */
	virtual bool hasPrevious() = 0;

	/**
	 * Returns the previous element in the list.
	 *
	 * @exception xNoSuchElementException
	 */
	virtual TClass& previous() throw(xNoSuchElementException) = 0;

	/**
	 * Replaces the last element returned by next or previous with the
	 * specified element
	 *
	 * @exception xIllegalStateException
	 */
	virtual void set(TClass o) throw(xIllegalStateException) = 0;
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################


/**
 * Represents a template collection of elements.
 */
template <class TClass>
class XTKAPI xTCollection : public virtual xObject,public xTIterable<TClass>
{
public:
	virtual ~xTCollection()
	{}

	/**
	 * Add the specified element to this collection
	 */
	virtual void add(TClass o) = 0;

	/**
	 * Removes the specified element from the collection.
	 */
	virtual void removeElement(TClass el) = 0;

	/**
	 * Returns true if this collection does not contains any element
	 */
	virtual bool isEmpty() = 0;

	/**
	 * Removes all elements from the collection
	 */
	virtual void clear() = 0;

	/**
	 * Returns the number of elements in this collection
	 */
	virtual int size() = 0;

	/**
	 * Returns an array containing all of the elements in this collection.
	 */
	virtual xArray<TClass> toArray() = 0;

	/**
	 * Returns true if the specified element is already present in the collection.
	 */
	virtual bool contains(TClass o) = 0;
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/**
 * An ordered template collection (also known as a sequence).
 */
template <class TClass>
class XTKAPI xTList : public xTCollection<TClass>
{
public:
	virtual ~xTList()
	{}

	/**
	 * Returns the element at the specified position in this list.
	 *
	 * @exception xIndexOutOfBoundsException
	 */
	virtual TClass& get(int index) throw(xIndexOutOfBoundsException) = 0;

	/**
	 *  Replaces the element at the specified position in this list with
	 * the specified element.
	 *
	 * @exception xIndexOutOfBoundsException
	 */
	virtual TClass set(int index,TClass o) throw(xIndexOutOfBoundsException) = 0;

	/**
	 * Inserts the specified element at the specified position in this list.
	 *
	 * @exception xIndexOutOfBoundsException
	 */
	virtual void insert(int index,TClass o) throw(xIndexOutOfBoundsException)= 0;

	/**
	 *  Removes the element at the specified position in this list.
	 */
	virtual TClass remove(int index) throw(xIndexOutOfBoundsException) = 0;

	/**
	 * Returns a list iterator of the elements in this list.
	 */
	virtual xTListIterator<TClass>* listIterator() = 0;
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/**
 * A template object that maps keys to values. A map cannot contain duplicate keys;
 * each key can map to at most one value. Key objects are unconditional ownership
 * of the xMap object. For correct usage TClassKey and TClassValue must override operator==.
 */
template <class TClassKey,class TClassValue>
class XTKAPI xTMap : public virtual xObject,public xTIterable<TClassValue>
{
public:
	virtual ~xTMap()
	{}

	/**
	 * Removes all mappings from this map
	 */
	virtual void clear() = 0;

	/**
	 * Returns true if this map contains a mapping for the specified key.
	 *
	 * @exception xClassCastException
	 */
	virtual bool containsKey(TClassKey key) = 0;

    /**
     *  Returns true if this map maps one or more keys to the specified value.
     */
	virtual bool containsValue(TClassValue value) = 0;

	/**
	 * Returns the value to which this map maps the specified key.
	 * If such map does not exists false is returned.
	 */
	virtual bool get(TClassKey key,TClassValue** ret) = 0;

    /**
     * Returns true if this map contains no key-value mappings.
     */
	virtual bool isEmpty() = 0;

    /**
     * Associates the specified value with the specified key in this map.
	 * Return true if another mapping with the same key was found.
     */
	virtual bool put(TClassKey key, TClassValue value,TClassValue* removed = NULL) = 0;

	/**
	 * Removes the mapping for this key from this map if it is present
	 * Return true if the mapping is effectively found and removed.
	 */
	virtual bool remove(TClassKey key,TClassValue* removed = NULL) = 0;

	/**
     * Returns the number of key-value mappings in this map.
     */
	virtual int size() = 0;

	/**
	 * Returns an array containing all of the values in this map.
	 */
	virtual xArray<TClassValue> toArray() = 0;
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/**
 * Linked list template implementation of the xTList interface.
 * May provide better performance than the xArrayList implementation if elements
 * are frequently added,inserted or deleted within the list. It has also a
 * better memory management. It offers const time add and linear
 * get,insert and delete in the middle and const delete at end or begin of the list
 *
 * See single functions for details about costs.
 */
template <class TClass>
class XTKAPI xTLinkedList : public xTList<TClass>
{
private:

	struct Node : public virtual xObject
	{
		Node*			next;
		Node*			previous;
		TClass			data;

		Node(TClass data,Node* next = NULL,Node* previous = NULL)
		{
			this->data = data;
			this->next = next;
			this->previous = previous;
		}
	};


	class xTLinkedListIterator : public xTIterator<TClass>
	{
	protected:
		Node*					m_pNextNode;
	 	Node*					m_pCurrentNode;
	 	xTLinkedList<TClass>*	m_list;


	public:
		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		xTLinkedListIterator(xTLinkedList& list)
		{
			m_pNextNode = list.m_pFirstNode;
			m_pCurrentNode = NULL;
			m_list = &list;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual ~xTLinkedListIterator()
		{}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual bool hasNext()
		{
			return m_pNextNode != NULL;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual TClass& next() throw(xNoSuchElementException)
		{
			if(!hasNext())
				throw xNoSuchElementException();
			m_pCurrentNode = m_pNextNode;
			m_pNextNode = m_pNextNode->next;
			return m_pCurrentNode->data;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual void remove() throw(xIllegalStateException)
		{
			if(m_pCurrentNode == NULL)
				throw xIllegalStateException();
			m_list->executeRemove(m_pCurrentNode);
			m_pCurrentNode = NULL;
		}
	};


	class xTLinkedListListIterator : public xTListIterator<TClass>
	{
	protected:
		Node*					m_pNextNode;
		Node*					m_pPreviousNode;
	 	Node*					m_pCurrentNode;
	 	xTLinkedList<TClass>*	m_list;

	public:
		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		xTLinkedListListIterator(xTLinkedList<TClass>& list)
		{
			m_list = &list;
			m_pNextNode = m_list->m_pFirstNode;
			m_pPreviousNode = NULL;
			m_pCurrentNode = NULL;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual ~xTLinkedListListIterator()
		{}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual bool hasPrevious()
		{
			return m_pPreviousNode != NULL;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual TClass& previous() throw(xNoSuchElementException)
		{
			if(!hasPrevious())
				throw xNoSuchElementException();

			m_pCurrentNode = m_pPreviousNode;
			m_pNextNode = m_pCurrentNode->next;
			m_pPreviousNode = m_pCurrentNode->previous;

			return m_pCurrentNode->data;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual void set(TClass o) throw(xIllegalStateException)
		{
			m_pCurrentNode->data = o;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual bool hasNext()
		{
			return m_pNextNode != NULL;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual TClass& next() throw(xNoSuchElementException)
		{
			if(!hasNext())
				throw xNoSuchElementException();

			m_pCurrentNode = m_pNextNode;
			m_pPreviousNode = m_pCurrentNode->previous;
			m_pNextNode = m_pCurrentNode->next;

			return m_pCurrentNode->data;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual void remove() throw(xIllegalStateException)
		{
			if(m_pCurrentNode == NULL)
				throw xIllegalStateException();

			m_list->executeRemove(m_pCurrentNode);
		}
	};


	Node*				m_pFirstNode;
	Node*				m_pLastNode;
	xEquality<TClass>*	m_equality;
	int					m_elementCount;

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	void executeRemove(Node* toRemove)
	{
		if(toRemove->previous != NULL)
			toRemove->previous->next = toRemove->next;
		else
			m_pFirstNode = toRemove->next;

		if(toRemove->next != NULL)
			toRemove->next->previous = toRemove->previous;
		else
			m_pLastNode = toRemove->previous;

		delete toRemove;

		m_elementCount--;
	}
public:
	/**
	 * Creates an empty linked list.
	 */
	xTLinkedList(YOUROWNERSHIP xEquality<TClass>* eq)
	{
		m_elementCount = 0;
		m_pFirstNode = NULL;
		m_pLastNode = NULL;
		m_equality = eq;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual ~xTLinkedList()
	{
		clear();
	}

	/**
	 * Convenience method for fast removal of first element
	 */
	virtual void removeFirst()
	{
		if(m_pFirstNode != NULL)
			executeRemove(m_pFirstNode);
	}

	/**
	 * Convenience method for fast removal of last element
	 */
	virtual void removeLast()
	{
		if(m_pLastNode != NULL)
			executeRemove(m_pLastNode);
	}

	/**
	 * @exception xNoSuchElementException
	 */
	virtual TClass& getFirst() throw(xNoSuchElementException)
	{
		if(m_pFirstNode == NULL)
			throw xNoSuchElementException();

		return m_pFirstNode->data;
	}

	/**
	 * @exception xNoSuchElementException
	 */
	virtual TClass& getLast() throw(xNoSuchElementException)
	{
		if(m_pLastNode == NULL)
			throw xNoSuchElementException();

		return m_pLastNode->data;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual void add(TClass o)
	{
		Node* newElem = new Node(o);
		if(m_pLastNode != NULL)
		{
			m_pLastNode->next = newElem;
			newElem->previous = m_pLastNode;
			m_pLastNode = newElem;
		}
		else
		{
			m_pLastNode = newElem;
			m_pFirstNode = newElem;
		}
		m_elementCount++;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual void removeElement(TClass o)
	{
		Node* tmp = m_pFirstNode;
		while(tmp != NULL)
		{
			if(m_equality->equals(tmp->data,o))
			{
				executeRemove(tmp);
				break;
			}
			else
			{
				tmp = tmp->next;
			}
		}
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool isEmpty()
	{
		return m_pFirstNode == NULL;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual void clear()
	{
		Node* tmp = m_pFirstNode;
		while(tmp != NULL)
		{
			Node* tmp2 = tmp->next;
			delete tmp;
			tmp = tmp2;
		}
		m_pFirstNode = NULL;
		m_pLastNode = NULL;
		m_elementCount = 0;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual int size()
	{
		return m_elementCount;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool contains(TClass o)
	{
		Node* tmp = m_pFirstNode;
		while(tmp != NULL)
		{
			if(m_equality->equals(tmp->data,o))
				return true;
			tmp = tmp->next;
		}

		return false;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual TClass& get(int index) throw(xIndexOutOfBoundsException)
	{
		if(index < 0 || index >= size())
			throw xIndexOutOfBoundsException();

		Node* tmp = m_pFirstNode;
		for(int i = 0; i != index ;i++)
			tmp = tmp->next;

		return tmp->data;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual TClass set(int index,TClass o) throw(xIndexOutOfBoundsException)
	{
		if(index < 0 || index >= size())
			throw xIndexOutOfBoundsException();

		Node* tmp = m_pFirstNode;
		for(int i = 0; i != index ;i++)
			tmp = tmp->next;

		TClass ret = tmp->data;
		tmp->data = o;
		return ret;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual void insert(int index,TClass o) throw(xIndexOutOfBoundsException)
	{
		if(index < 0 || index >= size())
			throw xIndexOutOfBoundsException();

		Node* tmp = m_pFirstNode;
		for(int i = 0; i != index ;i++)
			tmp = tmp->next;

		Node* newElem = new Node(o);
		if(tmp != NULL)
		{
			if(tmp->previous != NULL)
				tmp->previous->next = newElem;
			else
				m_pFirstNode = newElem;

			newElem->previous = tmp->previous;
			tmp->previous = newElem;
			newElem->next = tmp;

			m_elementCount++;
		}
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual TClass remove(int index) throw(xIndexOutOfBoundsException)
	{
		if(index < 0 || index >= size())
			throw xIndexOutOfBoundsException();

		Node* tmp = m_pFirstNode;
		for(int i = 0; i != index ;i++)
			tmp = tmp->next;

		TClass ret = tmp->data;
		executeRemove(tmp);
		return ret;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual xTListIterator<TClass>* listIterator()
	{
		return new xTLinkedListListIterator(*this);
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual xTIterator<TClass>* iterator()
	{
		return new xTLinkedListIterator(*this);
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual xArray<TClass> toArray()
	{
		xArray<TClass> array(size());
		Node* tmp = m_pFirstNode;
		for(int i = 0; tmp != NULL; i++)
		{
			array[i] = tmp->data;
			tmp = tmp->next;
		}

		return array;
	}
};



//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
/**
 * xTArrayList is a simple,generic, and powerful xTList implementation.
 * It uses resizable-array for data management, and offers O(const) time
 * for better case(when not resizing) add and get,O(n) for the worst case add,
 * all other operation costs amortized O(n) time.
 *
 * See single functions for details about costs.
 */
template <class TClass>
class XTKAPI xTArrayList : public xTList<TClass>
{
private:
	int						m_elementCount;
	xArray<TClass>	 		m_elementData;
	int						m_capacityIncrementMultiplier;
	xEquality<TClass>*		m_equality;


	class xTArrayListIterator : public xTIterator<TClass>
	{
	protected:
		xTArrayList& 	m_list;
		int				m_currentPosition;
		bool			m_LastReturnedElementRemoved;

	public:
		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		xTArrayListIterator(xTArrayList& vector) : m_list(vector)
		{
			m_currentPosition = -1;
			m_LastReturnedElementRemoved = false;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual ~xTArrayListIterator()
		{}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual bool hasNext()
		{
			return (m_currentPosition + 1) < m_list.size();
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual TClass& next() throw(xNoSuchElementException)
		{
			if(!hasNext())
				throw xNoSuchElementException();

			m_currentPosition++;
			m_LastReturnedElementRemoved = false;
			return  m_list.get(m_currentPosition);
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual void remove() throw(xIllegalStateException)
		{
			if(m_currentPosition < 0 || m_LastReturnedElementRemoved)
				throw xIllegalStateException();

			m_list.remove(m_currentPosition);
			m_LastReturnedElementRemoved = true;
			m_currentPosition--;
		}
	};


	class xTArrayListListIterator : public xTListIterator<TClass>
	{
	protected:
		xTArrayList& 	m_list;
		int				m_currentPosition;
		bool			m_LastReturnedElementRemoved;

	public:
		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		xTArrayListListIterator(xTArrayList& list) : m_list(list)
		{
			m_currentPosition = -1;
			m_LastReturnedElementRemoved = false;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual ~xTArrayListListIterator()
		{}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual bool hasPrevious()
		{
			return (m_currentPosition - 1) >= 0;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual TClass& previous() throw(xNoSuchElementException)
		{
			if(!hasPrevious())
				throw xNoSuchElementException();

			m_currentPosition--;
			m_LastReturnedElementRemoved = false;
			return  m_list.get(m_currentPosition);
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual void set(TClass el) throw(xIllegalStateException)
		{
			if(m_currentPosition < 0 || m_LastReturnedElementRemoved)
				throw xIllegalStateException();

			 m_list.set(m_currentPosition,el);
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual bool hasNext()
		{
			return (m_currentPosition + 1) <  m_list.size();
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual TClass& next() throw(xNoSuchElementException)
		{
			if(!hasNext())
				throw xNoSuchElementException();

			m_currentPosition++;
			m_LastReturnedElementRemoved = false;
			return  m_list.get(m_currentPosition);
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual void remove() throw(xIllegalStateException)
		{
			if(m_currentPosition < 0 || m_LastReturnedElementRemoved)
				throw xIllegalStateException();

			m_list.remove(m_currentPosition);
			m_LastReturnedElementRemoved = true;
			m_currentPosition--;
		}
	};

public:
	/**
	 * Creates an empty TArrayList.
	 *
	 * @exception xIllegalArgumentException
	 */
	xTArrayList(YOUROWNERSHIP xEquality<TClass>* eq,
		int initialCapacity = 0,int capacityIncrementMultiplier = 2)
		throw(xIllegalArgumentException)
		: m_elementData(initialCapacity)
	{
		if(capacityIncrementMultiplier <= 1)
		{
			throw xIllegalArgumentException(_T("capacityIncrement cannot be negative!"));
		}

		m_capacityIncrementMultiplier = capacityIncrementMultiplier;
		m_elementCount = 0;
		m_equality = eq;
	}

	~xTArrayList()
	{
		clear();
	}

	/**
	 * Increases the capacity of this vector, use this method to increase
	 * drastically performances in the case of witch you already know to what
	 * size will grow this vector.
	 */
	virtual void ensureCapacity(int minCapacity) throw(xIllegalArgumentException)
	{
		if(minCapacity > size())
			m_elementData.resize(minCapacity);
	}

	/**
	 *  Trims the capacity of this vector to be the vector's current size.
	 */
	virtual void trimToSize()
	{m_elementData.resize(size());}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual void add(TClass o)
	{
		if(size() >= m_elementData.size())
			m_elementData.resize((size() + 1) * m_capacityIncrementMultiplier);

		m_elementData[m_elementCount] = o;
		m_elementCount++;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual void removeElement(TClass o)
	{
		bool beginShift = false;
		for(int i = 0; i < size(); i++)
		{
			if(m_equality->equals(o,get(i)))
				beginShift = true;

			if(beginShift)
				m_elementData[i] = m_elementData[i + 1];
		}
		if(beginShift)
			m_elementCount--;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool isEmpty()
	{return m_elementCount == 0;}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual void clear()
	{
		m_elementData.clear();
		m_elementCount = 0;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual int size()
	{return m_elementCount;}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool contains(TClass o)
	{
		for(int i = 0; i < size(); i++)
		{
			if(m_equality->equals(o,get(i)))
				return true;
		}
		return false;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual TClass& get(int index) throw(xIndexOutOfBoundsException)
	{
		return m_elementData[index];
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual TClass set(int index,TClass o) throw(xIndexOutOfBoundsException)
	{
		TClass tmp = m_elementData[index];
		m_elementData[index] = o;
		return tmp;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual void insert(int index,TClass o) throw(xIndexOutOfBoundsException)
	{
		//here we must do coerency test before resizing
		if(index < 0 || index >= size())
			throw xIndexOutOfBoundsException();

		//resize if necessary
		if(size() >= m_elementData.size())
			m_elementData.resize(size() * m_capacityIncrementMultiplier);

		//shift forward elements
		for(int i = size() + 1;i > index;i--)
			m_elementData[i] = m_elementData[i - 1];

		m_elementData[index] = o;
		m_elementCount++;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual TClass remove(int index) throw(xIndexOutOfBoundsException)
	{
		//index sanity is implicitly checked here
		TClass tmp = m_elementData[index];

		//shift backward elements
		for(int i = index;i < size();i++)
			m_elementData[i] = m_elementData[i + 1];

		m_elementCount--;
		return tmp;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual xTListIterator<TClass>* listIterator()
	{return new xTArrayListListIterator(*this);}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual xTIterator<TClass>* iterator()
	{return new xTArrayListIterator(*this);}

	/**
	 * Convenience method that calls sort() on internal array.
	 */
	virtual void sort(xComparator<TClass>& comparator)
	{
		m_elementData.sort(comparator,0,size() - 1);
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual xArray<TClass> toArray()
	{
		xArray<TClass> array(size());
		for(int i = 0; i < size(); i++)
			array[i] = m_elementData[i];

		return array;
	}
};


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/**
 * xTAvlMap offer O(log(n)) insertion,search and remove.
 * It uses AVL tree concept.
 */
template <class TClassKey,class TClassValue>
class XTKAPI xTAvlMap : public xTMap<TClassKey,TClassValue>
{
private:
	struct Node : public virtual xObject
	{
		Node*			left;
		Node*			right;
		TClassKey		key;
		TClassValue		data;
		int				height;

		Node(TClassValue data,TClassKey key,Node* left = NULL,Node* right = NULL)
		{
			this->left = left;
			this->right = right;
			this->key = key;
			this->data = data;
			this->height = 0;
		}
	};

	class xTAvlMapIterator : public xTIterator<TClassValue>
	{
	protected:
		xArray<Node*> 		m_stack;
		int					m_position;
		bool				m_alreadyRemoved;
		xTAvlMap&			m_map;

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		void buildStack(Node* root)
		{
			if(root != NULL)
			{
				buildStack(root->left);
				m_stack[++m_position] = root;
				buildStack(root->right);
			}
		}

	public:

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		xTAvlMapIterator(xTAvlMap& map): m_stack(0),m_map(map)
		{
			m_stack.resize(map.size());
			m_position = -1;
			buildStack(map.m_pRootNode);
			m_position = -1;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual ~xTAvlMapIterator()
		{}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual bool hasNext()
		{
			return (m_position + 1 < m_stack.size());
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual TClassValue& next() throw(xNoSuchElementException)
		{
			if(!hasNext())
				throw xNoSuchElementException();

			m_alreadyRemoved = false;
			return m_stack[++m_position]->data;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual void remove() throw(xIllegalStateException)
		{
			if(m_position < 0 || m_alreadyRemoved)
				throw xIllegalStateException();

			m_alreadyRemoved = true;
			m_map.remove(m_stack[m_position]->key);
		}
	};

	Node*					m_pRootNode;
	Node* 					nodeToDeleteWith2Child;
	TClassValue				m_lastDeletedValue;
	bool					m_isLastDeletedValueValid;
	xEquality<TClassValue>*	m_equality;
	xComparator<TClassKey>*	m_comparator;

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

	/*
	 * Recursive insert. Return the new root
	 */
	Node* insert(TClassKey compKey,TClassValue value,Node* root)
	{
		if(root == NULL)
			root = new Node(value,compKey);
		else
		{
			//insert in left child
			if(m_comparator->compare(compKey,root->key) < 0)
			{
				//recursive pass
				root->left = insert(compKey,value,root->left);

				//if after insertion the tree is not balanced
				if((getHeightOf(root->left) - getHeightOf(root->right)) == 2)
				{
					if(m_comparator->compare(compKey,root->left->key) < 0)
						root = LLrotation(root); //LL
					else
						root = LRrotation(root); //LR
				}
			}
			//insert in right child
			else if(m_comparator->compare(compKey,root->key) > 0)
			{
				//recursive pass
				root->right = insert(compKey,value,root->right);

				//if after insertion the tree is not balanced
				if((getHeightOf(root->right) - getHeightOf(root->left)) == 2)
				{
					if(m_comparator->compare(compKey,root->right->key) > 0)
						root = RRrotation(root); //RR
					else
						root = RLrotation(root); //RL
				}
			}
			//if duplicate key
			else
			{
				m_lastDeletedValue = root->data;
				m_isLastDeletedValueValid = true;

				//substitute the value of current root
				root->data = value;
			}
		}
		root->height = MAX(getHeightOf(root->left),getHeightOf(root->right)) + 1;

		return root;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

	/*
	 * LL rotation
	 */
	Node* LLrotation(Node* root)
	{
		Node* tmp = root->left;
		root->left = tmp->right;
		tmp->right = root;
		root->height = MAX(getHeightOf(root->left),getHeightOf(root->right)) + 1;
		tmp->height = MAX(getHeightOf(tmp->left),getHeightOf(root)) + 1;
		return tmp;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

	/*
	 * LR rotation
	 */
	Node* LRrotation(Node* root)
	{
		root->left = RRrotation(root->left);
		return LLrotation(root);
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

	/*
	 * RR rotation
	 */
	Node* RRrotation(Node* root)
	{
		Node* tmp = root->right;
		root->right = tmp->left;
		tmp->left = root;
		root->height = MAX(getHeightOf(root->left),getHeightOf(root->right)) + 1;
		tmp->height = MAX(getHeightOf(tmp->left),getHeightOf(root)) + 1;
		return tmp;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

	/*
	 * RL rotation
	 */
	Node* RLrotation(Node* root)
	{
		root->right = LLrotation(root->right);
		return RRrotation(root);
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

	/*
	 * Find the maximum between of all nodes < of "node"
	 */
	Node* predecessor(TClassKey key)
	{
		Node* pred = NULL;
		Node* root = m_pRootNode;
		while(root != NULL)
		{
			if(m_comparator->compare(root->key,key) >= 0)
				root = root->left;
			else if(m_comparator->compare(root->key,key) < 0)
			{
				pred = root;
				root = root->right;
      		}
		}
		return pred;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

	/*
	 * return new root
	 */
	Node* remove(TClassKey key,Node* root)
	{
		if(root != NULL)
		{
			//begin search for the element to delete
			if(m_comparator->compare(key,root->key) < 0)
			{
				root->left = remove(key,root->left);
			}
			else if(m_comparator->compare(key,root->key) > 0)
			{
				root->right = remove(key,root->right);
			}
			else//we found it
			{
				//Case 1: node is a leaf, just delete it
				if(root->left == NULL && root->right == NULL)
				{
					m_lastDeletedValue = root->data;
					m_isLastDeletedValueValid = true;
					delete root;

					root = NULL;
					return root;
				}
				//case 2.1 node with only left child
				else if(root->left != NULL && root->right == NULL)
				{
					Node* tmp = root->left;

					m_lastDeletedValue = root->data;
					m_isLastDeletedValueValid = true;
					delete root;

					root = tmp;
				}
				//case 2.2 node with only right child
				else if(root->left == NULL && root->right != NULL)
				{
					Node* tmp = root->right;

					m_lastDeletedValue = root->data;
					m_isLastDeletedValueValid = true;
					delete root;

					root = tmp;
				}
				//case 3 node with two child
				else
				{
					//return to root node and delete the predecessor
					nodeToDeleteWith2Child = root;
					if(root != m_pRootNode)
						return root;
				}
			}

			if((root == m_pRootNode) && nodeToDeleteWith2Child != NULL)
			{
				//if we are here we are in the root node and we should delete
				//the predecessor of the node

				//find predecessor
				Node* pred = predecessor(nodeToDeleteWith2Child->key);
				Node* temp = nodeToDeleteWith2Child;

				TClassValue oldData = nodeToDeleteWith2Child->data;
				//TClassKey oldKey = nodeToDeleteWith2Child->key;

				//copy precedessor data at place of root
				nodeToDeleteWith2Child->data = pred->data;
				TClassKey tmpKey = pred->key;

				//delete predecessor
				nodeToDeleteWith2Child = NULL;
				m_pRootNode = remove(pred->key,m_pRootNode);
				temp->key = tmpKey;

				m_lastDeletedValue = oldData;
				m_isLastDeletedValueValid = true;

				return m_pRootNode;
			}

			//now balance if necessary
			int balFact = getHeightOf(root->left) - getHeightOf(root->right);
			//right imbalanced
			if(balFact == -2)
			{
				//root->right MUST be != NULL
				if(getHeightOf(root->right->right) > getHeightOf(root->right->left))
					root = RRrotation(root); //RR
				else
					root = RLrotation(root); //RL
			}
			//left imbalanced
			else if(balFact == 2)
			{
				//root->left MUST be != NULL
				if(getHeightOf(root->left->left) > getHeightOf(root->left->right))
					root = LLrotation(root); //LL
				else
					root = LRrotation(root); //LR
			}
			//else is balanced

			//now update height
			root->height = MAX(getHeightOf(root->left),getHeightOf(root->right)) + 1;
		}
		else //if key not found
		{
			m_isLastDeletedValueValid = false;
		}

		return root;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

	void clear(Node* root)
	{
		if(root != NULL)
		{
			clear(root->left);
			clear(root->right);
			delete root;
		}
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	int getHeightOf(Node* node)
	{
		if(node == NULL)
			return 0;
		else
			return node->height;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	int size(Node* root)
	{
		if(root != NULL)
		{
 			int subSize = 0;
 			subSize += size(root->left);
 			subSize += size(root->right);
 			return subSize + 1;
 		}
 		else
 			return 0;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	bool containsValue(Node* root,TClassValue value)
	{
		if(m_equality->equals(value,root->data))
			return true;
		else
		{
			if(containsValue(root->left,value))
				return true;
			if(containsValue(root->right,value))
				return true;
		}
		return false;
	}

	void toArray(Node* root,xArray<TClassValue>* arr,int* position)
	{
		if(root != NULL)
		{
			toArray(root->left,arr,position);
			(*arr)[++(*position)] = root->data;
			toArray(root->right,arr,position);
		}
	}

public:

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	xTAvlMap(YOUROWNERSHIP xComparator<TClassKey>* comp,YOUROWNERSHIP xEquality<TClassValue>* eq)
	{
		m_pRootNode = NULL;
		nodeToDeleteWith2Child = NULL;
		m_isLastDeletedValueValid = false;;
		m_equality = eq;
		m_comparator = comp;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual ~xTAvlMap()
	{
		clear();
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool put(TClassKey key, TClassValue value,TClassValue* removed = NULL)
	{
 		m_pRootNode = insert(key,value,m_pRootNode);

		if(removed != NULL && m_isLastDeletedValueValid)
			*removed = m_lastDeletedValue;

		bool tmp = m_isLastDeletedValueValid;
		m_isLastDeletedValueValid = false;

		return tmp;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual void clear()
	{
		clear(m_pRootNode);
		m_pRootNode = NULL;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool containsKey(TClassKey key)
	{
		Node* root = m_pRootNode;
		while(root != NULL)
		{
			if(m_comparator->compare(key,root->key) < 0)
				root = root->left;
			else if(m_comparator->compare(key,root->key) > 0)
				root = root->right;
			else
				return true;
		}
		return false;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool containsValue(TClassValue value)
	{
		return containsValue(m_pRootNode,value);
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool get(TClassKey key,TClassValue** ret)
	{
		Node* root = m_pRootNode;
		while(root != NULL)
		{
			if(m_comparator->compare(key,root->key) < 0)
				root = root->left;
			else if(m_comparator->compare(key,root->key) > 0)
				root = root->right;
			else
			{
				*ret = &(root->data);
				return true;
			}
		}
		return false;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool isEmpty()
	{
		return m_pRootNode == NULL;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool remove(TClassKey key,TClassValue* removed = NULL)
	{
		m_pRootNode = remove(key,m_pRootNode);
		if(removed != NULL && m_isLastDeletedValueValid)
			*removed = m_lastDeletedValue;

		bool tmp = m_isLastDeletedValueValid;
		m_isLastDeletedValueValid = false;

		return tmp;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual int size()
	{
		return size(m_pRootNode);
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual xTIterator<TClassValue>* iterator()
	{
		return new xTAvlMapIterator(*this);
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual xArray<TClassValue> toArray()
	{
		xArray<TClassValue> arr(size());
		int position;
		toArray(m_pRootNode,&arr,&position);

		return arr;
	}

	#ifdef XTK_TEST
		/**
		 * check if the this is a valid AVL Tree
		 */
		bool checkAVL(Node* root)
		{
			bool result = true;
			if(root != NULL)
			{
				if(abs(getHeightOf(root->left) - getHeightOf(root->right)) < 2)
				{
					if(root->left != NULL)
					{
						if(compare(root,root->left) < 0)
							return false;
						result = checkAVL(root->left);
					}
					else
						result = true;
					
					if(root->right != NULL)
					{
						if(compare(root,root->right) > 0)
							return false;
						result &= checkAVL(root->left);
					}
					else
						result &= true;
				}
				else
					result = false;
			}
				
			return result;
		}

		/**
		 * check if the this is a valid AVL Tree
		 */
		bool checkAVL()
		{
			return checkAVL(m_pRootNode);
		}

		int compare(Node* node1,Node* node2)
		{
			if(node1 == NULL)
				return -1;
			if(node2 == NULL)
				return 1;
			if(m_comparator->compare(node1->key,node2->key) < 0)
				return -1;
			if(m_comparator->compare(node1->key,node2->key) > 0)
				return 1;
			if(m_comparator->compare(node1->key,node2->key) == 0)
				return 0;
				
			//never arrive here
			return 0;
		}
	#endif
};



//#define TEST_FIND_GOOD_HASH

/**
 * xTHashMap offer O(const) insertion,search and remove.
 * The Objects representing the keys must implement a good hashCode() function.
 */
template <class TClassKey,class TClassValue>
class XTKAPI xTHashMap : public xTMap<TClassKey,TClassValue>
{
private:
	struct Node
	{
		TClassValue	data;
		TClassKey	key;
		Node*		next;

		Node(TClassValue data,TClassKey key,Node* next = NULL)
		{
			this->data = data;
			this->next = next;
			this->key = key;
		}
	};

	class xTHashMapIterator : public xTIterator<TClassValue>
	{
	private:
		xTHashMap&		m_map;
		int				m_index;
		Node*			m_currentNode;
		Node*			m_nextNode;

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		void findNextNode()
		{
			if(m_currentNode != NULL)
			{
				if(m_currentNode->next != NULL)
				{
					m_nextNode = m_currentNode->next;
					return;
				}
			}
			
			for(m_index++;m_index < m_map.m_table->size();m_index++)
			{
				if((*(m_map.m_table))[m_index] != NULL)
				{
					m_nextNode = (*(m_map.m_table))[m_index];
					return;
				}	
			}
			
			m_nextNode = NULL;
		}

	public:
		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		xTHashMapIterator(xTHashMap& map) : m_map(map)
		{
			m_index = -1;
			m_currentNode = NULL;
			m_nextNode = NULL;
			findNextNode();
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual ~xTHashMapIterator()
		{}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual bool hasNext()
		{return m_nextNode != NULL;}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual TClassValue& next() throw(xNoSuchElementException)
		{
			if(!hasNext())
				throw xNoSuchElementException();
				
			TClassValue& o = m_nextNode->data;
			m_currentNode = m_nextNode;
			findNextNode();

			return o;
		}

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		virtual void remove() throw(xIllegalStateException)
		{
			if(m_currentNode == NULL)
				throw xIllegalStateException();
				
			m_map.remove(m_currentNode->key);
			m_currentNode = NULL;
		}
	};


	xArray<Node*>*			 	m_table;
	int							m_count;
	float						m_loadFactor;
	xEquality<TClassKey>*		m_equalityK;
	xEquality<TClassValue>*		m_equalityV;
	xHashable<TClassKey>*		m_hashable;
	TClassValue					m_lastDeletedValue;
	bool						m_isLastDeletedValueValid;

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	int hash(TClassKey key)
	{
		int h = m_hashable->hashOf(key);
		
		//first test 23,26,17
		#ifdef TEST_FIND_GOOD_HASH
			int h1 = h << m_dim1;
			h ^= h1;
			int h2 = (h << m_dim2);
			h += h2;
			int h3 = (h >> m_dim3);
			h ^= h3;
		    
			return (int)xMath::abs(h);
		#else
		
			//test 7,2,3
			int h1 = h << 7;
			h ^= h1;
			int h2 = (h << 2);
			h += h2;
			int h3 = (h >> 3);
			h ^= h3;
		    
			return (int)xMath::abs(h);
		#endif
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	int indexOf(int hash)
	{
		return hash & (m_table->size() - 1);
	}

	/**
	 * Return new root
	 */
	Node* addInList(TClassValue data,TClassKey key,Node* root)
	{
		Node* tmp = root;
		for(;;)
		{
			if(tmp == NULL)
			{
				m_count++;
				return new Node(data,key);
			}
			else if(tmp->next == NULL)
			{
				tmp->next = new Node(data,key);
				m_count++;
				return root;
			}
			//if duplicate, substitute
			else if(m_equalityK->equals(tmp->key,key))
			{
				m_isLastDeletedValueValid = true;
				m_lastDeletedValue = tmp->data;
				tmp->data = data;
				return root;
			}
			tmp = tmp->next;
		}
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	Node* delFromList(TClassKey key,Node* root)
	{
		Node* tmp = root;
		Node* prev = NULL;
		while(tmp != NULL)
		{
			if(m_equalityK->equals(tmp->key,key))
			{
				Node* tmpNext = tmp->next;
				m_isLastDeletedValueValid = true;
				m_lastDeletedValue = tmp->data;
				delete tmp;
				
				m_count--;
				
				if(prev != NULL)
				{
					prev->next = tmp->next;
					return root;
				}
				else
					return tmpNext;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		
		return root;
	}

	/**
	 * Return found object or NULL if not found
	 */
	Node* findValInList(TClassValue o,Node* root)
	{
		Node* tmp = root;
		while(tmp != NULL)
		{
			if(m_equalityV->equals(tmp->data,o))
				return tmp;
			tmp = tmp->next;
		}
		return NULL;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	Node* findKeyInList(TClassKey o,Node* root)
	{
		Node* tmp = root;
		while(tmp != NULL)
		{
			if(m_equalityK->equals(tmp->key,o))
				return tmp;
			tmp = tmp->next;
		}
		return NULL;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	void rehash()
	{
		xArray<Node*>* oldTable = m_table;
		m_table = new xArray<Node*>(m_table->size() * 2);
		m_table->initElements(NULL);
		
		for(register int i = 0;i < oldTable->size();i++)
		{
			Node* currentNode = (*oldTable)[i];
			while(currentNode != NULL)
			{
				Node* nextNode = currentNode->next;
				
				int index = indexOf(hash(currentNode->key));
				currentNode->next = (*m_table)[index];
				(*m_table)[index] = currentNode;
				
				currentNode = nextNode;
			}
		}
		delete oldTable;
	}

	#ifdef TEST_FIND_GOOD_HASH
		int m_dim1,m_dim2,m_dim3;
	#endif

public:

	/**
	 * Constructor.
	 * After the size reach the value of tableSize * loadFactor, the table
	 * will be resized.
	 */
	xTHashMap(YOUROWNERSHIP xHashable<TClassKey>* hashable,YOUROWNERSHIP xEquality<TClassKey>* eq1,
		YOUROWNERSHIP xEquality<TClassValue>* eq2,
		int initialCapacity = 32,float loadFactor = 0.75f)
	throw(xIllegalArgumentException)
	{
		m_isLastDeletedValueValid = false;
		m_equalityK = eq1;
		m_equalityV = eq2;
		m_table = new xArray<Node*>(initialCapacity);
		m_table->initElements(NULL);
		m_loadFactor = loadFactor;
		m_count = 0;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual ~xTHashMap()
	{
		clear();
		delete m_table;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool put(TClassKey key, TClassValue value,TClassValue* removed = NULL)
	{
		int index = indexOf(hash(key));
		(*m_table)[index] = addInList(value,key,(*m_table)[index]);
		if((m_table->size() * m_loadFactor) < m_count)
			rehash();

		if(removed != NULL && m_isLastDeletedValueValid)
			*removed = m_lastDeletedValue;

		bool tmp = m_isLastDeletedValueValid;
		m_isLastDeletedValueValid = false;

		return tmp;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual void clear()
	{
		for(register int i = 0;i < m_table->size();i++)
		{
			if((*m_table)[i] != NULL)
			{
				Node* tmp = (*m_table)[i];
				while(tmp != NULL)
					tmp = delFromList(tmp->key,tmp);
				
				(*m_table)[i] = NULL;
			}
		}
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool containsKey(TClassKey key)
	{
		int index = indexOf(hash(key));
		if(findKeyInList(key,(*m_table)[index]) != NULL)
			return true;
		return false;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool containsValue(TClassValue value)
	{
		for(register int i = 0; i < m_table->size();i++)
		{
			if((*m_table)[i] != NULL)
			{
				if(findValInList(value,(*m_table)[i]) != NULL)
					return true;
			}
		}	
		return false;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool get(TClassKey key,TClassValue** ret)
	{
		int index = indexOf(hash(key));
		Node* found = findKeyInList(key,(*m_table)[index]);
		if(found != NULL)
		{
			*ret = &(found->data);
			return true;
		}
			
		return false;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool isEmpty()
	{
		return m_count == 0;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual bool remove(TClassKey key,TClassValue* removed = NULL)
	{
		int index = indexOf(hash(key));
		(*m_table)[index] = delFromList(key,(*m_table)[index]);
		
		if(removed != NULL && m_isLastDeletedValueValid)
			*removed = m_lastDeletedValue;

		bool tmp = m_isLastDeletedValueValid;
		m_isLastDeletedValueValid = false;

		return tmp;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual int size()
	{	
		return m_count;	
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual xTIterator<TClassValue>* iterator()
	{
		return new xTHashMapIterator(*this);
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	virtual xArray<TClassValue> toArray()
	{
		xArray<TClassValue> arr(size());
		
		xTIterator<TClassValue>* iter = iterator();
		for(register int i = 0;iter->hasNext();i++)
			arr[i] = iter->next();

		delete iter;

		return arr;
	}

	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


	#ifdef TEST_FIND_GOOD_HASH
		xHashMap(int dim1,int dim2,int dim3);
	#endif


	#ifdef XTK_TEST
		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		int countCollisions();

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		float averageObjsForBucket();

		//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		int maxObjsForBucket();
	#endif
};

}//namespace

#endif //XTK_TDATASTRUCTURES_H
