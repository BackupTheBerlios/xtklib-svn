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
* @file button_msw.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../../include/xtk/base/application.h"
#include "../../../include/xtk/widgets/button.h"
#include "../../../include/xtk/widgets/container.h"
#include "../../../include/xtk/widgets/widgetevent.h"
#include "../../../include/xtk/base/smartptr.h"
#include "widgets_msw_private.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <windows.h>

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xButton::xButton(xContainer* parent,xString label,int x,int y,int width,int height)
	: xWidget(parent)
{
	if(x == XTK_DEFAULT_WIDGET_POSITION)
	
		x = 0;
	if(y == XTK_DEFAULT_WIDGET_POSITION)
		y = 0;
	if(height == XTK_DEFAULT_WIDGET_SIZE)
		height = 30;
	
	HWND hwnd = ::CreateWindow(_T("button"),label.c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_NOTIFY | BS_TEXT,x,y,width,height,
		parent->getHWND(),NULL,xApplication::getHinstance(),NULL);
	
	assert(hwnd != NULL);

	//set the user data of the window to the current window object
	::SetWindowLongPtr(hwnd,GWL_USERDATA,(LONG_PTR)(xWidget*) this);
	setHWND(hwnd);
	
	//set font to default system font
	xFont* fn = xFont::getSystemFont(xFont::XTK_GUI_FONT);	
	::SendMessage(hwnd,(UINT) WM_SETFONT,(WPARAM) fn->getHFONT(),TRUE);
	delete fn;
	
	//if was default size update to best fit width
	if(width == XTK_DEFAULT_WIDGET_SIZE)
	{
		xFontMetrics* fm = getFontMetrics();
		int wid = fm->stringWidth(label);
		setSize(wid + 10,height);
		delete fm;
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xButton::~xButton()
{

}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xButton::getLabel()
{
	int iLength = ::GetWindowTextLength(getHWND());
	xArray<xchar> buff(iLength);
	::GetWindowText(getHWND(), buff.getRawData(), buff.size());
	
	return xString(buff.getRawData(),buff.size());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xButton::setLabel(xString label)
{
	::SetWindowText(getHWND(),label.c_str());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xButton::windowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
			case BN_CLICKED:
				xActionEvent ev(this,XWE_ACTION_PERFORMED,getActionCommand());
				processActionEvent(ev);
				break ;
			//should manage other events(maybe focus events)?
		}
		break;
	}
	
	return 0;
}




}//namespace

#endif //XTK_USE_WIDGETS
