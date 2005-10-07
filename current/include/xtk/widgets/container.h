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
#include "widgeteventgenerator.h"
#include "layout.h"

#ifdef XTK_USE_WIDGETS

namespace xtk
{

/**
 * Defines the interface for the various implementation of class xContainer.
 */
class XTKAPI xIContainer : public xMouseEventGenerator
{
protected:

	xIContainer(xLayoutManager* layout)
	{}
public:
	virtual ~xIContainer(){}
	
	/**
	* Removes the specified component from this container.
	*/
	virtual void removeChild(xWidget& comp) = 0;

	/**
	* Removes the specified component from this container.
	*/
	virtual void addChild(xWidget* comp) = 0;
	
	/**
	 * Gets the number of components in this panel.
	 */
	virtual int getComponentCount() = 0;
		
	/**
	 * Gets all the components in this container.
	 */
	virtual xArray<NODELETE xWidget*> getComponents() = 0;
	
	/**
	 * Determines the insets of this container, which indicate the size of the 
	 * container's border.
	 */
	//virtual xInsets* getInsets();
	
	/**
	 * Gets the layout manager for this container.
	 */	
	virtual xLayoutManager& getLayout() = 0;

	/**
	 * Checks if the component is contained in the component hierarchy of this container.
	 */
	virtual bool isAncestorOf(xWidget& c) = 0;
		
	/**
	 * Sets the layout manager for this container.
	 */
	virtual void setLayout(MYOWNERSHIP xLayoutManager* mgr) = 0;
};


}//namespace


//select include file
#ifdef XTK_GUI_MSW
	#include "msw/container_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/container_gtk2.h"
#endif


#endif //XTK_USE_WIDGETS

#endif //XTK_CONTAINER_H
