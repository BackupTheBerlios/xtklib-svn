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
	static xObject	s_guiMutex;
	xWidget*		m_parent;
	
	GtkWidget*	m_gtkWidget;

protected:
	xWidget* m_external;

	/**
	 * 
	 */
	static xObject& getSyncObj(){return s_guiMutex;}
	
	/**
	 * 
	 */
	void setGtkWidget(GtkWidget* gtkWidget)
	{m_gtkWidget = gtkWidget;}
	
	xWidgetInternal(xWidget* parent,xWidget* external);
public:
	virtual ~xWidgetInternal();
	
	xWidget* getExternal()
	{return m_external;}
	
	GtkWidget* getGtkWidget()
	{return m_gtkWidget;}
	
	virtual bool contains(int x, int y)
	{	
		return 
			(x >= getX() && x <= getX()+getWidth()) &&
			(y >= getY() && y <= getY()+getHeight());
	}
	
	virtual void doLayout()
	{}

	virtual NODELETE xWidget* getParent()
	{return m_parent;}
	
	virtual void setBounds(xRectangle& r)
	{setBounds(r.getX(),r.getY(),r.getWidth(),r.getHeight());}
	
	//! @todo to implement (GTK2)
	virtual xColor* getBackground();
	virtual xRectangle* getBounds();
	//! @todo to implement (GTK2)
	virtual MYOWNERSHIP xWidget* getComponentAt(int x, int y);
	virtual xFont* getFont();
	virtual xFontMetrics* getFontMetrics();
	//! @todo to implement (GTK2)
	virtual xColor* getForeground();
	virtual int getHeight();
	//! @todo to implement (GTK2)
	virtual bool getIgnoreRepaint();
	virtual xPoint* getLocation();
	virtual xPoint* getLocationOnScreen();
	virtual xDimension* getSize();
	virtual int getWidth();
	virtual int getX();
	virtual int getY();
	virtual void invalidate();
	virtual bool isEnabled();
	virtual bool isFocusable();
	virtual bool isFocusOwner();
	virtual bool isShowing();
	//! @todo to implement (GTK2)
	virtual bool isValid();
	//! @todo to implement (GTK2)
	virtual bool isVisible();
	virtual void requestFocus();
	//! @todo to implement (GTK2)
	virtual void setBackground(xColor& c);
	virtual void setBounds(int x, int y, int width, int height);
	virtual void setEnabled(bool b);
	virtual void setFont(xFont& f);
	//! @todo to implement (GTK2)
	virtual void setForeground(xColor& c);
	virtual void setLocation(int x, int y);
	virtual void setSize(int width, int height);
	virtual void setVisible(boolean b);
	virtual void validate();
};

}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_WIDGET_GTK2_H
