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
* @file thread.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_THREAD_H
#define XTK_THREAD_H

#include "../globals.h"
#include "exception.h"
#include "datastructures.h"

#ifdef XTK_USE_MULTITHREAD

#ifdef XTK_OS_WINDOWS
    #include <windows.h>
    #include <process.h>
    typedef HANDLE threadId;
    typedef HANDLE CrossMutex;
	typedef	DWORD (WINAPI *exec_t)(void*);

#else //if UNIX
    #include <pthread.h>
	#include <errno.h>
    typedef pthread_t threadId;
    typedef pthread_mutex_t CrossMutex;
	typedef	void* (*exec_t)(void*);

#endif


namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
/**
* @brief Represent an object that can be triggered.
*
* It uses shared resources, so you should not care about the order of deletion of various
* objects.
*/
class XTKAPI xTriggerableEvent : public virtual xObject
{
private:
	class xTriggerableEventData : public virtual xObject
	{
	public:
		#ifdef XTK_OS_WINDOWS
				HANDLE win_event;
		#elif defined(XTK_OS_UNIX)
				pthread_mutex_t 	pt_mutex;
				pthread_cond_t 		pt_cond;
				bool 				m_signaled;
		#endif
		
		int m_referenceCount;
	};
	
	xTriggerableEventData*	m_data;
public:
	/**
	* Create a new xTriggerableEvent object,identified by an application-wide 
	* identifier.
	* 
	* @exception xSystemException
	*/
	xTriggerableEvent();
	
	xTriggerableEvent(const xTriggerableEvent& copy);
	
	virtual ~xTriggerableEvent();

	/** 
	* Signal the current xEvent, unlocking anyone waiting on this event
	*/
	void signal();

	/** 
	* Reset this xEvent if is signaled
	*/
	void reset();

	/** 
	* Wait for this xTriggerableEvent to be signaled
	*
	* @param timeout [in] timeout in milliseconds
	* 
	* @return true if the wait ended correctly, false if it ended due to a timeout
	*/
	bool wait(int milliseconds_timeout = 0);
	
	virtual bool equals(xTriggerableEvent& obj);
	virtual bool equals(xObject& obj);
};

#endif //#ifdef XTK_USE_MULTITHREAD

/**
* @brief Made the current scope a critical section.
* 
* You could use this class to allow only one thread to access the current scope.
*/
class XTKAPI xSynchronized
{
private:
	xObject*	m_object;
	
public:
	xSynchronized(xObject& o) : m_object(&o)
	{
		m_object->synchronizeStart();
	}

	~xSynchronized()
	{
		m_object->synchronizeEnd();
	}
};

#ifdef XTK_USE_MULTITHREAD

class xThreadGroup;

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

/** 
* @brief Cross-Platform wrapper class to manage threads
* 
* This is an abstract class that represent a thread.
* To utilize this class you must inherit it in your class
* and overwrite the virtual function run().It is also strongly
* recommended to override the terminate() function.
* xThread class is available only if XTK_USE_MULTITHREAD are defined.
* 
* @todo implements priority system
* @todo signal mask for threads
*/
class XTKAPI xThread : public virtual xObject
{
private:
	xThreadGroup*	 	m_threadGroup;
	bool 				m_bIsRunning;
	xException*			m_uncaughtException;
	bool				m_bDetached;
	xString				m_threadName;

	#ifdef XTK_OS_WINDOWS
		DWORD				m_winThreadIdentifier;
		HANDLE				m_hThread;
		static DWORD WINAPI Execute(void* arg);

	#elif defined( XTK_OS_UNIX )
		//For POSIX threads we use events for joining cause we create
		//them as detached for default
		xTriggerableEvent	m_threadJoinEvt;
		pthread_t			m_pthread;
		static void* Execute(void* arg);

	#endif

	/** 
	* This is the starting point of a new thread.
	* Override this function to implement your own xThread. You should never call
	* directly this function.
	*/
	virtual void run(){}

