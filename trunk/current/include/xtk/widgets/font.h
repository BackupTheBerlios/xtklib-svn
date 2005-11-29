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
* @file font.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_FONT_H
#define XTK_FONT_H

#include "../globals.h"
#include "../base/string.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

class xFontInternal;
class xFontMetricsInternal;

/**
 * Defines the interface for the various implementation of class xFont.
 */
class XTKAPI xFont : public virtual xObject
{
private:
	xFontInternal*	m_internal;
	
public:
	enum Family
	{
		XTK_STYLE_SERIF,
		XTK_STYLE_SANS_SERIF,
		XTK_STYLE_FANTASY,
		XTK_STYLE_MONOSPACE
	};
	
	enum Weight
	{
	
	};
	
	enum Style
	{
	
	};
	
	enum SystemFonts
	{
		XTK_SYSTEM_FONT,
		XTK_GUI_FONT
	};
	
	virtual ~xFont();
	xFont(xFontInternal* i);
	xFont(int size_pixel,xFont::Family family,int style,xFont::Weight weight);
	xFont(int size_pixel,xString fontface,int style,xFont::Weight weight);
	
	MYOWNERSHIP xFontInternal* getInternal()
	{return m_internal;}
	
	static xFont* getSystemFont(xFont::SystemFonts sf);
};


/**
 * Defines the interface for the various implementation of class xFontMetrics.
 */
class XTKAPI xFontMetrics : public virtual xObject
{
private:
	xFontMetricsInternal*	m_internal;
	
public:
	xFontMetrics(xFontMetricsInternal* i);
	virtual ~xFontMetrics();
	
	MYOWNERSHIP xFontMetricsInternal* getInternal()
	{return m_internal;}
	
	/**
	 * Returns the total advance width for showing the specified array of characters 
	 * in this Font.
	 */
	virtual int stringWidth(xString str);
		
	/**
	 * Returns the advance width of the specified character in this Font.
	 */
	virtual int charWidth(xchar ch);
	
	/**
	 * Determines the font ascent,the amount by which the character ascends above the baseline.
	 */	
	virtual int getAscent();
		
	/**
	 * Determines the font descent,the amount by which the character descends below the baseline
	 */
	virtual int getDescent();
	
	/**
	 * Gets the standard height of a line of text in this font.
	 */	
	virtual int getHeight();
};

}//namespace

#endif//XTK_USE_WIDGETS

#endif//XTK_FONT_H
