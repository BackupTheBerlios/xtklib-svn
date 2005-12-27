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
* @file container_gtk2.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "container_gtk2.h"
#include "layout_gtk2.h"
#include "../../../include/xtk/base/smartptr.h"
#include <assert.h>

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xContainerInternal::xContainerInternal(xWidget* parent, xLayoutManager* layout,xContainer* external) 
: xWidgetInternal(parent,external)
{
	m_layout = layout;
	m_components.rescindOwnership();
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
void xContainerInternal::gtkWidgetCreated()
{
	xWidgetInternal::gtkWidgetCreated();
	
	//setting layout
	assert(m_layout != NULL);
	gtk_container_add(GTK_CONTAINER(getGtkWidget()), m_layout->getInternal()->getGtkWidget());
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
void xContainerInternal::setLayout(MYOWNERSHIP xLayoutManager* mgr)
{
	assert(mgr != NULL);
	if(m_layout != NULL)
	{
		gtk_container_remove(GTK_CONTAINER(getGtkWidget()), m_layout->getInternal()->getGtkWidget());
		delete m_layout;
	}
	mgr->addComponents(getComponents());
	gtk_container_add(GTK_CONTAINER(getGtkWidget()), mgr->getInternal()->getGtkWidget());
	m_layout = mgr;
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


}//namespace


#endif //XTK_USE_WIDGETS