	void threadStarted();
	void threadTerminated();
	void threadRunning();
public:	
	/** 
	* Create a xThread Object, note that this wont start the execution of thread.
	* There are two types of xThread: DETACHED,UNDETACHED. \n
	* UNDETACHED threads, can be joined and must be deleted manually\n
	* DETACHED threads, will autodelete on their ends and cannot be joined singularly,
	* they only can be joined when added to a xThreadGroup.
	*/
	xThread(xString name = _T(""),bool detached = true,xThreadGroup* thread_group = NULL);

	virtual ~xThread();
	
	/** 
	* Starts a new Thread executing the member function run()
	*
	* @exception xIllegalStateException if the thread is already started
	* @exception xSystemException if it is impossible to start the thread
	*/
	void start() throw(xIllegalStateException,xSystemException);

	/** 
	* Wait until this thread terminates. Not try to join and autodeleting 
	* thread
	* 
	* @exception xIllegalOperationException if the thread is DETACHED
	*/
	void join() throw(xIllegalOperationException);

	/**
	* Force the exiting of this thread. Use this function with attention.
	* You can override this in your class to let you thread to terminate gracefully.
	*/
	virtual void terminate();

	/**
	* 
	*/
	xString& getName(){return m_threadName;}

	/**
	* Tell if the current Thread object is Running
	*/
	bool isRunning(){return m_bIsRunning;}

	/**
	* 
	*/
	xThreadGroup& getThreadGroup(){return *m_threadGroup;}

	/**
	* 
	*/
	bool isTerminatedWithException(){return m_uncaughtException != NULL;}

	/**
	* 
	*/
	xException& getUncaughtException(){return *m_uncaughtException;}

	/**
	* 
	*/
	bool isDetached(){return m_bDetached;}

	virtual int hashCode();
	virtual bool equals(xObject& obj);
	virtual xString toString();
	
	//----------------S T A T I C-------------------------------/
	static void sleep(int milliseconds);
	static void yield();
	
	/**
	 * Return the xThread object associated to the current running thread. 
	 * Return a NULL if the current thread is the main thread.
	 */
	static NODELETE xThread* getSelfThread();
	
	static void initialize();
	static void finalize();
};


/**
 * A thread group represents a set of threads. Only attached threads will be
 * added.
 */
class XTKAPI xThreadGroup : public virtual xObject
{
	friend class xThread;
private:
	xLinkedList				m_threadList;
	bool					m_bAutoclear;
	xThreadGroup*			m_parent;
	xArrayList				m_childs;
	xString					m_name;
	
	void addThread(xThread* thread);
	void delThread(xThread* thread);
	void addChild(xThreadGroup* child);
	void delChild(xThreadGroup* child);
public:

	/**
	 * Create a new xThreadGroup object and optionally specify if it shall
	 * be autocleaner, or in other words if it shall delete automatically
	 * the threads that are terminated. A thread will be added after it is startred.
	 * Remember: detached threads will autodelete however.
	 */
	xThreadGroup(xString name = _T(""),bool autoclear = true,xThreadGroup* parent = &(getMainGroup()));
	
	/**
	 * Desctructor, terminate and join all threads.
	 */
	virtual ~xThreadGroup();
	
	bool isAutoCleaner(){return m_bAutoclear;}
	
	/**
	 * Join all threads in the group and all subgroups
	 */
	void joinAll();
	
	/**
	* Call the terminate() function of every thread of the group and its subgroups.
	*/
	void terminateAll();
	
	/**
	 * If this thread group is autocleaner or if it contains some detached thread
	 * you should use freeze() and unfreeze() function before and after
	 * retrieving an enumeration of the threads.
	 */
	xArray<NODELETE xThread*> enumerate();
	
	/**
	 * Delete all terminated threads from the group
	 */
	void clearInactive();
	
	/**
	 * Return an estimated count of threads in the group.
	 */
	int getCount();
	
	/**
	 * Return an estimated count of running threads.
	 */
	int getRunningCount();
	
	/**
	 * No other thread can be added until unfreeze().
	 */
	void freeze(){synchronizeStart();}
	
	/**
	 * 
	 */
	void unfreeze(){synchronizeEnd();}
	
	xString getName(){return m_name;}
	
	static xThreadGroup& getMainGroup();
};




}//namespace

#endif //XTK_USE_MULTITHREAD

#endif //XTK_THREAD_H
