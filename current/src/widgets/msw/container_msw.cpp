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
#include "../../../include/xtk/widgets/msw/widgets_msw_private.h"
#include "../../../include/xtk/base/smartptr.h"

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xContainer::xContainer(xContainer* parent, xLayoutManager* layout) 
	: xAbstractContainer(layout),xWidget(parent)
{
	m_components.rescindOwnership();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xContainer::doLayout()
{
	//first childs
	smartPtr<xIterator> iter = m_components.iterator();
	while(iter->hasNext())
	{
		xContainer* c = dynamic_cast<xContainer*>(&(iter->next()));
		if(c != NULL)
			c->doLayout();
	}

	//then our layout
	xDimension* dim = getSize();
	getLayout().doLayout(*dim);
	delete dim;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
NODELETE xWidget* xContainer::getChildByHandle(HWND hWnd)
{
	xHwndObject h(hWnd);
	xObject& o = m_components.get(h);
	if(o.isNull())
		return NULL;
	return dynamic_cast<xWidget*>(&o);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xContainer::removeChild(xWidget& comp)
{
	xHwndObject h(comp.getHWND());
	m_components.remove(h);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xContainer::addChild(xWidget* comp)
{
	m_components.put(new xHwndObject(comp->getHWND()),comp);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArray<NODELETE xWidget*> xContainer::getComponents()
{
	return smartPtr<xCollection>(m_components.values())->toArray().castTo<xWidget*>();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xContainer::isAncestorOf(xWidget& c)
{
	//verify this level
	xHwndObject h(c.getHWND());
	if(m_components.containsKey(h))
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
