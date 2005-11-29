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
* @file button_gtk2.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_BUTTON_GTK2_H
#define XTK_BUTTON_GTK2_H

#include "../../../include/xtk/widgets/button.h"
#include "widget_gtk2.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)
#include <gtk/gtk.h>

namespace xtk
{

/**
 * 
 */
class XTKAPI xButtonInternal : public xWidgetInternal
{
protected:
	virtual void gtkWidgetCreated();
	
public:
	xButtonInternal(xWidget* parent,xString label,xButton* external)
	: xWidgetInternal(parent,external)
	{
		m_gtkWidget = gtk_button_new_with_label(label.mb_str(xCharset::CS_UTF8));
		gtkWidgetCreated();
		setVisible(true);
	}
	
	virtual ~xButtonInternal()
	{}

	virtual xString getLabel()
	{return xString(gtk_button_get_label(GTK_BUTTON(getGtkWidget())),xCharset::CS_UTF8);}
	
	virtual void setLabel(xString label)
	{gtk_button_set_label(GTK_BUTTON(getGtkWidget()),label.mb_str(xCharset::CS_UTF8));}
	
	virtual void onClicked()
	{xActionEvent ev(getExternal(),XWE_ACTION_PERFORMED);processEvent(ev);}
};



}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_BUTTON_GTK2_H
