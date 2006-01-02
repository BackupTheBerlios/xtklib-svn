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
XTKAPI void xtkUIEventLoop();

/**
 * 
 */
XTKAPI void xtkExitUIEventLoop();

/**
 *
 */
XTKAPI void xtkWidgetsInitialize(int* argc,char*** argv);

/**
 *
 */
XTKAPI void xtkProcessPendingUIEvent();

/**
 * A widget is the base class for all Gui components.
 * Note that all children of the window will be deleted automatically by 
 * the destructor before the window itself is deleted.
 */
class XTKAPI xWidget : public virtual xObject,public virtual xWidgetEventGenerator
{
private:
	xWidgetInternal*	m_internal;
	friend class xWidgetInternal;
	
protected:
	virtual void processEvent(xWidgetEvent& ev);
	virtual void addEventHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh,xWidgetEventID evmask);
	virtual void removeEventHandler(xWidgetEventHandlerBase& evh,xWidgetEventID evmask);
	
	
	xWidget(xWidget* parent,xWidgetInternal* i);
public:
	virtual ~xWidget();
	
	/**
	 * Destroy this widgets and frees its resources
	 */
	virtual void destroy();

	xWidgetInternal* getInternal()
	{return m_internal;}
	
	/**
	 * Gets the parent of this widget.NULL if there are no parent.
	 */
	virtual MYOWNERSHIP xWidget* getParent();
		
	/**
	 * Returns the size of this component in the form of a Dimension object.
	 */
	virtual void getPreferredSize(OUT xDimension& dim);
		
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
	 * Determines whether this component should be visible when its parent is visible.
	 */
	virtual bool isVisible();
		
	/**
	 * Requests that this Widget get the input focus, and that this Widget's 
	 * top-level ancestor become the focused Window.
	 */
	virtual void requestFocus();
		
	/**
	 * Enables or disables this component, depending on the value of the parameter b.
	 */
	virtual void setEnabled(bool b);

	/**
	 * Resizes this component so that it has width width and height height.
	 */
	virtual void setPreferredSize(int width, int height);
	
	/**
	 * Shows or hides this component depending on the value of parameter b.
	 */
	virtual void setVisible(bool b);
};




}//namespace

#endif //XTK_USE_WIDGETS
#endif //XTK_WIDGET_H
