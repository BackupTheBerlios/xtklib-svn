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
* @file widget_msw.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "widget_msw.h"
#include "font_msw.h"
#include "../../../include/xtk/widgets/container.h"
#include "../../../include/xtk/base/thread.h"
#include "../../../include/xtk/base/datastructures.h"
#include "../../../include/xtk/base/smartptr.h"

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
XTKAPI void xtkUIEventLoop()
{
	MSG message;
	while(::GetMessage(&message,NULL,0,0) != 0)
	{
		::TranslateMessage(&message) ;
		::DispatchMessage(&message) ;
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
XTKAPI void xtkExitUIEventLoop()
{
	::PostQuitMessage(1);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
XTKAPI void xtkWidgetsInitialize(int* argc,char*** argv)
{}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
XTKAPI void xtkProcessPendingUIEvent()
{
	MSG message;

	while(::PeekMessage(&message,NULL,0,0,PM_NOREMOVE) != 0)
	{
		if(::GetMessage(&message,NULL,0,0) == 0)
		{
			xtkExitUIEventLoop();
		}
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xWidgetInternal::xWidgetInternal(xWidget* parent,xWidget* external)
{
	m_parent = parent;
	m_hWidget = NULL;
	m_external = external;
	m_preferredSize.set(-1,-1);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xWidgetInternal::~xWidgetInternal()
{

}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::getBounds(OUT xRectangle& bounds)
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);
	bounds.set(rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFont* xWidgetInternal::getFont()
{
	HFONT hFont = (HFONT) ::SendMessage((HWND) getHWND(),(UINT) WM_GETFONT,0,0);
	return new xFont(new xFontInternal(hFont));
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidgetInternal::getHeight()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return rect.bottom - rect.top;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xPoint* xWidgetInternal::getLocation()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return new xPoint(rect.left,rect.top);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xPoint* xWidgetInternal::getLocationOnScreen()
{
	if(getParent() == NULL)
		return getLocation();
	else
	{
		smartPtr<xPoint> parent_loc = getParent()->getInternal()->getLocation();
		smartPtr<xPoint> my_loc = getLocation();

		return new xPoint(parent_loc->getX() + my_loc->getX(),parent_loc->getY() + my_loc->getY());
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::getSize(OUT xDimension& dim)
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	dim.set(rect.right - rect.left,rect.bottom - rect.top);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidgetInternal::getWidth()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return rect.right - rect.left;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidgetInternal::getX()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return rect.left;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidgetInternal::getY()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return rect.top;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidgetInternal::getModifiers(WPARAM wParam)
{
	int modifier = 0;

	if((wParam & MK_LBUTTON) != 0)
		modifier |= xMouseEvent::MODIFIER_MASK_BUTTON1_DOWN;
	if((wParam & MK_MBUTTON) != 0)
		modifier |= xMouseEvent::MODIFIER_MASK_BUTTON3_DOWN;
	if((wParam & MK_RBUTTON) != 0)
		modifier |= xMouseEvent::MODIFIER_MASK_BUTTON2_DOWN;
	if((wParam & MK_SHIFT) != 0)
		modifier |= xMouseEvent::MODIFIER_MASK_SHIFT_DOWN;
	if((wParam & MK_CONTROL) != 0)
		modifier |= xMouseEvent::MODIFIER_MASK_CTRL_DOWN;
	if((wParam & MK_ALT) != 0)
		modifier |= xMouseEvent::MODIFIER_MASK_ALT_DOWN;

	return modifier;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onLButtonPressed(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(getExternal(),XWE_MOUSE_PRESSED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON1);
	processEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onMButtonPressed(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(getExternal(),XWE_MOUSE_PRESSED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON3);
	processEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onRButtonPressed(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(getExternal(),XWE_MOUSE_PRESSED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON2);
	processEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onLButtonReleased(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(getExternal(),XWE_MOUSE_RELEASED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON1);
	processEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onMButtonReleased(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(getExternal(),XWE_MOUSE_RELEASED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON3);
	processEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onRButtonReleased(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(getExternal(),XWE_MOUSE_RELEASED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON2);
	processEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onLButtonDBLClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(getExternal(),XWE_MOUSE_DOUBLECLICKED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON1);
	processEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onMButtonDBLClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(getExternal(),XWE_MOUSE_DOUBLECLICKED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON3);
	processEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onRButtonDBLClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(getExternal(),XWE_MOUSE_DOUBLECLICKED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON2);
	processEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onCommand(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//forward this message to child window
	void* ptr = (void*)::GetWindowLongPtr((HWND)lParam,GWL_USERDATA);
	xWidgetInternal* widg = static_cast<xWidgetInternal*>(ptr);
	if(widg != this)
		return widg->windowProcedure(hwnd,uMsg,wParam,lParam);

	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onSetFocus(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onKillFocus(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onDestroy(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onNCDestroy(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(getParent() == NULL)
	{
		delete getExternal();
		return 0;
	}

	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onSize(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	doLayout();
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onPaint(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onSetFont(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onGetFont(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::onDefault(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidgetInternal::windowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	/*
	HDC         hdc ;
	PAINTSTRUCT ps ;
	RECT        rect ;
	*/

	switch(uMsg)
	{
		//Mouse events-------------------------------
	case WM_LBUTTONDOWN:
		return onLButtonPressed(hwnd,uMsg,wParam,lParam);
	case WM_MBUTTONDOWN:
		return onMButtonPressed(hwnd,uMsg,wParam,lParam);
	case WM_RBUTTONDOWN:
		return onRButtonPressed(hwnd,uMsg,wParam,lParam);
	case WM_LBUTTONUP:
		return onLButtonReleased(hwnd,uMsg,wParam,lParam);
	case WM_MBUTTONUP:
		return onMButtonReleased(hwnd,uMsg,wParam,lParam);
	case WM_RBUTTONUP:
		return onRButtonReleased(hwnd,uMsg,wParam,lParam);
	case WM_LBUTTONDBLCLK:
		return onLButtonDBLClicked(hwnd,uMsg,wParam,lParam);
	case WM_MBUTTONDBLCLK:
		return onMButtonDBLClicked(hwnd,uMsg,wParam,lParam);
	case WM_RBUTTONDBLCLK:
		return onRButtonDBLClicked(hwnd,uMsg,wParam,lParam);
		//Focus events-------------------------------
	case WM_SETFOCUS:
		return onSetFocus(hwnd,uMsg,wParam,lParam);
	case WM_KILLFOCUS:
		return onKillFocus(hwnd,uMsg,wParam,lParam);
		//Size and Move-------------------------------
	case WM_SIZE:
		return onSize(hwnd,uMsg,wParam,lParam);
		//Fonts-------------------------------
	case WM_SETFONT:
		return onSetFont(hwnd,uMsg,wParam,lParam);
	case WM_GETFONT:
		return onGetFont(hwnd,uMsg,wParam,lParam);
		//Other events-------------------------------
	case WM_PAINT:
		return onPaint(hwnd,uMsg,wParam,lParam);
	case WM_COMMAND:
		return onCommand(hwnd,uMsg,wParam,lParam);
	case WM_NCDESTROY:
		return onNCDestroy(hwnd,uMsg,wParam,lParam);
	case WM_DESTROY:
		return onDestroy(hwnd,uMsg,wParam,lParam);
	}

	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT CALLBACK xWidgetWindowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	void* ptr = (void*)(::GetWindowLongPtr(hwnd,GWL_USERDATA));
	if(ptr != NULL)
	{
		xWidgetInternal* cnt = (xWidgetInternal*)(ptr);
		return cnt->windowProcedure(hwnd,uMsg,wParam,lParam);
	}
	else
		return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

}//namespace


#endif //XTK_USE_WIDGETS
