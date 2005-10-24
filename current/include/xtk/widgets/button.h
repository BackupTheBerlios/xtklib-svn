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

class xButtonInternal;

/**
 * Defines the prototype for the various implementation of class xButton.
 */
class XTKAPI xButton : public xWidget,public xActionEventGenerator
{
private:
	xButtonInternal* m_internal;
	
protected:
	xButton(xWidget* parent,xString label,xButtonInternal* i);
	
public:
	xButton(xWidget* parent,xString label = _T(""));
		
	virtual ~xButton();
	
	/**
	 * Gets the label of this button.
	 */
	virtual xString getLabel();
	
	/**
	 * Sets the button's label to be the specified string.
	 */
	virtual void setLabel(xString label);
};


}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_BUTTON_H
