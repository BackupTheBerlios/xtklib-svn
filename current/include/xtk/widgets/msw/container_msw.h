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
* @file container_msw.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_CONTAINER_MSW_H
#define XTK_CONTAINER_MSW_H

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <Windows.h>

namespace xtk
{


/**
* A component that can contain other components.
*/
class XTKAPI xContainer : public xAbstractContainer,public xWidget
{
protected:
	xHashMap			m_components;

	MYOWNERSHIP xWidget* getChildByHandle(HWND hWnd);
	virtual void removeChild(xWidget& comp);
	virtual void addChild(xWidget* comp);
	
	xContainer(xContainer* parent, xLayoutManager* layout = new xBoxLayout(xBoxLayout::X_AXIS));
public:
	virtual ~xContainer(){}

	virtual void doLayout();
	virtual int getComponentCount(){return m_components.size();}
	virtual xArray<MYOWNERSHIP xWidget*> getComponents();
	virtual bool isAncestorOf(xWidget& c);
};




}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_CONTAINER_MSW_H
