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
* @file setup.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_SETUP_H
#define XTK_SETUP_H

///////////////////////////////////////////////////////////////
//C O M P I L E R   D E F I N E S   I N S T R U C T I O N S
///////////////////////////////////////////////////////////////
//
// #define __DEBUG__
//			for enabling log and other debug facilities
//
// #define __WINDOWS__ 
//			for windows compatibility
//
// #define __UNIX__ 
//			for UNIX compatibility
//
// #define __WIDGETS__
//			to enable widgets module
//
// #define __MAKEDLL__
//			to compile as DLL
//
// #define __USEDLL__
//			to use the dll
//
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// XLIB CONFIGURATION///////////////////////////////////////
////////////////////////////////////////////////////////////

//Enable the use of xApplication for wrapping main
//* <- Add/remove one '/' to/from the beginning of line to comment/uncomment the block
#ifndef XTK_USE_MAIN
	#define XTK_USE_MAIN
#endif
//*/


//Add Thread and Thread-safe support for some classes
//* <- Add/remove one '/' to/from the beginning of line to comment/uncomment the block
#ifndef XTK_USE_MULTITHREAD
	#define XTK_USE_MULTITHREAD
#endif
//*/

//Add Memory checking support,this will be effect only in debug mode
//* <- Add/remove one '/' to/from the beginning of line to comment/uncomment the block
#ifndef XTK_USE_MEMCHECK
	#define XTK_USE_MEMCHECK
#endif
//*/


//Enable unicode build
//* <- Add/remove one '/' to/from the beginning of line to comment/uncomment the block
#ifndef XTK_USE_UNICODE
	#define XTK_USE_UNICODE
#endif
//*/

//Enable zlib support
/* <- Add/remove one '/' to/from the beginning of line to comment/uncomment the block
#ifndef XTK_USE_ZLIB
	#define XTK_USE_ZLIB
#endif
//*/

//Provide some additional functions for testing xtklib
//* <- Add/remove one '/' to/from the beginning of line to comment/uncomment the block
#ifndef xlUSE_TESTFUNC
	#define xlUSE_TESTFUNC
#endif
//*/



#endif //XTK_SETUP_H
