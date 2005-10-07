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

#include "../../../include/xtk/base/application.h"
#include "../../../include/xtk/widgets/frame.h"
#include "widgets_msw_private.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

namespace xtk
{

extern LRESULT CALLBACK xContainerWindowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFrame::xFrame(xString title,int x,int y,int width,int height,xContainer* parent, xLayoutManager* layout)
: xWindow(parent,layout),m_title(title)
{	
	WNDCLASS wclass;
	if (!::GetClassInfo(xApplication::getHinstance(),XTK_MSW_FRAME_CLASS_NAME,&wclass))
	{	
		wclass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wclass.lpfnWndProc   = (WNDPROC)xContainerWindowProcedure;
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
	
	HWND hwnd = ::CreateWindow(XTK_MSW_FRAME_CLASS_NAME,title.c_str(),WS_OVERLAPPEDWINDOW,x,y,width,height,
		NULL,NULL,xApplication::getHinstance(),NULL);
	
	//set the user data of the window to the current window object
	::SetWindowLongPtr(hwnd,GWL_USERDATA,(LONG_PTR)(xWidget*)this);
		
	assert(hwnd != NULL);
	setHWND(hwnd);
	 
	m_resizable = true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFrame::~xFrame()
{
	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xFrame::setTitle(xString title)
{
	m_title = title;
	::SetWindowText(
		getHWND(),			// handle of window or control
		title.c_str() 		// address of string
		);
}


}//namespace

#endif //XTK_USE_WIDGETS
