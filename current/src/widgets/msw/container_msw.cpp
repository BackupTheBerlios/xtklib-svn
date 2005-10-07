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
* @file container_msw.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../../include/xtk/widgets/container.h"
#include "widgets_msw_private.h"
#include "../../../include/xtk/base/smartptr.h"

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT CALLBACK xContainerWindowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	void* ptr = (void*)(::GetWindowLongPtr(hwnd,GWL_USERDATA));
	if(ptr != NULL)
	{
		xContainer* cnt = static_cast<xContainer*>((xWidget*)(ptr));
		return cnt->windowProcedure(hwnd,uMsg,wParam,lParam);
	}
	else
		return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xContainer::xContainer(xContainer* parent, xLayoutManager* layout) 
	: xIContainer(layout),xWidget(parent)
{
	m_layout = layout;
	//m_components.rescindOwnership();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xContainer::~xContainer()
{
	delete m_layout;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xContainer::doLayout()
{
	//first childs
	smartPtr<xIterator> iter = m_components.iterator();
	while(iter->hasNext())
	{
		xContainer* c = dynamic_cast<xContainer*>(&(iter->next()));
		if(c != NULL)
			c->doLayout();
	}

	//then our layout
	xDimension* dim = getSize();
	getLayout().doLayout(*dim);
	delete dim;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
NODELETE xWidget* xContainer::getChildByHandle(HWND hWnd)
{
	xHwndObject h(hWnd);
	xObject& o = m_components.get(h);
	if(o.isNull())
		return NULL;
	return dynamic_cast<xWidget*>(&o);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xContainer::removeChild(xWidget& comp)
{
	xHwndObject h(comp.getHWND());
	m_components.remove(h);
	m_layout->removeComponent(comp);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xContainer::addChild(xWidget* comp)
{
	m_components.put(new xHwndObject(comp->getHWND()),comp);
	m_layout->addComponent(comp);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArray<NODELETE xWidget*> xContainer::getComponents()
{
	return smartPtr<xCollection>(m_components.values())->toArray().castTo<xWidget*>();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xContainer::isAncestorOf(xWidget& c)
{
	//verify this level
	xHwndObject h(c.getHWND());
	if(m_components.containsKey(h))
		return true;
		
	//now for every child container
	smartPtr<xIterator> iter = m_components.iterator();
	while(iter->hasNext())
	{
		xContainer* cn = dynamic_cast<xContainer*>(&(iter->next()));
		if(cn != NULL)
			if(cn->isAncestorOf(c))
				return true;
	}
	
	return false;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xContainer::getModifiers(WPARAM wParam)
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
LRESULT xContainer::onLButtonPressed(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_PRESSED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON1);
	processMouseEvent(ev);
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xContainer::onMButtonPressed(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_PRESSED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON3);
	processMouseEvent(ev);
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xContainer::onRButtonPressed(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_PRESSED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON2);
	processMouseEvent(ev);
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xContainer::onLButtonReleased(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_RELEASED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON1);
	processMouseEvent(ev);
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xContainer::onMButtonReleased(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_RELEASED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON3);
	processMouseEvent(ev);
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xContainer::onRButtonReleased(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_RELEASED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON2);
	processMouseEvent(ev);
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xContainer::onLButtonDBLClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_DOUBLECLICKED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON1);
	processMouseEvent(ev);
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xContainer::onMButtonDBLClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_DOUBLECLICKED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON3);
	processMouseEvent(ev);
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xContainer::onRButtonDBLClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	xMouseEvent ev(this,XWE_MOUSE_DOUBLECLICKED,getModifiers(wParam),LOWORD(lParam),
		HIWORD(lParam),xMouseEvent::BUTTON2);
	processMouseEvent(ev);
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xContainer::forwardToChild(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//forward this message to child window
	void* ptr = (void*)::GetWindowLongPtr((HWND)lParam,GWL_USERDATA);
	xWidget* widg = static_cast<xWidget*>(ptr);
	if(widg != this)
		return widg->windowProcedure(hwnd,uMsg,wParam,lParam);
	
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xContainer::onDestroy(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xContainer::onNCDestroy(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(getParent() == NULL)
		delete this;
		
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
LRESULT xContainer::windowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
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
	//Other events-------------------------------
	case WM_COMMAND:
		return forwardToChild(hwnd,uMsg,wParam,lParam);
	case WM_NCDESTROY:
		return onNCDestroy(hwnd,uMsg,wParam,lParam);
	case WM_DESTROY:
		return onDestroy(hwnd,uMsg,wParam,lParam);
	}

	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}

}//namspace

#endif//XTK_USE_WIDGETS
