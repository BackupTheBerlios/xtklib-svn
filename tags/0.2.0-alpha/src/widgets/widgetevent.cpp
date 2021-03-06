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
* @file widgetevent.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/widgets/widgetevent.h"
#include "../../include/xtk/widgets/widget.h"
#include "../../include/xtk/widgets/window.h"

#ifdef XTK_USE_WIDGETS

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xWidget& xWidgetEvent::getWidget()
{
	return dynamic_cast<xWidget&>(getSource());
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################	
xWindow& xWindowEvent::getWindow()
{
	return dynamic_cast<xWindow&>(getSource());
}



}//namespace

#endif //XTK_USE_WIDGETS
