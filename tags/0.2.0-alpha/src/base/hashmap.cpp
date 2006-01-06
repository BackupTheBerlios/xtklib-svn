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
* @file hashmap.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/base/datastructures.h"
#include "../../include/xtk/base/math.h"
#include "../../include/xtk/base/smartptr.h"

namespace xtk
{
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xHashMap::xHashMap(int initialCapacity,float loadFactor)
throw(xIllegalArgumentException)
{
	m_table = new xArray<Node*>(initialCapacity);
	m_table->initElements(NULL);
	m_loadFactor = loadFactor;
	m_count = 0;
}

#ifdef TEST_FIND_GOOD_HASH
	xHashMap::xHashMap(int dim1,int dim2,int dim3)
	{
		m_table = new xArray<Node*>(32);
		m_table->initElements(NULL);
		m_loadFactor = 0.75f;
		m_count = 0;
		m_dim1 = dim1;
		m_dim2 = dim2;
		m_dim3 = dim3;
	}
#endif

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xHashMap::~xHashMap()
{
	clear();
	delete m_table;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xHashMap::put(xObject* key, xObject* value)
throw(xClassCastException)
{
	int index = indexOf(hash(key));
	(*m_table)[index] = addInList(value,key,(*m_table)[index]);
	if((m_table->size() * m_loadFactor) < m_count)
		rehash();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xHashMap::clear()
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

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xHashMap::containsKey(xObject& key)
throw(xClassCastException)
{
	int index = indexOf(hash(&key));
	if(findKeyInList(&key,(*m_table)[index]) != NULL)
		return true;
	return false;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xHashMap::containsValue(xObject& value)
{
	for(register int i = 0; i < m_table->size();i++)
	{
		if((*m_table)[i] != NULL)
		{
			if(findValInList(&value,(*m_table)[i]) != NULL)
				return true;
		}
	}	
	return false;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xHashMap::get(xObject& key)
throw(xClassCastException)
{
	int index = indexOf(hash(&key));
	Node* found = findKeyInList(&key,(*m_table)[index]);
	if(found != NULL)
		return *(found->data);
		
	return xNullObject::getInstance();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xHashMap::isEmpty()
{
	return m_count == 0;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xHashMap::remove(xObject& key)
throw(xClassCastException)
{
	int index = indexOf(hash(&key));
	(*m_table)[index] = delFromList(&key,(*m_table)[index]);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xHashMap::size()
{
	return m_count;	
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xHashMap::Node* xHashMap::addInList(xObject* data,xObject* key,Node* root)
{
	if(root == NULL)
	{
		m_count++;
		return new Node(data,key);
	}
	else
	{
		Node* tmp = root;
		for(;;)
		{
			if(tmp->next == NULL)
			{
				tmp->next = new Node(data,key);
				m_count++;
				return root;
			}
			tmp = tmp->next;
		}
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xHashMap::Node* xHashMap::delFromList(xObject* key,Node* root)
{
	Node* tmp = root;
	Node* prev = NULL;
	while(tmp != NULL)
	{
		if(tmp->key->equals(*key))
		{
			Node* tmpNext = tmp->next;
			if(isOwner())
			{
				if(tmp->data != tmp->key)
					delete tmp->key;
				delete tmp->data;
			}
			else
			{
				if(tmp->data != tmp->key)
					delete tmp->key;
			}
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

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xHashMap::Node* xHashMap::findValInList(xObject* o,Node* root)
{
	Node* tmp = root;
	while(tmp != NULL)
	{
		if(tmp->data->equals(*o))
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xHashMap::Node* xHashMap::findKeyInList(xObject* o,Node* root)
{
	Node* tmp = root;
	while(tmp != NULL)
	{
		if(tmp->key->equals(*o))
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
inline int xHashMap::hash(xObject* o)
{
	int h =  o->hashCode();
	
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

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
inline int xHashMap::indexOf(int hash)
{
	return hash & (m_table->size() - 1);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xHashMap::rehash()
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

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#ifdef XTK_TEST
	int xHashMap::countCollisions()
	{
		int count = 0;
		for(register int i = 0;i < m_table->size();i++)
		{
			Node* currentNode = (*m_table)[i];
			while(currentNode != NULL)
			{
				if(currentNode != (*m_table)[i])
					count++;
				currentNode = currentNode->next;
			}
		}
		return count;
	}
	
	
	float xHashMap::averageObjsForBucket()
	{
		int countBucket = 0;
		for(register int i = 0;i < m_table->size();i++)
		{
			if((*m_table)[i] != NULL)
				countBucket++;
		}
		return ((float)m_count / (float)countBucket);
	}
	
	
	int xHashMap::maxObjsForBucket()
	{
		int maxBucket = 0;
		for(register int i = 0;i < m_table->size();i++)
		{
			Node* currentNode = (*m_table)[i];
			int objs = 0;
			while(currentNode != NULL)
			{
				objs++;
				currentNode = currentNode->next;
			}
			if(objs > maxBucket)
				maxBucket = objs;
		}
		return maxBucket;
	}

#endif //XTK_TEST

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xIterator* xHashMap::iterator()
{
	return new xHashMapIterator(*this);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xCollection* xHashMap::values()
{
	xArrayList* list = new xArrayList(size());
	list->rescindOwnership();
	
	smartPtr<xIterator> iter = iterator();
	while(iter->hasNext())
	{
		list->add(&(iter->next()));	
	}
	return list;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xHashMap::xHashMapIterator::xHashMapIterator(xHashMap& map) : m_map(map)
{
	m_index = -1;
	m_currentNode = NULL;
	m_nextNode = NULL;
	findNextNode();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xHashMap::xHashMapIterator::findNextNode()
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

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xHashMap::xHashMapIterator::hasNext()
{
	return m_nextNode != NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xHashMap::xHashMapIterator::next()
throw(xNoSuchElementException)
{
	if(!hasNext())
		throw xNoSuchElementException();
		
	xObject* o = m_nextNode->data;
	m_currentNode = m_nextNode;
	findNextNode();
	return *o;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xHashMap::xHashMapIterator::remove()
throw(xIllegalStateException)
{
	if(m_currentNode == NULL)
		throw xIllegalStateException();
		
	m_map.remove(*(m_currentNode->key));
	m_currentNode = NULL;
}



}//namespace
