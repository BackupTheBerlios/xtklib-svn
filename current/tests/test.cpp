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
* @file test.cpp
* @author Mario Casciaro (xshadow@email.it)
*/



#include "xtk/xtk.h"
using namespace xtk;

#include <stdio.h>

#include "stringTest.cpp"
//#include "dateTest.cpp"
//#include "loggingtest.cpp"
//#include "nettest.cpp"
//#include "preferencestest.cpp"
//#include "fileTest.cpp"
//#include "threadtest.cpp"
//#include "zstreamtest.cpp"
//#include "processtest.cpp"


//IMPLEMENT_MAIN

//int main(int argv,char** argc)
int xApplication::entryPoint()
{
	//xSystem::getStdout().write(xSystem::getHomeDir());
	//xThread::sleep(100);
	//xThread::initialize();
	//system("echo Hello");
	
	//xInitialize();
	//xLoggingTest l;
	//l.startTest();

	xStringTest s;
	s.startTest();

	//xDateTest d;
	//d.startTest();

	//xFileTest f;
	//f.startTest();

	//xThreadTest t;
	//t.startTest();

	//xNetTest n;
	//n.startTest();

	//xPreferencesTest p;
	//p.startTest();

	//xZStreamTest z;
	//z.startTest();


	//xProcessTest pr;
	//pr.startTest();

	system("PAUSE");
	return 0;
}
