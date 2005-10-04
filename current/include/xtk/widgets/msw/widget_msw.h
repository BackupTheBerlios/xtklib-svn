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

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <Windows.h>

namespace xtk
{

/**
 * A widget is the base class for all Gui components.
 * Note that all children of the window will be deleted automatically by 
 * the destructor before the window itself is deleted.
*/
class XTKAPI xWidget : public xObject,public xIWidget
{
private:
	static xObject	s_guiMutex;

	xLinkedList		m_componentListeners;
	xLinkedList		m_focusListeners;
	xLinkedList		m_keyListeners;
	xLinkedList		m_mouseListeners;
	xLinkedList		m_mouseMotionListeners;
	xContainer*		m_parent;
	
	//In MSW all principal widgets are windows
	HWND			m_hWidget;

protected:
	/**
	 * 
	 */
	static xObject& getSyncObj(){return s_guiMutex;}
	
	/**
	 * 
	 */
	void setHWND(HWND hWnd){m_hWidget = hWnd;}
	
	xWidget(xContainer* parent);
public:
	virtual ~xWidget();
	

	virtual void addComponentListener(YOUROWNERSHIP xComponentListener* l)
	{m_componentListeners.add(l);}

	virtual void addFocusListener(YOUROWNERSHIP xFocusListener* l)
	{m_focusListeners.add(l);}

	virtual void addKeyListener(YOUROWNERSHIP xKeyListener* l)
	{m_keyListeners.add(l);}

	virtual void addMouseListener(YOUROWNERSHIP xMouseListener* l)
	{m_mouseListeners.add(l);}

	virtual void addMouseMotionListener(YOUROWNERSHIP xMouseMotionListener* l)
	{m_mouseMotionListeners.add(l);}
	
	virtual bool contains(int x, int y)
	{	
		return 
			(x >= getX() && x <= getX()+getWidth()) &&
			(y >= getY() && y <= getY()+getHeight());
	}
	
	virtual void doLayout(){}
	
	virtual xArray<NODELETE xComponentListener*> getComponentListeners()
	{return m_componentListeners.toArray().castTo<xComponentListener*>();}
	
	virtual xArray<NODELETE xFocusListener*> getFocusListeners()
	{return m_focusListeners.toArray().castTo<xFocusListener*>();}
	
	virtual xArray<NODELETE xKeyListener*> getKeyListeners()
	{return m_keyListeners.toArray().castTo<xKeyListener*>();}
	
	virtual xArray<NODELETE xMouseListener*> getMouseListeners()
	{return m_mouseListeners.toArray().castTo<xMouseListener*>();}
	
	virtual xArray<NODELETE xMouseMotionListener*> getMouseMotionListeners()
	{return m_mouseMotionListeners.toArray().castTo<xMouseMotionListener*>();}

	virtual NODELETE xContainer* getParent(){return m_parent;}
	
	virtual void removeComponentListener(xComponentListener& l)
	{m_componentListeners.removeObject(l);}

	virtual void removeFocusListener(xFocusListener& l)
	{m_focusListeners.removeObject(l);}

	virtual void removeKeyListener(xKeyListener& l)
	{m_keyListeners.removeObject(l);}

	virtual void removeMouseListener(xMouseListener& l)
	{m_mouseListeners.removeObject(l);}

	virtual void removeMouseMotionListener(xMouseMotionListener& l)
	{m_mouseMotionListeners.removeObject(l);}
	
	virtual void setBounds(xRectangle& r)
	{
		setBounds(r.getX(),r.getY(),r.getWidth(),r.getHeight());
	}
	
	HWND getHWND(){return m_hWidget;}
	//! @todo to implement (MSW)
	virtual xColor* getBackground();
	virtual xRectangle* getBounds();
	//! @todo to implement (MSW)
	virtual NODELETE xWidget* getComponentAt(int x, int y);
	//! @todo to implement (MSW)
	virtual xFont* getFont();
	//! @todo to implement (MSW)
	virtual xColor* getForeground();
	virtual int getHeight();
	//! @todo to implement (MSW)
	virtual bool getIgnoreRepaint();
	virtual xPoint* getLocation();
	virtual xPoint* getLocationOnScreen();
	virtual xDimension* getSize();
	virtual int getWidth();
	virtual int getX();
	virtual int getY();
	virtual void invalidate();
	virtual bool isEnabled();
	virtual bool isFocusOwner();
	virtual bool isShowing();
	//! @todo to implement (MSW)
	virtual bool isValid();
	//! @todo to implement (MSW)
	virtual bool isVisible();
	virtual void requestFocus();
	//! @todo to implement (MSW)
	virtual void setBackground(xColor& c);
	virtual void setBounds(int x, int y, int width, int height);
	virtual void setEnabled(bool b);
	//! @todo to implement (MSW)
	virtual void setFont(xFont& f);
	//! @todo to implement (MSW)
	virtual void setForeground(xColor& c);
	virtual void setLocation(int x, int y);
	virtual void setSize(int width, int height);
	virtual void setVisible(boolean b);
	virtual void validate();
	
protected:
	virtual void processComponentEvent(xComponentEvent& e);
	virtual void processFocusEvent(xFocusEvent& e);
	virtual void processKeyEvent(xKeyEvent& e);
	virtual void processMouseEvent(xMouseEvent& e);
	virtual void processMouseMotionEvent(xMouseEvent& e);
};




}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_WIDGET_MSW_H
