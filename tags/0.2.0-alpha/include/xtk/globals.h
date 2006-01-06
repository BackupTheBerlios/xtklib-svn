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
* @file globals.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_GLOBALS_H
#define XTK_GLOBALS_H

#include "setup.h"
#include "version.h"

/**
* Xlib namespace
*/
namespace xtk
{
//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
#if defined(__WINDOWS__)
#	define XTK_OS_WINDOWS
#elif defined(__UNIX__)
#	define XTK_OS_UNIX
#else
#	error "Must define a valid target OS"
#endif

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
#if defined(_MSC_VER) && !defined(__MWERKS__)
#	define XTK_COMP_VISUAL_C
#elif defined(__GNUC__)
#	define XTK_COMP_GCC
#else
#	error "Compiler not supported"
#endif


//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################

#if defined(__LP64__) || defined(__ARCH_64__)
#	define XTK_ARCH_64
#else
#	define XTK_ARCH_32
#endif

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################


#ifdef __DEBUG__
#	define XTK_DEBUG
#endif


//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################

#if defined(xlUSE_TESTFUNC) && defined(XTK_DEBUG)
#	define XTK_TEST
#endif

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
#if defined(XTK_USE_MEMCHECK) && !defined(XTK_DEBUG)
	#undef XTK_USE_MEMCHECK
#endif

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
#if defined(__WIDGETS__)
	#define XTK_USE_WIDGETS
#endif

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################

#ifdef XTK_COMP_VISUAL_C
	//disable too long name in debug info warning for some stl classes
	//in visual c
	#pragma warning( disable : 4786 )

	//disable C++ Exception Specification ignored warning
	#pragma warning( disable : 4290 ) 

	//pointer truncation
	//#pragma warning( disable : 4311 ) 

	//There were two or more members with the same name. 
	//The one in class2 was inherited since it was a base class for the other 
	//classes that contained this member.
	#pragma warning( disable : 4250 ) 

	//No matching operator delete
	#pragma warning( disable : 4291 ) 

	//'this' : used in base member initializer list
	#pragma warning( disable : 4355 ) 

	//'argument' : conversion from 'LONG_PTR' to 'LONG', possible loss of data
	#pragma warning( disable : 4244 ) 

	//'type cast' : conversion from 'LONG' to 'WNDPROC' of greater size
	#pragma warning( disable : 4312 ) 

	#define _CRT_SECURE_NO_DEPRECATE
#endif

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
#if defined(XTK_OS_WINDOWS)
	#define XTK_EXPORT __declspec(dllexport)
	#define XTK_IMPORT __declspec(dllimport)
#else
	#define WXEXPORT
	#define WXIMPORT
#endif

#if defined(__MAKEDLL__)
	#define XTKAPI XTK_EXPORT
#elif defined(__USEDELL__)
	#define XTKAPI XTK_IMPORT
#else
	#define XTKAPI
#endif

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################

#ifdef XTK_OS_WINDOWS
	#define XTK_GUI_MSW
#else
	#define XTK_GUI_GTK2
#endif 


//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################

#ifndef OUT
	#define OUT
#endif


#define NODELETE
#define OWNERSHIP
#define MYOWNERSHIP
#define YOUROWNERSHIP

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
//define some types
#ifndef XTK_UNDEFINE_TYPES

	//#ifdef ARCHITECTURE
		#ifdef XTK_COMP_VISUAL_C
			typedef __int64					xi64;
			typedef unsigned __int64		xui64;
		#else
			typedef long long				xi64;
			typedef unsigned long long int	xui64;
		#endif
		
		typedef	unsigned char			xbyte;
		typedef long					xi32;
		typedef unsigned long			xui32;
		typedef short					xi16;
		typedef unsigned short			xui16;
		typedef char					xi8;
		typedef unsigned char			xui8;

	//#endif ARCHITECTURE

	#ifdef XTK_ARCH_32
		typedef xui32 ptr_t;
	#elif defined(XTK_ARCH_64)
		typedef xui64 ptr_t;
	#endif
#endif //XTK_UNDEFINE_TYPES


//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################

#ifndef XTK_DEBUG
	#define NDEBUG
#endif

//#include <assert.h>
//#include "memory.h"


//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################

//case Mac
//#define XTK_WORDS_BIGENDIAN

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
		
#if defined(XTK_USE_UNICODE)  &&  !(defined(XTK_COMP_GCC) && defined(XTK_OS_WINDOWS))
	#define XTK_UNICODE
#else
	#undef XTK_USE_UNICODE
#endif


#ifdef XTK_UNICODE
	#define _UNICODE
	#define UNICODE
	
	#ifndef _T
		#define _T(x) L ## x
	#endif

	#define	xchar wchar_t
#else
	#undef _UNICODE
	#undef UNICODE

	#ifndef _T
	#define _T(x) x
	#endif

	typedef	char xchar;
#endif

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################


}//end namespace

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
/*
#if defined(XTK_COMP_VISUAL_C) && defined(XTK_DEBUG) && defined(XTK_USE_MEMCHECK)
	#define _CRTDBG_MAP_ALLOC
	#define _INC_MALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	
	#include <stdlib.h>
	#include <crtdbg.h>
#endif
*/

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
#include "base/object.h"

#ifdef XTK_USE_MEMCHECK
	#ifdef new
		#undef new
	#endif
	
	#define new new(__FILE__,__LINE__)
#endif

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################
//define a usefull macro
#ifndef MAX
	#define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif

//define a usefull macro
#ifndef MIN
	#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################

//workaround for VC++ intellisense to work, enable only in windows debug
/* <- Add/remove one '/' to/from the beginning of line to comment/uncomment the block
#ifndef XTK_USE_WIDGETS
	#define XTK_USE_WIDGETS
#endif

#ifndef XTK_GUI_MSW
	#define XTK_GUI_MSW
#endif
//*/

#endif //XTK_GLOBALS_H
