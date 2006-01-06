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
* @file container_gtk2.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_CONTAINER_GTK2_H
#define XTK_CONTAINER_GTK2_H

#include "../../../include/xtk/widgets/container.h"
#include "widget_gtk2.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)
#include <gtk/gtk.h>

namespace xtk
{

/**
 *
 */
class XTKAPI xContainerInternal : public xWidgetInternal
{
friend class xContainer;
protected:
	xLinkedList			m_components;
	xLayoutManager*		m_layout;

	xContainerInternal(xWidget* parent, xLayoutManager* layout,xContainer* external);
	
	virtual void gtkWidgetCreated();
public:
	virtual ~xContainerInternal();
	
	virtual void removeChild(xWidget& comp);
	virtual void addChild(xWidget* comp);
	
	virtual xLayoutManager& getLayout()
	{return *m_layout;}
	
	virtual void setLayout(MYOWNERSHIP xLayoutManager* mgr);
	
	virtual int getComponentCount()
	{return m_components.size();}
	
	virtual xArray<MYOWNERSHIP xWidget*> getComponents();
	virtual bool isAnchestorOf(xWidget& c);
};

}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_CONTAINER_GTK2_H
