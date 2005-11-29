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

#include "../../include/xtk/base/thread.h"
#include "../../include/xtk/base/exception.h"
#include "../../include/xtk/base/typewrappers.h"

#ifdef XTK_USE_MULTITHREAD

namespace xtk
{

#ifdef XTK_OS_WINDOWS
	DWORD			gs_tlsSelfThread;
#elif defined(XTK_OS_UNIX)
	pthread_key_t	gs_keySelfThread;
	pthread_t		gs_tidMain;
#endif

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xThread::xThread(xString name,bool detached,xThreadGroup* thread_group)
	: m_threadName(name)
{
	#ifdef XTK_OS_WINDOWS
		m_winThreadIdentifier = 0;
		m_hThread = INVALID_HANDLE_VALUE;
	#elif defined(XTK_OS_UNIX)
		memset(&m_pthread,0,sizeof(m_pthread));
	#endif

	m_bIsRunning = false;
	m_threadGroup = thread_group;
	m_bDetached = detached;
	m_uncaughtException = NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xThread::~xThread()
{	
	if(m_uncaughtException != NULL)
		delete m_uncaughtException;

	if(getSelfThread() != this)
		terminate();

	#ifdef XTK_OS_WINDOWS
		::CloseHandle(m_hThread);
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#ifdef XTK_OS_WINDOWS
	DWORD WINAPI xThread::Execute(void* arg)
	{
		xThread* t = static_cast<xThread*>(arg);
		t->threadStarted();

		t->threadRunning();

		t->threadTerminated();

		return 1;
	}

#else
	void* xThread::Execute(void* arg)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		xThread* t = static_cast<xThread*>(arg);
		t->threadStarted();

		t->threadRunning();

		t->m_threadJoinEvt.signal();
		t->threadTerminated();

		pthread_exit(NULL);
	}

#endif

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThread::threadRunning()
{
	try
	{
		run();
	}
	catch(xException& exc)
	{
		xSynchronized s(*this);
		m_uncaughtException = dynamic_cast<xException*>(exc.clone());
	}
	catch(...)
	{
		xSynchronized s(*this);
		m_uncaughtException = new xUnknownException();
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThread::threadStarted()
{
	//set current tls/tsd
	#ifdef XTK_OS_WINDOWS
		::TlsSetValue(gs_tlsSelfThread,this);
	#elif defined(XTK_OS_UNIX)
		::pthread_setspecific(gs_keySelfThread,this);
	#endif

	if(m_threadGroup != NULL)
		m_threadGroup->addThread(this);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThread::threadTerminated()
{

	if(m_threadGroup != NULL && (m_threadGroup->isAutoCleaner() || isDetached()))
	{
		m_threadGroup->delThread(this);
		delete this;
	}
	else if(isDetached())
	{
		delete this;
	}
	else
	{	
		synchronizeStart();
		m_bIsRunning = false;
		synchronizeEnd();
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThread::start() throw (xIllegalStateException, xSystemException)
{
	xSynchronized s(*this);

	//reset exception
	if(m_uncaughtException != NULL)
		delete m_uncaughtException;
	m_uncaughtException = NULL;

	if(m_bIsRunning)
		throw xIllegalStateException(_T("Thread already started"));

	#ifdef XTK_OS_WINDOWS
		m_hThread = ::CreateThread(NULL,0,(exec_t) &Execute,(void*)this,0,&m_winThreadIdentifier);
		if(m_hThread == NULL)
		{
			m_bIsRunning = false;
			throw xSystemException(
				xString::getFormat(_T("Error starting thread: %s"),m_threadName.c_str()),
				::GetLastError()
				);
		}
		m_bIsRunning = true;

	#else
		int res;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		res = pthread_create(&m_pthread ,&attr ,(exec_t) &Execute,(void*) this);
		pthread_attr_destroy(&attr);
		if(res != 0)
		{
			m_bIsRunning = false;
			throw xSystemException(
				xString::getFormat(_T("Error starting thread: %s"),m_threadName.c_str()),
				errno
				);
		}
		m_bIsRunning = true;

	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThread::join() throw (xIllegalOperationException)
{
	if(isDetached())
		throw xIllegalOperationException(_T("Cannot join a detached thread"));

	#ifdef XTK_OS_WINDOWS
		::WaitForSingleObject(m_hThread, INFINITE);

	#else //POSIX systems
		m_threadJoinEvt.wait();

	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThread::terminate()
{

	#ifdef XTK_OS_WINDOWS
		::TerminateThread(m_hThread,0);

	#else //POSIX systems
		::pthread_cancel(m_pthread);

	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xThread::equals(xObject& obj)
{
	xThread* thread = dynamic_cast<xThread*>(&obj);
	if(thread == NULL)
		return false;

	#ifdef XTK_OS_WINDOWS
		return m_winThreadIdentifier == thread->m_winThreadIdentifier;

	#else //POSIX systems
		return ::pthread_equal(m_pthread,thread->m_pthread) != 0;

	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xThread::hashCode()
{
	#ifdef XTK_OS_WINDOWS
		return (int) m_winThreadIdentifier;
	#else
		return (int) m_pthread;
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xThread::toString()
{
	return xString::getFormat(_T("Thread name: %s"),m_threadName.c_str());	
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThread::initialize()
{

	#ifdef XTK_OS_WINDOWS
		// allocate TLS index for storing the pointer to the current thread
		gs_tlsSelfThread = ::TlsAlloc();

		//Main thread has associated no xThread object
		::TlsSetValue(gs_tlsSelfThread, (LPVOID)0);

	#elif defined(XTK_OS_UNIX)
		::pthread_key_create(&gs_keySelfThread,NULL);
		gs_tidMain = pthread_self();

	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThread::finalize()
{
	#ifdef XTK_OS_WINDOWS
		::TlsFree(gs_tlsSelfThread);

	#elif defined(XTK_OS_UNIX)
		::pthread_key_delete(gs_keySelfThread);

	#endif
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThread::sleep(int msec)
{
	#ifdef XTK_OS_WINDOWS
		DWORD lmsec = (DWORD)msec;
		::Sleep(lmsec);
     
	#else //POSIX systems
		timespec tmReq;
	    tmReq.tv_sec = (time_t)(msec / 1000);
	    tmReq.tv_nsec = (msec % 1000) * 1000 * 1000;
	    
	    ::nanosleep(&tmReq, (timespec *)NULL);
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xThread::yield()
{
	#ifdef XTK_OS_WINDOWS
		::Sleep(1);
	#else
		::pthread_yield();
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
NODELETE xThread* xThread::getSelfThread()
{
	#ifdef XTK_OS_WINDOWS
		void* vt = ::TlsGetValue(gs_tlsSelfThread);
		if(vt == NULL)
			return NULL;
			
	#elif defined(XTK_OS_UNIX)
		if(::pthread_equal(pthread_self(),gs_tidMain));
			return NULL;
		void* vt = ::pthread_getspecific(gs_keySelfThread);
		
	#endif
	
	return (xThread*)(vt);
}


}//namespace

#endif //#ifdef XTK_USE_MULTITHREAD

