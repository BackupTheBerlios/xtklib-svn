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
* @file panel.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/widgets/panel.h"

#ifdef XTK_USE_WIDGETS

//select include file
#ifdef XTK_GUI_MSW
	#include "msw/panel_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/panel_gtk2.h"
#endif

namespace xtk
{

xPanel::xPanel(xWidget* parent,xPanel::Border border,xLayoutManager* layout,xString label,xPanelInternal* i)
: xContainer(parent,layout,i)
{
	m_internal = static_cast<xPanelInternal*>(getInternal());
}


xPanel::xPanel(xWidget* parent,xPanel::Border border,xLayoutManager* layout,xString label)
: xContainer(parent,layout,new xPanelInternal(parent,border,layout,label,this))
{
	m_internal = static_cast<xPanelInternal*>(getInternal());
}

xPanel::~xPanel()
{}

xString xPanel::getLabel()
{return m_internal->getLabel();}

void xPanel::setLabel(xString label)
{m_internal->setLabel(label);}



}//namespace

#endif //XTK_USE_WIDGETS
