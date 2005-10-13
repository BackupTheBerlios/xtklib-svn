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
* @file widget.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_WIDGET_H
#define XTK_WIDGET_H

#include "../globals.h"
#include "widgetevent.h"
#include "widgeteventhandler.h"
#include "widgeteventgenerator.h"
#include "font.h"
#include "../base/color.h"
#include "../base/geometry.h"
#include "../base/datastructures.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

//forward
class xWidgetInternal;

/**
 * Process the next message of the windowing system message or event queue.
 * The function blocks until a new event are received.
 * Return false if a quit message was received.
 */
XTKAPI bool xtkProcessNextUIEvent();

/**
 *
 */
XTKAPI bool xtkUIEventPending();

/**
 *
 */
XTKAPI bool xtkProcessPendingUIEvent()
{
	while(xtkUIEventPending())
	{
		if(xtkProcessNextUIEvent() == false)
			return false;
	}
	
	return true;
}

/**
 * A widget is the base class for all Gui components.
 * Note that all children of the window will be deleted automatically by 
 * the destructor before the window itself is deleted.
 */
class XTKAPI xWidget : public xMouseEventGenerator,public virtual xObject
{
private:
	xWidgetInternal*	m_internal;
	friend class xWidgetInternal;
	
protected:
	xWidget(xWidget* parent,xWidgetInternal* i);
	
	virtual void processEvent(xWidgetEvent& ev);
	virtual void addEventHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh,xWidgetEventID evmask);
	virtual void removeEventHandler(xWidgetEventHandlerBase& evh,xWidgetEventID evmask);
public:
	enum Defaults
	{
		XTK_DEFAULT_WIDGET_SIZE = -1,
		XTK_DEFAULT_WIDGET_POSITION = -1, 
	};

	virtual ~xWidget();

	/**
	 * Checks whether this component "contains" the specified point, 
	 * where x and y are defined to be relative to the coordinate system of this component.
	 */
	virtual bool contains(int x, int y);
	
	/**
	 * Prompts the layout manager to lay out this component.
	 */
	virtual void doLayout();
	
	/**
	 * Gets the background color of this component.
	 */
	virtual xColor* getBackground();
	
	/**
	 * Gets the bounds of this component in the form of a Rectangle object.
	 */
	virtual xRectangle* getBounds();
	
	/**
	 * Determines if this component or one of its immediate subcomponents 
	 * contains the (x, y) location, and if so, returns the containing component.
	 */
	virtual MYOWNERSHIP xWidget* getComponentAt(int x, int y);
	
	/**
	 * Gets the font of this component.
	 */	
	virtual xFont* getFont();
	
	/**
	* Gets the font of this component.
	*/	
	virtual xFontMetrics* getFontMetrics();
	
	/**
	 * Gets the foreground color of this component.
	 */
	virtual xColor* getForeground();
	
	/**
	 * Returns the current height of this component.
	 */
	virtual int getHeight();
	
	xWidgetInternal* getInternal()
	{return m_internal;}
	
	/**
	 * 
	 */
	virtual bool getIgnoreRepaint();
		
	/**
	 * Gets the location of this component in the form of a point specifying the component's 
	 * top-left corner.
	 */
	virtual xPoint* getLocation();
	
	/**
	 * Gets the location of this component in the form of a point specifying the 
	 * component's top-left corner in the screen's coordinate space.
	 */
	virtual xPoint* getLocationOnScreen();
	
	/**
	 * Gets the parent of this widget.NULL if there are no parent.
	 */
	virtual MYOWNERSHIP xWidget* getParent();
		
	/**
	 * Returns the size of this component in the form of a Dimension object.
	 */
	virtual xDimension* getSize();
		
	/**
	 * Returns the current width of this component.
	 */
	virtual int getWidth();
	
	/**
	 * Returns the current x coordinate of the components origin.
	 */
	virtual int getX();
	
	/**
	 * Returns the current y coordinate of the components origin.
	 */
	virtual int getY();
		
	/**
	 * Invalidates this component.
	 */
	virtual void invalidate();
		
	/**
	 * Determines whether this component is enabled.
	 */
	virtual bool isEnabled();
	
	/**
	 * Returns whether this Component can be focused.
	 */
	virtual bool isFocusable();
	
	/**
	 * Returns true if this Component is the focus owner.
	 */
	virtual bool isFocusOwner();
		
	/**
	 * Determines whether this component is showing on screen.
	 */
	virtual bool isShowing();
	
	/**
	 * Determines whether this component is valid.
	 */
	virtual bool isValid();
	
	/**
	 * Determines whether this component should be visible when its parent is visible.
	 */
	virtual bool isVisible();
		
	/**
	 * Requests that this Widget get the input focus, and that this Widget's 
	 * top-level ancestor become the focused Window.
	 */
	virtual void requestFocus();
		
	/**
	 * Sets the background color of this component.
	 */
	virtual void setBackground(xColor& c);
	
	/**
	 * Moves and resizes this component.
	 */
	virtual void setBounds(int x, int y, int width, int height);
	
	/**
	 * Moves and resizes this component to conform to the new bounding rectangle r.
	 */
	virtual void setBounds(xRectangle& r);
		
	/**
	 * Enables or disables this component, depending on the value of the parameter b.
	 */
	virtual void setEnabled(bool b);
		
	/**
	 * Sets the font of this component.
	 */
	virtual void setFont(xFont& f);
	
	/**
	 * Sets the foreground color of this component.
	 */
	virtual void setForeground(xColor& c);
	
	/**
	 * Moves this component to a new location.
	 */
	virtual void setLocation(int x, int y);

	/**
	 * Resizes this component so that it has width width and height height.
	 */
	virtual void setSize(int width, int height);
	
	/**
	 * Shows or hides this component depending on the value of parameter b.
	 */
	virtual void setVisible(boolean b);
		
	/**
	* Returns a string representation of this component and its values.
	*/
	//virtual xString toString() = 0;
	
	/**
	 * Ensures that this component has a valid layout.
	 */
	virtual void validate();
};




}//namespace

#endif //XTK_USE_WIDGETS
#endif //XTK_WIDGET_H
