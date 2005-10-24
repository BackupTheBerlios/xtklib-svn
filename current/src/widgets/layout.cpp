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

xConstraint::~xConstraint()
{delete m_internal;}

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
xBoxConstraint::xBoxConstraint(int place,bool expand,bool fill,int padding)
: xConstraint(new xBoxConstraintInternal(place,expand,fill,padding))
{}

xBoxConstraintInternal* xBoxConstraint::getInternal()
{return static_cast<xBoxConstraintInternal*>(xConstraint::getInternal());}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xBoxLayoutInternal* xBoxLayout::getInternal()
{return static_cast<xBoxLayoutInternal*>(xLayoutManager::getInternal());}

xBoxLayout::xBoxLayout(xBoxLayout::BoxOrientation orientation,bool homogeneous)
: xLayoutManager(new xBoxLayoutInternal(orientation,homogeneous,this))
{}
	
void xBoxLayout::setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr)
{
	xBoxConstraint* boxc = dynamic_cast<xBoxConstraint*>(cnstr);
	assert(boxc != NULL);
	if(boxc != NULL)
		xLayoutManager::setConstraints(c,cnstr);
}

xConstraint* xBoxLayout::defaultConstraintFactory()
{return new xBoxConstraint(0,false,false,0);}


}//namespace

#endif //XTK_USE_WIDGETS
