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
* @file widgeteventgenerator.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/widgets/widgeteventgenerator.h"
#include "../../include/xtk/base/smartptr.h"

#if defined(XTK_USE_WIDGETS)

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xMouseEventGenerator::processMouseEvent(xMouseEvent& e)
{
	if(m_mouseListeners.isEmpty())
		return;
	
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
void xActionEventGenerator::processActionEvent(xActionEvent& e)
{
	if(m_actionListeners.isEmpty())
		return;

	smartPtr<xIterator> iter = m_actionListeners.iterator();
	while(iter->hasNext())
	{
		xActionListener* l = dynamic_cast<xActionListener*>(&(iter->next()));
		assert(l != NULL);
		switch(e.getID())
		{
		case XWE_ACTION_PERFORMED:		l->actionPerformed(e);		break;
		default:						assert(false);				break;
		}
	}
}


}//namespace

#endif //XTK_USE_WIDGETS