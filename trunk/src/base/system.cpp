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
* @file system.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/base/system.h"
#include "../../include/xtk/base/exception.h"
#include "../../include/xtk/base/file.h"

#include <stdio.h>
#include <wchar.h>
#if defined(XTK_OS_WINDOWS)
	#include <windows.h>
	#include <io.h>
	#include <direct.h>
	
#elif defined(XTK_OS_UNIX)
	#include <unistd.h>
	#include <dirent.h>

#endif


namespace xtk
{

xSystem::xSTDWriter	xSystem::s_stdo;
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xSystem::getWorkingDir()
{
	#ifdef XTK_OS_WINDOWS
		size_t size = _MAX_PATH;
		xchar *buffer = (xchar *) malloc (size * sizeof(xchar));
		
		#ifndef XTK_UNICODE
			if(::_getcwd(buffer, (int) size) == NULL)
		#else
			if(::_wgetcwd(buffer, (int) size) == NULL)
		#endif
		{
			free(buffer);
			throw xSystemException(_T("Cannot retrieve current working directory"));
		}
		else
		{
				xString str(buffer);
				free(buffer);
				return str;
		}
	
	#elif defined(XTK_OS_UNIX)
	
		size_t size = 1000;
		while(true)
		{
			char *buffer = (char *) malloc (size);
			
			if(getcwd(buffer, size) == NULL)
			{
				if(errno != ERANGE)
				{
					free(buffer);
					throw xSystemException(_T("Cannot retrieve current working directory"));
				}
			}
			else
			{
				xString str(xString::getFromOS(buffer));
				free(buffer);
				return str;
			}
			free (buffer);
			size *= 2;
		}
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xSystem::xSTDWriter::writeChar(xchar c) throw(xIOException)
{
	synchronizeStart();
	
	#ifndef XTK_UNICODE
		if(putchar(c) == EOF)
			throw xIOException(_T("Error writing to stdout"));
	#else
		if(putwc(c,stdout) == WEOF)
			throw xIOException(_T("Error writing to stdout"));
	#endif
	
	synchronizeEnd();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xSystem::xSTDWriter::write(xString s) throw(xIOException)
{
	synchronizeStart();
	
	#ifndef XTK_UNICODE
		if(puts(s.c_str()) == EOF)
			throw xIOException(_T("Error writing to stdout"),getSysErrorCode());
	#else
		if(fputws(s.c_str(),stdout) == (int)WEOF)
			throw xIOException(_T("Error writing to stdout"),getSysErrorCode());
	#endif
	
	synchronizeEnd();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xSystem::getEnv(xString name,OUT xString& value)
{
	#if defined(XTK_UNICODE) && defined(XTK_OS_WINDOWS)
		xchar* arg = ::_wgetenv(name.c_str());
	#else
		char* arg = ::getenv(name.mb_str());
	#endif
	
	if(arg == NULL)
		return false;
		
	value = xString::getFromOS(arg);
	return true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xSystem::getTempDir()
{
	xString ret;
	if(getEnv(_T("TEMP"),ret))
		return ret;
	else //use the current dir
		return xString::getFormat(_T(".%c"),xFile::separator);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xSystem::getHomeDir()
{
	xString ret;
	bool res;
	#ifdef XTK_OS_WINDOWS
		//try to get userprofile (work under winNt 2000 xp)
		res = getEnv(_T("USERPROFILE"),ret);
		
		if(res)
			return ret;
			
		//try to get HOMEDRIVE + HOMEPATH
		res = getEnv(_T("HOMEDRIVE"),ret);
		
		if(res)
		{
			xString ret2;
			res = getEnv(_T("HOMEPATH"),ret2);
			if(res)
			{
				ret.append(ret2);
				return ret;
			}
		}
	#elif defined(XTK_OS_UNIX)
		res = getEnv(_T("HOME"),ret);
		if(res)
			return ret;
			
	#endif
	
	//use the current dir
	return xString::getFormat(_T(".%c"),xFile::separator);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xSystem::getTotalMemorySize()
{ 
	#if defined(XTK_OS_WINDOWS)
		MEMORYSTATUS mem_info;
		::GlobalMemoryStatus(&mem_info);

		return (int) (mem_info.dwTotalPhys / (1024 * 1024));

	#elif defined(XTK_OS_UNIX)

		throw xNotImplementedException();
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xSystem::getAvailableMemorySize()
{
	#if defined(XTK_OS_WINDOWS)
		MEMORYSTATUS mem_info;
		::GlobalMemoryStatus(&mem_info);

		return (int) (mem_info.dwAvailPhys / (1024 * 1024));

	#elif defined(XTK_OS_UNIX)

		throw xNotImplementedException();
	#endif
}



}//namespace
