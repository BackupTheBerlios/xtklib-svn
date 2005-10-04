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
xWidget::xWidget(xContainer* parent) : xIWidget(parent)
{
	m_componentListeners.rescindOwnership();
	m_focusListeners.rescindOwnership();
	m_keyListeners.rescindOwnership();
	m_mouseListeners.rescindOwnership();
	m_mouseMotionListeners.rescindOwnership();
	m_parent = parent;
	m_hWidget = NULL;
	if(parent != NULL)
		((xIContainer*)parent)->addChild(this);
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
void xWidget::processComponentEvent(xComponentEvent& e)
{
	smartPtr<xIterator> iter = m_componentListeners.iterator();
	while(iter->hasNext())
	{
		xComponentListener* l = dynamic_cast<xComponentListener*>(&(iter->next()));
		assert(l != NULL);
		switch(e.getID())
		{
		case XWE_COMPONENT_HIDDEN:		l->componentHidden(e);	break;
		case XWE_COMPONENT_MOVED:		l->componentMoved(e);	break;
		case XWE_COMPONENT_RESIZED:		l->componentResized(e); break;
		case XWE_COMPONENT_SHOWN:		l->componentShown(e);	break;
		default:						assert(false);			break;
		}
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidget::processFocusEvent(xFocusEvent& e)
{
	smartPtr<xIterator> iter = m_focusListeners.iterator();
	while(iter->hasNext())
	{
		xFocusListener* l = dynamic_cast<xFocusListener*>(&(iter->next()));
		assert(l != NULL);
		switch(e.getID())
		{
		case XWE_FOCUS_GAINED:		l->focusGained(e);	break;
		case XWE_FOCUS_LOST:		l->focusLost(e);	break;
		default:					assert(false);		break;
		}
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidget::processKeyEvent(xKeyEvent& e)
{
	smartPtr<xIterator> iter = m_keyListeners.iterator();
	while(iter->hasNext())
	{
		xKeyListener* l = dynamic_cast<xKeyListener*>(&(iter->next()));
		assert(l != NULL);
		switch(e.getID())
		{
		case XWE_KEY_PRESSED:		l->keyPressed(e);	break;
		case XWE_KEY_RELEASED:		l->keyReleased(e);	break;
		case XWE_KEY_TYPED:			l->keyTyped(e);		break;
		default:					assert(false);		break;
		}
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidget::processMouseEvent(xMouseEvent& e)
{
	smartPtr<xIterator> iter = m_mouseListeners.iterator();
	while(iter->hasNext())
	{
		xMouseListener* l = dynamic_cast<xMouseListener*>(&(iter->next()));
		assert(l != NULL);
		switch(e.getID())
		{
		case XWE_MOUSE_CLICKED:			l->mouseClicked(e);			break;
		case XWE_MOUSE_DOUBLECLICKED:	l->mouseDoubleClicked(e);	break;
		case XWE_MOUSE_PRESSED:			l->mousePressed(e);			break;
		case XWE_MOUSE_RELEASED:		l->mouseReleased(e);		break;
		default:						assert(false);				break;
		}
	}
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidget::processMouseMotionEvent(xMouseEvent& e) 
{
	smartPtr<xIterator> iter = m_mouseMotionListeners.iterator();
	while(iter->hasNext())
	{
		xMouseMotionListener* l = dynamic_cast<xMouseMotionListener*>(&(iter->next()));
		assert(l != NULL);
		switch(e.getID())
		{
		case XWE_MOUSE_MOVED:			l->mouseMoved(e);		break;
		case XWE_MOUSE_DRAGGED:			l->mouseDragged(e);		break;
		default:						assert(false);			break;
		}
	}
}

}//namespace

#endif //XTK_USE_WIDGETS
