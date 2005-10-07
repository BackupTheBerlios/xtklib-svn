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
* 
* @ramarks xContainer class and subclasses does not sends "mouseClicked"
* (other mouse events are handled normally).
*/
class XTKAPI xContainer : public xIContainer,public xWidget
{
protected:
	xHashMap			m_components;
	xLayoutManager*		m_layout;
	
	int getModifiers(WPARAM wParam);
	virtual LRESULT onLButtonPressed(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onMButtonPressed(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onRButtonPressed(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onLButtonReleased(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onMButtonReleased(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onRButtonReleased(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onLButtonDBLClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onMButtonDBLClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onRButtonDBLClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onDestroy(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onNCDestroy(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT forwardToChild(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	/*
	virtual LRESULT onLButtonClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onMButtonClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onRButtonClicked(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	*/

	MYOWNERSHIP xWidget* getChildByHandle(HWND hWnd);
	virtual void removeChild(xWidget& comp);
	virtual void addChild(xWidget* comp);
	
	xContainer(xContainer* parent, xLayoutManager* layout = new xBoxLayout(xBoxLayout::X_AXIS));
public:
	virtual ~xContainer();
	
	virtual xLayoutManager& getLayout()
	{return *m_layout;}
	
	virtual void setLayout(MYOWNERSHIP xLayoutManager* mgr)
	{
		assert(mgr != NULL);
		mgr->addComponents(getComponents());
		delete m_layout;
		m_layout = mgr;
	}
	
	virtual void doLayout();
	virtual int getComponentCount(){return m_components.size();}
	virtual xArray<MYOWNERSHIP xWidget*> getComponents();
	virtual bool isAncestorOf(xWidget& c);
	virtual LRESULT windowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
};




}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_CONTAINER_MSW_H
