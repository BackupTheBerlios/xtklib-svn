/*
* This file is part of XTKLib project.
*
* Copyright(C) 2003-2006 Mario Casciaro xshadow[AT]email(DOT)it
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
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_LAYOUT_H
#define XTK_LAYOUT_H

#include "../globals.h"
#include "widget.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

class xConstraintInternal;
class xLayoutManagerInternal;
class xBoxConstraintInternal;
class xBoxLayoutInternal;
	
/**
 * Tag class for all layout constraints class
 */
class XTKAPI xConstraint : public virtual xObject
{
protected:
	xConstraint()
	{}

public:
	virtual ~xConstraint()
	{}
};



/**
 * Defines the interface for classes that know how to lay out xContainers.
 */
class XTKAPI xLayoutManager : public virtual xObject
{
friend class xContainerInternal;
private:
	xLayoutManagerInternal* m_internal;

protected:
	xLayoutManager(xLayoutManagerInternal* i)
	{m_internal = i;}
	
	/**
	 *
	 */
	virtual void addComponent(YOUROWNERSHIP xWidget* c,MYOWNERSHIP xConstraint* cnstr = NULL);
	
	/**
	 *
	 */
	virtual void addComponents(xArray<YOUROWNERSHIP xWidget*> components);
	
	/**
	 *
	 */
	virtual void removeComponent(xWidget& c);
public:
	virtual ~xLayoutManager();
	
	/**
	* Sets the constraints for the specified component in this layout.
	*/
	virtual void setConstraints(xWidget& c,MYOWNERSHIP xConstraint* cnstr = NULL);
	virtual void setPlace(xWidget& c,int place);
	
	xLayoutManagerInternal* getInternal()
	{return m_internal;}
	
	/**
	 *
	 */
	virtual xConstraint* defaultConstraintFactory() = 0;
};




/**
 * The constraint for xBoxLayout
 */
class XTKAPI xBoxConstraint : public xConstraint
{
private:
	bool 	m_expand;
	bool	m_fill;
	
public:
	
	/**
	 * @param expand TRUE if the child is to be given extra space allocated 
     * to box.
	 * @param fill TRUE if space given to child by the expand option is actually
	 * allocated to child
	 */
	xBoxConstraint(bool expand,bool fill)
	{m_expand = expand;m_fill = fill;}
	
	virtual ~xBoxConstraint()
	{}
	
	bool getExpand()
	{return m_expand;}

	bool getFill()
	{return m_fill;}
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
		BOX_ORIENTATION_Y_AXIS,
		BOX_ORIENTATION_X_AXIS
	};

	xBoxLayout(xBoxLayout::BoxOrientation orientation,bool homogeneous,int padding);

	virtual ~xBoxLayout()
	{}
		
	virtual xConstraint* defaultConstraintFactory();
		
	xBoxLayoutInternal* getInternal();
};


	
}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_LAYOUT_H
