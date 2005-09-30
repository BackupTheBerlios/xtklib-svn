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
* @file application.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/application.h"
#include "../../include/xtk/base/thread.h"
#include "../../include/xtk/base/net.h"


namespace xtk
{

#ifdef XTK_OS_WINDOWS
	HINSTANCE xApplication::s_hInstance;
#endif

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xApplication::initialize()
{
	#ifdef XTK_USE_MULTITHREAD
		xThread::initialize();
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
		xtk::xApplication::initialize();
		xtk::xStartMemoryTracking();
		xtk::xApplication::entryPoint();
		xtk::xDumpMemoryLeaks();
		xtk::xApplication::finalize();
	}

#endif

int main(int argc,char** argv)
{
	xtk::xApplication::initialize();
	xtk::xStartMemoryTracking();
	xtk::xApplication::entryPoint();
	xtk::xDumpMemoryLeaks();
	xtk::xApplication::finalize();
}

#endif //#ifdef XTK_USE_MAIN
