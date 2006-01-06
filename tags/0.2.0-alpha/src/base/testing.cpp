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
* @file testing.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#include "../../include/xtk/base/testing.h"


namespace xtk
{
	
//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
xProfiler::xProfiler()
{
	startTicks = 0;
	sumTicks = 0;
	count = 0;
}

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
void xProfiler::startSession()
{
	startTicks = clock();
}

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
long xProfiler::endSession(int iter_number)
{
	clock_t ticks = clock();
	sumTicks += ticks - startTicks;

	if(count >= iter_number)
	{
		count = 0;
		clock_t tmp = sumTicks;
		sumTicks = 0;

		return (long) tmp;
	}

	count++;
	return (long) 0;
}

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
long xProfiler::endSession()
{
	clock_t currTiks;
	currTiks = clock();
	return (long)(currTiks - startTicks);
}

	
}//namespace
