/*
* This file is part of XTKLib project.
*
* Copyright(C) 2003-2005 Mario Casciaro (xshadow@email.it)
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
* @file datastructures.h
* @author Mario Casciaro (xshadow@email.it)
*/


#ifndef XTK_DATASTRUCTURES_H
#define XTK_DATASTRUCTURES_H

#include "../globals.h"
#include "interfaces.h"
#include "array.h"
#include "exception.h"
#include <math.h>

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/**
 * An iterator over a collection.
 * 
 * @warning Iterators in xtklib are not guaranteed to be coerent with the underling data
 * structure if concurrent modifications occurs none exceptions are risen. This control 
 * is left to user.
 */
class XTKAPI xIterator : virtual public xObject
{
public:
	/**
	 * Returns true if the iteration has more elements.
	 */
	virtual bool hasNext() = 0;
	
	/**
	 * Returns the next element in the iteration.
	 * @exception xNoSuchElementException
	 */
	virtual xObject& next() throw(xNoSuchElementException) = 0;
	
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
 * An iterator for lists that allows the programmer to traverse the list in 
 * either direction and modify the list during iteration.
 * 
 * @warning Iterators in xtklib are not guaranteed to be coerent with the underling data
 * structure if concurrent modifications occurs none exceptions are risen. This control 
 * is left to user.
 */
class XTKAPI xListIterator : public xIterator
{
public:

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
	virtual xObject& previous() throw(xNoSuchElementException) = 0;
	
	/**
	 * Replaces the last element returned by next or previous with the
	 * specified element
	 * 
	 * @exception xIllegalStateException
	 */
	virtual void set(xObject* o) throw(xIllegalStateException) = 0;
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################


/**
 * Represents a collection of elements.
 */
class XTKAPI xCollection : virtual public xObject,public xOwnership
{
public:
	/**
	 * Add the specified element to this collection
	 */
	virtual void add(xObject* o) = 0;
	
	/**
	 * Removes the specified element from the collection
	 */
	virtual void removeObject(xObject& o) = 0;
	
	/**
	 * Returns the
	 */
	virtual xObject& getByObject(xObject& o) = 0;
	
	/**
	 * Returns true if this collection does not contains any element
	 */
	virtual bool isEmpty() = 0;
	
	/**
	 * Removes all elements from the collection
	 */
	virtual void clear() = 0;
	
	/**
	 * Returns an iterator over this collection
	 */
	virtual xIterator* iterator() = 0;
	
	/**
	 * Returns the number of elements in this collection
	 */
	virtual int size() = 0;
	
	/**
	 * Returns an array containing all of the elements in this collection.
	 */
	virtual xArray<xObject*> toArray() = 0;
	
	/**
	 * Returns true if the specified element is already present in the collection.
	 */
	virtual bool contains(xObject& o) = 0;
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/**
 * An ordered collection (also known as a sequence).
 */
class XTKAPI xList : public xCollection
{
public:
	/**
	 * Returns the element at the specified position in this list.
	 * 
	 * @exception xIndexOutOfBoundsException
	 */
	virtual xObject& get(int index) throw(xIndexOutOfBoundsException) = 0;
	
	/**
	 *  Replaces the element at the specified position in this list with 
	 * the specified element.
	 * 
	 * @exception xIndexOutOfBoundsException
	 */
	virtual void set(int index,xObject* o) throw(xIndexOutOfBoundsException) = 0;
	
	/**
	 * Inserts the specified element at the specified position in this list.
	 * 
	 * @exception xIndexOutOfBoundsException
	 */
	virtual void insert(int index,xObject* o) throw(xIndexOutOfBoundsException)= 0;
	
	/**
	 *  Removes the element at the specified position in this list.
	 */
	virtual void removeIndex(int index) throw(xIndexOutOfBoundsException) = 0;
	
	/**
	 * Returns a list iterator of the elements in this list.
	 */
	virtual xListIterator* listIterator() = 0;
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/**
 * An object that maps keys to values. A map cannot contain duplicate keys;
 * each key can map to at most one value. Key objects are unconditional ownership 
 * of the xMap object.
 */
class XTKAPI xMap : public xObject,public xOwnership
{
public:
	/**
	 * Removes all mappings from this map
	 */
	virtual void clear() = 0;

	/**
	 * Returns true if this map contains a mapping for the specified key.
	 * 
	 * @exception xClassCastException
	 */
	virtual bool containsKey(xObject& key) throw(xClassCastException) = 0;
         
