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
* @file window_msw.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_WINDOW_MSW_H
#define XTK_WINDOW_MSW_H

#include "../../globals.h"
#include "../window.h"
#include "../layout.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <Windows.h>

namespace xtk
{

/**
 * A xWindow object is a top-level window.
 */
class XTKAPI xWindow : public xAbstractWindow,public xContainer
{
protected:
	xWindow(xContainer* parent, xLayoutManager* layout = new xBoxLayout(xBoxLayout::X_AXIS))
	: xContainer(parent,layout)
	{}
public:
	virtual ~xWindow(){}

	virtual bool isActive();
	virtual void toBack();
	virtual void toFront();
};




}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_WINDOW_MSW_H
