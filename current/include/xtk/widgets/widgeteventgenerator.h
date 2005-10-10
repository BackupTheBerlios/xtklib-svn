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
* @file widgeteventgenerator.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_WIDGETEVENTGENERATOR_H
#define XTK_WIDGETEVENTGENERATOR_H

#include "../globals.h"
#include "../base/string.h"
#include "../base/datastructures.h"
#include "widgetevent.h"
#include "widgetlisteners.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

/**
 * Tagging class for all xWidgetEventGenerator subclasses
 */
class XTKAPI xWidgetEventGenerator : public virtual xObject
{
protected:
	xWidgetEventGenerator(){}
public:
	virtual ~xWidgetEventGenerator(){}
};




/**
* A class able to send xMouseEvent events.
*/
class XTKAPI xMouseEventGenerator : public virtual xWidgetEventGenerator
{
private:
	xLinkedList		m_mouseListeners;
protected:
	xMouseEventGenerator(){m_mouseListeners.rescindOwnership();}
	
	virtual void processMouseEvent(xMouseEvent& e);
public:
	virtual ~xMouseEventGenerator(){}
	
	/**
	* Adds the specified mouse listener to receive mouse events from this generator.
	*/
	virtual void addMouseListener(YOUROWNERSHIP xMouseListener* l)
	{m_mouseListeners.add(l);}
	
	virtual xArray<NODELETE xMouseListener*> getMouseListeners()
	{return m_mouseListeners.toArray().castTo<xMouseListener*>();}
	
	virtual void removeMouseListener(xMouseListener& l)
	{m_mouseListeners.removeObject(l);}
};




/**
* A class able to send xActionEvent events.
*/
class XTKAPI xActionEventGenerator : public virtual xWidgetEventGenerator
{
private:
	xLinkedList		m_actionListeners;
	xString			m_actionCommand;
	
protected:
	xActionEventGenerator(){m_actionListeners.rescindOwnership();}

	virtual void processActionEvent(xActionEvent& e);
public:
	virtual ~xActionEventGenerator(){}

	virtual xString getActionCommand()
	{return m_actionCommand;}
	
	virtual void setActionCommand(xString ac)
	{m_actionCommand = ac;}
	
	/**
	* Adds the specified mouse listener to receive mouse events from this generator.
	*/
	virtual void addActionListener(YOUROWNERSHIP xActionListener* l)
	{m_actionListeners.add(l);}

	virtual xArray<NODELETE xActionListener*> getActionListeners()
	{return m_actionListeners.toArray().castTo<xActionListener*>();}

	virtual void removeActionListener(xActionListener& l)
	{m_actionListeners.removeObject(l);}
};


/**
* A class able to send xFocusEvent events.
*/
class XTKAPI xFocusEventGenerator : public virtual xWidgetEventGenerator
{
private:
	xLinkedList		m_focusListeners;

protected:
	xFocusEventGenerator(){m_focusListeners.rescindOwnership();}

	virtual void processFocusEvent(xFocusEvent& e);
public:
	virtual ~xFocusEventGenerator(){}

	/**
	* Adds the specified mouse listener to receive mouse events from this generator.
	*/
	virtual void addFocusListener(YOUROWNERSHIP xFocusListener* l)
	{m_focusListeners.add(l);}

	virtual xArray<NODELETE xFocusListener*> getFocusListeners()
	{return m_focusListeners.toArray().castTo<xFocusListener*>();}

	virtual void removeFocusListener(xFocusListener& l)
	{m_focusListeners.removeObject(l);}
};

}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_WIDGETEVENTGENERATOR_H