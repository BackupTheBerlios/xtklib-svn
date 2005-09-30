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
* @file widget_msw.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_WIDGET_MSW_H
#define XTK_WIDGET_MSW_H

#include "../../globals.h"
#include "../widget.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <Windows.h>

namespace xtk
{

/**
 * A widget is the base class for all Gui components.
 * Note that all children of the window will be deleted automatically by 
 * the destructor before the window itself is deleted.
*/
class XTKAPI xWidget : public xAbstractWidget
{
private:
	//In MSW all principal widgets are windows
	HWND			m_hWidget;

protected:
	void setHWND(HWND hWnd){m_hWidget = hWnd;}
	
	xWidget(xContainer* parent);
public:
	virtual ~xWidget();
	
	HWND getHWND(){return m_hWidget;}
		
	//! @todo to implement (MSW)
	virtual xColor* getBackground();
	virtual xRectangle* getBounds();
	//! @todo to implement (MSW)
	virtual NODELETE xWidget* getComponentAt(int x, int y);
	//! @todo to implement (MSW)
	virtual xFont* getFont();
	//! @todo to implement (MSW)
	virtual xColor* getForeground();
	virtual int getHeight();
	//! @todo to implement (MSW)
	virtual bool getIgnoreRepaint();
	virtual xPoint* getLocation();
	virtual xDimension* getSize();
	virtual int getWidth();
	virtual int getX();
	virtual int getY();
	virtual void invalidate();
	virtual bool isEnabled();
	virtual bool isFocusOwner();
	virtual bool isShowing();
	//! @todo to implement (MSW)
	virtual bool isValid();
	//! @todo to implement (MSW)
	virtual bool isVisible();
	virtual void requestFocus();
	//! @todo to implement (MSW)
	virtual void setBackground(xColor& c);
	virtual void setBounds(int x, int y, int width, int height);
	virtual void setEnabled(bool b);
	//! @todo to implement (MSW)
	virtual void setFont(xFont& f);
	//! @todo to implement (MSW)
	virtual void setForeground(xColor& c);
	virtual void setLocation(int x, int y);
	virtual void setSize(int width, int height);
	virtual void setVisible(boolean b);
	virtual void validate();
};




}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_WIDGET_MSW_H
