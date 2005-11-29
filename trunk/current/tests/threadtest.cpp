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
* @file threadtest.cpp
* @author Mario Casciaro (xshadow@email.it)
*/


#include "xtk/xtk.h"
using namespace xtk;
#include <stdio.h>

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xTestThread : public xThread
{
private:
	int sleeptime;
	
public:
	xTestThread(xString name,bool detatched, xThreadGroup* thread_group,int sleeptime) 
	: xThread(name,detatched,thread_group){this->sleeptime = sleeptime;}


	virtual void run()
	{
		
		xSystem::getStdout().write(xString::getFormat(_T("Started thread %s\n"),getName().c_str()));
		//xSystem::getStdout().write(xString::getFormat(_T("Try to retrieve current thread name: %s \n"),
		//	 xThread::getSelfThread()->getName().c_str()));
		xThread::sleep(sleeptime);
		xSystem::getStdout().write(xString::getFormat(_T("Terminated thread %s\n"),getName().c_str()));
		
	}	
};


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xThreadTest : public xTest
{
private:
	virtual void doTest()
	{
		xThreadGroup tg;
		xThread* t1 = new xTestThread(_T("Thread1"),false,&tg,1010);
		xThread* t2 = new xTestThread(_T("Thread2"),true,&tg,1020);
		xThread* t3 = new xTestThread(_T("Thread3"),true,&tg,1010);
		xThread* t4 = new xTestThread(_T("Thread4"),true,&tg,1030);
		xThread* t5 = new xTestThread(_T("Thread5"),false,&tg,1020);
		xThread* t6 = new xTestThread(_T("Thread6"),false,&tg,1020);
		xThread* t7 = new xTestThread(_T("Thread7"),false,&tg,1010);
		xThread* t8 = new xTestThread(_T("Thread8"),false,&tg,1010);
		
		t1->start();
		t3->start();
		t2->start();
		t5->start();
		t4->start();
		t7->start();
		xSystem::getStdout().write(xString::getFormat(_T("Count: %d\n"),tg.getRunningCount()));
		t6->start();
		t8->start();
		
		tg.joinAll();
		xSystem::getStdout().write(_T("Joined all\n"));
		
		t1 = new xTestThread(_T("Thread1"),false,NULL,1010);
		t2 = new xTestThread(_T("Thread2"),true,NULL,1020);
		t3 = new xTestThread(_T("Thread3"),true,NULL,1010);
		t4 = new xTestThread(_T("Thread4"),true,NULL,1030);
		t5 = new xTestThread(_T("Thread5"),false,NULL,1020);
		t1->start();
		t3->start();
		t2->start();
		t5->start();
		t4->start();
	}
	
public:
	xThreadTest() : xTest("Thread Test"){}
};



//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################


