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
* @file threadgroup.cpp
* @author Mario Casciaro (xshadow@email.it)
*/


#include "../../include/xtk/base/thread.h"
#include "../../include/xtk/base/smartptr.h"

#ifdef XTK_USE_MULTITHREAD

namespace xtk
{
xThreadGroup	gs_mainThreadGroup(_T("Main Thread Group"),true,NULL);

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xThreadGroup& xThreadGroup::getMainGroup()
{
	return gs_mainThreadGroup;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xThreadGroup::xThreadGroup(xString name,bool autoclear,xThreadGroup* parent)
	: m_name(name)
{
	m_bAutoclear = autoclear;
	m_parent = parent;
	if(m_parent != NULL)
		m_parent->addChild(this);
	
	m_threadList.rescindOwnership();
	m_childs.rescindOwnership();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xThreadGroup::~xThreadGroup()
{
	if(m_parent != NULL)
		m_parent->delChild(this);
		
	terminateAll();
	joinAll();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThreadGroup::joinAll()
{
	
	synchronizeStart();
	
	//first join all subgroups
	smartPtr<xIterator> iter = m_childs.iterator();
	while(iter->hasNext())
		(dynamic_cast<xThreadGroup*>(&(iter->next())))->joinAll();
	
	//we loop until all xThread are finished to run or
	//if the vector become empty
	iter = m_threadList.iterator();
	while(iter->hasNext())
	{
		if((dynamic_cast<xThread*>(&(iter->next())))->isRunning())
		{
			synchronizeEnd();
			xThread::sleep(50);
			synchronizeStart();
			
			//reset the iterator
			iter = m_threadList.iterator();
		}
	}
	
	synchronizeEnd();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThreadGroup::terminateAll()
{
	synchronizeStart();
	
	//first terminate all subgroups
	smartPtr<xIterator> iter = m_childs.iterator();
	while(iter->hasNext())
		(dynamic_cast<xThreadGroup*>(&(iter->next())))->terminateAll();

	iter = m_threadList.iterator();
	while(iter->hasNext())
		dynamic_cast<xThread*>(&(iter->next()))->terminate();

	synchronizeEnd();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThreadGroup::addThread(xThread* thread)
{
	xSynchronized sync(*this);
	m_threadList.add(thread);
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThreadGroup::delThread(xThread* thread)
{
	xSynchronized sync(*this);
	m_threadList.removeObject(*thread);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThreadGroup::addChild(xThreadGroup* child)
{
	xSynchronized sync(*this);
	m_childs.add(child);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThreadGroup::delChild(xThreadGroup* child)
{
	xSynchronized sync(*this);
	m_childs.removeObject(*child);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArray<NODELETE xThread*> xThreadGroup::enumerate()
{
	return m_threadList.toArray().castTo<xThread*>();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThreadGroup::clearInactive()
{
	xSynchronized sync(*this);
	
	smartPtr<xIterator> iter = m_threadList.iterator();
	while(iter->hasNext())
	{
		if(!(dynamic_cast<xThread*>(&iter->next()))->isRunning())
			iter->remove();
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xThreadGroup::getCount()
{
	xSynchronized sync(*this);
	return m_threadList.size();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xThreadGroup::getRunningCount()
{
	xSynchronized sync(*this);
	
	int count = 0;
	smartPtr<xIterator> iter = m_threadList.iterator();
	while(iter->hasNext())
	{
		if((dynamic_cast<xThread*>(&iter->next()))->isRunning())
			count++;
	}
	
	return count;
}


	
}//namespace

#endif //#ifdef XTK_USE_MULTITHREAD
