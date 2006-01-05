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
* @file version.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_VERSION_H
#define XTK_VERSION_H


namespace xtk
{

#define XTKLIB_VERSION_MAJOR 0
#define XTKLIB_VERSION_MINOR 1
#define XTKLIB_VERSION_RELEASE 0
#define XTKLIB_VERSION_SUBRELEASE 0
#define XTKLIB_VERSION_STRING	_T("0.1.0")


/*  check if the current version is at least major.minor.release.subrelease */
#define XTK_CHECK_VERSION(major,minor,release,subrel) \
    (XTKLIB_VERSION_MAJOR > (major) || \
    (XTKLIB_VERSION_MAJOR == (major) && XTKLIB_VERSION_MINOR > (minor)) || \
    (XTKLIB_VERSION_MAJOR == (major) && XTKLIB_VERSION_MINOR == (minor) && XTKLIB_VERSION_RELEASE >= (release)) || \
	(XTKLIB_VERSION_MAJOR == (major) && XTKLIB_VERSION_MINOR == (minor) && XTKLIB_VERSION_RELEASE == (release) && XTKLIB_VERSION_SUBRELEASE >= (subrel)))
                    
}//namespace

#endif //XTK_VERSION_H

