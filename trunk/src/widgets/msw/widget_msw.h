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
* @file widget_msw.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_WIDGET_MSW_H
#define XTK_WIDGET_MSW_H

#include "../../../include/xtk/widgets/widget.h"
#include "../widgeteventgenerator_internal.h"
#include "font_msw.h"
#include <assert.h>

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <Windows.h>

namespace xtk
{

/**
 *
 */
class XTKAPI xWidgetInternal : public virtual xObject,public xWidgetEventGeneratorInternal
{
private:
	xWidget*		m_parent;
	HWND			m_hWidget;
	xDimension		m_preferredSize;

protected:

	//-------INIT-------------------------------------------------------------//
	//-------MSW SPECIFIC-----------------------------------------------------//
	//------------------------------------------------------------------------//
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
	virtual LRESULT onCommand(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onSetFocus(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onKillFocus(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onSize(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LRESULT onDefault(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	
	void setHWND(HWND hWnd)
	{m_hWidget = hWnd;}
	
	//------------------------------------------------------------------------//
	//-------MSW SPECIFIC-----------------------------------------------------//
	//-------END--------------------------------------------------------------//
	
	xWidget* m_external;
	
	xWidgetInternal(xWidget* parent,xWidget* external);
public:
	virtual ~xWidgetInternal();
	
	xWidget* getExternal()
	{return m_external;}
	
	virtual void doLayout()
	{}

	virtual NODELETE xWidget* getParent()
	{return m_parent;}
	
	virtual void getPreferredSize(OUT xDimension& dim)
	{dim.set(m_preferredSize.getWidth(),m_preferredSize.getHeight());}
	
	virtual bool isEnabled()
	{return ::IsWindowEnabled(getHWND()) == TRUE;}
	
	virtual bool isFocusable()
	{return false;}
	
	virtual bool isFocusOwner()
	{
		HWND hWnd = ::GetFocus();
		assert(hWnd != NULL);
		return hWnd == getHWND();
	}
	
	virtual bool isVisible()
	{return ::IsWindowVisible(getHWND()) == TRUE;}
	
	virtual void requestFocus()
	{
		HWND hWnd = ::SetFocus(getHWND());
		assert(hWnd != NULL);
	}
	
	virtual void setEnabled(bool b)
	{::EnableWindow(getHWND(),b);}
	
	virtual void setPreferredSize(int width, int height)
	{m_preferredSize.set(width,height);}
	
	virtual void setVisible(boolean b)
	{b ? ::ShowWindow(getHWND(),SW_SHOW) : ::ShowWindow(getHWND(),SW_HIDE);}
	
	//-------INIT-------------------------------------------------------------//
	//-------MSW SPECIFIC-----------------------------------------------------//
	//------------------------------------------------------------------------//
	virtual void setBounds(xRectangle& r)
	{setBounds(r.getX(),r.getY(),r.getWidth(),r.getHeight());}

	HWND getHWND()
	{return m_hWidget;}
	
	virtual void getBounds(OUT xRectangle& bounds);
	
	virtual xFont* getFont();
	
	virtual xFontMetrics* getFontMetrics()
	{return new xFontMetrics(new xFontMetricsInternal(getHWND()));}
	
	virtual int getHeight();
	
	virtual xPoint* getLocation();
	
	virtual xPoint* getLocationOnScreen();
	
	virtual void getSize(OUT xDimension& dim);
	
	virtual int getWidth();
	
	virtual int getX();
	
	virtual int getY();
	
	virtual void invalidate()
	{::InvalidateRect(getHWND(),NULL,TRUE);}
	
	virtual void setBounds(int x, int y, int width, int height)
	{::SetWindowPos(getHWND(),NULL,x,y,width,height,SWP_NOZORDER | SWP_NOACTIVATE);}
	
	virtual void setFont(xFont& f)
	{::SendMessage((HWND) getHWND(),(UINT) WM_SETFONT,(WPARAM) f.getInternal()->getHFONT(),(LPARAM) TRUE);}
	
	virtual void setLocation(int x, int y)
	{::SetWindowPos(getHWND(),NULL,x,y,0,0,SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);}
	
	virtual void setSize(int width, int height)
	{::SetWindowPos(getHWND(),NULL,0,0,width,height,SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);}
	
	/**
	 * Compute the optimum size of the widget.
	 */
	virtual void sizeRequest(xDimension& dim) = 0;
	
	virtual void validate()
	{::ValidateRect(getHWND(),NULL);}
	
	virtual LRESULT windowProcedure(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	//------------------------------------------------------------------------//
	//-------MSW SPECIFIC-----------------------------------------------------//
	//-------END--------------------------------------------------------------//
};




}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_WIDGET_MSW_H
