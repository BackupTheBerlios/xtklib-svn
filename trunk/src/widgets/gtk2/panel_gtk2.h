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
* @file panel_gtk2.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_PANEL_GTK2_H
#define XTK_PANEL_GTK2_H

#include "../../../include/xtk/widgets/panel.h"
#include "container_gtk2.h"
#include "layout_gtk2.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)
#include <gtk/gtk.h>

namespace xtk
{

/**
 * 
 */
class XTKAPI xPanelInternal : public xContainerInternal
{
private:
	xPanel::Border m_border;
	
protected:
	/**
	 * Do all necessary initializations
	 */
	virtual void gtkWidgetCreated()
	{xWidgetInternal::gtkWidgetCreated();}
	
public:
	xPanelInternal(xWidget* parent,xPanel::Border border,xLayoutManager* layout,xString label,xPanel* external)
	: xContainerInternal(parent,layout,external)
	{
		m_border = border;
		if(border == xPanel::BORDER_TITLED)
		{
			m_gtkWidget = gtk_frame_new(label.mb_str(xCharset::CS_UTF8));
			xContainerInternal::gtkWidgetCreated();
			gtk_widget_show(getGtkWidget());
		}
		else
		{
			m_gtkWidget = layout->getInternal()->getGtkWidget();
			gtkWidgetCreated();
		}
	}

	virtual ~xPanelInternal()
	{}

	virtual xString getLabel()
	{
		if(m_border != xPanel::BORDER_TITLED)
			return xString();
		else
			return xString(gtk_frame_get_label(GTK_FRAME(getGtkWidget())),xCharset::CS_UTF8);
	}
	
	virtual void setLabel(xString label)
	{if(m_border == xPanel::BORDER_TITLED) gtk_frame_set_label(GTK_FRAME(getGtkWidget()),label.mb_str(xCharset::CS_UTF8));}
	
	/**
	 *@todo TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
	*/
	virtual void setLayout(MYOWNERSHIP xLayoutManager* mgr)
	{}
};
	



}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_PANEL_GTK2_H
