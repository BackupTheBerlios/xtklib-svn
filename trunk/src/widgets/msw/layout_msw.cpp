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
* @file layout_msw.cpp
* @author Mario Casciaro (xshadow@email.it)
*/


#include "layout_msw.h"

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLayoutManagerInternal::addComponent(YOUROWNERSHIP xWidget* c,MYOWNERSHIP xConstraint* cnstr)
{
	m_components.add(new xComponentWithConstraint(c,cnstr));
	doLayout();
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLayoutManagerInternal::setPlace(xWidget& c,int place)
{
	xComponentWithConstraint cc(&c,NULL);
	xComponentWithConstraint* cwc = static_cast<xComponentWithConstraint*>(&(m_components.getByObject(cc)));
	xConstraint* con = cwc->m_constraint;
	//if not it will be deleted
	cwc->m_constraint = NULL;
	m_components.removeObject(cc);
	m_components.insert(place,new xComponentWithConstraint(&c,con));
	doLayout();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLayoutManagerInternal::setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr)
{
	xComponentWithConstraint cc(&c,NULL);
	xObject& o = m_components.getByObject(cc);
	if(o.isNull())
		return;
	xComponentWithConstraint* cc2 = dynamic_cast<xComponentWithConstraint*>(&o);
	if(cc2->m_constraint != NULL)
		delete cc2->m_constraint;
	cc2->m_constraint = cnstr;
	doLayout();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xBoxLayoutInternal::xBoxLayoutInternal(xBoxLayout::BoxOrientation orientation,bool homogeneous,int padding,xBoxLayout* external)
: xLayoutManagerInternal(external)
{
	m_homogeneous = homogeneous;
	m_orientation = orientation;
	m_padding = padding;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBoxLayoutInternal::setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr)
{
	assert(dynamic_cast<xBoxConstraint*>(cnstr) != NULL);
	xLayoutManagerInternal::setConstraints(c,cnstr);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBoxLayoutInternal::addComponent(YOUROWNERSHIP xWidget* c,MYOWNERSHIP xConstraint* cnstr)
{
	assert(dynamic_cast<xBoxConstraint*>(cnstr) != NULL);
	xLayoutManagerInternal::addComponent(c,cnstr);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBoxLayoutInternal::doLayout()
{
	int nVisibleChildren = 0;
	int nExpandedChildren = 0;
	
	//calculate the number of visible and expanded widgets
	xIterator* iter = m_components.iterator();
	while(iter->hasNext())
	{
		xComponentWithConstraint* cmpWithCnstr = static_cast<xComponentWithConstraint*>(&(iter->next()));
		if(cmpWithCnstr->m_component->isVisible())
		{
			nVisibleChildren++;
			if(static_cast<xBoxConstraint*>(cmpWithCnstr->m_constraint)->getExpand())
				nExpandedChildren++;
		}
	}
	delete iter;
	
	//case xBoxLayout::BOX_ORIENTATION_X_AXIS
	if(m_orientation == xBoxLayout::BOX_ORIENTATION_X_AXIS)
	{
		int childWidth = 0;
		int width = 0;
		int extra = 0;
		int x = 0;
		
		if(nVisibleChildren > 0)
		{
			if(m_homogeneous)
			{
				width = m_clientSize.getWidth();
				extra = (int)(width / nVisibleChildren);
			}
			else if(nExpandedChildren > 0)
			{
				width = m_clientSize.getWidth();
				extra = (int)(width / nExpandedChildren);
			}
			else
			{
				width = 0;
				extra = 0;
			}
			
			int child_y = 0;
			int child_x = 0;
			int child_width = 0;
			int child_height = MAX(1, m_clientSize.getHeight());
			
			iter = m_components.iterator();
			while(iter->hasNext())
			{
				xComponentWithConstraint* cmpWithCnstr = static_cast<xComponentWithConstraint*>(&(iter->next()));
				xBoxConstraint* boxcnstr = static_cast<xBoxConstraint*>(cmpWithCnstr->m_constraint);
				
				if(cmpWithCnstr->m_component->isVisible())
				{
					if(m_homogeneous)
					{
						if(nVisibleChildren == 1)
							childWidth = width;
						else
							childWidth = extra;

						nVisibleChildren -= 1;
						width -= extra;
					}
					else
					{
						xDimension dim;
						//reset the size
						cmpWithCnstr->m_component->getInternal()->negotiateSize();
						cmpWithCnstr->m_component->getInternal()->getSize(dim);
						childWidth = dim.getWidth() + getPadding() * 2;

						if(boxcnstr->getExpand())
						{
							if(nExpandedChildren == 1)
								childWidth = width;
							else
								childWidth = extra;

							nExpandedChildren -= 1;
							width -= extra;
						}
						else
							width -= childWidth;
					}

					if(boxcnstr->getFill())
					{
						child_width = MAX(1, childWidth - getPadding() * 2);
						child_x = x + getPadding();
					}
					else
					{
						xDimension dim;
						//reset the size
						cmpWithCnstr->m_component->getInternal()->negotiateSize();
						cmpWithCnstr->m_component->getInternal()->getSize(dim);

						child_width = dim.getWidth();
						child_x = x + (childWidth - dim.getWidth()) / 2;
					}

					cmpWithCnstr->m_component->getInternal()->setBounds(child_x,child_y,child_width,child_height);

					x += childWidth;
				}
			}
			delete iter;
		}
	}
	else //BOX_ORIENTATION_Y_AXIS
	{
		int childHeight = 0;
		int height = 0;
		int extra = 0;
		int y = 0;

		if(nVisibleChildren > 0)
		{
			if(m_homogeneous)
			{
				height = m_clientSize.getHeight();
				extra = (int)(height / nVisibleChildren);
			}
			else if(nExpandedChildren > 0)
			{
				height = m_clientSize.getHeight();
				extra = (int)(height / nExpandedChildren);
			}
			else
			{
				height = 0;
				extra = 0;
			}

			int child_y = 0;
			int child_x = 0;
			int child_width = MAX(1, m_clientSize.getWidth());
			int child_height = 0;

			iter = m_components.iterator();
			while(iter->hasNext())
			{
				xComponentWithConstraint* cmpWithCnstr = static_cast<xComponentWithConstraint*>(&(iter->next()));
				xBoxConstraint* boxcnstr = static_cast<xBoxConstraint*>(cmpWithCnstr->m_constraint);

				if(cmpWithCnstr->m_component->isVisible())
				{
					if(m_homogeneous)
					{
						if(nVisibleChildren == 1)
							childHeight = height;
						else
							childHeight = extra;

						nVisibleChildren -= 1;
						height -= extra;
					}
					else
					{
						xDimension dim;
						//reset the size
						cmpWithCnstr->m_component->getInternal()->negotiateSize();
						cmpWithCnstr->m_component->getInternal()->getSize(dim);
						childHeight = dim.getHeight() + getPadding() * 2;

						if(boxcnstr->getExpand())
						{
							if(nExpandedChildren == 1)
								childHeight = height;
							else
								childHeight = extra;

							nExpandedChildren -= 1;
							height -= extra;
						}
						else
							height -= childHeight;
					}

					if(boxcnstr->getFill())
					{
						child_height = MAX(1, childHeight - getPadding() * 2);
						child_y = y + getPadding();
					}
					else
					{
						xDimension dim;
						//reset the size
						cmpWithCnstr->m_component->getInternal()->negotiateSize();
						cmpWithCnstr->m_component->getInternal()->getSize(dim);

						child_height = dim.getHeight();
						child_y = y + (childHeight - dim.getHeight()) / 2;
					}

					cmpWithCnstr->m_component->getInternal()->setBounds(child_x,child_y,child_width,child_height);

					y += childHeight;
				}
			}
			delete iter;
		}
	}
}




}//namspace

#endif//XTK_USE_WIDGETS
