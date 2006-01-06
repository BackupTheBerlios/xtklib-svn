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
* @file exception.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_EXCEPTION_H
#define XTK_EXCEPTION_H


#include "../globals.h"
#include <typeinfo>
#include "string.h"
//#include "smartptr.h"
//#include <stdarg.h>

namespace xtk
{

template<class TClass> class xArray;
class xStackFrame;
class xWriter;


#ifdef XTK_OS_WINDOWS
	typedef DWORD sys_error_code_t;
#elif defined(XTK_OS_UNIX)
	typedef int sys_error_code_t;
#endif


inline sys_error_code_t getSysErrorCode()
{
	#ifdef XTK_OS_WINDOWS
		return GetLastError();
	#elif defined(XTK_OS_UNIX)
		return errno;
	#endif
}

/**
 * @brief The superclass for all exceptions in xtklib
 */
class XTKAPI xException : public virtual xObject
{
protected:
	xString					m_description;
	xException* 			m_cause;
	xArray<xStackFrame*>*	m_stackTrace;
	sys_error_code_t		m_errorSysCode;

	void captureStackTrace();
public:
	virtual ~xException();
	
	xException()
	{
		m_cause = NULL;
		m_stackTrace = NULL;
		captureStackTrace();
		m_errorSysCode = 0;
	}

	xException(const xException& copy);
	
	/**
	 * Constructor.
	 *
	 * @param description A textual description of the exception
	 * @param errSysCode An error code as returned by the underlying OS
	 * (GetLastError() in Windows and errno in Unix)
	 */
	xException(const xString& description,sys_error_code_t errSysCode)
	{
		m_description = description;
		m_cause = NULL;
		m_stackTrace = NULL;
		m_errorSysCode = errSysCode;
		captureStackTrace();
	}
	
	/**
	 * Constructor.
	 *
	 * @param description A textual description of the exception
	 * @param cause The xException that caused this exception(if available)
	 */
	xException(const xString& description,xException* cause = NULL)
	{
		m_description = description;
		m_cause = cause;
		m_stackTrace = NULL;
		m_errorSysCode = 0;
		captureStackTrace();
	}
	
	/**
	* Print the exception backtrace to the specified character stream
	*/
	void printStackTrace(xWriter& wr);
	
	/**
	 * Return a description of the exception
	 */
	virtual xString& getDescription(){return m_description;}
	
	/**
	 * Return the cause that have raised this exception
	 */
	virtual xException& getCause(){return *m_cause;}
	
	/**
	 * Return a string representing the type of the exception,override this
	 * function when implementing your own exception
	 */
	virtual xString getType()
	{
		#ifndef XTK_UNICODE
			return typeid(*this).name();
		#else
			const char* t = typeid(*this).name();
			return xString(t,xCharset::CS_SYSTEM);
		#endif
		//return _T("");
	}
	
	/**
	 * Returns the error code associated with this exception provided by the underlying OS.
	 */
	virtual sys_error_code_t getSysErrorCode(){return m_errorSysCode;}
	
	/**
	 * Maps the system error code associated with the current exception to a string describing
	 * such error.
	 */
	virtual xString getSysErrorCodeDescription();
	
	virtual xObject* clone()
	{
		return new xException(*this);
	}
	
	virtual xString toString()
	{
		return xString::getFormat(_T("%s: %s"),getType().c_str(),getDescription().c_str());
	}
};


/**
 * @brief Represent an exception in I/O operations
 */
class XTKAPI xIOException : public xException
{
public:
	virtual ~xIOException(){}

	xIOException(){}
	xIOException(const xString& description,sys_error_code_t errSysCode) 
	: xException(description,errSysCode){}
	xIOException(const xIOException& copy) : xException(copy){}
	xIOException(xString description,xException* cause = NULL)
	: xException(description,cause){}
	
	virtual xObject* clone()
	{
		return new xIOException(*this);
	}
};

/**
 * @brief Thrown to indicate that a method has been passed an illegal or 
 * inappropriate argument.
 */
class XTKAPI xIllegalArgumentException : public xException
{
protected:
	
public:
	virtual ~xIllegalArgumentException(){}

	xIllegalArgumentException(){}
	xIllegalArgumentException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xIllegalArgumentException(const xIllegalArgumentException& copy) : xException(copy){}
	xIllegalArgumentException(xString description,xException* cause = NULL)
		: xException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xIllegalArgumentException(*this);
	}
};

