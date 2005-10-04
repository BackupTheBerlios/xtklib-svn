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

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xBoxConstraintComparator : public xComparator<xLayoutManager::xComponentWithConstraint*>
{
public:
	virtual ~xBoxConstraintComparator(){}

	virtual int compare(xLayoutManager::xComponentWithConstraint* o1,xLayoutManager::xComponentWithConstraint* o2)
	{
		xBoxConstraint* c1 = static_cast<xBoxConstraint*>(o1->m_constraint);
		xBoxConstraint* c2 = static_cast<xBoxConstraint*>(o2->m_constraint);
		
		if(c1->m_place < c2->m_place)
			return -1;
		if(c1->m_place > c2->m_place)
			return 1;
			
		return 0;
	}
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBoxLayout::doLayout(xDimension& parentClientAreaSize)
{
	if(m_components.size() == 0)
		return;
		
	if(m_orientation == X_AXIS)
	{
		xArray<xComponentWithConstraint*> components = m_components.toArray().castTo<xComponentWithConstraint*>();
		
		//calculate total weight
		int totWeight = 0;
		for(int i = 0; i < components.size();i++)
			totWeight += (static_cast<xBoxConstraint*>(components[i]->m_constraint))->m_weight;
			
		//calculate the size to give to 1 weight
		int weightSize = parentClientAreaSize.getWidth() / totWeight;
		
		//sort array by place
		xBoxConstraintComparator comp;
		components.sort(comp);
		
		int assignedWeight = 0;
		//now for every component we set position and size
		for(int i = 0; i < components.size();i++)
		{
			int x,y,width,height;
			x=y=width=height = 0;
			xBoxConstraint* cnstr = static_cast<xBoxConstraint*>(components[i]->m_constraint);
			
			switch(cnstr->m_fill)
			{
			case xBoxConstraint::FILL_HORIZONTAL:
				width = cnstr->m_weight * weightSize;
				height = components[i]->m_component->getHeight();
				break;
			case xBoxConstraint::FILL_VERTICAL:
				width = components[i]->m_component->getWidth();
				height = parentClientAreaSize.getHeight();
				break;
			case xBoxConstraint::FILL_BOTH:
				width = cnstr->m_weight * weightSize;
				height = parentClientAreaSize.getHeight();
				break;
			default:
			case xBoxConstraint::FILL_NONE:
				width = components[i]->m_component->getWidth();
				height = components[i]->m_component->getHeight();
				break;
			}
				
			switch(cnstr->m_anchor)
			{
			case xBoxConstraint::ANCHOR_EAST:
				y = (int)(parentClientAreaSize.getHeight() / 2) - (int)(height / 2);
				x = (assignedWeight * weightSize) + (cnstr->m_weight * weightSize) - width;
				break;
			case xBoxConstraint::ANCHOR_NORTH:
				y = 0;
				x = (assignedWeight * weightSize) + ((cnstr->m_weight * weightSize) / 2) - (width / 2);
				break;
			case xBoxConstraint::ANCHOR_NORTHEAST:
				y = 0;
				x = (assignedWeight * weightSize) + (cnstr->m_weight * weightSize) - width;
				break;
			case xBoxConstraint::ANCHOR_NORTHWEST:
				y = 0;
				x = 0;
				break;
			case xBoxConstraint::ANCHOR_SOUTH:
				y = parentClientAreaSize.getHeight() - height;
				x = (assignedWeight * weightSize) + ((cnstr->m_weight * weightSize) / 2) - (width / 2);
				break;
			case xBoxConstraint::ANCHOR_SOUTHEAST:
				y = parentClientAreaSize.getHeight() - height;
				x = (assignedWeight * weightSize) + (cnstr->m_weight * weightSize) - width;
				break;
			case xBoxConstraint::ANCHOR_SOUTHWEST:
				y = parentClientAreaSize.getHeight() - height;
				x = 0;
				break;		
			case xBoxConstraint::ANCHOR_WEST:
				y = (int)(parentClientAreaSize.getHeight() / 2) - (int)(height / 2);
				x = 0;
				break;		
			case xBoxConstraint::ANCHOR_CENTER:
				y = (int)(parentClientAreaSize.getHeight() / 2) - (int)(height / 2);
				x = (assignedWeight * weightSize) + ((cnstr->m_weight * weightSize) / 2) - (width / 2);
				break;
			}
			
			assignedWeight += cnstr->m_weight;
			components[i]->m_component->setBounds(x,y,width,height);
		}
	}
	else //Y_AXIS
	{
		xArray<xComponentWithConstraint*> components = m_components.toArray().castTo<xComponentWithConstraint*>();

		//calculate total weight
		int totWeight = 0;
		for(int i = 0; i < components.size();i++)
			totWeight += static_cast<xBoxConstraint*>(components[i]->m_constraint)->m_weight;

		//calculate the size to give to 1 weight
		int weightSize = parentClientAreaSize.getHeight() / totWeight;

		//sort array by place
		xBoxConstraintComparator comp;
		components.sort(comp);

		int assignedWeight = 0;
		//now for every component we set position and size
		for(int i = 0; i < components.size();i++)
		{
			int x,y,width,height;
			x=y=width=height = 0;
			xBoxConstraint* cnstr = static_cast<xBoxConstraint*>(components[i]->m_constraint);

			switch(cnstr->m_fill)
			{
			case xBoxConstraint::FILL_HORIZONTAL:
				width = parentClientAreaSize.getWidth();
				height = components[i]->m_component->getHeight();
				break;
			case xBoxConstraint::FILL_VERTICAL:
				width = components[i]->m_component->getWidth();
				height = cnstr->m_weight * weightSize;
				break;
			case xBoxConstraint::FILL_BOTH:
				width = parentClientAreaSize.getWidth();
				height = cnstr->m_weight * weightSize;
				break;
			default:
			case xBoxConstraint::FILL_NONE:
				width = components[i]->m_component->getWidth();
				height = components[i]->m_component->getHeight();
				break;
			}

			switch(cnstr->m_anchor)
			{
			case xBoxConstraint::ANCHOR_EAST:
				y = (assignedWeight * weightSize) + ((cnstr->m_weight * weightSize) / 2) - (height / 2);
				x = parentClientAreaSize.getWidth() - width;
				break;
			case xBoxConstraint::ANCHOR_NORTH:
				y = 0;
				x = (int)(parentClientAreaSize.getWidth() / 2) - (int)(width / 2);
				break;
			case xBoxConstraint::ANCHOR_NORTHEAST:
				y = 0;
				x = parentClientAreaSize.getWidth() - width;
				break;
			case xBoxConstraint::ANCHOR_NORTHWEST:
				y = 0;
				x = 0;
				break;
			case xBoxConstraint::ANCHOR_SOUTH:
				y = (assignedWeight * weightSize) + (cnstr->m_weight * weightSize) - height;
				x = (int)(parentClientAreaSize.getWidth() / 2) - (int)(width / 2);
				break;
			case xBoxConstraint::ANCHOR_SOUTHEAST:
				y = (assignedWeight * weightSize) + (cnstr->m_weight * weightSize) - height;
				x = parentClientAreaSize.getWidth() - width;
				break;
			case xBoxConstraint::ANCHOR_SOUTHWEST:
				y = (assignedWeight * weightSize) + (cnstr->m_weight * weightSize) - height;
				x = 0;
				break;		
			case xBoxConstraint::ANCHOR_WEST:
				y = (assignedWeight * weightSize) + ((cnstr->m_weight * weightSize) / 2) - (height / 2);
				x = 0;
				break;		
			case xBoxConstraint::ANCHOR_CENTER:
				y = (assignedWeight * weightSize) + ((cnstr->m_weight * weightSize) / 2) - (height / 2);
				x = (int)(parentClientAreaSize.getWidth() / 2) - (int)(width / 2);
				break;
			}
			
			assignedWeight += cnstr->m_weight * weightSize;
			components[i]->m_component->setBounds(x,y,width,height);
		}
	}
	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################


}//namespace

#endif //XTK_USE_WIDGETS