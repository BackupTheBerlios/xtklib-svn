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
* @file window_msw.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_WINDOW_MSW_H
#define XTK_WINDOW_MSW_H

#include "../../../include/xtk/widgets/window.h"
#include "container_msw.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)
#include <Windows.h>

namespace xtk
{

/**
 * 
 */
class XTKAPI xWindowInternal : public xContainerInternal
{
private:
	xWindow::CloseAction		m_closeAction;
	
protected:
	virtual LRESULT onDestroy(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	xWindowInternal(xWidget* parent, xLayoutManager* layout,xWindow* external)
	: xContainerInternal(parent,layout,external)
	{m_closeAction = xWindow::XTK_DESTROY_ON_CLOSE;}
	
public:
	virtual ~xWindowInternal(){}

	virtual void setDefaultCloseAction(xWindow::CloseAction caction)
	{m_closeAction = caction;}

	virtual xWindow::CloseAction getDefaultCloseAction()
	{return m_closeAction;}
	
	virtual bool isActive();
	virtual void toBack();
	virtual void toFront();
};




}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_WINDOW_MSW_H
