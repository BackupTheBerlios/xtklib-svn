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
* @file window.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_WINDOW_H
#define XTK_WINDOW_H

#include "../globals.h"
#include "../base/array.h"
#include "../base/datastructures.h"
#include "container.h"
#include "widgetevent.h"
#include "widgetlisteners.h"

#ifdef XTK_USE_WIDGETS

namespace xtk
{


/**
 * Defines the interface for the various implementation of class xWindow.
 */
class XTKAPI xIWindow
{
protected:

	xIWindow(){}
public:
	enum CloseAction
	{
		XTK_EXIT_ON_CLOSE,
		XTK_DESTROY_ON_CLOSE,
		XTK_HIDE_ON_CLOSE
	};
	
	virtual ~xIWindow(){}

	/**
	 * Returns whether this Window is active.
	 */
	virtual bool isActive() = 0;
		
	/**
	 * If this Window is visible, sends this Window to the back and may cause it to lose focus or activation if it is the focused or active Window.
	 */
	virtual void toBack() = 0;
		
	/**
	 * If this Window is visible, brings this Window to the front and may make it the focused Window.
	 */
	virtual void toFront() = 0;
	
	/**
	 * 
	 */
	virtual void setDefaultCloseAction(CloseAction caction) = 0;
	
	/**
	* 
	*/
	virtual CloseAction getDefaultCloseAction() = 0;
};

}//namespace


//select include file
#ifdef XTK_GUI_MSW
	#include "msw/window_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/window_gtk2.h"
#endif

#endif //XTK_USE_WIDGETS

#endif //XTK_WINDOW_H
