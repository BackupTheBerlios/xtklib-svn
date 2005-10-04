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

/**
 * Defines the interface for the various implementation of class xFont.
 */
class XTKAPI xIFont
{
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
	
	virtual ~xIFont(){}
	
protected:
	xIFont(){}
	xIFont(int size_pixel,xIFont::Family family,int style,xIFont::Weight weight){}
	xIFont(int size_pixel,xString fontface,int style,xIFont::Weight weight){}
	
	//plus function for reterieve system fonts
};


/**
 * Defines the interface for the various implementation of class xFontMetrics.
 */
class xIFontMetrics
{
protected:
	xIFontMetrics(){}
public:
	virtual ~xIFontMetrics(){}
	
	/**
	 * Returns the total advance width for showing the specified array of characters 
	 * in this Font.
	 */
	virtual int stringWidth(xString str) = 0;
		
	/**
	 * Returns the advance width of the specified character in this Font.
	 */
	virtual int charWidth(xchar ch) = 0;
	
	/**
	 * Determines the font ascent,the amount by which the character ascends above the baseline.
	 */	
	virtual int getAscent() = 0;
		
	/**
	 * Determines the font descent,the amount by which the character descends below the baseline
	 */
	virtual int getDescent() = 0;
	
	/**
	 * Gets the standard height of a line of text in this font.
	 */	
	virtual int getHeight() = 0;
};

}//namespace


//select include file
#ifdef XTK_GUI_MSW
	#include "msw/font_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/font_gtk2.h"
#endif

#endif//XTK_USE_WIDGETS
#endif//XTK_FONT_H
