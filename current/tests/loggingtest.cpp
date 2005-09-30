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
* @file loggingtest.cpp
* @author Mario Casciaro (xshadow@email.it)
*/


#include "xtk/xtk.h"
using namespace xtk;

#include <stdio.h>
#include <wchar.h>


class xLoggingTest : public xTest
{
private:
	virtual void doTest()
	{
		xLogger logger(_T("Test"));
		
		logger.addHandler(new xLogHandlerSTD());
		logger.addHandler(new xLogHandlerFile(_T("log.test")));
		
		logger.log(xLogLevel::SEVERE,_T("Test Log message 1"));
		logger.log(xLogLevel::INFO,_T("Test Log message 2"));
	}

public:
	xLoggingTest() : xTest("Logging Test"){}
};
