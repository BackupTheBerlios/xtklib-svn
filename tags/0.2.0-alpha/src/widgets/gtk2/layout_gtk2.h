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
* @file layout_gtk2.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_LAYOUT_GTK2_H
#define XTK_LAYOUT_GTK2_H

#include "../../../include/xtk/widgets/widget.h"
#include "../../../include/xtk/widgets/layout.h"
#include "../widgeteventgenerator_internal.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)
#include <gtk/gtk.h>

namespace xtk
{


/**
 * Defines the interface for classes that know how to lay out xContainers.
 */
class XTKAPI xLayoutManagerInternal : public virtual xObject
{
protected:
	xLayoutManager* m_external;
	GtkWidget*		m_gtkWidget;

	xLayoutManagerInternal(xLayoutManager* external)
	{m_external = external;m_gtkWidget = NULL;}
	
public:
	virtual ~xLayoutManagerInternal()
	{}
	
	GtkWidget* getGtkWidget()
	{return m_gtkWidget;}
		
	xLayoutManager* getExternal()
	{return m_external;}
	
	/**
	* Sets the constraints for the specified component in this layout.
	*/
	virtual void setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr) = 0;
	
	virtual void setPlace(xWidget& c,int place) = 0;
	
	/**
	 *
	 */
	virtual void addComponent(YOUROWNERSHIP xWidget* c,MYOWNERSHIP xConstraint* cnstr) = 0;
	
	/**
	 *
	 */
	virtual void addComponents(xArray<YOUROWNERSHIP xWidget*> components) = 0;
	
	/**
	 *
	 */
	virtual void removeComponent(xWidget& c) = 0;
};


/**
 * A simple Layout manager for ordering components along
 * a vertical or horizontal axis
 */
class XTKAPI xBoxLayoutInternal : public xLayoutManagerInternal
{
private:
	int m_padding;
	
public:
	xBoxLayoutInternal(xBoxLayout::BoxOrientation orientation,bool homogeneous,int padding,xBoxLayout* external);

	virtual ~xBoxLayoutInternal()
	{}
	
	virtual void setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr);
	virtual void addComponent(YOUROWNERSHIP xWidget* c,MYOWNERSHIP xConstraint* cnstr);
	virtual void addComponents(xArray<YOUROWNERSHIP xWidget*> components);
	virtual void setPlace(xWidget& c,int place);
	virtual void removeComponent(xWidget& c);
	
	int getPadding()
	{return m_padding;}
};



}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_LAYOUT_GTK2_H
