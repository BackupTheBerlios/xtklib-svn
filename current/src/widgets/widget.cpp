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
* @file widget.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/widgets/widget.h"
#include "../../include/xtk/widgets/container.h"
#include "../../include/xtk/base/smartptr.h"
//#include "widgets_private.h"

#ifdef XTK_USE_WIDGETS

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject xAbstractWidget::s_guiMutex;

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xAbstractWidget::xAbstractWidget(xContainer* parent)
{
	m_componentListeners.rescindOwnership();
	m_focusListeners.rescindOwnership();
	m_keyListeners.rescindOwnership();
	m_mouseListeners.rescindOwnership();
	m_mouseMotionListeners.rescindOwnership();
	if(parent != NULL)
		((xAbstractContainer*)parent)->addChild((xWidget*)this);
		
	m_parent = parent;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xPoint* xAbstractWidget::getLocationOnScreen()
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
void xAbstractWidget::processComponentEvent(xComponentEvent& e)
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
void xAbstractWidget::processFocusEvent(xFocusEvent& e)
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
void xAbstractWidget::processKeyEvent(xKeyEvent& e)
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
void xAbstractWidget::processMouseEvent(xMouseEvent& e)
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
void xAbstractWidget::processMouseMotionEvent(xMouseEvent& e) 
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
