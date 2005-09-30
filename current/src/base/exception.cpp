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
* @file thread.cpp
* @author Mario Casciaro (xshadow@email.it)
*/


#include "../../include/xtk/base/exception.h"
#include "../../include/xtk/base/string.h"
#include "../../include/xtk/base/array.h"
#include "../../include/xtk/base/system.h"
#include "../../include/xtk/base/stacktrace.h"


#ifdef XTK_OS_WINDOWS
	#include <Windows.h>
#elif defined(XTK_OS_UNIX)
	#include <string.h>
#endif

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xException::captureStackTrace()
{
	if(m_stackTrace == NULL)
		m_stackTrace = new xArray<xStackFrame*>();
	else
		m_stackTrace->deleteData();

	(*m_stackTrace) = xStackTrace::getCurrentStackTrace(2);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xException::printStackTrace(xWriter& wr)
{
	for(int i = 0;i < m_stackTrace->size();i++)
	{
		wr.write(xString::getFormat(_T("[%d] "),(*m_stackTrace)[i]->getLevel()));
		
		if(! (*m_stackTrace)[i]->getModule().equals(_T("")))
		{
			wr.write(_T("{"));
			wr.write((*m_stackTrace)[i]->getModule());
			wr.write(_T("}"));
		}
		
		
		wr.write((*m_stackTrace)[i]->getName());
		wr.write(_T("\n"));
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xException::xException(const xException& copy)
{
	m_cause = copy.m_cause;
	m_description = copy.m_description;
	m_errorSysCode = copy.m_errorSysCode;
	m_stackTrace = new xArray<xStackFrame*>(*(copy.m_stackTrace));
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xException::~xException()
{
	if(m_stackTrace != NULL)
	{
		m_stackTrace->deleteData();
		delete m_stackTrace;
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xException::getSysErrorCodeDescription()
{
	#ifdef XTK_OS_WINDOWS
		LPVOID lpMsgBuf;

		::FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			m_errorSysCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
			);
			
		xString ret((const xchar*)lpMsgBuf);
		// Free the buffer.
		::LocalFree( lpMsgBuf );
		return ret;
		
	#elif defined(XTK_OS_UNIX)
		char buff[1024];
		char* ret = strerror_r(m_errorSysCode, buff, 1024);
		return xString::getFromOS(ret);
		
	#endif
}
	
}//namespace

