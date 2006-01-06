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
* @file processtest.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#include "xtk/xtk.h"
using namespace xtk;


class xProcessTest : public xTest
{
private:
	virtual void doTest()
	{
		smartPtr<xProcess> pr = xProcess::execute(_T("stringtest.exe"),xArray<xString*>(),true);
		smartPtr<xInputStream> is = pr->getStdOut();
		xFile log(_T("processtest.log"));
		smartPtr<xOutputStream> os = log.getOutputStream();
		xArray<xbyte> buff(1000);
		int read = 0;
		while((read = is->read(buff)) != xEOF)
		{
			os->write(buff,0,read);
		}
	}

public:
	xProcessTest() : xTest("Process Test"){}
};



