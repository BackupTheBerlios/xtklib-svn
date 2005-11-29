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
* @file triggerableevent.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/thread.h"
#include "../../include/xtk/base/exception.h"
#include "../../include/xtk/base/string.h"

#ifdef XTK_USE_MULTITHREAD

#ifdef XTK_OS_UNIX
	#include <sys/time.h>
#endif

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xTriggerableEvent::xTriggerableEvent()
{
	m_data = new xTriggerableEventData();
	m_data->m_referenceCount = 1;
	
	#ifdef XTK_OS_WINDOWS
		m_data->win_event = ::CreateEvent(NULL, true, false, NULL);
		if(m_data->win_event == NULL)
		{
			throw xSystemException(_T("TriggerableEvent creation error"));
		}

	#else //Unix systems	
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutex_init(&(m_data->pt_mutex), &attr);
		pthread_mutexattr_destroy(&attr);
		pthread_cond_init(&(m_data->pt_cond), NULL);
		m_data->m_signaled = false;
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xTriggerableEvent::xTriggerableEvent(const xTriggerableEvent& copy)
{
	copy.m_data->synchronizeStart();
	copy.m_data->m_referenceCount++;
	copy.m_data->synchronizeEnd();
	
	m_data = copy.m_data;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xTriggerableEvent::~xTriggerableEvent()
{
	m_data->synchronizeStart();
	m_data->m_referenceCount--;
	
	if(m_data->m_referenceCount <= 0)
	{
		#ifdef XTK_OS_WINDOWS
			::CloseHandle((HANDLE)m_data->win_event);

		#else //POSIX systems
			pthread_cond_destroy(&(m_data->pt_cond));
			pthread_mutex_destroy(&(m_data->pt_mutex));
		#endif
		
		m_data->synchronizeEnd();
		delete m_data;
	}
	else
		m_data->synchronizeEnd();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xTriggerableEvent::signal()
{
	#ifdef XTK_OS_WINDOWS
		::SetEvent((HANDLE)m_data->win_event);

	#elif defined(XTK_OS_UNIX)
	
		::pthread_mutex_lock(&(m_data->pt_mutex));
		m_data->m_signaled = true;
		::pthread_cond_broadcast(&(m_data->pt_cond));
		::pthread_mutex_unlock(&(m_data->pt_mutex));
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xTriggerableEvent::reset()
{
	#ifdef XTK_OS_WINDOWS
		::ResetEvent((HANDLE)m_data->win_event);
	#else
		pthread_mutex_lock(&(m_data->pt_mutex));
		m_data->m_signaled = false;
		pthread_mutex_unlock(&(m_data->pt_mutex));
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xTriggerableEvent::wait(int milliseconds_timeout)
{
	#ifdef XTK_OS_WINDOWS
		if(!(::WaitForSingleObject(m_data->win_event,milliseconds_timeout) == WAIT_OBJECT_0))
		{
			return false;
		}

	#else //UNIX	
		
		int	res = 0;
		pthread_mutex_lock(&(m_data->pt_mutex));
		while(!m_data->m_signaled)
		{
			if(milliseconds_timeout != 0)
			{
				struct timespec spec;
				struct timeval current;
				::gettimeofday(&current,NULL);
				spec.tv_sec = current.tv_sec + ((milliseconds_timeout + current.tv_usec / 1000) / 1000);
				spec.tv_nsec = ((current.tv_usec / 1000 + milliseconds_timeout) % 1000) * 1000000;
	
				res = pthread_cond_timedwait(&(m_data->pt_cond), &(m_data->pt_mutex), &spec);
			}
			else
				pthread_cond_wait(&(m_data->pt_cond), &(m_data->pt_mutex));
		}
		pthread_mutex_unlock(&(m_data->pt_mutex));
		
		if(res == ETIMEDOUT)
			return false;
	#endif
	
	return true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xTriggerableEvent::equals(xTriggerableEvent& obj)
{
	return obj.m_data == m_data;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xTriggerableEvent::equals(xObject& obj)
{
	xTriggerableEvent* evt = dynamic_cast<xTriggerableEvent*>(&obj);
	if(evt == NULL)
		return false;
	
	return equals(*evt);
}
	
	
	
}//namespace

#endif //#ifdef XTK_USE_MULTITHREAD
