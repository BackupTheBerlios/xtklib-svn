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
* @file application.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_APPLICATION_H
#define XTK_APPLICATION_H

#include "../globals.h"
#include "string.h"
#include "array.h"

#ifdef XTK_OS_WINDOWS
	#include <windows.h>
#endif


namespace xtk
{

#define SET_APPLICATION(ClassApp)					\
	xApplication& xApplication::getApplication()	\
	{												\
		static ClassApp theApp;						\
		return theApp;								\
	}												


/**
 * The xApplication class represents the application itself and provides the
 * program entry point. You must override this to create your own application.
 */
class XTKAPI xApplication 
{
private:
	#ifdef XTK_OS_WINDOWS
		static HINSTANCE s_hInstance;
	#endif

	#if defined(XTK_OS_WINDOWS) && !defined(_CONSOLE)
		static xArray<xString> commandLineToArgv(LPSTR lpCmdLine);
	#else
		static xArray<xString> commandLineToArgv(int argc,char** argv);
	#endif

	int					m_argc;
	xArray<xString>		m_argv;
public:
	#ifdef XTK_OS_WINDOWS
		/**
		* Defined only in MSW builds
		*/
		static HINSTANCE getHinstance(){return s_hInstance;}
		static void setHinstance(HINSTANCE hInstance){s_hInstance = hInstance;}
	#endif

	static xApplication& getApplication();

	int getArgc()
	{return m_argc;}

	xArray<xString> getArgv()
	{return m_argv;}

	#ifdef XTK_USE_WIDGETS
		/**
		 * Enter inside the ui event loop.
		 */
		static void uiEventLoop();

		/**
		 * Force the exiting from the ui event loop.
		 */
		static void uiExitEventLoop();

		/**
		 *
		 */
		static void uiProcessPendingEvent();
	#endif

	/**
	 * Override this to provide initialization of application (eg. creation of GUI).
	 */
	virtual void onInit()
	{}

	/**
	 * Here starts the application and if widget module is enabled it manages the gui event loop.
	 * If you not use the gui you can freely override this, otherwise if should manage manually
	 * the event loop by calling uiEventLoop(). The return value of this method is the exit 
	 * code of the application.
	 */
	virtual int onRun();

	/**
	 *
	 */
	virtual void onExit()
	{}
};



}//namespace

#endif // XTK_APPLICATION_H
