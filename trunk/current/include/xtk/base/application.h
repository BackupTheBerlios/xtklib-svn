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
* @file application.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_APPLICATION_H
#define XTK_APPLICATION_H

#include "../globals.h"

#ifdef XTK_OS_WINDOWS
	#include <windows.h>
#endif


namespace xtk
{


class XTKAPI xApplication 
{
private:
	#ifdef XTK_OS_WINDOWS
		static HINSTANCE s_hInstance;
	#endif
	
public:
	
	/**
	* Initializes all xtklib facilities
	*/
	static void initialize(int* argc,char*** argv);

	/**
	* Finalizes all xtklib facilities
	*/
	static void finalize();
	
	/**
	* @todo implement argc argv
	*/
	static int entryPoint();	
	
	#ifdef XTK_OS_WINDOWS
		/**
		* Defined only in MSW builds
		*/
		static HINSTANCE getHinstance(){return s_hInstance;}
		static void setHinstance(HINSTANCE hInstance){s_hInstance = hInstance;}
	#endif
};



}//namespace

#endif // XTK_APPLICATION_H
