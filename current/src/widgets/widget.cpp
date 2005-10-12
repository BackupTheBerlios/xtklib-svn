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
{m_internal = i;}
	
void xWidget::processEvent(xWidgetEvent& ev)
{m_internal->processEvent(ev);}

void xWidget::addEventHandler(MYOWNERSHIP xWidgetEventHandlerBase* evh,xWidgetEventID evmask)
{m_internal->addEventHandler(evh,evmask);}

void xWidget::removeEventHandler(xWidgetEventHandlerBase& evh,xWidgetEventID evmask)
{m_internal->removeEventHandler(evh,evmask);}

xWidget::~xWidget()
{delete m_internal;}

bool xWidget::contains(int x, int y)
{return m_internal->contains(x,y);}
	
void xWidget::doLayout()
{m_internal->doLayout();}
	
xColor* xWidget::getBackground()
{return m_internal->getBackground();}
	
xRectangle* xWidget::getBounds()
{return m_internal->getBounds();}
	
MYOWNERSHIP xWidget* xWidget::getComponentAt(int x, int y)
{return m_internal->getComponentAt(x,y);}
	
xFont* xWidget::getFont()
{return m_internal->getFont();}
	
xFontMetrics* xWidget::getFontMetrics()
{return m_internal->getFontMetrics();}
	
xColor* xWidget::getForeground()
{return m_internal->getForeground();}
	
int xWidget::getHeight()
{return m_internal->getHeight();}
	
bool xWidget::getIgnoreRepaint()
{return m_internal->getIgnoreRepaint();}
		
xPoint* xWidget::getLocation()
{return m_internal->getLocation();}
	
xPoint* xWidget::getLocationOnScreen()
{return m_internal->getLocationOnScreen();}
	
MYOWNERSHIP xWidget* xWidget::getParent()
{return m_internal->getParent();}
		
xDimension* xWidget::getSize()
{return m_internal->getSize();}
		
int xWidget::getWidth()
{return m_internal->getWidth();}
	
int xWidget::getX()
{return m_internal->getX();}
	
int xWidget::getY()
{return m_internal->getY();}
		
void xWidget::invalidate()
{m_internal->invalidate();}
		
bool xWidget::isEnabled()
{return m_internal->isEnabled();}
	
bool xWidget::isFocusable()
{return m_internal->isFocusable();}
	
bool xWidget::isFocusOwner()
{return m_internal->isFocusOwner();}
		
bool xWidget::isShowing()
{return m_internal->isShowing();}
	
bool xWidget::isValid()
{return m_internal->isValid();}
	
bool xWidget::isVisible()
{return m_internal->isVisible();}
		
void xWidget::requestFocus()
{m_internal->requestFocus();}
		
void xWidget::setBackground(xColor& c)
{m_internal->setBackground(c);}
	
void xWidget::setBounds(int x, int y, int width, int height)
{m_internal->setBounds(x,y,width,height);}
	
void xWidget::setBounds(xRectangle& r)
{m_internal->setBounds(r);}
		
void xWidget::setEnabled(bool b)
{m_internal->setEnabled(b);}
		
void xWidget::setFont(xFont& f)
{m_internal->setFont(f);}
	
void xWidget::setForeground(xColor& c)
{m_internal->setForeground(c);}
	
void xWidget::setLocation(int x, int y)
{m_internal->setLocation(x,y);}

void xWidget::setSize(int width, int height)
{m_internal->setSize(width,height);}
	
void xWidget::setVisible(boolean b)
{m_internal->setVisible(b);}
		
void xWidget::validate()
{m_internal->validate();}


}//namespace

#endif //XTK_USE_WIDGETS