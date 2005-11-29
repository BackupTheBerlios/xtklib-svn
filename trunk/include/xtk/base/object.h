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
* @file object.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_OBJECT_H
#define XTK_OBJECT_H

#ifndef XTK_GLOBALS_H
#	error "Must include ../globals.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

#ifdef XTK_OS_WINDOWS
    #include <windows.h>
    #include <process.h>
    typedef HANDLE xgc_mutex_id;

#else //if UNIX
    #include <pthread.h>
	#include <errno.h>
    typedef pthread_mutex_t xgc_mutex_id;

#endif
extern "C"
{
	int checkLibFunc();
}

namespace xtk
{

#ifdef XTK_USE_MEMCHECK
	extern xAvlMemTrace g_memoryTrace;
#endif

class xMutex;
class xString;

/**
 * @brief The super class for all xtklib objects
 */
class XTKAPI xObject
{
private:
	#ifdef XTK_USE_MULTITHREAD
		#ifdef XTK_OS_WINDOWS
		    HANDLE m_mutex_id;
		#else //if UNIX
		    pthread_mutex_t m_mutex_id;
		#endif
		
		bool m_bMutexCreated;
		void createMutex();
	#endif

public:	
	
	xObject();
	
	virtual ~xObject();
	
	/**
	 * The equals method implementation for class Object is the following: 
	 * for any non-null reference values x and y, this method returns true 
	 * if and only if x and y refer to the same object (x == y has the value 
	 * true).
	 */
	virtual bool equals(xObject& obj)
	{
		return (&obj == this);
	}
	
	/**
	 * Return a clone of this object. The xObject's clone() function will throw
	 * a xCloneNotSupportedException
	 * 
	 * @exception xCloneNotSupportedException
	 */
	virtual xObject* clone();
	
	/**
	 * Return a string representation of this object. The default xObject's
	 * toString() function will return a string containing the address of the 
	 * object.
	 */
	virtual xString toString();
	
	/**
	 * Return a hashcode for this object
	 */
	virtual int hashCode()
	{
		#ifdef XTK_COMP_VISUAL_C
			#pragma warning( disable : 4311 ) 
		#endif

		return (int)(ptr_t)this ^ (int)((ptr_t)this >> (sizeof(void*) - sizeof(int)));
		
		#ifdef XTK_COMP_VISUAL_C
			#pragma warning( default : 4311 ) 
		#endif
	}
	
	/**
	 * Tells if current object is a NullObject
	 */
	virtual bool isNull()
	{
		return false;
	}
	
	/**
	 * Start a synchronized block for this object. 
	 * Only a thread a time can access a synchronized block of an object.
	 */
	void synchronizeStart()
	{
		#ifdef XTK_USE_MULTITHREAD
			if(!m_bMutexCreated)
				createMutex();
			
			#ifdef XTK_OS_WINDOWS
				::WaitForSingleObject((HANDLE)m_mutex_id, INFINITE);
			#else //POSIX systems
				::pthread_mutex_lock(&m_mutex_id);
			#endif
		#endif
	}
		
	/**
	 * End a synchronized block
	 */
	void synchronizeEnd()
	{
		#ifdef XTK_USE_MULTITHREAD
			if(m_bMutexCreated)
			{
				#ifdef XTK_OS_WINDOWS
					::ReleaseMutex((HANDLE)m_mutex_id);
				#else //POSIX systems
					::pthread_mutex_unlock(&m_mutex_id);
				#endif
			}
		#endif
	}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#ifdef XTK_USE_MEMCHECK
	inline void* operator new(size_t size,const char* file,int line)
	{
		void* ret = xtk_malloc(size);
		g_memoryTrace.put(ret,file,line,size);
		return ret;
	}

	inline void operator delete( void* obj )
	{
		g_memoryTrace.remove(obj);
		xtk_free(obj);
	}

	inline void* operator new[](size_t size,const char* file,int line)
	{
		void* ret = xtk_malloc(size);
		g_memoryTrace.put(ret,file,line,size);
		return ret;
	}

	inline void operator delete[]( void* obj )
	{
		g_memoryTrace.remove(obj);
		xtk_free(obj);
	}
	friend void xDumpMemoryLeaks();
#else //XTK_USE_MEMCHECK

	inline void* operator new(size_t size)
	{return xtk_malloc(size);}

	inline void operator delete(void* obj)
	{xtk_free(obj);}

	inline void* operator new[](size_t size)
	{return xtk_malloc(size);}

	inline void operator delete[](void* obj)
	{xtk_free(obj);}
	
#endif //XTK_USE_MEMCHECK

};//end class xObject



/**
 * A class for representing a NULL object
 */
class XTKAPI xNullObject : public virtual xObject
{
private:
	xNullObject(){}
	static xNullObject nullObject;
	
public:
	virtual bool equals(xObject& obj)
	{
		return obj.isNull();
	}
	
	virtual xString toString();

	virtual bool isNull()
	{
		return true;
	}

	virtual int hashCode()
	{
		return 0;
	}
	
	static xNullObject& getInstance()
	{
		return nullObject;	
	}
};


#ifdef XTK_USE_MEMCHECK
	inline void xStartMemoryTracking()
	{
		xAvlMemTrace::s_startTrackMemory = true;
	}
		
	/**
	 * Dump memory leaks
	 */
	XTKAPI void xDumpMemoryLeaks();

#else
	inline void xDumpMemoryLeaks(){}
	inline void xStartMemoryTracking(){}
#endif


	
}//end namespace


#ifdef XTK_USE_MEMCHECK
	/*
	* This are global overloaded operators. They will not be monitored.
	* 
	*/

	inline void* operator new(size_t size,const char* file,int line)
	{return ::malloc(size);}

	inline void operator delete( void* obj )
	{::free(obj);}

	inline void* operator new[](size_t size,const char* file,int line)
	{return ::malloc(size);}

	inline void operator delete[]( void* obj )
	{::free(obj);}
#endif



#endif //XTK_OBJECT_H
