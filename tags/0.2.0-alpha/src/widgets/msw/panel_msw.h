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
* @file panel_msw.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_PANEL_MSW_H
#define XTK_PANEL_MSW_H

#include "../../../include/xtk/widgets/panel.h"
#include "container_msw.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)
#include <Windows.h>

namespace xtk
{


/**
* A panel is a widget used to organize and group other widgets
*/
class XTKAPI xPanelInternal : public xContainerInternal
{
private:
	WNDPROC m_baseWndProc;
	xPanel::Border	m_border;
	
protected:
	virtual LRESULT onDefault(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

public:
	xPanelInternal(xWidget* parent,xPanel::Border border,xLayoutManager* layout,xString label,xPanel* external);

	virtual ~xPanelInternal();

	virtual xString getLabel();
	virtual void setLabel(xString label);

	//-------INIT-------------------------------------------------------------//
	//-------MSW SPECIFIC-----------------------------------------------------//
	//------------------------------------------------------------------------//

	/**
	 * @todo resolve the workaround about offsets set statically
	 */
	virtual void doLayout();
	//------------------------------------------------------------------------//
	//-------MSW SPECIFIC-----------------------------------------------------//
	//-------END--------------------------------------------------------------//
};




}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_BUTTON_MSW_H
