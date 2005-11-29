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
* @file widget.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/widgets/widget.h"
#include "../../include/xtk/widgets/container.h"

#ifdef XTK_USE_WIDGETS

//select include file
#ifdef XTK_GUI_MSW
	#include "msw/widget_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/widget_gtk2.h"
#endif

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xWidget::xWidget(xWidget* parent,xWidgetInternal* i)
{
	m_internal = i;
	if(parent != NULL)
	{
		xContainer* c = dynamic_cast<xContainer*>(parent);
		if(c != NULL)
			c->addChild(this);
	}
}
	
void xWidget::processEvent(xWidgetEvent& ev)
{m_internal->processEvent(ev);}

void xWidget::addEventHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh,xWidgetEventID evmask)
{m_internal->addEventHandler(evh,evmask);}

void xWidget::removeEventHandler(xWidgetEventHandlerBase& evh,xWidgetEventID evmask)
{m_internal->removeEventHandler(evh,evmask);}

xWidget::~xWidget()
{delete m_internal;}
	
void xWidget::doLayout()
{m_internal->doLayout();}
	
MYOWNERSHIP xWidget* xWidget::getParent()
{return m_internal->getParent();}
		
void xWidget::getPreferredSize(xDimension& dim)
{m_internal->getPreferredSize(dim);}
		
bool xWidget::isEnabled()
{return m_internal->isEnabled();}
	
bool xWidget::isFocusable()
{return m_internal->isFocusable();}
	
bool xWidget::isFocusOwner()
{return m_internal->isFocusOwner();}
	
bool xWidget::isVisible()
{return m_internal->isVisible();}
		
void xWidget::requestFocus()
{m_internal->requestFocus();}
		
void xWidget::setEnabled(bool b)
{m_internal->setEnabled(b);}

void xWidget::setPreferredSize(int width, int height)
{m_internal->setPreferredSize(width,height);}
	
void xWidget::setVisible(bool b)
{m_internal->setVisible(b);}

}//namespace

#endif //XTK_USE_WIDGETS
