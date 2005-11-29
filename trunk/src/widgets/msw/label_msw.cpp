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
* @file label_msw.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../../include/xtk/base/application.h"
#include "label_msw.h"
#include "widgets_msw_private.h"


#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <windows.h>

namespace xtk
{

extern LRESULT CALLBACK xWidgetWindowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xLabelInternal::xLabelInternal(xWidget* parent,xString text,int x,int y,int width,int height,xLabel* external)
: xWidgetInternal(parent,external)
{
	if(x == xWidget::XTK_DEFAULT_WIDGET_POSITION)
		x = 0;
	if(y == xWidget::XTK_DEFAULT_WIDGET_POSITION)
		y = 0;
	if(height == xWidget::XTK_DEFAULT_WIDGET_SIZE)
		height = 30;

	HWND hwnd;
	hwnd = ::CreateWindow(_T("static"),text.c_str(),WS_CHILD | WS_VISIBLE | BS_TEXT,
		x,y,width,height,parent->getInternal()->getHWND(),NULL,xApplication::getHinstance(),NULL);

	assert(hwnd != NULL);

	//set the user data of the window to the current window object
	::SetWindowLongPtr(hwnd,GWL_USERDATA,(LONG_PTR)(xWidget*) this);

	//subclassing
	m_baseWndProc = (WNDPROC) ::SetWindowLongPtr(hwnd,GWL_WNDPROC,(LONG_PTR)xWidgetWindowProcedure);

	setHWND(hwnd);

	//set font to default system font
	xFont* fn = xFont::getSystemFont(xFont::XTK_GUI_FONT);	
	setFont(*fn);
	delete fn;

	//if was default size update to best fit width
	if(width == xWidget::XTK_DEFAULT_WIDGET_SIZE)
	{
		xFontMetrics* fm = getFontMetrics();
		int wid = fm->stringWidth(text);
		setSize(wid + 10,height);
		delete fm;
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xLabelInternal::~xLabelInternal()
{

}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xLabelInternal::getText()
{
	int iLength = ::GetWindowTextLength(getHWND());
	xArray<xchar> buff(iLength);
	::GetWindowText(getHWND(), buff.getRawData(), buff.size());

	return xString(buff.getRawData(),buff.size());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLabelInternal::setText(xString text)
{
	::SetWindowText(getHWND(),text.c_str());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xLabelInternal::onDefault(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return ::CallWindowProc(m_baseWndProc,hwnd,uMsg,wParam,lParam);
}


}//namespace

#endif //XTK_USE_WIDGETS