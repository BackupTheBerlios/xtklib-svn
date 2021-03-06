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
* @file datetest.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "xtk/xtk.h"
using namespace xtk;


class xDateTest : public xTest
{
private:
	virtual void doTest()
	{
		xTime t(2005,8,3,17,5,7);

 		ensureTrue(t.format(_T("%A %B %d %m %Y")).equals(_T("Wednesday August 03 08 2005")),"Format");
		
		testManualStart("Print current date");
		t.now();
		testLog(t.toString().mb_str());
		testOK();
	}

public:
	xDateTest() : xTest("Date Test"){}
};