/**
 * @brief Signals that a method has been invoked at an illegal or inappropriate 
 * time.
 */
class XTKAPI xIllegalStateException : public xException
{
protected:
	
public:
	virtual ~xIllegalStateException(){}

	xIllegalStateException(){}
	xIllegalStateException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xIllegalStateException(const xIllegalStateException& copy) : xException(copy){}
	xIllegalStateException(xString description,xException* cause = NULL)
	: xException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xIllegalStateException(*this);
	}
};

/**
 * @brief Thrown to indicate that an index of some sort is out of allowed range.
 */
class XTKAPI xIndexOutOfBoundsException : public xException
{
public:
	virtual ~xIndexOutOfBoundsException(){}

	xIndexOutOfBoundsException(){}
	xIndexOutOfBoundsException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xIndexOutOfBoundsException(const xIndexOutOfBoundsException& copy) : xException(copy){}
	xIndexOutOfBoundsException(xString description,xException* cause = NULL)
	: xException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xIndexOutOfBoundsException(*this);
	}
};

/**
 * @brief Thrown to indicate that system memory is not sufficient to allocate
 * more space
 */
class XTKAPI xOutOfMemoryException : public xException
{
public:
	virtual ~xOutOfMemoryException(){}

	xOutOfMemoryException(){}
	xOutOfMemoryException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xOutOfMemoryException(const xOutOfMemoryException& copy) : xException(copy){}
	xOutOfMemoryException(xString description,xException* cause = NULL)
	: xException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xOutOfMemoryException(*this);
	}
};


/**
 * @brief Thrown to indicate that it's trying to use a
 *  functionality (a function,a class,...)that is not currently implemented
 */
class XTKAPI xNotImplementedException : public xException
{
public:
	virtual ~xNotImplementedException(){}

	xNotImplementedException(){}
	xNotImplementedException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xNotImplementedException(const xNotImplementedException& copy) : xException(copy){}
	xNotImplementedException(xString description,xException* cause = NULL)
	: xException(description,cause)
	{}

	
	virtual xObject* clone()
	{
		return new xNotImplementedException(*this);
	}
};


/**
 * @brief Thrown 
 */
class XTKAPI xClassCastException : public xException
{
public:
	virtual ~xClassCastException(){}

	xClassCastException(){}
	xClassCastException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xClassCastException(const xClassCastException& copy) : xException(copy){}
	xClassCastException(xString description,xException* cause = NULL)
	: xException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xClassCastException(*this);
	}
};


/**
 * @brief Thrown to indicate that the underlying operating system has failed
 * to execute the requested operation due to an unexpected error.
 */
class XTKAPI xSystemException : public xException
{
public:
	virtual ~xSystemException(){}

	xSystemException(){}
	xSystemException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xSystemException(const xSystemException& copy) : xException(copy){}
	xSystemException(xString description,xException* cause = NULL)
	: xException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xSystemException(*this);
	}
};

/**
 * @brief Thrown to indicate that has been caugth an exception that cannot
 * be identified.
 */
class XTKAPI xUnknownException : public xException
{
public:
	virtual ~xUnknownException(){}

	xUnknownException(){}
	xUnknownException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xUnknownException(const xUnknownException& copy) : xException(copy){}
	xUnknownException(xString description,xException* cause = NULL)
	: xException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xUnknownException(*this);
	}
};

/**
 * @brief Thrown to indicate that a NULL pointer is encountered in a place
 * that it cannot be managed.
 */
class XTKAPI xNullPointerException : public xException
{
public:
	virtual ~xNullPointerException(){}

	xNullPointerException(){}
	xNullPointerException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xNullPointerException(const xNullPointerException& copy) : xException(copy){}
	xNullPointerException(xString description,xException* cause = NULL)
	: xException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xNullPointerException(*this);
	}
};


/**
 * @brief Thrown to indicate that a the requested element does not exists
 * or is not available.
 */
class XTKAPI xNoSuchElementException : public xException
{
public:
	virtual ~xNoSuchElementException(){}

	xNoSuchElementException(){}
	xNoSuchElementException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xNoSuchElementException(const xNoSuchElementException& copy) : xException(copy){}
	xNoSuchElementException(xString description,xException* cause = NULL)
	: xException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xNoSuchElementException(*this);
	}
};

