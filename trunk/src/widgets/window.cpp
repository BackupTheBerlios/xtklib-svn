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
* @file window.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/widgets/window.h"

#ifdef XTK_USE_WIDGETS

//select include file
#ifdef XTK_GUI_MSW
	#include "msw/window_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/window_gtk2.h"
#endif

namespace xtk
{

xWindow::xWindow(xWidget* parent,xLayoutManager* layout,xWindowInternal* i)
: xContainer(parent,layout,i)
{m_internal = static_cast<xWindowInternal*>(getInternal());}

xWindow::~xWindow()
{}

bool xWindow::isActive()
{return m_internal->isActive();}

void xWindow::setSize(int width,int height)
{m_internal->setSize(width,height);}

void xWindow::toBack()
{m_internal->toBack();}

void xWindow::toFront()
{m_internal->toFront();}

void xWindow::setDefaultCloseAction(xWindow::CloseAction caction)
{m_internal->setDefaultCloseAction(caction);}

xWindow::CloseAction xWindow::getDefaultCloseAction()
{return m_internal->getDefaultCloseAction();}


}//namespace

#endif //XTK_USE_WIDGETS
