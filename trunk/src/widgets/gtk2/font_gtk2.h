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
* @file font_gtk2.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_FONT_GTK2_H
#define XTK_FONT_GTK2_H

#include "../../../include/xtk/widgets/font.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)
#include <gtk/gtk.h>

namespace xtk
{


/**
 * Defines the interface for the various implementation of class xFont.
 */
class XTKAPI xFontInternal : public virtual xObject
{
public:
	xFontInternal()
	{}
	
	xFontInternal(int size_pixel,xFont::Family family,int style,xFont::Weight weight)
	{}
		
	xFontInternal(int size_pixel,xString fontface,int style,xFont::Weight weight)
	{}
	
	virtual ~xFontInternal()
	{}
	
	//--------------STATIC-----------
	static xFont* getSystemFont(xFont::SystemFonts sf);
};
	
	
/**
 * Defines a font metrics object, which encapsulates information 
 * about the rendering of a particular font on a particular device context.
 */
class XTKAPI xFontMetricsInternal : public virtual xObject
{
public:
	xFontMetricsInternal()
	{}

	virtual ~xFontMetricsInternal()
	{}

	virtual int stringWidth(xString str)
	{return 0;}
	
	virtual int charWidth(xchar ch)
	{return 0;}
	
	virtual int getAscent()
	{return 0;}
	
	virtual int getDescent()
	{return 0;}
	
	virtual int getHeight()
	{return 0;}
};


}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_FONT_GTK2_H
