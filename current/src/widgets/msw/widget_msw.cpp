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

#include "../../../include/xtk/widgets/widget.h"
#include "../../../include/xtk/widgets/container.h"
#include "../../../include/xtk/base/thread.h"
#include "../../../include/xtk/base/datastructures.h"
#include "../../../include/xtk/base/smartptr.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

namespace xtk
{

xObject xWidget::s_guiMutex;

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xtkProcessNextUIMessage()
{
	MSG message;

	if(::PeekMessage(&message,NULL,0,0,PM_REMOVE) == 0)
	{
		xThread::sleep(50);
	}
	else
	{
		if(message.message == WM_QUIT)
			return false;

		::TranslateMessage(&message) ;
		::DispatchMessage(&message) ;
	}

	return true;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xWidget::xWidget(xWidget* parent) : xIWidget(parent)
{
	m_parent = parent;
	m_hWidget = NULL;
	if(parent != NULL)
	{
		xIContainer* c = dynamic_cast<xIContainer*>(parent);
		if(c != NULL)
			c->addChild(this);
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xWidget::~xWidget()
{

}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xColor* xWidget::getBackground()
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xRectangle* xWidget::getBounds()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return new xRectangle(rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
NODELETE xWidget* xWidget::getComponentAt(int x, int y)
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFont* xWidget::getFont()
{
	HFONT hFont = (HFONT) ::SendMessage((HWND) getHWND(),(UINT) WM_GETFONT,0,0);
	return new xFont(hFont);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFontMetrics* xWidget::getFontMetrics()
{
	return new xFontMetrics(getHWND());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xColor* xWidget::getForeground()
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidget::getHeight()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return rect.bottom - rect.top;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidget::getIgnoreRepaint()
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xPoint* xWidget::getLocation()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return new xPoint(rect.left,rect.top);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xPoint* xWidget::getLocationOnScreen()
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
xDimension* xWidget::getSize()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return new xDimension(rect.right - rect.left,rect.bottom - rect.top);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidget::getWidth()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return rect.right - rect.left;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidget::getX()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return rect.left;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidget::getY()
{
	RECT rect;
	::GetClientRect(getHWND(),&rect);

	return rect.top;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidget::invalidate()
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
bool xWidget::isEnabled()
{
	return ::IsWindowEnabled(getHWND()) == TRUE;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidget::isFocusOwner()
{
	 HWND hWnd = ::GetFocus();
	 assert(hWnd != NULL);
	 return hWnd == getHWND();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidget::isShowing()
{
	return ::IsWindowVisible(getHWND()) == TRUE;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidget::isValid()
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidget::isVisible()
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidget::requestFocus()
{
	HWND hWnd = ::SetFocus(getHWND());
	assert(hWnd != NULL);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidget::setBackground(xColor& c)
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidget::setBounds(int x, int y, int width, int height)
{
	::MoveWindow(getHWND(),x,y,width,height,true);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidget::setEnabled(bool b)
{
	::EnableWindow(getHWND(),b);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidget::setFont(xFont& f)
{
	::SendMessage((HWND) getHWND(),(UINT) WM_SETFONT,(WPARAM) f.getHFONT(),(LPARAM) TRUE);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidget::setForeground(xColor& c)
{
	throw xNotImplementedException();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidget::setLocation(int x, int y)
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
void xWidget::setSize(int width, int height)
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
void xWidget::setVisible(boolean b)
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
void xWidget::validate()
{
	::ValidateRect(getHWND(),NULL);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidget::getModifiers(WPARAM wParam)
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
LRESULT xWidget::onLButtonPressed(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_PRESSED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON1);
	processMouseEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onMButtonPressed(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_PRESSED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON3);
	processMouseEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onRButtonPressed(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_PRESSED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON2);
	processMouseEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onLButtonReleased(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_RELEASED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON1);
	processMouseEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onMButtonReleased(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_RELEASED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON3);
	processMouseEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onRButtonReleased(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_RELEASED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON2);
	processMouseEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onLButtonDBLClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_DOUBLECLICKED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON1);
	processMouseEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onMButtonDBLClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_DOUBLECLICKED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON3);
	processMouseEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onRButtonDBLClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_DOUBLECLICKED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON2);
	processMouseEvent(ev);
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onCommand(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//forward this message to child window
	void* ptr = (void*)::GetWindowLongPtr((HWND)lParam,GWL_USERDATA);
	xWidget* widg = static_cast<xWidget*>(ptr);
	if(widg != this)
		return widg->windowProcedure(hwnd,uMsg,wParam,lParam);

	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onSetFocus(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onKillFocus(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onDestroy(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onNCDestroy(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(getParent() == NULL)
	{
		delete this;
		return 0;
	}

	return onDefault(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::onDefault(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xWidget::windowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
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
		xWidget* cnt = (xWidget*)(ptr);
		return cnt->windowProcedure(hwnd,uMsg,wParam,lParam);
	}
	else
		return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}


}//namespace


#endif //XTK_USE_WIDGETS