    /**
     *  Returns true if this map maps one or more keys to the specified value.
     */
	virtual bool containsValue(xObject& value) = 0;

	/**
	 * Returns the value to which this map maps the specified key. 
	 * If such map does not exists a xNullObject is returned 
	 * (test the result with xObject::isNull())
	 *
	 * @exception xClassCastException
	 */        
	virtual xObject& get(xObject& key) throw(xClassCastException) = 0;
    
    /**
     * Returns true if this map contains no key-value mappings.
     */
	virtual bool isEmpty() = 0;
    
    /**
     * Associates the specified value with the specified key in this map.
     * 
     * @exception xClassCastException
     */
	virtual void put(xObject* key, xObject* value) throw(xClassCastException) = 0;
    
	/**
	 * Removes the mapping for this key from this map if it is present 
	 * 
	 * @exception xClassCastException
	 */
	virtual void remove(xObject& key) throw(xClassCastException) = 0;
	
	/**
     * Returns the number of key-value mappings in this map.
     */
	virtual int size() = 0;
	
	/**
	 * Returns an iterator over this map
	 */
	 virtual xIterator* iterator() = 0;
	
	/**
	 * Returns a collection view of the values contained in this map
	 */
	virtual xCollection* values() = 0;
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/**
 * Linked list implementation of the List interface.
 * May provide better performance than the xArrayList implementation if elements 
 * are frequently added,inserted or deleted within the list. It has also a 
 * better memory management. It offers const time add and linear
 * get,insert and delete in the middle and const delete at end or begin of the list
 * 
 * See single functions for details about costs.
 */
class XTKAPI xLinkedList : public xList
{
private:

	struct Node : public xObject
	{
		Node*			next;
		Node*			previous;
		xObject*		data;
		
		Node(xObject* data,Node* next = NULL,Node* previous = NULL)
		{
			this->data = data;
			this->next = next;
			this->previous = previous;
		}
	};
	
	
	class xLinkedListIterator : public xIterator
	{
	protected:
		Node*			m_pNextNode;
	 	Node*			m_pCurrentNode;
	 	xLinkedList*	m_list;
	 	
		
	public:
		xLinkedListIterator(xLinkedList& list);
		virtual ~xLinkedListIterator(){}
		

		virtual bool hasNext();
		virtual xObject& next() throw(xNoSuchElementException) ;
		virtual void remove() throw(xIllegalStateException);
	};
	
	
	class xLinkedListListIterator : public xListIterator
	{
	protected:
		Node*			m_pNextNode;
		Node*			m_pPreviousNode;
	 	Node*			m_pCurrentNode;
	 	xLinkedList*	m_list;
		
	public:
		xLinkedListListIterator(xLinkedList& list);
		virtual ~xLinkedListListIterator(){}
		

		virtual bool hasPrevious();
		virtual xObject& previous() throw(xNoSuchElementException);
		virtual void set(xObject* o) throw(xIllegalStateException);
		virtual bool hasNext();
		virtual xObject& next() throw(xNoSuchElementException);
		virtual void remove() throw(xIllegalStateException);
	};
	
	
	Node*	m_pFirstNode;
	Node*	m_pLastNode;
	int		m_elementCount;
	

	void executeRemove(Node* toRemove);
public:
	/**
	 * Creates an empty linked list
	 */
	xLinkedList();
	
	virtual ~xLinkedList(){clear();}
	

	virtual void removeFirst();
	virtual void removeLast();
	
	/**
	 * @xNoSuchElementException
	 */
	virtual xObject& getFirst();
	
	/**
	 * @xNoSuchElementException
	 */
	virtual xObject& getLast();
	
	
	virtual void add(xObject* o);
	virtual void removeObject(xObject& o);
	virtual bool isEmpty();
	virtual void clear();
	virtual int size();
	virtual bool contains(xObject& o);
	virtual xObject& getByObject(xObject& o);
	virtual xObject& get(int index) throw(xIndexOutOfBoundsException);
	virtual void set(int index,xObject* o) throw(xIndexOutOfBoundsException);
	virtual void insert(int index,xObject* o) throw(xIndexOutOfBoundsException);
	virtual void removeIndex(int index) throw(xIndexOutOfBoundsException);
	virtual xListIterator* listIterator();
	virtual xIterator* iterator();
	virtual xObject* clone();
	virtual xString toString();
	
