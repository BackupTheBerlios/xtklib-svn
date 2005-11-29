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
* @file widgeteventgenerator_internal.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_WIDGETEVENTGENERATOR_INTERNAL_H
#define XTK_WIDGETEVENTGENERATOR_INTERNAL_H

#include "../../include/xtk/globals.h"
#include "../../include/xtk/widgets/widgeteventgenerator.h"
#include "../../include/xtk/base/datastructures.h"
#include "../../include/xtk/widgets/widgetevent.h"
#include "../../include/xtk/widgets/widgeteventhandler.h"


#ifdef XTK_USE_WIDGETS
namespace xtk
{

/**
 * The superclass for all xWidgetEventGenerator subclasses
 */
class XTKAPI xWidgetEventGeneratorInternal : public virtual xObject
{
private:
	class xWidgetEventHandlerEntry : public xObject
	{
	public:
		xWidgetEventHandlerBase*	m_evtHandler;
		xWidgetEventID				m_evtMask;

		xWidgetEventHandlerEntry(MYOWNERSHIP xWidgetEventHandlerBase* evtHandler,xWidgetEventID	evtMask)
		{m_evtHandler = evtHandler; m_evtMask = evtMask;}

		virtual ~ xWidgetEventHandlerEntry()
		{delete m_evtHandler;}
		
		virtual bool equals(xObject& obj)
		{
			xWidgetEventHandlerEntry* c = static_cast<xWidgetEventHandlerEntry*>(&obj);
			return c->m_evtHandler->equals(*m_evtHandler) && c->m_evtMask == m_evtMask;
		}
	};
	
	xLinkedList m_evtHandlers;
protected:
	xWidgetEventGeneratorInternal(){m_evtHandlers.giveOwnership();}
	
public:
	void processEvent(xWidgetEvent& ev);

	void addEventHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh,xWidgetEventID evmask)
	{m_evtHandlers.add(new xWidgetEventHandlerEntry(evh,evmask));}

	void removeEventHandler(xWidgetEventHandlerBase& evh,xWidgetEventID evmask)
	{xWidgetEventHandlerEntry tmp(&evh,evmask);m_evtHandlers.removeObject(tmp);}

	virtual ~xWidgetEventGeneratorInternal(){}
};


}//namespace
#endif //XTK_USE_WIDGETS

#endif //XTK_WIDGETEVENTGENERATOR_INTERNAL_H
