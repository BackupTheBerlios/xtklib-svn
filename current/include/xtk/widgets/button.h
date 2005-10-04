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
* @file button.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_BUTTON_H
#define XTK_BUTTON_H

#include "../globals.h"
#include "widget.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

/**
 * Defines the prototype for the various implementation of class xButton.
 */
class XTKAPI xIButton
{
protected:
	xIButton(){}
	
public:
	virtual ~xIButton(){}

	/**
	 * Adds the specified action listener to receive action events from this button.
	 */
	virtual void addActionListener(YOUROWNERSHIP xActionListener* l) = 0;
	
	/**
	 * Returns the command name of the action event fired by this button.
	 */
	virtual xString getActionCommand() = 0;
		
	/**
	 * Returns an array of all the action listeners registered on this button.
	 */
	virtual xArray<NODELETE xActionListener*> getActionListeners() = 0;
	
	/**
	 * Gets the label of this button.
	 */
	virtual xString getLabel() = 0;
	
	/**
	 * Removes the specified action listener so that it no longer receives action events from this button.
	 */	
	virtual void removeActionListener(xActionListener& l) = 0;
	
	/**
	 * Sets the command name for the action event fired by this button.
	 */
	virtual void setActionCommand(xString command) = 0;
	
	/**
	 * Sets the button's label to be the specified string.
	 */
	virtual void setLabel(xString label) = 0;
	
protected:

	/**
	 * Processes action events occurring on this button by dispatching them to any registered ActionListener objects.
	 */
	virtual void processActionEvent(xActionEvent& e) = 0;
};

}//namespace

//select include file
#ifdef XTK_GUI_MSW
	#include "msw/button_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/button_gtk2.h"
#endif

#endif //XTK_USE_WIDGETS

#endif //XTK_BUTTON_H
