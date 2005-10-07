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
* @file font_msw.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_FONT_MSW_H
#define XTK_FONT_MSW_H

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <Windows.h>

namespace xtk
{


/**
* Defines the interface for the various implementation of class xFont.
*/
class XTKAPI xFont : public virtual xObject,public xIFont
{
private:
	HFONT	m_hFont;
	
public:
	xFont(MYOWNERSHIP HFONT font){m_hFont = font;}
	xFont(int size_pixel,xIFont::Family family,int style,xIFont::Weight weight){}
	xFont(int size_pixel,xString fontface,int style,xIFont::Weight weight){}
	virtual ~xFont(){::DeleteObject(m_hFont);}
	
	//plus function for retrieve system fonts
	
	virtual HFONT getHFONT()
	{return m_hFont;}
	
	//--------------STATIC-----------
	static xFont* getSystemFont(xIFont::SystemFonts sf);
};
	
	
/**
 * Defines a font metrics object, which encapsulates information 
 * about the rendering of a particular font on a particular device context.
 */
class XTKAPI xFontMetrics : public virtual xObject,public xIFontMetrics
{
private:
	HDC		m_deviceContext;
	HWND	m_hWnd;
	
public:
	xFontMetrics(HWND hWnd){m_deviceContext = ::GetDC(hWnd);m_hWnd = hWnd;}

	virtual ~xFontMetrics(){::ReleaseDC(m_hWnd,m_deviceContext);}

	virtual int stringWidth(xString str);
	virtual int charWidth(xchar ch);
	virtual int getAscent();
	virtual int getDescent();
	virtual int getHeight();
};

}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_FONT_MSW_H
