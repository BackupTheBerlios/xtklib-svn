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

#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)
	#include "../../include/xtk/widgets/widget.h"
#endif

namespace xtk
{

#ifdef XTK_OS_WINDOWS
	HINSTANCE xApplication::s_hInstance;
#endif

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xApplication::initialize(int* argc,char*** argv)
{
	#ifdef XTK_USE_MULTITHREAD
		xThread::initialize();
	#endif

	#if defined(XTK_USE_WIDGETS) && defined(XTK_GUI_GTK2)
		xtkWidgetsInitialize(argc,argv);
	#endif

	xSocket::initialize();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xApplication::finalize()
{
	xSocket::finalize();

	#ifdef XTK_USE_MULTITHREAD
		xThread::finalize();
	#endif
}

}//namespace

#ifdef XTK_USE_MAIN
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#ifdef XTK_OS_WINDOWS
	int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
	{
		xtk::xApplication::setHinstance(hInstance);
		xtk::xApplication::initialize(NULL,NULL);
		xtk::xStartMemoryTracking();
		int ret = xtk::xApplication::entryPoint();
		xtk::xDumpMemoryLeaks();
		xtk::xApplication::finalize();

		return ret;
	}

#endif

int main(int argc,char** argv)
{
	xtk::xApplication::initialize(&argc,&argv);
	xtk::xStartMemoryTracking();
	int ret = xtk::xApplication::entryPoint();
	xtk::xDumpMemoryLeaks();
 	xtk::xApplication::finalize();

	return ret;
}

#endif //#ifdef XTK_USE_MAIN
