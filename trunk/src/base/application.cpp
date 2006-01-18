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
* @file application.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/base/application.h"
#include "../../include/xtk/base/thread.h"
#include "../../include/xtk/base/net.h"
#include "../../include/xtk/base/system.h"

namespace xtk
{

#ifdef XTK_OS_WINDOWS
	HINSTANCE xApplication::s_hInstance;
#endif

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
extern void xtkUiInitialize(int* argc,char*** argv);

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xtkInitialize(int* argc,char*** argv)
{
	#ifdef XTK_USE_MULTITHREAD
		xThread::initialize();
	#endif

	#if defined(XTK_USE_WIDGETS)
		xtkUiInitialize(argc,argv);
	#endif

	xSocket::initialize();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xtkFinalize()
{
	xSocket::finalize();

	#ifdef XTK_USE_MULTITHREAD
		xThread::finalize();
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xApplication::onRun()
{
	#ifdef XTK_USE_WIDGETS
		uiEventLoop();
	#endif

	return 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
	#if defined(XTK_OS_WINDOWS) && !defined(_CONSOLE)
		static xArray<xString> commandLineToArgv(LPSTR lpCmdLine)
		{
			xString cmd(lpCmdLine,xCharset::CS_SYSTEM);
			xStringTokenizer tok(cmd,_T(" "));
			xArray<xString> arr(tok.countTokens());
			for(int i = 0;tok.hasMoreTokens();i++)
			{
				arr[i] = tok.nextToken();
			}
			return arr;
		}
	#else
		static xArray<xString> commandLineToArgv(int argc,char** argv)
		{
			xArray<xString> arr(argc);
			for(int i = 0;i < argc;i++)
			{
				arr[i] = xString(argv[i],xCharset::CS_SYSTEM);
			}
			return arr;
		}
	#endif

}//namespace

#ifdef XTK_USE_MAIN
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#if defined(XTK_OS_WINDOWS) && !defined(_CONSOLE)
	int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
	{
		xtk::xApplication::getApplication();
		xtk::xApplication::setHinstance(hInstance);
		xtk::xtkInitialize(NULL,NULL);
		xtk::xStartMemoryTracking();
		int ret = -1;
		try
		{
			xtk::xApplication::getApplication().onInit();
			ret = xtk::xApplication::getApplication().onRun();
			xtk::xApplication::getApplication().onExit();
		}
		catch(xtk::xException& ex)
		{
			xtk::xSystem::getStdout().write(xtk::xString::getFormat(_T("Uncached exception %s,%s\nStack trace:\n"),
			ex.getClassString().c_str(),ex.getDescription().c_str()));
			ex.printStackTrace(xtk::xSystem::getStdout());
		}
		xtk::xDumpMemoryLeaks();
		xtk::xtkFinalize();

		return ret;
	}
#endif

int main(int argc,char** argv)
{
	xtk::xApplication::getApplication();
	xtk::xtkInitialize(&argc,&argv);
	xtk::xStartMemoryTracking();
	int ret = -1;
	try
	{
		xtk::xApplication::getApplication().onInit();
		ret = xtk::xApplication::getApplication().onRun();
		xtk::xApplication::getApplication().onExit();
	}
	catch(xtk::xException& ex)
	{
		xtk::xSystem::getStdout().write(xtk::xString::getFormat(_T("Uncached exception %s,%s\nStack trace:\n"),
		ex.getClassString().c_str(),ex.getDescription().c_str()));
		ex.printStackTrace(xtk::xSystem::getStdout());
	}
	xtk::xDumpMemoryLeaks();
 	xtk::xtkFinalize();

	return ret;
}

#endif //#ifdef XTK_USE_MAIN
