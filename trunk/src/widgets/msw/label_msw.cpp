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
#include <assert.h>


#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <windows.h>

namespace xtk
{

extern LRESULT CALLBACK xWidgetWindowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xLabelInternal::xLabelInternal(xWidget* parent,xString text,xLabel* external)
: xWidgetInternal(parent,external)
{
	m_text = text;
	HWND hwnd;
	WNDCLASS wclass;
	if (!::GetClassInfo(xApplication::getHinstance(),XTK_MSW_LABEL_CLASS_NAME,&wclass))
	{	
		wclass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wclass.lpfnWndProc   = (WNDPROC)xWidgetWindowProcedure;
		wclass.cbClsExtra    = 0;
		wclass.cbWndExtra    = 0;
		wclass.hInstance	 = xApplication::getHinstance();
		wclass.hIcon		 = ::LoadIcon(NULL, IDI_APPLICATION);
		wclass.hCursor       = ::LoadCursor (NULL, IDC_ARROW);
		wclass.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
		wclass.lpszMenuName  = 0;
		wclass.lpszClassName = XTK_MSW_LABEL_CLASS_NAME;
		::RegisterClass(&wclass);
	}

	hwnd = ::CreateWindow(XTK_MSW_LABEL_CLASS_NAME,text.c_str(),WS_CHILD | WS_VISIBLE,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		parent->getInternal()->getHWND(),NULL,xApplication::getHinstance(),NULL);
		
	assert(hwnd != NULL);

	//set font to default system font
	m_font = xFont::getSystemFont(xFont::XTK_GUI_FONT);

	//set the user data of the window to the current window object
	::SetWindowLongPtr(hwnd,GWL_USERDATA,(LONG_PTR)(xWidget*) this);

	//no subclassing
	m_baseWndProc = (WNDPROC)DefWindowProc;

	setHWND(hwnd);
	
	xDimension dim;
	sizeRequest(dim);
	setSize(dim.getWidth(),dim.getHeight());
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
void xLabelInternal::sizeRequest(xDimension& dim)
{
	xDimension preferredSize;
	getPreferredSize(preferredSize);
	
	xFontMetrics* fm = getFontMetrics();
	
	if(preferredSize.getHeight() < 0)
		dim.setHeight(fm->getHeight() + 10);
	else
		dim.setHeight(preferredSize.getHeight());
		
	if(preferredSize.getWidth() < 0)
		dim.setWidth(fm->stringWidth(getText()));
	else
		dim.setWidth(preferredSize.getWidth());
	
	delete fm;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xLabelInternal::onDefault(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return ::CallWindowProc(m_baseWndProc,hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xLabelInternal::onSetFont(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	m_font = new xFont(new xFontInternal((HFONT) wParam));
	return 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xLabelInternal::onGetFont(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return (LRESULT)m_font->getInternal()->getHFONT();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xLabelInternal::onPaint(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	HDC         hdc ;
	PAINTSTRUCT ps ;
	RECT        rect ;

	hdc = ::BeginPaint (hwnd, &ps) ;
	
	//init attributes
	::SelectObject(hdc,m_font->getInternal()->getHFONT());
	::SetBkColor(hdc,::GetSysColor(COLOR_3DFACE));

	::GetClientRect (hwnd, &rect) ;

	::DrawText(hdc,m_text.c_str(), (int) m_text.length(), &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;

	::EndPaint (hwnd, &ps) ;
	return 0 ;
}


}//namespace

#endif //XTK_USE_WIDGETS
