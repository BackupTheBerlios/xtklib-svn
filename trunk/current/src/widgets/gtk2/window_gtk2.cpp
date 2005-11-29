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

#include "window_gtk2.h"

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
gboolean callback_delete_event(GtkWidget* widget,GdkEvent* event,gpointer data)
{
	static_cast<xWindowInternal*>(data)->onDeleteEvent(widget,event);
	return true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWindowInternal::gtkWidgetCreated()
{
	xContainerInternal::gtkWidgetCreated();
	g_signal_connect(G_OBJECT(getGtkWidget()),"delete_event",G_CALLBACK(callback_delete_event),(xWindowInternal*)this);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWindowInternal::onDeleteEvent(GtkWidget* widget,GdkEvent* event)
{
	if(m_closeAction == xWindow::XTK_EXIT_ON_CLOSE || m_closeAction == xWindow::XTK_DESTROY_ON_CLOSE)
		gtk_widget_destroy(getGtkWidget());
	else
		setVisible(false);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWindowInternal::onDestroy(GtkWidget* widget)
{
	xWidgetInternal::onDestroy(widget);
	if(m_closeAction == xWindow::XTK_EXIT_ON_CLOSE)
		gtk_main_quit();
}


}//namespace


#endif //XTK_USE_WIDGETS
