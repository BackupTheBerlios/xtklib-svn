/*
* This file is part of XTKLib project.
*
* Copyright(C) 2003-2006 Mario Casciaro xshadow[AT]email(DOT)it
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
* @file panel_msw.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../../include/xtk/base/application.h"
#include "panel_msw.h"
#include "widgets_msw_private.h"
#include "layout_msw.h"


#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <windows.h>

namespace xtk
{

extern LRESULT CALLBACK xWidgetWindowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xPanelInternal::xPanelInternal(xWidget* parent,xPanel::Border border,xLayoutManager* layout,xString label,xPanel* external)
: xContainerInternal(parent,layout, external)
{
	m_border = border;
	HWND hwnd;
	if(m_border == xPanel::BORDER_TITLED)
	{
		hwnd = ::CreateWindow(_T("button"),label.c_str(),
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_TEXT,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
			parent->getInternal()->getHWND(),NULL,xApplication::getHinstance(),NULL);
		
		assert(hwnd != NULL);

		//set the user data of the window to the current window object
		::SetWindowLongPtr(hwnd,GWL_USERDATA,(LONG_PTR)(xWidget*) this);
		
		//subclassing
		m_baseWndProc = (WNDPROC) ::SetWindowLongPtr(hwnd,GWL_WNDPROC,(LONG_PTR)xWidgetWindowProcedure);
	}
	else
	{
		WNDCLASS wclass;
		if (!::GetClassInfo(xApplication::getHinstance(),XTK_MSW_PANEL_NOBORDER_CLASS_NAME,&wclass))
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
			wclass.lpszClassName = XTK_MSW_PANEL_NOBORDER_CLASS_NAME;
			::RegisterClass(&wclass);
		}

		hwnd = ::CreateWindow(XTK_MSW_PANEL_NOBORDER_CLASS_NAME,label.c_str(),WS_CHILD | WS_VISIBLE,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
			parent->getInternal()->getHWND(),NULL,xApplication::getHinstance(),NULL);
			
		assert(hwnd != NULL);

		//set the user data of the window to the current window object
		::SetWindowLongPtr(hwnd,GWL_USERDATA,(LONG_PTR)(xWidget*) this);

		//no subclassing
		m_baseWndProc = (WNDPROC)DefWindowProc;
	}	

	setHWND(hwnd);

	//set font to default system font
	xFont* fn = xFont::getSystemFont(xFont::XTK_GUI_FONT);	
	setFont(*fn);
	delete fn;

	xDimension dim;
	sizeRequest(dim);
	setSize(dim.getWidth(),dim.getHeight());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xPanelInternal::~xPanelInternal()
{
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xPanelInternal::getLabel()
{
	int iLength = ::GetWindowTextLength(getHWND());
	xArray<xchar> buff(iLength);
	::GetWindowText(getHWND(), buff.getRawData(), buff.size());

	return xString(buff.getRawData(),buff.size());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPanelInternal::setLabel(xString label)
{
	::SetWindowText(getHWND(),label.c_str());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xPanelInternal::onDefault(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return ::CallWindowProc(m_baseWndProc,hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPanelInternal::doLayout()
{
	if(m_border == xPanel::BORDER_TITLED)
	{
		xDimension dim;
		getSize(dim);
		dim.set(dim.getWidth() - 8,dim.getHeight() - 20);
		getLayout().getInternal()->setClientSize(dim,4,15);
		getLayout().getInternal()->doLayout();
	}
	else
		xContainerInternal::doLayout();
}


}//namespace

#endif //XTK_USE_WIDGETS
