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
#include "container.h"
#include "../base/string.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

class xPanelInternal;

/**
* A panel is a widget used to organize and group other widgets.
*/
class XTKAPI xPanel : public xContainer
{
public:
	enum Border
	{
		BORDER_TITLED,
		BORDER_NONE
	};
	
protected:
	xPanelInternal* m_internal;

	xPanel(xWidget* parent,xPanel::Border border,xLayoutManager* layout,xString label,xPanelInternal* i);
	
public:
	
	xPanel(xWidget* parent,xPanel::Border border,xLayoutManager* layout = new xBoxLayout(xBoxLayout::X_AXIS),xString label = _T(""));
	
	virtual ~xPanel();

	/**
	* Gets the label of this panel.
	*/
	virtual xString getLabel();

	/**
	* Sets the button's label to be the specified string.
	*/
	virtual void setLabel(xString label);
};


}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_PANEL_H
