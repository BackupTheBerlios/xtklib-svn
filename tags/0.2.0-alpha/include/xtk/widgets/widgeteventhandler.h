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
* @file widgeteventhandler.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_WIDGETEVENTHANDLER_H
#define XTK_WIDGETEVENTHANDLER_H

#include "../globals.h"
#include "widgetevent.h"
#include "../base/functor.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

/**
 * 
 */
class XTKAPI xWidgetEventHandlerBase : public xFunctor
{
protected:
	xWidgetEventHandlerBase(){}
public:
	virtual ~xWidgetEventHandlerBase(){}
	
	virtual void handleEvent(xWidgetEvent& ev) = 0;
};

/**
 * 
 */
template<class TClass>
class XTKAPI xWidgetEventHandler : public xWidgetEventHandlerBase
{
private:
	void (TClass::*m_fpt)(xWidgetEvent&);		// pointer to member function
	TClass* m_pt2Object;						// pointer to object
	
public:
	xWidgetEventHandler(YOUROWNERSHIP TClass* pt2Object, void(TClass::*fpt)(xWidgetEvent&))
	{m_pt2Object = pt2Object;  m_fpt = fpt;}
	
	virtual ~xWidgetEventHandler(){}

	virtual void handleEvent(xWidgetEvent& ev)
	{(*m_pt2Object.*m_fpt)(ev);}
	
	virtual bool equals(xObject& o)
	{
		xWidgetEventHandler<TClass>* c = dynamic_cast<xWidgetEventHandler<TClass>*>(&o);
		if(c == NULL)
			return false;
		return c->m_fpt == m_fpt && m_pt2Object == c->m_pt2Object;
	}
};


}//namespace
#endif // XTK_USE_WIDGETS

#endif //XTK_WIDGETEVENTHANDLER_H