	virtual xArray<xObject*> toArray()
	{
		xArray<xObject*> array(size());
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
 * @brief ArrayList is a simple,generic, and powerful xList implementation.
 * It uses resizable-array for data management, and offers O(const) time 
 * for better case(when not resizing) add and get,O(n) for the worst case add, 
 * all other operation costs amortized O(n) time.
 * 
 * See single functions for details about costs.
 */
class XTKAPI xArrayList : public xList
{
private:
	int						m_elementCount;
	xArray<xObject*>	 	m_elementData;
	int						m_capacityIncrementMultiplier;
	
	
	class xArrayListIterator : public xIterator
	{
	protected:
		xArrayList& 	m_list;
		int				m_currentPosition;
		int				m_LastReturnedElementRemoved;
		
	public:
		xArrayListIterator(xArrayList& vector);
		virtual ~xArrayListIterator(){}
		
		virtual bool hasNext();
		virtual xObject& next() throw(xNoSuchElementException);
		virtual void remove() throw(xIllegalStateException) ;
	};
	
	
	class xArrayListListIterator : public xListIterator
	{
	protected:
		xArrayList& 	m_list;
		int				m_currentPosition;
		int				m_LastReturnedElementRemoved;
		
	public:
		xArrayListListIterator(xArrayList& list);
		virtual ~xArrayListListIterator(){}
		
		virtual bool hasPrevious();
		virtual xObject& previous() throw(xNoSuchElementException);
		virtual void set(xObject* o) throw(xIllegalStateException);
		virtual bool hasNext();
		virtual xObject& next() throw(xNoSuchElementException);
		virtual void remove() throw(xIllegalStateException);
	};
	
public:
	/**
	 * Creates an empty vector.
	 * 
	 * @exception xIllegalArgumentException
	 */
	xArrayList(int initialCapacity = 0,int capacityIncrementMultiplier = 2) 
	throw(xIllegalArgumentException);
	
	~xArrayList();
	
	/**
	 * Increases the capacity of this vector, use this method to increase
	 * drastically performances in the case of witch you already know to what
	 * size will grow this vector.
	 */
	virtual void ensureCapacity(int minCapacity) throw(xIllegalArgumentException);
	
	/**
	 *  Trims the capacity of this vector to be the vector's current size.
	 */
	virtual void trimToSize();
	
	virtual void add(xObject* o);
	virtual void removeObject(xObject& o);
	virtual bool isEmpty();
	virtual void clear();
	virtual int size();

	virtual bool contains(xObject& o);
	virtual xObject& getByObject(xObject& o);
	virtual xObject& get(int index) throw(xIndexOutOfBoundsException);
	virtual void set(int index,xObject* o) throw(xIndexOutOfBoundsException);
	virtual void insert(int index,xObject* o) throw(xIndexOutOfBoundsException);
	virtual void removeIndex(int index) throw(xIndexOutOfBoundsException);
	virtual xListIterator* listIterator();
	virtual xIterator* iterator();
	virtual xObject* clone();
	virtual xString toString();
	
	virtual xArray<xObject*> toArray()
	{
		xArray<xObject*> array(size());
		for(int i = 0; i < size(); i++)
			array[i] = m_elementData[i];
		
		return array;	
	}
};


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/**
 * xAvlMap offer O(log(n)) insertion,search and remove. 
 * It uses AVL tree concept. The Objects representing the keys must implement
 * the xComparable interface. 
 */
class XTKAPI xAvlMap : public xMap
{
private:
	struct Node : public xObject
	{
		Node*			left;
		Node*			right;
		xComparable*	key;
		xObject*		data;
		int				height;
		
		Node(xObject* data,xComparable* key,Node* left = NULL,
				Node* right = NULL)
		{
			this->left = left;
			this->right = right;
			this->key = key;
			this->data = data;
			this->height = 0;
		}
	};
	
	class xAvlMapIterator : public xIterator
	{
	protected:
		xArray<Node*> 		m_stack;
		int					m_position;
		bool				m_alreadyRemoved;
		xAvlMap&			m_map;
		
		void buildStack(Node* root);
	public:
		xAvlMapIterator(xAvlMap& map);
	
		virtual ~xAvlMapIterator(){}

		virtual bool hasNext();
		virtual xObject& next() throw(xNoSuchElementException);
		virtual void remove() throw(xIllegalStateException);
	};
	
	Node*		m_pRootNode;
	Node* 		nodeToDeleteWith2Child;
	
	/*
	 * Recursive insert. Return the new root
	 */
	Node* insert(xComparable* compKey,xObject* value,Node* root);
	
	/*
	 * LL rotation
	 */
	Node* LLrotation(Node* root);
	
	/*
	 * LR rotation
	 */
	Node* LRrotation(Node* root);
	
	/*
	 * RR rotation
	 */
	Node* RRrotation(Node* root);
	
	/*
	 * RL rotation
	 */
	Node* RLrotation(Node* root);
	
	/*
	 * Find the maximum between of all nodes < of "node"
	 */
	Node* predecessor(xComparable* key);
	
	/*
	 * return new root
	 */
	Node* remove(xComparable* key,Node* root);
	
	void clear(Node* root);
	
	int getHeightOf(Node* node);
	
	int size(Node* root);
	 
	bool containsValue(Node* root,xObject* value);
	
public:
	xAvlMap();

	virtual ~xAvlMap();
	

	virtual void put(xObject* key, xObject* value) throw(xClassCastException);
	virtual void clear();
	virtual bool containsKey(xObject& key) throw(xClassCastException);
	virtual bool containsValue(xObject& value);   
	virtual xObject& get(xObject& key) throw(xClassCastException);
	virtual bool isEmpty();
	virtual void remove(xObject& key) throw(xClassCastException);
	virtual int size();
	virtual xIterator* iterator();
	virtual xCollection* values();
	
	#ifdef XTK_TEST
		/**
		 * check if the this is a valid AVL Tree
		 */
		bool checkAVL(Node* root);
		
		/**
		 * check if the this is a valid AVL Tree
		 */
		bool checkAVL();
		
		int compare(Node* node1,Node* node2);
	#endif
};



//#define TEST_FIND_GOOD_HASH

/**
 * xHashMap offer O(const) insertion,search and remove. 
 * The Objects representing the keys must implement a good hashCode() function. 
 */
class XTKAPI xHashMap : public xMap
{
private:
	struct Node
	{
		xObject*	data;
		xObject*	key;
		Node*		next;
		
		Node(xObject* data = NULL,xObject* key = NULL,Node* next = NULL)
		{
			this->data = data;
			this->next = next;
			this->key = key;
		}
	};

	class xHashMapIterator : public xIterator
	{
	private:
		xHashMap&		m_map;
		int				m_index;
		Node*			m_currentNode;
		Node*			m_nextNode;
		
		void findNextNode();
	public:
		xHashMapIterator(xHashMap& map);
	
		virtual ~xHashMapIterator(){}
		
		virtual bool hasNext();
		virtual xObject& next() throw(xNoSuchElementException);
		virtual void remove() throw(xIllegalStateException);
	};
	
	
	xArray<Node*>*			 	m_table;
	int							m_count;
	float						m_loadFactor;
	
	inline int hash(xObject* o);
	inline int indexOf(int hash);
	
	/**
	 * Return new root
	 */
	Node* addInList(xObject* data,xObject* key,Node* root);
	Node* delFromList(xObject* key,Node* root);
	
	/**
	 * Return found object or NULL if not found
	 */
	Node* findValInList(xObject* o,Node* root);
	Node* findKeyInList(xObject* o,Node* root);
	
	void rehash();
	
	#ifdef TEST_FIND_GOOD_HASH
		int m_dim1,m_dim2,m_dim3;
	#endif
	
public:

	/**
	 * Constructor.
	 * After the size reach the value of tableSize * loadFactor, the table
	 * will be resized.
	 */
	xHashMap(int initialCapacity = 32,float loadFactor = 0.75f) 
	throw(xIllegalArgumentException);

	virtual ~xHashMap();
	

	virtual void put(xObject* key, xObject* value) throw(xClassCastException);
	virtual void clear();
	virtual bool containsKey(xObject& key) throw(xClassCastException);
	virtual bool containsValue(xObject& value);   
	virtual xObject& get(xObject& key) throw(xClassCastException);
	virtual bool isEmpty();
	virtual void remove(xObject& key) throw(xClassCastException);
	virtual int size();
	virtual xIterator* iterator();
	virtual xCollection* values();
	
	
	#ifdef TEST_FIND_GOOD_HASH
		xHashMap(int dim1,int dim2,int dim3);
	#endif
	
	
	#ifdef XTK_TEST
		int countCollisions();
		float averageObjsForBucket();
		int maxObjsForBucket();
	#endif
};

}//namespace

#endif //XTK_DATASTRUCTURES_H
