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

LRESULT CALLBACK xButtonWindowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	void* ptr = (void*)(::GetWindowLongPtr(hwnd,GWL_USERDATA));
	if(ptr != NULL)
	{
		xButton* cnt = dynamic_cast<xButton*>((xWidget*)(ptr));
		if(cnt != NULL)
			return cnt->windowProcedure(hwnd,uMsg,wParam,lParam);
		else
			return -1;
	}
	else
		return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}
	
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
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_TEXT,
		x,y,width,height,parent->getHWND(),NULL,xApplication::getHinstance(),NULL);
	
	assert(hwnd != NULL);

	//set the user data of the window to the current window object
	::SetWindowLongPtr(hwnd,GWL_USERDATA,(LONG_PTR)(xWidget*) this);
	
	//subclassing
	m_baseWndProc = (WNDPROC) ::SetWindowLongPtr(hwnd,GWL_WNDPROC,(LONG_PTR)xButtonWindowProcedure);
	
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
LRESULT xButton::onCommand(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//forward this message to child window
	if(wParam == BN_CLICKED)
	{
		xActionEvent ev(this,XWE_ACTION_PERFORMED,getActionCommand());
		processActionEvent(ev);
	}
	return 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xButton::onSetFocus(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xFocusEvent ev(this,XWE_FOCUS_GAINED);
	processFocusEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xButton::onKillFocus(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xFocusEvent ev(this,XWE_FOCUS_LOST);
	processFocusEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xButton::onDefault(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return ::CallWindowProc(m_baseWndProc,hwnd,uMsg,wParam,lParam);
}


}//namespace

#endif //XTK_USE_WIDGETS