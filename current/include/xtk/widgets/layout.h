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
* @file layout.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_LAYOUT_H
#define XTK_LAYOUT_H

#include "../globals.h"
#include "widget.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

/**
 * Tag class for all layout constraints class
 */
class XTKAPI xConstraint : public virtual xObject
{
public:
	virtual ~xConstraint(){}
};



/**
* Defines the interface for classes that know how to lay out xContainers.
*/
class XTKAPI xLayoutManager : public virtual xObject
{
friend class xBoxConstraintComparator;
protected:

	class xComponentWithConstraint : public virtual xObject
	{
	public:
		xWidget*		m_component;
		xConstraint*	m_constraint;
		
		xComponentWithConstraint(xWidget* component,xConstraint* constraint)
		{
			m_component = component;
			m_constraint = constraint;
		}
		
		virtual ~xComponentWithConstraint()
		{
			if(m_constraint != NULL)
				delete m_constraint;
		}
		
		virtual bool equals(xComponentWithConstraint& cc)
		{
			return m_component->equals(*(cc.m_component));
		}
		
		virtual bool equals(xObject& o)
		{
			xComponentWithConstraint* cc = dynamic_cast<xComponentWithConstraint*>(&o);
			if(cc == NULL)
				return false;
				
			return equals(*cc);
		}
	};

	xArrayList		m_components;
	
	void doSetConstraints(xWidget& c,xConstraint* co)
	{
		xComponentWithConstraint cc(&c,NULL);
		xObject& o = m_components.getByObject(cc);
		if(o.isNull())
			return;
		xComponentWithConstraint* cc2 = dynamic_cast<xComponentWithConstraint*>(&o);
		if(cc2->m_constraint != NULL)
			delete cc2->m_constraint;
		cc2->m_constraint = co;
	}
	
	xLayoutManager()
	{m_components.giveOwnership();}
public:
	virtual ~xLayoutManager(){}
	
	/**
	 * Sets the constraints for the specified component in this layout to the default
	 * constraint of this layout.
	 */
	virtual void setConstraints(xWidget& c) = 0;
	
	/**
	* Sets the constraints for the specified component in this layout.
	*/
	virtual void setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr) = 0;
	
	/**
	 * Execute the layout of the components
	 */
	virtual void doLayout(xDimension& parentClientAreaSize) = 0;
	
	void addComponents(xArray<YOUROWNERSHIP xWidget*> components)
	{
		for(int i = 0;i < components.size();i++)
			addComponent(components[i]);
	}
	void addComponent(YOUROWNERSHIP xWidget* c)
	{
		m_components.add(new xComponentWithConstraint(c,NULL));
		setConstraints(*c);
	}
	
	void removeComponent(xWidget& c)
	{
		xComponentWithConstraint cc(&c,NULL);
		m_components.removeObject(cc);
	}
};




/**
 * The constraint for xBoxLayout
 */
class XTKAPI xBoxConstraint : public xConstraint
{
friend class xBoxLayout;
friend class xBoxConstraintComparator;
public:
	enum Fill
	{
		FILL_VERTICAL,
		FILL_HORIZONTAL,
		FILL_BOTH,
		FILL_NONE
	};
	
	enum Anchor
	{
		ANCHOR_NORTH,
		ANCHOR_SOUTH,
		ANCHOR_EAST,
		ANCHOR_WEST,
		ANCHOR_CENTER,
		ANCHOR_NORTHWEST,
		ANCHOR_NORTHEAST,
		ANCHOR_SOUTHWEST,
		ANCHOR_SOUTHEAST,
	};
private:
	int		m_place;
	int		m_weight;
	Fill	m_fill;
	Anchor	m_anchor;
	
public:
	xBoxConstraint(int place = 1,int weight = 1,Fill fill = FILL_NONE,Anchor anchor = ANCHOR_CENTER)
	{
		m_place = place;
		m_fill = fill;
		m_anchor = anchor;
		weight >= 1 ? m_weight = weight : m_weight = 1;
	}
	
	virtual ~xBoxConstraint(){}
};




/**
 * A simple Layout manager for ordering components along
 * a vertical or horizontal axis
 */
class XTKAPI xBoxLayout : public xLayoutManager
{
public:
	enum BoxOrientation
	{
		Y_AXIS,
		X_AXIS
	};
	
private:
	BoxOrientation	m_orientation;

public:
	xBoxLayout(xBoxLayout::BoxOrientation orientation){m_orientation = orientation;}
	virtual ~xBoxLayout(){}
	
	virtual void setConstraints(xWidget& c)
	{doSetConstraints(c,new xBoxConstraint());}
	
	virtual void setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr)
	{
		xBoxConstraint* boxc = dynamic_cast<xBoxConstraint*>(cnstr);
		if(boxc == NULL)
			throw xClassCastException();
			
		doSetConstraints(c,cnstr);
	}
	
	virtual void doLayout(xDimension& parentClientAreaSize);
};
	
}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_LAYOUT_H
