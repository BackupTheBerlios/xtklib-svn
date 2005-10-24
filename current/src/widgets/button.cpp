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
* @file container.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/widgets/button.h"

#ifdef XTK_USE_WIDGETS

//select include file
#ifdef XTK_GUI_MSW
	#include "msw/button_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/button_gtk2.h"
#endif

namespace xtk
{

xButton::xButton(xWidget* parent,xString label,xButtonInternal* i)
: xWidget(parent,i)
{
	m_internal = static_cast<xButtonInternal*>(getInternal());
}

xButton::xButton(xWidget* parent,xString label)
: xWidget(parent,new xButtonInternal(parent,label,this))
{
	m_internal = static_cast<xButtonInternal*>(getInternal());
}

xButton::~xButton()
{}

xString xButton::getLabel()
{return m_internal->getLabel();}

void xButton::setLabel(xString label)
{m_internal->setLabel(label);}


}//namespace

#endif //XTK_USE_WIDGETS
