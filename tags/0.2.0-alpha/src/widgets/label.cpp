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
* @file label.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/widgets/label.h"

#ifdef XTK_USE_WIDGETS

//select include file
#ifdef XTK_GUI_MSW
	#include "msw/label_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/label_gtk2.h"
#endif

namespace xtk
{

xLabel::xLabel(xWidget* parent,xString text,xLabelInternal* i)
: xWidget(parent,i)
{
	m_internal = static_cast<xLabelInternal*>(getInternal());
}


xLabel::xLabel(xWidget* parent,xString text)
: xWidget(parent,new xLabelInternal(parent,text,this))
{
	m_internal = static_cast<xLabelInternal*>(getInternal());
}

xLabel::~xLabel()
{
}

xString xLabel::getText()
{return m_internal->getText();}

void xLabel::setText(xString label)
{m_internal->setText(label);}


}//namespace

#endif //XTK_USE_WIDGETS