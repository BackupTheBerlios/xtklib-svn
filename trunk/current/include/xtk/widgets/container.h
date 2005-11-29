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
* @file container.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_CONTAINER_H
#define XTK_CONTAINER_H

#include "../globals.h"
#include "widget.h"
#include "layout.h"

#ifdef XTK_USE_WIDGETS

namespace xtk
{

class xContainerInternal;

/**
 * A component that can contain other components.
 * 
 * @ramarks xContainer class and subclasses does not sends "mouseClicked"
 * (other mouse events are handled normally).
 */
class XTKAPI xContainer : public xWidget
{
friend class xWidget;
	
private:
	xContainerInternal* m_internal;
	
protected:
	xContainer(xWidget* parent,xLayoutManager* layout,xContainerInternal* i);
	
	/**
	 * Removes the specified component from this container.
	 */
	virtual void removeChild(xWidget& comp);

	/**
	 * Removes the specified component from this container.
	 */
	virtual void addChild(xWidget* comp);

public:
	virtual ~xContainer();
	
	/**
	 * Gets the number of components in this panel.
	 */
	virtual int getComponentCount();
		
	/**
	 * Gets all the components in this container.
	 */
	virtual xArray<MYOWNERSHIP xWidget*> getComponents();
	
	/**
	 * Determines the insets of this container, which indicate the size of the 
	 * container's border.
	 */
	//virtual xInsets* getInsets();
	
	/**
	 * Gets the layout manager for this container.
	 */	
	virtual xLayoutManager& getLayout();

	/**
	 * Checks if the component is contained in the component hierarchy of this container.
	 */
	virtual bool isAncestorOf(xWidget& c);
		
	/**
	 * Sets the layout manager for this container.
	 */
	virtual void setLayout(MYOWNERSHIP xLayoutManager* mgr);
};


}//namespace


#endif //XTK_USE_WIDGETS

#endif //XTK_CONTAINER_H
