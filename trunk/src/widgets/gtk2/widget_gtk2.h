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
* @file widget_gtk2.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_WIDGET_GTK2_H
#define XTK_WIDGET_GTK2_H

#include "../../../include/xtk/widgets/widget.h"
#include "../widgeteventgenerator_internal.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)
#include <gtk/gtk.h>

namespace xtk
{

/**
 *
 */
class XTKAPI xWidgetInternal : public virtual xObject,public xWidgetEventGeneratorInternal
{
private:
	xWidget*		m_parent;

protected:
	GtkWidget*		m_gtkWidget;
	xWidget* 		m_external;
	
	/**
	 * 
	 */
	void setGtkWidget(GtkWidget* gtkWidget)
	{m_gtkWidget = gtkWidget;}
	
	xWidgetInternal(xWidget* parent,xWidget* external);
	
	/**
	 * Do all necessary initializations
	 */
	virtual void gtkWidgetCreated();
	
public:
	virtual ~xWidgetInternal();
	
	xWidget* getExternal()
	{return m_external;}
	
	xWidget* getParent()
	{return m_parent;}
	
	GtkWidget* getGtkWidget()
	{return m_gtkWidget;}
		
	virtual void getPreferredSize(OUT xDimension& dim);
	virtual bool isEnabled();
	virtual bool isFocusable();
	virtual bool isFocusOwner();
	virtual bool isVisible();
	virtual void requestFocus();
	virtual void setEnabled(bool b);
	virtual void setPreferredSize(int width, int height);
	virtual void setVisible(bool b);
	
	virtual void onDestroy(GtkWidget* widget);
};

}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_WIDGET_GTK2_H
