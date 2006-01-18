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
* @file test.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/



#include "../include/xtk/xtk.h"
using namespace xtk;

#include <stdio.h>

//#include "stringTest.cpp"
//#include "dateTest.cpp"
//#include "loggingtest.cpp"
//#include "nettest.cpp"
//#include "preferencestest.cpp"
//#include "fileTest.cpp"
//#include "threadtest.cpp"
//#include "zstreamtest.cpp"
//#include "processtest.cpp"
#include "widgetstest.h"
#include "tdatastructurestest.h"

//IMPLEMENT_MAIN

class TestApp : public xApplication
{
public:
	virtual int onRun()
	{
		xTDataStructuresTest tdst;
		tdst.startTest();

		xSystem::pause();
		return 0;
	}
};

SET_APPLICATION(TestApp);