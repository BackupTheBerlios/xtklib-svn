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
* @file container_msw.cpp
* @author Mario Casciaro (xshadow@email.it)
*/


#include "../../../include/xtk/widgets/container.h"
#include "container_msw.h"
#include "layout_msw.h"
#include "../../../include/xtk/base/smartptr.h"

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

namespace xtk
{
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xContainerInternal::xContainerInternal(xWidget* parent, xLayoutManager* layout,xContainer* external) 
	: xWidgetInternal(parent,external)
{
	m_layout = layout;
	m_components.giveOwnership();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xContainerInternal::~xContainerInternal()
{
	delete m_layout;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xContainerInternal::setLayout(MYOWNERSHIP xLayoutManager* mgr)
{
	assert(mgr != NULL);
	mgr->addComponents(getComponents());
	delete m_layout;
	m_layout = mgr;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xContainerInternal::doLayout()
{
	//first our layout
	xDimension dim;
	getSize(dim);
	getLayout().getInternal()->setClientSize(dim);
	getLayout().getInternal()->doLayout();
	
	//then childs
	//automatically called for every child when resized
	
	/*
	smartPtr<xIterator> iter = m_components.iterator();
	while(iter->hasNext())
	{
		xContainer* c = dynamic_cast<xContainer*>(&(iter->next()));
		if(c != NULL)
			c->doLayout();
	}
	*/
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xContainerInternal::removeChild(xWidget& comp)
{
	m_components.removeObject(comp);
	m_layout->removeComponent(comp);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xContainerInternal::addChild(xWidget* comp)
{
	m_components.add(comp);
	m_layout->addComponent(comp);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArray<NODELETE xWidget*> xContainerInternal::getComponents()
{
	return m_components.toArray().castTo<xWidget*>();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xContainerInternal::isAnchestorOf(xWidget& c)
{
	//verify this level
	if(m_components.contains(c))
		return true;
		
	//now for every child container
	smartPtr<xIterator> iter = m_components.iterator();
	while(iter->hasNext())
	{
		xContainer* cn = dynamic_cast<xContainer*>(&(iter->next()));
		if(cn != NULL)
			if(cn->isAncestorOf(c))
				return true;
	}
	return false;	
}


}//namspace

#endif//XTK_USE_WIDGETS
