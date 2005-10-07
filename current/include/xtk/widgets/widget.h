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
#include "widgetlisteners.h"
#include "font.h"
#include "../base/color.h"
#include "../base/geometry.h"
#include "../base/datastructures.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

//forward
class xWidget;
class xContainer;

/**
* Process the next message of the windowing system message or event queue;
* Return false if a quit message was received or if your
* program is a console application.
*/
XTKAPI bool xtkProcessNextUIMessage();


/**
 * Defines the interface for the various implementation of class xWidget.
 */
class XTKAPI xIWidget
{
protected:
	
	 
	xIWidget(xContainer* parent){}
public:
	enum Defaults
	{
		XTK_DEFAULT_WIDGET_SIZE = -1,
		XTK_DEFAULT_WIDGET_POSITION = -1, 
	};

	virtual ~xIWidget(){}

	/*  ???
	 * Adds the specified mouse wheel listener to receive mouse wheel events from this 
	 * component.
	virtual void addMouseWheelListener(xMouseWheelListener* l) = 0;	
	*/
		
	/*   ???
	void 	applyComponentOrientation(ComponentOrientation orientation)
	Sets the ComponentOrientation property of this component and all components contained within it.
	boolean 	areFocusTraversalKeysSet(int id)
	Returns whether the Set of focus traversal keys for the given focus traversal operation has been explicitly defined for this Component.
	*/
	
	
	/**
	 * Checks whether this component "contains" the specified point, 
	 * where x and y are defined to be relative to the coordinate system of this component.
	 */
	virtual bool contains(int x, int y) = 0;

	
	/**
	 * Prompts the layout manager to lay out this component.
	 */
	virtual void doLayout() = 0;
		
	
	/* ???
	float 	getAlignmentX()
	Returns the alignment along the x axis.
	float 	getAlignmentY()
	Returns the alignment along the y axis.
	*/
	
	/**
	 * Gets the background color of this component.
	 */
	virtual xColor* getBackground() = 0;
	
	/**
	 * Gets the bounds of this component in the form of a Rectangle object.
	 */
	virtual xRectangle* getBounds() = 0;
	
	/**
	 * Determines if this component or one of its immediate subcomponents 
	 * contains the (x, y) location, and if so, returns the containing component.
	 */
	virtual NODELETE xWidget* getComponentAt(int x, int y) = 0;
		
	/* ???
	Cursor 	getCursor()
	Gets the cursor set in the component.
	DropTarget 	getDropTarget()
	Gets the DropTarget associated with this Component.
	Container 	getFocusCycleRootAncestor()
	Returns the Container which is the focus cycle root of this Component's focus traversal cycle.
	*/
		
		
	/* ??
	Set 	getFocusTraversalKeys(int id)
	Returns the Set of focus traversal keys for a given traversal operation for this Component.
	boolean 	getFocusTraversalKeysEnabled()
	Returns whether focus traversal keys are enabled for this Component.
	boolean 	getFocusTraversalKeysEnabled()
	Returns whether focus traversal keys are enabled for this Component.
	*/
	
	/**
	 * Gets the font of this component.
	 */	
	virtual xFont* getFont() = 0;
	
	/**
	* Gets the font of this component.
	*/	
	virtual xFontMetrics* getFontMetrics() = 0;
	
	/**
	 * Gets the foreground color of this component.
	 */
	virtual xColor* getForeground() = 0;
	
	/* ???
	xGraphics getGraphics()
	Creates a graphics context for this component.
	GraphicsConfiguration 	getGraphicsConfiguration()
	Gets the GraphicsConfiguration associated with this Component.
	*/
	
	/**
	 * Returns the current height of this component.
	 */
	virtual int getHeight() = 0;
	
	/* ??
	HierarchyBoundsListener[] 	getHierarchyBoundsListeners()
	Returns an array of all the hierarchy bounds listeners registered on this component.
	HierarchyListener[] 	getHierarchyListeners()
	Returns an array of all the hierarchy listeners registered on this component.
	*/
	
	/**
	 * 
	 */
	virtual bool getIgnoreRepaint() = 0;
		
	/**
	 * Gets the location of this component in the form of a point specifying the component's 
	 * top-left corner.
	 */
	virtual xPoint* getLocation() = 0;
	
	/**
	 * Gets the location of this component in the form of a point specifying the 
	 * component's top-left corner in the screen's coordinate space.
	 */
	virtual xPoint* getLocationOnScreen() = 0;
	
