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
* @file font_msw.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../../include/xtk/widgets/font.h"
#include "../../../include/xtk/base/exception.h"

#if defined( XTK_USE_WIDGETS) && defined(XTK_GUI_MSW)

#include <windows.h>

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFont* xFont::getSystemFont(xIFont::SystemFonts sf)
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
	return new xFont(font);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xFontMetrics::stringWidth(xString str)
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
	return strSize.cx - (str.size() * 2.3) + 5;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xFontMetrics::charWidth(xchar ch)
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
int xFontMetrics::getAscent()
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
int xFontMetrics::getDescent()
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
int xFontMetrics::getHeight()
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
