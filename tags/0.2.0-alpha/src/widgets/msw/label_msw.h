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
* @file label_msw.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_LABEL_MSW_H
#define XTK_LABEL_MSW_H


#include "../../../include/xtk/widgets/label.h"
#include "../../../include/xtk/base/smartptr.h"
#include "widget_msw.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)
#include <Windows.h>

namespace xtk
{


/**
* A label is a static widget that cannot interact with the user,used to display texts 
* other drawings
*/
class XTKAPI xLabelInternal : public xWidgetInternal
{
private:
	WNDPROC				m_baseWndProc;
	smartPtr<xFont>		m_font;
	xString				m_text;

protected:
	virtual LRESULT onDefault(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onPaint(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onSetFont(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onGetFont(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

public:
	xLabelInternal(xWidget* parent,xString text,xLabel* external);

	virtual ~xLabelInternal();

	virtual xString getText()
	{return m_text;}

	virtual void setText(xString label)
	{m_text = label;}

	virtual void sizeRequest(xDimension& dim);
};




}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_LABEL_MSW_H
