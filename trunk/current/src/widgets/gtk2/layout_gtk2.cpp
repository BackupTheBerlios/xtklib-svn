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
* @file layout_gtk2.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "layout_gtk2.h"
#include "widget_gtk2.h"

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)

namespace xtk
{

	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xBoxLayoutInternal::xBoxLayoutInternal(xBoxLayout::BoxOrientation orientation,bool homogeneous,xBoxLayout* external)
: xLayoutManagerInternal(external)
{
	if(orientation == xBoxLayout::BOX_ORIENTATION_X_AXIS)
		m_gtkWidget = gtk_hbox_new(homogeneous,0);
	else if(orientation == xBoxLayout::BOX_ORIENTATION_Y_AXIS)
		m_gtkWidget = gtk_vbox_new(homogeneous,0);
	
	gtk_widget_show(getGtkWidget());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBoxLayoutInternal::setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr)
{
	xBoxConstraint* box = dynamic_cast<xBoxConstraint*>(cnstr);
	assert(box != NULL);
	
	gtk_box_set_child_packing(GTK_BOX(getGtkWidget()),
							c.getInternal()->getGtkWidget(),
							box->getExpand(),
							box->getFill(),
							box->getPadding(),
							GTK_PACK_START
							);
	delete cnstr;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBoxLayoutInternal::addComponent(YOUROWNERSHIP xWidget* c,MYOWNERSHIP xConstraint* cnstr)
{
	xBoxConstraint* box = dynamic_cast<xBoxConstraint*>(cnstr);
	assert(box != NULL);
	gtk_box_pack_start(GTK_BOX(getGtkWidget()),
						c->getInternal()->getGtkWidget(),
						box->getExpand(),
						box->getFill(),
						box->getPadding()
						);
	delete cnstr;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBoxLayoutInternal::addComponents(xArray<YOUROWNERSHIP xWidget*> components)
{
	for(int i = 0;i < components.size();i++)
		addComponent(components[i],getExternal()->defaultConstraintFactory());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBoxLayoutInternal::removeComponent(xWidget& c)
{
	gtk_container_remove(GTK_CONTAINER(getGtkWidget()),c.getInternal()->getGtkWidget());	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBoxLayoutInternal::setPlace(xWidget& c,int place)
{
	 gtk_box_reorder_child(GTK_BOX(getGtkWidget()),c.getInternal()->getGtkWidget(),place);
}


}//namespace


#endif //XTK_USE_WIDGETS
