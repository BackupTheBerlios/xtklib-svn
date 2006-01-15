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
* @file linkedlist.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/base/datastructures.h"

namespace xtk
{
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xLinkedList::xLinkedList()
{
	m_elementCount = 0;	
	m_pFirstNode = NULL;
	m_pLastNode = NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLinkedList::add(xObject* o)
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

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLinkedList::executeRemove(Node* toRemove)
{
	if(toRemove->previous != NULL)
		toRemove->previous->next = toRemove->next;
	else
		m_pFirstNode = toRemove->next;
	
	
	if(toRemove->next != NULL)
		toRemove->next->previous = toRemove->previous;
	else
		m_pLastNode = toRemove->previous;
		
	if(isOwner())
		delete toRemove->data;
	delete toRemove;
	
	m_elementCount--;
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLinkedList::removeObject(xObject& o)
{
	Node* tmp = m_pFirstNode;
	while(tmp != NULL)
	{
		if(tmp->data->equals(o))
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


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLinkedList::removeFirst()
{
	if(m_pFirstNode != NULL)
		executeRemove(m_pFirstNode);
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLinkedList::removeLast()
{
	if(m_pLastNode != NULL)
		executeRemove(m_pLastNode);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xLinkedList::getFirst()
{
	if(m_pFirstNode == NULL)
		throw xNoSuchElementException();
		
	return *(m_pFirstNode->data);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xLinkedList::getLast()
{
	if(m_pLastNode == NULL)
		throw xNoSuchElementException();
		
	return *(m_pLastNode->data);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xLinkedList::isEmpty()
{
	return m_pFirstNode == NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLinkedList::clear()
{
	Node* tmp = m_pFirstNode;
	while(tmp != NULL)
	{
		Node* tmp2 = tmp->next;
		if(isOwner())
			delete tmp->data;
		delete tmp;
		
		tmp = tmp2;
	}
	m_pFirstNode = NULL;
	m_pLastNode = NULL;
	m_elementCount = 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xLinkedList::size()
{
	return m_elementCount;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xLinkedList::contains(xObject& o)
{
	Node* tmp = m_pFirstNode;
	while(tmp != NULL)
	{
		if(tmp->data->equals(o))
			return true;
		tmp = tmp->next;
	}
	
	return false;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xLinkedList::getByObject(xObject& o)
{
	Node* tmp = m_pFirstNode;
	while(tmp != NULL)
	{
		if(tmp->data->equals(o))
			return *(tmp->data);
		tmp = tmp->next;
	}

	return xNullObject::getInstance();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xLinkedList::get(int index)
throw(xIndexOutOfBoundsException)
{
	if(index < 0 || index >= m_elementCount)
		throw xIndexOutOfBoundsException();
		
	Node* tmp = m_pFirstNode;
	for(int i = 0; i != index ;i++)
	{
		tmp = tmp->next;
	}
	
	return *(tmp->data);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLinkedList::set(int index,xObject* o)
throw(xIndexOutOfBoundsException)
{
	if(index < 0 || index >= m_elementCount)
		throw xIndexOutOfBoundsException();
	
	Node* tmp = m_pFirstNode;
	for(int i = 0; i != index ;i++)
	{
		tmp = tmp->next;
	}
	
	if(isOwner())
		delete tmp->data;
	
	tmp->data = o;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLinkedList::insert(int index,xObject* o)
throw(xIndexOutOfBoundsException)
{
	if(index < 0 || index >= m_elementCount)
		throw xIndexOutOfBoundsException();
		
	Node* tmp = m_pFirstNode;
	for(int i = 0; i != index ;i++)
	{
		tmp = tmp->next;
	}
	
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

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLinkedList::removeIndex(int index)
throw(xIndexOutOfBoundsException)
{
	if(index < 0 || index >= m_elementCount)
		throw xIndexOutOfBoundsException();
	
	Node* tmp = m_pFirstNode;
	for(int i = 0; i != index ;i++)
	{
		tmp = tmp->next;
	}
	executeRemove(tmp);
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject* xLinkedList::clone()
{
	xLinkedList* list = new xLinkedList();
	Node* tmp = m_pFirstNode;
	while(tmp != NULL)
	{
		list->add(tmp->data);
	}
	
	return list;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xLinkedList::toString()
{
	xString string;
	Node* tmp = m_pFirstNode;
	for(int i = 0;tmp != NULL;i++)
	{
		xString str = tmp->toString();
		string.appendFormat(_T("[%d]"),i);
		string.append(str);
		string.append(_T(" , "));
		tmp = tmp->next;
	}
	return string;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xListIterator* xLinkedList::listIterator()
{
	return new xLinkedListListIterator(*this);	
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xIterator* xLinkedList::iterator()
{
	return new xLinkedListIterator(*this);		
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
xLinkedList::xLinkedListIterator::xLinkedListIterator(xLinkedList& list)
{
	m_pNextNode = list.m_pFirstNode;
	m_pCurrentNode = NULL;
	m_list = &list;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xLinkedList::xLinkedListIterator::hasNext()
{
	return m_pNextNode != NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xLinkedList::xLinkedListIterator::next()
throw(xNoSuchElementException)
{
	if(!hasNext())
		throw xNoSuchElementException();
		
	m_pCurrentNode = m_pNextNode;
	m_pNextNode = m_pNextNode->next;
	return *(m_pCurrentNode->data);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLinkedList::xLinkedListIterator::remove()
throw(xIllegalStateException)
{
	if(m_pCurrentNode == NULL)
		throw xIllegalStateException();
		
	m_list->executeRemove(m_pCurrentNode);
	m_pCurrentNode = NULL;
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
xLinkedList::xLinkedListListIterator::xLinkedListListIterator(xLinkedList& list)
{
	m_list = &list;
	m_pNextNode = m_list->m_pFirstNode;
	m_pPreviousNode = NULL;
	m_pCurrentNode = NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xLinkedList::xLinkedListListIterator::hasPrevious()
{
	return m_pPreviousNode != NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xLinkedList::xLinkedListListIterator::previous()
throw(xNoSuchElementException)
{
	if(!hasPrevious())
		throw xNoSuchElementException();
	
	m_pCurrentNode = m_pPreviousNode;
	m_pNextNode = m_pCurrentNode->next;
	m_pPreviousNode = m_pCurrentNode->previous;
	
	return *(m_pCurrentNode->data);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLinkedList::xLinkedListListIterator::set(xObject* o)
throw(xIllegalStateException)
{
	if(m_list->isOwner())
		delete m_pCurrentNode->data;
		
	m_pCurrentNode->data = o;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xLinkedList::xLinkedListListIterator::hasNext()
{
	return m_pNextNode != NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xLinkedList::xLinkedListListIterator::next()
throw(xNoSuchElementException)
{
	if(!hasNext())
		throw xNoSuchElementException();
	
	m_pCurrentNode = m_pNextNode;
	m_pPreviousNode = m_pCurrentNode->previous;
	m_pNextNode = m_pCurrentNode->next;
	
	return *(m_pCurrentNode->data);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLinkedList::xLinkedListListIterator::remove()
throw(xIllegalStateException)
{
	if(m_pCurrentNode == NULL)
		throw xIllegalStateException();
		
	m_list->executeRemove(m_pCurrentNode);
}


}//namespace