	/* ??
	xDimension 	getMaximumSize()
	Gets the maximum size of this component.
	Dimension 	getMinimumSize()
	Gets the minimum size of this component.
	*/
	
	/**
	 * Gets the parent of this widget.
	 */
	virtual NODELETE xContainer* getParent() = 0;
		
	/**
	 * Returns the size of this component in the form of a Dimension object.
	 */
	virtual xDimension* getSize() = 0;
		
	/**
	 * Returns the current width of this component.
	 */
	virtual int getWidth() = 0;
	
	/**
	 * Returns the current x coordinate of the components origin.
	 */
	virtual int getX() = 0;
	
	/**
	 * Returns the current y coordinate of the components origin.
	 */
	virtual int getY() = 0;
		
	/**
	 * Invalidates this component.
	 */
	virtual void invalidate() = 0;
		
	/**
	 * Determines whether this component is enabled.
	 */
	virtual bool isEnabled() = 0;
	
	/**
	 * Returns whether this Component can be focused.
	 */
	//virtual bool isFocusable() = 0;
	
	/* ??
	boolean 	isFocusCycleRoot(Container container)
	Returns whether the specified Container is the focus cycle root of this Component's focus traversal cycle.
	*/
	
	/**
	 * Returns true if this Component is the focus owner.
	 */
	virtual bool isFocusOwner() = 0;
		
	/**
	 * Determines whether this component is showing on screen.
	 */
	virtual bool isShowing() = 0;
	
	/**
	 * Determines whether this component is valid.
	 */
	virtual bool isValid() = 0;
	
	/**
	 * Determines whether this component should be visible when its parent is visible.
	 */
	virtual bool isVisible() = 0;
	
	/**
	 * Prints a listing of this component to the standard system output stream.
	 */
	//virtual void list() = 0;
	
	/**
	 * Prints a listing of this component to the specified output stream.
	 */
	//virtual void list(xWriter& out) = 0;
	
	/**
	 * Prints out a list, starting at the specified indentation, to the specified char stream.
	 */
	//virtual void list(xWriter& out, int indent) = 0;
		
	/**
	 * Requests that this Widget get the input focus, and that this Widget's 
	 * top-level ancestor become the focused Window.
	 */
	virtual void requestFocus() = 0;
		
	/**
	 * Sets the background color of this component.
	 */
	virtual void setBackground(xColor& c) = 0;
	
	/**
	 * Moves and resizes this component.
	 */
	virtual void setBounds(int x, int y, int width, int height) = 0;
	
	/**
	 * Moves and resizes this component to conform to the new bounding rectangle r.
	 */
	virtual void setBounds(xRectangle& r) = 0;
		
	/**
	 * Enables or disables this component, depending on the value of the parameter b.
	 */
	virtual void setEnabled(bool b) = 0;
	
	/* ???
	void setFocusable(boolean focusable)
	Sets the focusable state of this Component to the specified value.
	void 	setFocusTraversalKeys(int id, Set keystrokes)
	Sets the focus traversal keys for a given traversal operation for this Component.
	void 	setFocusTraversalKeysEnabled(boolean focusTraversalKeysEnabled)
	Sets whether focus traversal keys are enabled for this Component.
	*/
		
	/**
	 * Sets the font of this component.
	 */
	virtual void setFont(xFont& f) = 0;
	
	/**
	 * Sets the foreground color of this component.
	 */
	virtual void setForeground(xColor& c) = 0;
	
	/**
	 * Moves this component to a new location.
	 */
	virtual void setLocation(int x, int y) = 0;

	/**
	 * Resizes this component so that it has width width and height height.
	 */
	virtual void setSize(int width, int height) = 0;
	
	/**
	 * Shows or hides this component depending on the value of parameter b.
	 */
	virtual void setVisible(boolean b) = 0;
		
	/**
	* Returns a string representation of this component and its values.
	*/
	//virtual xString toString() = 0;
		
	/* ???
	void 	transferFocus()
	Transfers the focus to the next component, as though this Component were the focus owner.
	void 	transferFocusBackward()
	Transfers the focus to the previous component, as though this Component were the focus owner.
	void 	transferFocusUpCycle()
	Transfers the focus up one focus traversal cycle.
	*/
	
	/**
	 * Ensures that this component has a valid layout.
	 */
	virtual void validate() = 0;
};




}//namespace


//select include file
#ifdef XTK_GUI_MSW
	#include "msw/widget_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/widget_gtk2.h"
#endif



#endif //XTK_USE_WIDGETS
#endif //XTK_WIDGET_H
