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
#include "../../../include/xtk/base/thread.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

namespace xtk
{

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
xWidget::xWidget(xContainer* parent) : xAbstractWidget(parent)
{
	m_hWidget = NULL;
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
	::GetWindowRect(getHWND(),&rect);

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
	throw xNotImplementedException();
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
	::GetWindowRect(getHWND(),&rect);

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
	::GetWindowRect(getHWND(),&rect);

	return new xPoint(rect.left,rect.top);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xDimension* xWidget::getSize()
{
	RECT rect;
	::GetWindowRect(getHWND(),&rect);

	return new xDimension(rect.right - rect.left,rect.bottom - rect.top);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidget::getWidth()
{
	RECT rect;
	::GetWindowRect(getHWND(),&rect);

	return rect.right - rect.left;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidget::getX()
{
	RECT rect;
	::GetWindowRect(getHWND(),&rect);

	return rect.left;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xWidget::getY()
{
	RECT rect;
	::GetWindowRect(getHWND(),&rect);

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
	throw xNotImplementedException();
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


}//namespace

#endif //XTK_USE_WIDGETS
