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
* @file frame_msw.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_FRAME_MSW_H
#define XTK_FRAME_MSW_H

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <windows.h>

namespace xtk
{

/**
* A xFrame object is a top-level window.
*/
class XTKAPI xFrame : public xAbstractFrame,public xWindow
{
private:
	xString		m_title;
	bool		m_resizable;
	
public:
	xFrame(xString title,int x = xFrame::XTK_DEFAULT_WINDOW_POSITION,int y = xFrame::XTK_DEFAULT_WINDOW_POSITION,
		int width = xFrame::XTK_DEFAULT_WINDOW_SIZE,int height = xFrame::XTK_DEFAULT_WINDOW_SIZE,
		xContainer* parent = NULL, xLayoutManager* layout = new xBoxLayout(xBoxLayout::X_AXIS));
	
	virtual ~xFrame();


	virtual xString getTitle(){return m_title;}
	virtual bool isResizable(){return m_resizable;}
	virtual void setResizable(bool resizable){m_resizable = resizable;}
	virtual void setTitle(xString title);
	virtual LRESULT windowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
};




}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_FRAME_MSW_H
