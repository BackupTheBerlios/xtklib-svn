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
* @file object.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#include "../../include/xtk/globals.h"
#include "../../include/xtk/base/exception.h"
#include "../../include/xtk/base/string.h"
#include "../../include/xtk/base/thread.h"


//used for AC_CHECK_LIB
extern "C"
{
	int checkLibFunc()
	{	
		int i = 0;
		const char* t = "TEST";
		i += (int) strlen(t);
		return i;
	}
}


namespace xtk
{

xNullObject xNullObject::nullObject;

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#ifdef XTK_USE_MULTITHREAD
	void xObject::createMutex()
	{
		#ifdef XTK_OS_WINDOWS

			m_mutex_id = ::CreateMutex( NULL, TRUE, NULL );
			if(m_mutex_id == NULL)
			{
				throw xSystemException(_T("Error while creating Mutex!"));
			}    
			else
			{
				//windows mutex are locked by default
				::ReleaseMutex((HANDLE)m_mutex_id);
			}    
	
		#else //POSIX systems
	
			int res;

			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
			res = pthread_mutex_init(&m_mutex_id, &attr);
			pthread_mutexattr_destroy(&attr);
			if(res != 0)
			{
				throw xSystemException(_T("Error while creating Mutex!"));
			}
		#endif
		
		m_bMutexCreated = true;
	}
#endif

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject::xObject()
{
	#ifdef XTK_USE_MULTITHREAD
		m_bMutexCreated = false;
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject::~xObject()
{
	#ifdef XTK_USE_MULTITHREAD
		if(m_bMutexCreated)
		{
			#ifdef XTK_OS_WINDOWS
				::CloseHandle((HANDLE)m_mutex_id);
			#else //POSIX systems
				::pthread_mutex_destroy(&m_mutex_id);
			#endif
		}
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject* xObject::clone()
{
	throw xCloneNotSupportedException();
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xObject::toString()
{
	#ifdef XTK_COMP_VISUAL_C
		#pragma warning( disable : 4311 ) 
	#endif

	return xString::getFormat(_T("%d"),(int) this);

	#ifdef XTK_COMP_VISUAL_C
		#pragma warning( default : 4311 ) 
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xNullObject::toString()
{
	return xString(_T("Null Object"));
}

}//end namespace
