/*
* This file is part of XTKLib project.
*
* Copyright(C) 2003-2006 Mario Casciaro xshadow[AT]email(DOT)it
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
* @file label_gtk2.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_LABEL_GTK2_H
#define XTK_LABEL_GTK2_H

#include "../../../include/xtk/widgets/label.h"
#include "widget_gtk2.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)
#include <gtk/gtk.h>

namespace xtk
{

/**
 * 
 */
class XTKAPI xLabelInternal : public xWidgetInternal
{
public:
	xLabelInternal(xWidget* parent,xString label,xLabel* external)
	: xWidgetInternal(parent,external)
	{
		m_gtkWidget = gtk_label_new(label.mb_str(xCharset::CS_UTF8));
		gtkWidgetCreated();
		setVisible(true);
	}
	
	virtual ~xLabelInternal()
	{}

	virtual xString getText()
	{return xString(gtk_label_get_text(GTK_LABEL(getGtkWidget())),xCharset::CS_UTF8);}
	
	virtual void setText(xString label)
	{gtk_label_set_text(GTK_LABEL(getGtkWidget()),label.mb_str(xCharset::CS_UTF8));}
};



}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_LABEL_GTK2_H

