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
LRESULT xWindow::onDestroy(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(getDefaultCloseAction() == XTK_EXIT_ON_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}
	else if(getDefaultCloseAction() == XTK_HIDE_ON_CLOSE)
		setVisible(false);
		
	return ::DefWindowProc(hwnd,uMsg,wParam,lParam);
}
	
}//namspace

#endif//XTK_USE_WIDGETS
