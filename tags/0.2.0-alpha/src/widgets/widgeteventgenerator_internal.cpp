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
* @file widgeteventgenerator_internal.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "widgeteventgenerator_internal.h"
#include <assert.h>

#if defined(XTK_USE_WIDGETS)

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWidgetEventGeneratorInternal::processEvent(xWidgetEvent& ev)
{
	if(m_evtHandlers.isEmpty())
		return;

	xIterator* iter = m_evtHandlers.iterator();
	while(iter->hasNext())
	{
		xWidgetEventHandlerEntry* evh = static_cast<xWidgetEventHandlerEntry*>(&(iter->next()));
		assert(evh != NULL);
		if(ev.getID() == evh->m_evtMask)
			evh->m_evtHandler->handleEvent(ev);
	}
	delete iter;
}


}//namespace

#endif //XTK_USE_WIDGETS
