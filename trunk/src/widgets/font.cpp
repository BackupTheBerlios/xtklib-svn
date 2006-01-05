/*
* This file is part of XTKLib project.
*
* Copyright(C) 2003-2006 Mario Casciaro xshadow[AT]email(DOT)it
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
* @file frame.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/widgets/font.h"

#ifdef XTK_USE_WIDGETS

//select include file
#ifdef XTK_GUI_MSW
	#include "msw/font_msw.h"
#elif defined(XTK_GUI_GTK2)
	#include "gtk2/font_gtk2.h"
#endif

namespace xtk
{


xFont::xFont(xFontInternal* i)
{m_internal = i;}

xFont::xFont(int size_pixel,xFont::Family family,int style,xFont::Weight weight)
{m_internal = NULL;/*new xFontInternal(size_pixel,family,style,weight);*/}

xFont::xFont(int size_pixel,xString fontface,int style,xFont::Weight weight)
{m_internal = NULL;/*new xFontInternal(size_pixel,fontface,style,weight);*/}

xFont::~xFont()
{delete m_internal;}

xFont* xFont::getSystemFont(xFont::SystemFonts sf)
{return xFontInternal::getSystemFont(sf);}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

xFontMetrics::xFontMetrics(xFontMetricsInternal* i)
{m_internal = i;}

xFontMetrics::~xFontMetrics()
{delete m_internal;}
	
int xFontMetrics::stringWidth(xString str)
{return m_internal->stringWidth(str);}
		
int xFontMetrics::charWidth(xchar ch)
{return m_internal->charWidth(ch);}
	
int xFontMetrics::getAscent()
{return m_internal->getAscent();}
		
int xFontMetrics::getDescent()
{return m_internal->getDescent();}
	
int xFontMetrics::getHeight()
{return m_internal->getHeight();}


}//namespace

#endif //XTK_USE_WIDGETS
