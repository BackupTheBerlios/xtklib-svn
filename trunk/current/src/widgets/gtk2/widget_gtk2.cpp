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

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void callback_destroy(GtkWidget* widget,gpointer data)
{
	xWidgetInternal* tmp = static_cast<xWidgetInternal*>(data);
	tmp->onDestroy(widget);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xtkUIEventLoop()
{
	gtk_main();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xtkExitUIEventLoop()
{
	gtk_main_quit();	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xtkProcessPendingUIEvent()
{
	while(gtk_events_pending())
	{
		if(gtk_main_iteration())
		{
			gtk_main_quit();
		}
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xtkWidgetsInitialize(int* argc,char*** argv)
{
	gtk_init(argc,argv);
	xThread::sleep(100);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xWidgetInternal::xWidgetInternal(xWidget* parent,xWidget* external)
{
	m_parent = parent;
	m_gtkWidget = NULL;
	m_external = external;
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
void xWidgetInternal::gtkWidgetCreated()
{
	//connect main signals
	g_signal_connect(G_OBJECT(getGtkWidget()),"destroy",G_CALLBACK(callback_destroy),(xWidgetInternal*)this);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::getPreferredSize(OUT xDimension& dim)
{
	GtkRequisition req;
	gtk_widget_size_request(getGtkWidget(),&req);
	
	dim.set(req.width,req.height);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidgetInternal::isEnabled()
{
	return GTK_WIDGET_IS_SENSITIVE(getGtkWidget());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidgetInternal::isFocusable()
{
	return GTK_WIDGET_CAN_FOCUS(getGtkWidget());
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidgetInternal::isFocusOwner()
{
	return GTK_WIDGET_HAS_FOCUS(getGtkWidget());
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xWidgetInternal::isVisible()
{
	return GTK_WIDGET_VISIBLE(getGtkWidget());
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::requestFocus()
{
	if(isFocusable())
		gtk_widget_grab_focus(getGtkWidget());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::setEnabled(bool b)
{
	gtk_widget_set_sensitive(getGtkWidget(),b);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::setPreferredSize(int width, int height)
{
	gtk_widget_set_size_request(getGtkWidget(),(gint)width,(gint)height);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::setVisible(bool b)
{
	if(b)
		gtk_widget_show(getGtkWidget());
	else
		gtk_widget_hide(getGtkWidget());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetInternal::onDestroy(GtkWidget* widget)
{
	delete getExternal();
}



}//namespace


#endif //XTK_USE_WIDGETS