/**
 * @brief Thrown to indicate that the IP address of a host could not be 
 * determined.
 */
class XTKAPI xUnknownHostException : public xException
{
public:
	virtual ~xUnknownHostException(){}

	xUnknownHostException(){}
	xUnknownHostException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xUnknownHostException(const xUnknownHostException& copy) : xException(copy){}
	xUnknownHostException(xString description,xException* cause = NULL)
	: xException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xUnknownHostException(*this);
	}
};


/**
 * 
 */
class XTKAPI xCloneNotSupportedException : public xException
{
public:
	virtual ~xCloneNotSupportedException(){}

	xCloneNotSupportedException(){}
	xCloneNotSupportedException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xCloneNotSupportedException(const xCloneNotSupportedException& copy) : xException(copy){}	
	xCloneNotSupportedException(xString description,xException* cause = NULL)
	: xException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xCloneNotSupportedException(*this);
	}
};


/**
 *
 */
class XTKAPI xIllegalOperationException : public xException
{
protected:
	
public:
	virtual ~xIllegalOperationException(){}

	xIllegalOperationException(){}
	xIllegalOperationException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xIllegalOperationException(const xIllegalOperationException& copy) : xException(copy){}	
	xIllegalOperationException(xString description,xException* cause = NULL)
	: xException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xIllegalOperationException(*this);
	}
};


/**
 * 
 */
class XTKAPI xEOFException : public xIOException
{
protected:
	
public:
	virtual ~xEOFException(){}

	xEOFException(){}
	xEOFException(const xString& description,sys_error_code_t errSysCode) 
		: xIOException(description,errSysCode){}
	xEOFException(const xEOFException& copy) : xIOException(copy){}	
	xEOFException(xString description,xException* cause = NULL)
	: xIOException(description,cause)
	{}
	
	
	virtual xObject* clone()
	{
		return new xEOFException(*this);
	}
};


/**
 * 
 * 
 */
class XTKAPI xFileNotFoundException : public xIOException
{
protected:
	
public:
	virtual ~xFileNotFoundException(){}

	xFileNotFoundException(){}
	xFileNotFoundException(const xString& description,sys_error_code_t errSysCode) 
		: xIOException(description,errSysCode){}
	xFileNotFoundException(const xFileNotFoundException& copy) : xIOException(copy){}	
	xFileNotFoundException(xString description,xException* cause = NULL)
	: xIOException(description,cause)
	{}
	
	virtual xObject* clone()
	{
		return new xFileNotFoundException(*this);
	}
};


/**
* Thrown to indicate that the requested data is not available.
* 
*/
class XTKAPI xNotAvailableDataException : public xException
{
protected:

public:
	virtual ~xNotAvailableDataException(){}

	xNotAvailableDataException(){}
	xNotAvailableDataException(const xString& description,sys_error_code_t errSysCode) 
		: xException(description,errSysCode){}
	xNotAvailableDataException(const xNotAvailableDataException& copy) : xException(copy){}
	xNotAvailableDataException(xString description,xException* cause = NULL)
		: xException(description,cause)
	{}

	virtual xObject* clone()
	{
		return new xNotAvailableDataException(*this);
	}
};

/**
*
* 
*/
class XTKAPI xIOTimeoutException : public xIOException
{
protected:

public:
	virtual ~xIOTimeoutException(){}

	xIOTimeoutException(){}
	xIOTimeoutException(const xString& description,sys_error_code_t errSysCode) 
		: xIOException(description,errSysCode){}
	xIOTimeoutException(const xIOTimeoutException& copy) : xIOException(copy){}
	xIOTimeoutException(xString description,xException* cause = NULL)
		: xIOException(description,cause)
	{}

	virtual xObject* clone()
	{
		return new xIOTimeoutException(*this);
	}
};


/**
 * 
 */
class XTKAPI xDataFormatException : public xIOException
{
protected:

public:
	virtual ~xDataFormatException(){}

	xDataFormatException(){}
	xDataFormatException(const xString& description,sys_error_code_t errSysCode) 
		: xIOException(description,errSysCode){}
	xDataFormatException(const xDataFormatException& copy) : xIOException(copy){}
	xDataFormatException(xString description,xException* cause = NULL)
		: xIOException(description,cause)
	{}

	virtual xObject* clone()
	{
		return new xDataFormatException(*this);
	}
};

}//namespace


#endif //XTK_EXCEPTION_H
