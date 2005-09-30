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
* @file widgetlisteners.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_WIDGETLISTENERS_H
#define XTK_WIDGETLISTENERS_H

#include "../globals.h"
#include "widgetevent.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

/**
 * A tagging class that all event listener interfaces must extend.
 */
class XTKAPI xWidgetListener : public virtual xObject
{
public:
	virtual ~xWidgetListener(){}
};


/**
 * The listener interface for receiving component events.
 */
class XTKAPI xComponentListener : public xWidgetListener
{	
public:
	virtual ~xComponentListener(){}
	
	/**
	 * Invoked when the component has been made invisible.
	 */
	virtual void componentHidden(xComponentEvent& e){}
		
	/**
	 * Invoked when the component's position changes.
	 */
	virtual void componentMoved(xComponentEvent& e){}
		
	/**
	 * Invoked when the component's size changes.
	 */
	virtual void componentResized(xComponentEvent& e){}
		
	/**
	 * Invoked when the component has been made visible.
	 */
	virtual void componentShown(xComponentEvent& e){}
};



/**
 * The listener interface for receiving action events.
 */
class XTKAPI xActionListener : public xWidgetListener
{	
public:
	virtual ~xActionListener(){}

	/**
	* Invoked when an action occurs.
	*/
	virtual void actionPerformed(xActionEvent& e){}
};



/**
* The listener interface for receiving container events.
*/
class XTKAPI xContainerListener : public xWidgetListener
{	
public:
	virtual ~xContainerListener(){}

	/**
	 * Invoked when a component has been added to the container.
	 */
	virtual void componentAdded(xContainerEvent& e){}
		
	/**
	 * Invoked when a component has been removed from the container.
	 */
	virtual void componentRemoved(xContainerEvent& e){}
};


/**
* The listener interface for receiving keyboard events (keystrokes).
*/
class XTKAPI xKeyListener : public xWidgetListener
{	
public:
	virtual ~xKeyListener(){}

	/**
	 * Invoked when a key has been pressed.
	 */
	virtual void keyPressed(xKeyEvent& e){}
		
	/**
	 * Invoked when a key has been released.
	 */
	virtual void keyReleased(xKeyEvent& e){}
	
	/**
	 * Invoked when a key has been typed.
	 */
	virtual void keyTyped(xKeyEvent& e){}
};



/**
* The listener interface for receiving mouse events.
*/
class XTKAPI xMouseListener : public xWidgetListener
{	
public:
	virtual ~xMouseListener(){}

	/**
	 * Invoked when the mouse button has been clicked (pressed and released) on a component.
	 */
	virtual void mouseClicked(xMouseEvent& e){}
	
	/**
	* Invoked when the mouse button has been double clicked on a component.
	*/
	virtual void mouseDoubleClicked(xMouseEvent& e){}
		
	/**
	 * Invoked when a mouse button has been pressed on a component.
	 */
	virtual void mousePressed(xMouseEvent& e){}
		
	/**
	 * Invoked when a mouse button has been released on a component.
	 */
	virtual void mouseReleased(xMouseEvent& e){}
		
};


/**
* The listener interface for receiving mouse motion events.
*/
class XTKAPI xMouseMotionListener : public xWidgetListener
{	
public:
	virtual ~xMouseMotionListener(){}

	/**
	 * Invoked when a mouse button is pressed on a component and then dragged.
	 */
	virtual void mouseDragged(xMouseEvent& e){}
		
	/**
	 * Invoked when the mouse cursor has been moved onto a component but no buttons have been pushed.
	 */
	virtual void mouseMoved(xMouseEvent& e){}
};



/**
* The listener interface for receiving focus events.
*/
class XTKAPI xFocusListener : public xWidgetListener
{	
public:
	virtual ~xFocusListener(){}

	/**
	 * Invoked when a component gains the keyboard focus.
	 */
	virtual void focusGained(xFocusEvent& e){}
		
	/**
	 * Invoked when a component loses the keyboard focus.
	 */
	virtual void focusLost(xFocusEvent& e){}
};



/**
* The listener interface for receiving window events.
*/
class XTKAPI xWindowListener : public xWidgetListener
{	
public:
	virtual ~xWindowListener(){}

	/**
	 * Invoked when the Window is set to be the active Window.
	 */
	virtual void windowActivated(xWindowEvent& e){}
	
	/**
	 * Invoked when a window has been closed as the result of calling dispose on the window.
	 */
	virtual void windowClosed(xWindowEvent& e){}
	
	/**
	 * Invoked when the user attempts to close the window from the window's system menu.
	 */
	virtual void windowClosing(xWindowEvent& e){}
	
	/**
	 * Invoked when a Window is no longer the active Window.
	 */
	virtual void windowDeactivated(xWindowEvent& e){}
	
	/**
	 * Invoked when a window is changed from a minimized to a normal state.
	 */
	virtual void windowDeiconified(xWindowEvent& e){}
	
	/**
	 * Invoked when a window is changed from a normal to a minimized state.
	 */
	virtual void windowIconified(xWindowEvent& e){}
	
	/**
	 * Invoked the first time a window is made visible.
	 */
	virtual void windowOpened(xWindowEvent& e){}
	
	/**
	 * 
	 */
	virtual void windowFirst(xWindowEvent& e){}
	
	/**
	 * 
	 */
	virtual void windowLast(xWindowEvent& e){}
};



}//namespace
#endif // XTK_USE_WIDGETS

#endif //XTK_WIDGETLISTENERS_H
