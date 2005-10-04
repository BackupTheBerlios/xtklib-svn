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
* @file window_msw.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../../include/xtk/widgets/window.h"
#include "../../../include/xtk/base/smartptr.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWindow::isActive()
{
	return ::GetActiveWindow() == getHWND();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWindow::toBack()
{
	::SetWindowPos(
				getHWND(),				// handle of window
				HWND_NOTOPMOST,			// placement-order handle
				0,						// horizontal position
				0,						// vertical position
				0,						// width
				0,						// height
				SWP_NOMOVE|SWP_NOSIZE	// window-positioning flags
				);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWindow::toFront()
{
	::SetWindowPos(
		getHWND(),				// handle of window
		HWND_TOP,				// placement-order handle
		0,						// horizontal position
		0,						// vertical position
		0,						// width
		0,						// height
		SWP_NOMOVE|SWP_NOSIZE	// window-positioning flags
		);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWindow::processWindowEvent(xWindowEvent& e)
{	
	smartPtr<xIterator> iter = m_windowListeners.iterator();
	while(iter->hasNext())
	{
		xWindowListener* l = dynamic_cast<xWindowListener*>(&(iter->next()));
		assert(l != NULL);
		switch(e.getID())
		{
		case XWE_WINDOW_ACTIVATED:		l->windowActivated(e);		break;
		case XWE_WINDOW_CLOSED:			l->windowClosed(e);			break;
		case XWE_WINDOW_CLOSING:		l->windowClosing(e);		break;
		case XWE_WINDOW_DEACTIVATED:	l->windowDeactivated(e);	break;
		case XWE_WINDOW_DEICONIFIED:	l->windowDeiconified(e);	break;
		case XWE_WINDOW_FIRST:			l->windowFirst(e);			break;
		case XWE_WINDOW_ICONIFIED:		l->windowIconified(e);		break;
		case XWE_WINDOW_LAST:			l->windowLast(e);			break;
		case XWE_WINDOW_OPENED:			l->windowOpened(e);			break;
		default:						assert(false);				break;
		}
	}
}
	
}//namspace

#endif//XTK_USE_WIDGETS
