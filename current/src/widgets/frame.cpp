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
* @file frame.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/widgets/frame.h"

#ifdef XTK_USE_WIDGETS

//select include file
#ifdef XTK_GUI_MSW
	#include "msw/frame_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/frame_gtk2.h"
#endif

namespace xtk
{

xFrame::xFrame(xWidget* parent,xString title,int x,int y,
			   int width,int height,
			   xLayoutManager* layout,
			   xFrameInternal* i)
:xWindow(parent,layout,i)
{
	m_internal = static_cast<xFrameInternal*>(getInternal());
}

xFrame::xFrame(xWidget* parent,xString title,int x,int y,
			   int width,int height,
			   xLayoutManager* layout)
:xWindow(parent,layout,new xFrameInternal(parent,title,x,y,width,height,layout,this))
{
	m_internal = static_cast<xFrameInternal*>(getInternal());
}

xFrame::~xFrame()
{}

xString xFrame::getTitle()
{return m_internal->getTitle();}

bool xFrame::isResizable()
{return m_internal->isResizable();}

void xFrame::setResizable(bool resizable)
{m_internal->setResizable(resizable);}

void xFrame::setTitle(xString title)
{m_internal->setTitle(title);}


}//namespace

#endif //XTK_USE_WIDGETS