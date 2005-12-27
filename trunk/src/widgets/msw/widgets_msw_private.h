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
* @file widgets_msw_private.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_WIDGETS_MSW_PRIVATE_H
#define XTK_WIDGETS_MSW_PRIVATE_H

#include "../../../include/xtk/globals.h"
#include "../../../include/xtk/base/interfaces.h"
#include "../../../include/xtk/base/exception.h"
#include <windows.h>

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

namespace xtk
{

/*
 *
 */
#define XTK_MSW_FRAME_CLASS_NAME _T("xFrame")
#define XTK_MSW_PANEL_NOBORDER_CLASS_NAME _T("xPanelNoBorder")
#define XTK_MSW_LABEL_CLASS_NAME _T("xLabel")

/*
* A wrapper class around MSW HWND type
*/
class xHwndObject : public virtual xObject,public virtual xComparable
{
private:
	HWND	m_hWnd;
	
public:
	xHwndObject(HWND hWnd){m_hWnd = hWnd;}
	virtual ~xHwndObject(){}
	
	HWND getHWND(){return m_hWnd;}
	
	bool equals(xHwndObject& h){return m_hWnd == h.m_hWnd;}
	virtual bool equals(xObject& o)
	{
		xHwndObject* h = dynamic_cast<xHwndObject*>(&o);
		if(h == NULL)
			return false;
			
		return equals(*h);
	}
	
	virtual int hashCode(){return ((int)(xui64)m_hWnd) ^ (int)((xui64)m_hWnd >> (sizeof(HWND) - sizeof(int))); }
	
	int compareTo(xHwndObject& h)
	{
		if(m_hWnd > h.m_hWnd)
			return 1;
		if(m_hWnd < h.m_hWnd)
			return -1;
			
		return 0;
	}
	
	int compareTo(xObject& o)
	{
		xHwndObject* h = dynamic_cast<xHwndObject*>(&o);
		if(h == NULL)
			throw xClassCastException();

		return compareTo(*h);
	}
};	


/*
* An interface for all class implementing a MSW window procedure
*/
class xWindowProcedure
{
protected:
	xWindowProcedure(){}

public:	
	~xWindowProcedure(){}
	
	virtual LRESULT windowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) = 0;
};




}//namespace
#endif //XTK_USE_WIDGETS

#endif //XTK_WIDGETS_MSW_PRIVATE_H
