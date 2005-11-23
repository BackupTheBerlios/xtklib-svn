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
* @file layout_msw.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_LAYOUT_MSW_H
#define XTK_LAYOUT_MSW_H


#include "../../../include/xtk/widgets/layout.h"
#include "widget_msw.h"

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)
#include <Windows.h>

namespace xtk
{

/**
* Defines the interface for classes that know how to lay out xContainers.
*/
class XTKAPI xLayoutManagerInternal : public virtual xObject
{
protected:
	class xComponentWithConstraint : public xObject
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
	xLayoutManager* m_external;
	int				m_nextPlace;
	xDimension		m_clientSize;
	
	xLayoutManagerInternal(xLayoutManager* external)
	{
		m_external = external;
		m_components.giveOwnership();
		m_clientSize.set(-1,-1);
		m_nextPlace = 0;
	}

public:
	virtual ~xLayoutManagerInternal()
	{}

	xLayoutManager* getExternal()
	{return m_external;}

	/**
	 * Sets the constraints for the specified component in this layout.
	 */
	virtual void setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr);

	/**
	 *
	 */
	virtual void addComponents(xArray<YOUROWNERSHIP xWidget*> components)
	{
		for(int i = 0;i < components.size();i++)
			addComponent(components[i],getExternal()->defaultConstraintFactory());
	}
	
	/**
	 *
	 */
	virtual void addComponent(YOUROWNERSHIP xWidget* c,MYOWNERSHIP xConstraint* cnstr);
	
	/**
	 *
	 */
	virtual void removeComponent(xWidget& c)
	{
		xComponentWithConstraint cc(&c,NULL);
		m_components.removeObject(cc);
		doLayout();
	}
	
	virtual void setPlace(xWidget& c,int place);
	
	//-------INIT-------------------------------------------------------------//
	//-------MSW SPECIFIC-----------------------------------------------------//
	//------------------------------------------------------------------------//
	virtual void setClientSize(xDimension& clientSize)
	{m_clientSize.set(clientSize.getWidth(),clientSize.getHeight());}
	
	virtual void doLayout() = 0;
	//------------------------------------------------------------------------//
	//-------MSW SPECIFIC-----------------------------------------------------//
	//-------END--------------------------------------------------------------//
};


/**
* A simple Layout manager for ordering components along
* a vertical or horizontal axis
*/
class XTKAPI xBoxLayoutInternal : public xLayoutManagerInternal
{
private:
	bool						m_homogeneous;
	xBoxLayout::BoxOrientation	m_orientation;
	
public:
	xBoxLayoutInternal(xBoxLayout::BoxOrientation orientation,bool homogeneous,xBoxLayout* external);

	virtual ~xBoxLayoutInternal()
	{}

	virtual void setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr);
	virtual void addComponent(YOUROWNERSHIP xWidget* c,MYOWNERSHIP xConstraint* cnstr);
	
	//-------INIT-------------------------------------------------------------//
	//-------MSW SPECIFIC-----------------------------------------------------//
	//------------------------------------------------------------------------//
	virtual void doLayout();
	//------------------------------------------------------------------------//
	//-------MSW SPECIFIC-----------------------------------------------------//
	//-------END--------------------------------------------------------------//
};


}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_LAYOUT_MSW_H