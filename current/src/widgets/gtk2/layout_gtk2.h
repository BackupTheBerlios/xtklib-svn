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
* @file layout_gtk2.h
* @author Mario Casciaro (xshadow@email.it)
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
 *
 */	
class XTKAPI xConstraintInternal : public virtual xObject
{
protected:
	xConstraintInternal()
	{}

public:
	virtual ~xConstraintInternal()
	{}
};


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
 * The constraint for xBoxLayout
 */
class XTKAPI xBoxConstraintInternal : public xConstraintInternal
{
private:
	int 	m_place;
	bool 	m_expand;
	bool	m_fill;
	int		m_padding;

public:
	xBoxConstraintInternal(int place,bool expand,bool fill,int padding)
	{m_place = place;m_expand = expand;m_fill = fill;m_padding = padding;}
	
	virtual ~xBoxConstraintInternal()
	{}
		
	int getPlace()
	{return m_place;}
	
	bool getExpand()
	{return m_expand;}
	
	bool getFill()
	{return m_fill;}
	
	int getPadding()
	{return m_padding;}
};


/**
 * A simple Layout manager for ordering components along
 * a vertical or horizontal axis
 */
class XTKAPI xBoxLayoutInternal : public xLayoutManagerInternal
{
public:
	xBoxLayoutInternal(xBoxLayout::BoxOrientation orientation,bool homogeneous,xBoxLayout* external);

	virtual ~xBoxLayoutInternal()
	{}
	
	virtual void setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr);
	virtual void addComponent(YOUROWNERSHIP xWidget* c,MYOWNERSHIP xConstraint* cnstr);
	virtual void addComponents(xArray<YOUROWNERSHIP xWidget*> components);
	virtual void removeComponent(xWidget& c);
};



}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_LAYOUT_GTK2_H
