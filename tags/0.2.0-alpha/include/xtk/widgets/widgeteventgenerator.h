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
* @file widgeteventgenerator.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_WIDGETEVENTGENERATOR_H
#define XTK_WIDGETEVENTGENERATOR_H

#include "../globals.h"
#include "../base/string.h"
#include "../base/datastructures.h"
#include "widgetevent.h"
#include "widgeteventhandler.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

/**
 * The superclass for all xWidgetEventGenerator subclasses
 */
class XTKAPI xWidgetEventGenerator : public virtual xObject
{
protected:
	virtual void processEvent(xWidgetEvent& ev) = 0;
	virtual void addEventHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh,xWidgetEventID evmask) = 0;
	virtual void removeEventHandler(xWidgetEventHandlerBase& evh,xWidgetEventID evmask) = 0;
	
	xWidgetEventGenerator(){}
public:
	virtual ~xWidgetEventGenerator(){}
};




/**
* A class able to send xMouseEvent events.
*/
class XTKAPI xMouseEventGenerator : public virtual xWidgetEventGenerator
{
protected:
	xMouseEventGenerator(){}
	
public:
	virtual ~xMouseEventGenerator(){}
	
	/**
	 * 
	 */
	virtual void addMouseClickedHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh)
	{addEventHandler(evh,XWE_MOUSE_CLICKED);}
	
	virtual void removeMouseClickedHandler(xWidgetEventHandlerBase& l)
	{removeEventHandler(l,XWE_MOUSE_CLICKED);}
	
	/**
	 * 
	 */
	virtual void addMouseDoubleClickedHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh)
	{addEventHandler(evh,XWE_MOUSE_DOUBLECLICKED);}

	virtual void removeMouseDoubleClickedHandler(xWidgetEventHandlerBase& l)
	{removeEventHandler(l,XWE_MOUSE_DOUBLECLICKED);}
	
	/**
	 * 
	 */
	virtual void addMousePressedHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh)
	{addEventHandler(evh,XWE_MOUSE_PRESSED);}

	virtual void removeMousePressedHandler(xWidgetEventHandlerBase& l)
	{removeEventHandler(l,XWE_MOUSE_PRESSED);}
	
	/**
	 * 
	 */
	virtual void addMouseReleasedHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh)
	{addEventHandler(evh,XWE_MOUSE_RELEASED);}

	virtual void removeMouseReleasedHandler(xWidgetEventHandlerBase& l)
	{removeEventHandler(l,XWE_MOUSE_RELEASED);}
};




/**
* A class able to send xActionEvent events.
*/
class XTKAPI xActionEventGenerator : public virtual xWidgetEventGenerator
{
protected:
	xActionEventGenerator(){}

public:
	virtual ~xActionEventGenerator(){}
	
	/**
	* 
	*/
	virtual void addActionPerformedHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh)
	{addEventHandler(evh,XWE_ACTION_PERFORMED);}

	virtual void removeActionPerformedHandler(xWidgetEventHandlerBase& l)
	{removeEventHandler(l,XWE_ACTION_PERFORMED);}
};


/**
* A class able to send xFocusEvent events.
*/
class XTKAPI xFocusEventGenerator : public virtual xWidgetEventGenerator
{
protected:
	xFocusEventGenerator(){}

public:
	virtual ~xFocusEventGenerator(){}

	/**
	* 
	*/
	virtual void addFocusGainedHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh)
	{addEventHandler(evh,XWE_FOCUS_GAINED);}

	virtual void removeFocusGainedHandler(xWidgetEventHandlerBase& l)
	{removeEventHandler(l,XWE_FOCUS_GAINED);}
	
	/**
	* 
	*/
	virtual void addFocusLostHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh)
	{addEventHandler(evh,XWE_FOCUS_LOST);}

	virtual void removeFocusLostHandler(xWidgetEventHandlerBase& l)
	{removeEventHandler(l,XWE_FOCUS_LOST);}
};


}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_WIDGETEVENTGENERATOR_H
