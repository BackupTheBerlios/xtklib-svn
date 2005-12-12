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
* @file vector.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/datastructures.h"

namespace xtk
{
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArrayList::xArrayList(int initialCapacity ,int capacityIncrementMultiplier)
throw(xIllegalArgumentException)
: m_elementData(initialCapacity)
{
	if(capacityIncrementMultiplier <= 1)
		throw xIllegalArgumentException(_T("capacityIncrement cannot be negative!"));
		
	m_capacityIncrementMultiplier = capacityIncrementMultiplier;
	m_elementCount = 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArrayList::~xArrayList()
{
	clear();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xArrayList::ensureCapacity(int minCapacity)
throw(xIllegalArgumentException)
{
	if(minCapacity > size())
		m_elementData.resize(minCapacity);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xArrayList::add(xObject* o)
{
	if(size() >= m_elementData.size())
	{
		m_elementData.resize((size() + 1) * m_capacityIncrementMultiplier);
	}
	m_elementData[m_elementCount] = o;
	m_elementCount++;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xArrayList::removeObject(xObject& o)
{
	bool beginShift = false;
	for(int i = 0; i < size(); i++)
	{
		if(o.equals(get(i)))
		{
			beginShift = true;
			if(isOwner())
				delete &get(i);
		}
		
		if(beginShift && (i < size() - 1))
			m_elementData[i] = m_elementData[i + 1];
		else if(beginShift && (i == size() - 1))
			m_elementData[i] = NULL;
	}
	if(beginShift)
		m_elementCount--;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xArrayList::isEmpty()
{
	return m_elementCount == 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xArrayList::clear()
{
	if(isOwner())
	{
		for(int i = 0; i < size(); i++)
			delete m_elementData[i];
	}
	
	m_elementData.clear();
	m_elementCount = 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
inline int xArrayList::size()
{
	return m_elementCount;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xArrayList::contains(xObject& o)
{
	for(int i = 0; i < size(); i++)
	{
		if(o.equals(get(i)))
			return true;
	}
	return false;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xArrayList::getByObject(xObject& o)
{
	for(int i = 0; i < size(); i++)
	{
		if(o.equals(get(i)))
			return get(i);
	}
	return xNullObject::getInstance();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xArrayList::get(int index)
throw(xIndexOutOfBoundsException)
{
	return *m_elementData[index];
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xArrayList::set(int index,xObject* o)
throw(xIndexOutOfBoundsException)
{
	if(isOwner() && m_elementData[index] != o)
		delete m_elementData[index];
	m_elementData[index] = o;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xArrayList::insert(int index,xObject* o)
throw(xIndexOutOfBoundsException)
{
	//here we must do coerency test before resizing
	if(index < 0 || index >= size())
		throw xIndexOutOfBoundsException();
	
	//resize if necessary
	if(size() >= m_elementData.size())
	{
		m_elementData.resize(size() * m_capacityIncrementMultiplier);
	}
	
	//shift forward elements
	for(int i = index;i < size() + 1;i++)
	{
		m_elementData[i + 1] = m_elementData[i];
	}
	m_elementData[index] = o;
	m_elementCount++;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xArrayList::removeIndex(int index)
throw(xIndexOutOfBoundsException)
{
	if(index < 0 || index >= size())
		throw xIndexOutOfBoundsException();
	
	if(isOwner())
		delete &get(index);
	
	//shift backward elements
	for(int i = index;i < size();i++)
	{	
		if(i < size() - 1)
			m_elementData[i] = m_elementData[i + 1];
		else if(i == size() - 1)
			m_elementData[m_elementCount - 1] = NULL;
	}
	m_elementCount--;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xArrayList::trimToSize()
{
	m_elementData.resize(size());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject* xArrayList::clone()
{
	xArrayList* list = new xArrayList(size());
	list->rescindOwnership();
	
	for(int i = 0;i < size();i++)
	{
		list->add(&get(i));	
	}
	
	return list;
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xArrayList::toString()
{
	xString string;
	for(int i = 0;i < size();i++)
	{		
		xString str = get(i).toString();
		string.appendFormat(_T("[%d]"),i);
		string.append(str);
		string.append(_T(" , "));	
	}
	return string;
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xIterator* xArrayList::iterator()
{
	return new xArrayListIterator(*this);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xListIterator* xArrayList::listIterator()
{
	return new xArrayListListIterator(*this);
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
xArrayList::xArrayListIterator::xArrayListIterator(xArrayList& vector) : m_list(vector)
{
	m_currentPosition = -1;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xArrayList::xArrayListIterator::hasNext()
{
	return (m_currentPosition + 1) < m_list.size();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xArrayList::xArrayListIterator::next()
throw(xNoSuchElementException)
{
	if(!hasNext())
		throw xNoSuchElementException();
		
	m_currentPosition++;
	m_LastReturnedElementRemoved = false;
	return  m_list.get(m_currentPosition);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xArrayList::xArrayListIterator::remove()
throw(xIllegalStateException)
{
	if(m_currentPosition < 0 || m_LastReturnedElementRemoved)
		throw xIllegalStateException();
		
	 m_list.removeIndex(m_currentPosition);
	m_currentPosition--;
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
bool xArrayList::xArrayListListIterator::hasPrevious()
{
	return (m_currentPosition - 1) >= 0;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xArrayList::xArrayListListIterator::previous()
throw(xNoSuchElementException)
{
	if(!hasPrevious())
		throw xNoSuchElementException();
		
	m_currentPosition--;
	m_LastReturnedElementRemoved = false;
	return  m_list.get(m_currentPosition);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xArrayList::xArrayListListIterator::set(xObject* o)
throw(xIllegalStateException)
{
	if(m_currentPosition < 0 || m_LastReturnedElementRemoved)
		throw xIllegalStateException();
		
	 m_list.set(m_currentPosition,o);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArrayList::xArrayListListIterator::xArrayListListIterator(xArrayList& vector) : m_list(vector)
{
	m_currentPosition = -1;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xArrayList::xArrayListListIterator::hasNext()
{
	return (m_currentPosition + 1) <  m_list.size();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject& xArrayList::xArrayListListIterator::next()
throw(xNoSuchElementException)
{
	if(!hasNext())
		throw xNoSuchElementException();
		
	m_currentPosition++;
	m_LastReturnedElementRemoved = false;
	return  m_list.get(m_currentPosition);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xArrayList::xArrayListListIterator::remove()
throw(xIllegalStateException)
{
	if(m_currentPosition < 0 || m_LastReturnedElementRemoved)
		throw xIllegalStateException();
		
	m_list.removeIndex(m_currentPosition);
	m_currentPosition--;
}



}//end namespace
