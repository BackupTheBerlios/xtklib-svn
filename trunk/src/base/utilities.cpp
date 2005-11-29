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
* @file Utilities.cpp
* @author Mario Casciaro (xshadow@email.it)
*/


#include <sstream> 
#include <stdlib.h>
#include <stdarg.h>
#include "../../include/xtk/base/utilities.h"

#ifdef XTK_UNIX
	#include <signal.h>
#endif

namespace xtk
{

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
/*void xlFormatHexData(const ui8* rawData,size_t rawData_size,std::string& formattedString)
{
    ostringstream sstr;
    sstr.setf(ios::hex, ios::basefield);
    sstr.fill(48);
    for(size_t i = 0;i < rawData_size;i++)
	{
        sstr.width(2);
        unsigned short dt = (unsigned short) rawData[i];
        sstr << dt;
    }
    formattedString = sstr.str();
}
*/
//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
void xlTrap()
{
	#if defined(XTK_WINDOWS)
    	DebugBreak();
	#elif defined(XTK_UNIX)
    	raise(SIGTRAP);
	#endif
}

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
int xlRandomInt()
{
	static int seed = 1;
	int new0 = (((int)time(NULL)+(int)clock()) % 256);
	int new1 = abs(((new0*seed - (int)clock())) - new0);
	int new2 = abs((58*(new1*seed) - new0) % 96777211);
	seed  = new2;
	
	return new2;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
xTimer::xTimer(bool autostart)
{
	reset();
	if(autostart)
		start();
}

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
xTimer::~xTimer()
{
}

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
void xTimer::start()
{
	m_bStarted = true;
	m_ticksAtStart = clock();
}

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
void xTimer::reset()
{
	m_bStarted = false;
	m_ticksAtStart = 0;
}

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
void xTimer::restart()
{
	reset();
	start();
}

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
long xTimer::getSecs()
{
	if(m_bStarted)
	{
		return (long)((clock() - m_ticksAtStart) / CLOCKS_PER_SEC);
	}
	else
		return 0;
}

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
long xTimer::getMsecs()
{
	if(m_bStarted)
	{
		return (long)((clock() - m_ticksAtStart) / (CLOCKS_PER_SEC / 1000));
	}
	else
		return 0;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------


}//end namespace