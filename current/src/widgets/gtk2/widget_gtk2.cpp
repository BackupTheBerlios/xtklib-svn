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
* @file widget_gtk2.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "widget_gtk2.h"
#include "font_gtk2.h"
#include "../../../include/xtk/widgets/container.h"
#include "../../../include/xtk/base/thread.h"
#include "../../../include/xtk/base/datastructures.h"
#include "../../../include/xtk/base/smartptr.h"

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)

namespace xtk
{

xObject xWidgetInternal::s_guiMutex;

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xtkProcessNextUIEvent()
{
	return !gtk_main_iteration();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xtkUIEventPending()
{
	return ::gtk_event_pending();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xWidgetInternal::xWidgetInternal(xWidget* parent,xWidget* external)
{
	m_parent = parent;
	m_gtkWidget = NULL;
	m_external = external;
	if(parent != NULL)
	{
		xContainer* c = dynamic_cast<xContainer*>(parent);
		if(c != NULL)
			c->addChild(external);
	}
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
xColor* xWidgetInternal::getBackground()
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xRectangle* xWidgetInternal::getBounds()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return new xRectangle(rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
NODELETE xWidget* xWidgetInternal::getComponentAt(int x, int y)
{
	throw xNotImplementedException();
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
xFontMetrics* xWidgetInternal::getFontMetrics()
{
	return new xFontMetrics(new xFontMetricsInternal(getHWND()));
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xColor* xWidgetInternal::getForeground()
{
	throw xNotImplementedException();
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
bool xWidgetInternal::getIgnoreRepaint()
{
	throw xNotImplementedException();
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
		smartPtr<xPoint> parent_loc = getParent()->getLocation();
		smartPtr<xPoint> my_loc = getLocation();

		return new xPoint(parent_loc->getX() + my_loc->getX(),parent_loc->getY() + my_loc->getY());
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xDimension* xWidgetInternal::getSize()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return new xDimension(rect.right - rect.left,rect.bottom - rect.top);
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
void xWidgetInternal::invalidate()
{
	::InvalidateRect(
		getHWND(),	// handle of window with changed update region  
		NULL,		// address of rectangle coordinates,NULL for all client rect
		TRUE	// erase-background flag 
		);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidgetInternal::isEnabled()
{
	return ::IsWindowEnabled(getHWND()) == TRUE;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidgetInternal::isFocusable()
{
	return false;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidgetInternal::isFocusOwner()
{
	 HWND hWnd = ::GetFocus();
	 assert(hWnd != NULL);
	 return hWnd == getHWND();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidgetInternal::isShowing()
{
	return ::IsWindowVisible(getHWND()) == TRUE;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidgetInternal::isValid()
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidgetInternal::isVisible()
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::requestFocus()
{
	HWND hWnd = ::SetFocus(getHWND());
	assert(hWnd != NULL);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::setBackground(xColor& c)
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::setBounds(int x, int y, int width, int height)
{
	::MoveWindow(getHWND(),x,y,width,height,true);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::setEnabled(bool b)
{
	::EnableWindow(getHWND(),b);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::setFont(xFont& f)
{
	::SendMessage((HWND) getHWND(),(UINT) WM_SETFONT,(WPARAM) f.getInternal()->getHFONT(),(LPARAM) TRUE);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::setForeground(xColor& c)
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::setLocation(int x, int y)
{
	::SetWindowPos(
		getHWND(),					// handle of window
		NULL,						// placement-order handle (ignored)
		x,							// horizontal position
		y,							// vertical position
		0,							// width (ignored)
		0,							// height (ignored)
		SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE // window-positioning flags
		);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::setSize(int width, int height)
{
	::SetWindowPos(
		getHWND(),					// handle of window
		NULL,						// placement-order handle (ignored)
		0,							// horizontal position (ignored)
		0,							// vertical position (ignored)
		width,						// width 
		height,						// height 
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE 	// window-positioning flags
		);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::setVisible(boolean b)
{
	if(b)
	{
		::ShowWindow(getHWND(),SW_SHOW);
	}
	else
	{
		::ShowWindow(getHWND(),SW_HIDE);
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::validate()
{
	::ValidateRect(getHWND(),NULL);
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
	//Other events-------------------------------
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
