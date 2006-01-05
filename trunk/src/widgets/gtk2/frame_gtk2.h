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
* @file frame_gtk2.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_FRAME_GTK2_H
#define XTK_FRAME_GTK2_H

#include "../../../include/xtk/widgets/frame.h"
#include "window_gtk2.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)
#include <gtk/gtk.h>

namespace xtk
{

/**
 * 
 */
class XTKAPI xFrameInternal : public xWindowInternal
{
public:
	xFrameInternal(xWidget* parent,xString title,xLayoutManager* layout,xFrame* external)
	: xWindowInternal(parent,layout,external)
	{
		m_gtkWidget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		setTitle(title);
		gtkWidgetCreated();
	}
	
	virtual ~xFrameInternal()
	{}

	virtual xString getTitle()
	{return xString(gtk_window_get_title(GTK_WINDOW(getGtkWidget())),xCharset::CS_UTF8);}
	
	virtual bool isResizable()
	{return gtk_window_get_resizable(GTK_WINDOW(getGtkWidget()));}
	
	virtual void setResizable(bool resizable)
	{gtk_window_set_resizable(GTK_WINDOW(getGtkWidget()),TRUE);}
	
	virtual void setTitle(xString title)
	{gtk_window_set_title(GTK_WINDOW(getGtkWidget()),title.mb_str(xCharset::CS_UTF8));}
};
	



}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_FRAME_GTK2_H
