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

#include "../../../include/xtk/widgets/msw/frame_msw.h"
#include "../../../include/xtk/application.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT CALLBACK xFrameWindowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	void* ptr = (void*)::GetWindowLongPtr(hwnd,GWL_USERDATA);
	xFrame* frame = static_cast<xFrame*>(ptr);
	return frame->windowProcedure(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFrame::xFrame(xString title,int x,int y,int width,int height,xContainer* parent, xLayoutManager* layout)
: xWindow(parent,layout),m_title(title)
{	
	WNDCLASS wclass;
	if (!::GetClassInfo(xGetHinstance(),XTK_MSW_FRAME_CLASS_NAME,&wclass))
	{	
		wclass.style         = CS_HREDRAW | CS_VREDRAW;
		wclass.lpfnWndProc   = (WNDPROC)xFrameWindowProcedure;
		wclass.cbClsExtra    = 0;
		wclass.cbWndExtra    = 0;
		wclass.hInstance	 = xGetHinstance();
		wclass.hIcon		 = ::LoadIcon(NULL, IDI_APPLICATION);
		wclass.hCursor       = ::LoadCursor (NULL, IDC_ARROW);
		wclass.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
		wclass.lpszMenuName  = 0;
		wclass.lpszClassName = XTK_MSW_FRAME_CLASS_NAME;
		::RegisterClass(&wclass);
	}
	
	HWND hwnd = ::CreateWindow(XTK_MSW_FRAME_CLASS_NAME,title.c_str(),WS_OVERLAPPEDWINDOW,x,y,width,height,
		NULL,NULL,xGetHinstance(),NULL);
	
	//set the user data of the window to the current window object
	::SetWindowLongPtr(hwnd,GWL_USERDATA,(LONG_PTR) this);
		
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

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xFrame::windowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}


}//namespace

#endif //XTK_USE_WIDGETS
