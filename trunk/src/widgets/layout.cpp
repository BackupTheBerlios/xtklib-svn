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
* @file layout.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/widgets/layout.h"

#ifdef XTK_USE_WIDGETS

//select include file
#ifdef XTK_GUI_MSW
	#include "msw/layout_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/layout_gtk2.h"
#endif

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

void xLayoutManager::addComponents(xArray<YOUROWNERSHIP xWidget*> components)
{m_internal->addComponents(components);}

xLayoutManager::~xLayoutManager()
{delete m_internal;}
	
void xLayoutManager::setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr)
{
	if(cnstr == NULL)
		m_internal->setConstraints(c,this->defaultConstraintFactory());
	else
		m_internal->setConstraints(c,cnstr);
}

void xLayoutManager::setPlace(xWidget& c,int place)
{
	m_internal->setPlace(c,place);
}


void xLayoutManager::addComponent(YOUROWNERSHIP xWidget* c,MYOWNERSHIP xConstraint* cnstr)
{
	if(cnstr == NULL)
		m_internal->addComponent(c,this->defaultConstraintFactory());
	else
		m_internal->addComponent(c,cnstr);
}

void xLayoutManager::removeComponent(xWidget& c)
{m_internal->removeComponent(c);}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xBoxLayoutInternal* xBoxLayout::getInternal()
{return static_cast<xBoxLayoutInternal*>(xLayoutManager::getInternal());}

xBoxLayout::xBoxLayout(xBoxLayout::BoxOrientation orientation,bool homogeneous,int padding)
: xLayoutManager(new xBoxLayoutInternal(orientation,homogeneous,padding,this))
{}

xConstraint* xBoxLayout::defaultConstraintFactory()
{return new xBoxConstraint(false,false);}


}//namespace

#endif //XTK_USE_WIDGETS
