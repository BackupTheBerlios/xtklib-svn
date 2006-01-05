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
* @file font_msw.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "font_msw.h"
#include "../../../include/xtk/base/exception.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <windows.h>

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFont* xFontInternal::getSystemFont(xFont::SystemFonts sf)
{
	int fnObject = 0;
	switch(sf)
	{
	case xFont::XTK_GUI_FONT:
		fnObject = DEFAULT_GUI_FONT;
		break;
	case xFont::XTK_SYSTEM_FONT:
		fnObject = SYSTEM_FONT;
		break;
	}

	HFONT font = (HFONT) ::GetStockObject(fnObject);
	return new xFont(new xFontInternal(font));
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xFontMetricsInternal::stringWidth(xString str)
{
	SIZE strSize;
	
	if(::GetTextExtentPoint32(
		m_deviceContext,		  // handle to DC
		str.c_str(),				// text string
		(int) str.size(),					// characters in string
		&strSize				 // string size
		) == 0)
	{
		throw xSystemException(_T("GetTextExtentPoint32 Failed"),GetLastError());
	}
	
	//this is an empiric value that trye to fit the best size
	return strSize.cx - (int)(str.size() * 2.3) + 5;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xFontMetricsInternal::charWidth(xchar ch)
{
	SIZE strSize;
	if(::GetTextExtentPoint32(
		m_deviceContext,		  // handle to DC
		&ch,				// text string
		1,					// characters in string
		&strSize				 // string size
		) == 0)
	{
		throw xSystemException(_T("GetTextExtentPoint32 Failed"),GetLastError());
	}

	return strSize.cx;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xFontMetricsInternal::getAscent()
{
	TEXTMETRIC met;
	if(::GetTextMetrics(
		m_deviceContext,   // handle to DC
		&met			   // text metrics
		) == 0)
	{
		throw xSystemException(_T("GetTextMetrics Failed"),GetLastError());
	}
	
	return met.tmAscent;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xFontMetricsInternal::getDescent()
{
	TEXTMETRIC met;
	if(::GetTextMetrics(
		m_deviceContext,   // handle to DC
		&met			   // text metrics
		) == 0)
	{
		throw xSystemException(_T("GetTextMetrics Failed"),GetLastError());
	}

	return met.tmDescent;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xFontMetricsInternal::getHeight()
{
	TEXTMETRIC met;
	if(::GetTextMetrics(
		m_deviceContext,   // handle to DC
		&met			   // text metrics
		) == 0)
	{
		throw xSystemException(_T("GetTextMetrics Failed"),GetLastError());
	}

	return met.tmHeight;
}


}//namespace

#endif //XTK_USE_WIDGETS
