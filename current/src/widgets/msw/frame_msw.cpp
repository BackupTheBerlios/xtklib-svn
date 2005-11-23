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
* @file frame_msw.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "frame_msw.h"
#include "../../../include/xtk/base/application.h"
#include "../../../include/xtk/widgets/frame.h"
#include "widgets_msw_private.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

namespace xtk
{

extern LRESULT CALLBACK xWidgetWindowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFrameInternal::xFrameInternal(xWidget* parent,xString title,xLayoutManager* layout,xFrame* external)
: xWindowInternal(parent,layout,external),m_title(title)
{	
	WNDCLASS wclass;
	if (!::GetClassInfo(xApplication::getHinstance(),XTK_MSW_FRAME_CLASS_NAME,&wclass))
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
		wclass.lpszClassName = XTK_MSW_FRAME_CLASS_NAME;
		::RegisterClass(&wclass);
	}
	
	HWND hwnd = ::CreateWindow(XTK_MSW_FRAME_CLASS_NAME,title.c_str(),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,xApplication::getHinstance(),NULL);
	
	//set the user data of the window to the current window object
	::SetWindowLongPtr(hwnd,GWL_USERDATA,(LONG_PTR)(xWidgetInternal*)this);
		
	assert(hwnd != NULL);
	setHWND(hwnd);
	 
	m_resizable = true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFrameInternal::~xFrameInternal()
{
	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xFrameInternal::setTitle(xString title)
{
	m_title = title;
	::SetWindowText(
		getHWND(),			// handle of window or control
		title.c_str() 		// address of string
		);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xFrameInternal::negoziateSize()
{
	int height = 200;
	int width = 200;
	xDimension preferredSize;
	getPreferredSize(preferredSize);
	if(preferredSize.getWidth() != -1)
		width = preferredSize.getWidth();
	if(preferredSize.getHeight() != -1)
		height = preferredSize.getHeight();
		
	setSize(width,height);
}


}//namespace

#endif //XTK_USE_WIDGETS
