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
* @file frame_msw.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_FRAME_MSW_H
#define XTK_FRAME_MSW_H

#include "../../../include/xtk/widgets/frame.h"
#include "window_msw.h"

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)
#include <windows.h>

namespace xtk
{

/**
* 
*/
class XTKAPI xFrameInternal : public xWindowInternal
{
private:
	xString		m_title;
	bool		m_resizable;
	
public:
	xFrameInternal(xWidget* parent,xString title,xLayoutManager* layout,xFrame* external);
	
	virtual ~xFrameInternal();


	virtual xString getTitle()
	{return m_title;}
	
	virtual bool isResizable()
	{return m_resizable;}

	virtual void setResizable(bool resizable)
	{m_resizable = resizable;}
	
	virtual void setTitle(xString title);
};




}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_FRAME_MSW_H
