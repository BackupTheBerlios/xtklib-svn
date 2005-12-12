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

#include "../../include/xtk/widgets/container.h"

#ifdef XTK_USE_WIDGETS

//select include file
#ifdef XTK_GUI_MSW
	#include "msw/container_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/container_gtk2.h"
#endif

namespace xtk
{

xContainer::xContainer(xWidget* parent,xLayoutManager* layout,xContainerInternal* i) 
: xWidget(parent,i)
{m_internal = static_cast<xContainerInternal*>(getInternal());}

xContainer::~xContainer()
{}
	
void xContainer::removeChild(xWidget& comp)
{m_internal->removeChild(comp);}

void xContainer::addChild(xWidget* comp)
{m_internal->addChild(comp);}
	
int xContainer::getComponentCount()
{return m_internal->getComponentCount();}
		
xArray<MYOWNERSHIP xWidget*> xContainer::getComponents()
{return m_internal->getComponents();}
	
xLayoutManager& xContainer::getLayout()
{return m_internal->getLayout();}

bool xContainer::isAncestorOf(xWidget& c)
{return m_internal->isAnchestorOf(c);}
		
void xContainer::setLayout(MYOWNERSHIP xLayoutManager* mgr)
{return m_internal->setLayout(mgr);}

}//namespace

#endif //XTK_USE_WIDGETS
