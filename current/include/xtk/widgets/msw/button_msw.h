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
* @file button_msw.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_BUTTON_MSW_H
#define XTK_BUTTON_MSW_H

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <Windows.h>

namespace xtk
{


/**
* This class creates a labeled button
*/
class XTKAPI xButton : public xWidget,public xIButton
{
private:
	WNDPROC m_baseWndProc;
	
protected:
	virtual LRESULT onSetFocus(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onKillFocus(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onCommand(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onDefault(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	
public:
	xButton(xContainer* parent,xString label = _T(""),int x = XTK_DEFAULT_WIDGET_POSITION,
		int y = XTK_DEFAULT_WIDGET_POSITION,int width = XTK_DEFAULT_WIDGET_SIZE,
		int height = XTK_DEFAULT_WIDGET_SIZE);
		
	virtual ~xButton();
	
	virtual xString getLabel();
	virtual void setLabel(xString label);
};




}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_BUTTON_MSW_H