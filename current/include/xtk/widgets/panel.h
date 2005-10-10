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
* @file panel.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_PANEL_H
#define XTK_PANEL_H

#include "../globals.h"
#include "widgeteventgenerator.h"
#include "widget.h"
#include "../base/string.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

/**
* Defines the prototype for the various implementation of class xPanel.
*/
class XTKAPI xIPanel
{
protected:
	xIPanel(){}

public:
	enum Border
	{
		BORDER_TITLED,
		BORDER_NONE
	};

	virtual ~xIPanel(){}

	/**
	* Gets the label of this panel.
	*/
	virtual xString getLabel() = 0;

	/**
	* Sets the button's label to be the specified string.
	*/
	virtual void setLabel(xString label) = 0;
};


}//namespace

//select include file
#ifdef XTK_GUI_MSW
	#include "msw/panel_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/panel_gtk2.h"
#endif

#endif //XTK_USE_WIDGETS

#endif //XTK_PANEL_H
