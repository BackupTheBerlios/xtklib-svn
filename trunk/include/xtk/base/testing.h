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
* @file testing.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_TESTING_H
#define XTK_TESTING_H


#include "../globals.h"
#include "exception.h"
#include "system.h"
#include <stdio.h>
#include <time.h>


namespace xtk
{
	
/**
* Class used to measure the performance of your code.
*
* To use properly this class, you should call startSession() on the beginning of 
* the code you would like to test, and call endSession() at the end. To have a more
* accurate measure of the performance you may want to record the execution time
* after many iteration of the code, for this scope you can use the endSession(long)
* function that record the performance after a given number of iterations.
* A lower time value is the result of a faster code.
*/ 
class XTKAPI xProfiler : public virtual xObject
{
protected:
	clock_t startTicks;
	clock_t sumTicks;
	int 	count;

public:
	xProfiler();
	
	/**
	* Call this at the beginning of your profiled code
	*/
	void startSession();

	/**
	* Call this at the end of your profiled code
	*
	* @param iter_number [in] the function return a valid value after iter_number iteration.
	* @return if iter_number is reached return the sum of the time that your 
	* code has spent to execute, of every iteration.If iter_number is not reached the
	* return is 0. Note that for time is intended processor time.
	*/
	long endSession(int iter_number);

	/**
	* Call this at the end of your profiled code
	*
	* @return the time that your code has spent to execute, of every iteration.Note that 
	* for time is intended  processor time
	*/
	long endSession();
};


/**
 * This class defines the interface for the test logger.
 */
class XTKAPI xTestLogger
{
public:
	virtual void logTestStart(xString name) = 0;
	virtual void logTestRun(int testNumber,xString description) = 0;
	virtual void logTestOK() = 0;
	virtual void logTestFail() = 0;
	virtual void logTestFail(xString error) = 0;
	virtual void logTestFail(xException& ex) = 0;
	virtual void logTest(xString name) = 0;
	virtual void logProfileStart(int testNumber,xString description) = 0;
	virtual void logProfileEnd(long result) = 0;
};


/**
 * A standard logger for the test framework, it will log to the standard IO
 */
class XTKAPI xStandardTestLogger : public xTestLogger
{
public:
	virtual void logTestStart(xString name)
	{
		xSystem::getStdout().write(xString::getFormat(_T("\n\n----- Starting test suite: %s -----\n\n"),name.c_str()));
	}

	virtual void logTestRun(int testNumber,xString description)
	{
		if(!description.isEmpty())
			xSystem::getStdout().write(xString::getFormat(_T("%d)Running test [%s]..."),testNumber,description.c_str()));
		else
			xSystem::getStdout().write(xString::getFormat(_T("%d)Running test [NO DESCRIPTION]..."),testNumber));
	}
	
	virtual void logTestOK()
	{
		xSystem::getStdout().write(xString::getFormat(_T(" ...OK\n")));
	}
	
	virtual void logTestFail()
	{
		xSystem::getStdout().write(_T(" ...Failed\n"));
	}
	
	virtual void logTestFail(xString error)
	{
		xSystem::getStdout().write(xString::getFormat(_T(" ...Failed: %s\n"),error.c_str()));
	}

	virtual void logTestFail(xException& ex)
	{
		xSystem::getStdout().write(xString::getFormat(_T(" ...Failed: Uncached exception %s,%s\nStack trace:\n"),
			ex.getClassString().c_str(),ex.getDescription().c_str()));
		ex.printStackTrace(xSystem::getStdout());
	}
	
	virtual void logTest(xString name)
	{
		xSystem::getStdout().write(xString::getFormat(_T(" {%s}"),name.c_str()));
	}
	
	virtual void logProfileStart(int testNumber,xString description)
	{
		if(!description.isEmpty())
			xSystem::getStdout().write(xString::getFormat(_T("%d)Starting profile [%s]..."),testNumber,description.c_str()));
		else
			xSystem::getStdout().write(xString::getFormat(_T("%d)Starting profile [NO DESCRIPTION]..."),testNumber));
	}
	
	virtual void logProfileEnd(long result)
	{
		xSystem::getStdout().write(xString::getFormat(_T(" ...Result: %li\n"),result));
	}
};


/**
 * A class for execute unit testing. For using it you must inherit it in yor class
 * and override the virtual function doTest(), here you must insert all your test code.
 */
class XTKAPI xTest : public virtual xObject
{
protected:
	int m_testNumber;
	xTestLogger* m_logger;
	xString m_name;
	
	enum TestResult
	{
		TEST_OK,
		TEST_FAIL,
	};
	
	/**
	 * Override this. Insert here all your test code.
	 */
	virtual void doTest() = 0;

protected:
	/**
	 * Ensure that two xObjects are equals.
	 */
	void ensureObjectEquals(xObject& o1,xObject& o2,xString description = _T(""))
	{
		m_logger->logTestRun(m_testNumber++,description);
		if(!o1.equals(o2))
			throw TEST_FAIL;
		else
			m_logger->logTestOK();
	}
	
	/**
	 * Ensure that two specified values are equals
	 */
	template<class Value>
	void ensureValueEquals(Value& v1,Value& v2,xString description = _T(""))
	{
		m_logger->logTestRun(m_testNumber++,description);
		if(v1 != v2)
			throw TEST_FAIL;
		else
			m_logger->logTestOK();
	}
	
	/**
	 * Ensure that the given expression it's true.
	 */
	void ensureTrue(bool value,xString description = _T(""))
	{
		m_logger->logTestRun(m_testNumber++,description);
		if(!value)
			throw TEST_FAIL;
		else
			m_logger->logTestOK();	
	}
	
	
	
	/**
	 * Ensure that the given expression will throw the specified exception.
	 */
	#define ensureException(expression,exception,description) \
		m_logger->logTestRun(m_testNumber++,description);\
		try{expression}\
		catch(exception& e){m_logger->logTestOK(); return;}\
		throw TEST_FAIL;
	
	/**
	 * To indicate that the last test failed
	 */
	void testFail()
	{
		throw TEST_FAIL;
	}
	
	/**
	 * To indicate that the last test was successfull
	 */
	void testOK()
	{
		m_logger->logTestOK();
	}
	
	/**
	 * Log additional informations about the last started test
	 */
	void testLog(xString name)
	{
		m_logger->logTest(name);
	}
	
	/**
	 * Indicate the starting of a test
	 */
	void testManualStart(xString description)
	{
		m_logger->logTestRun(m_testNumber++,description);
	}
	
	/**
	 * Use this macro to execute a profile of the given expression
	 */
	#define profile(expression,iteration,description)\
		{\
		m_logger->logProfileStart(m_testNumber++,description);\
		xProfiler prof;\
		prof.startSession();\
		for(register int i = 0; i < iteration;i++)\
			expression\
		m_logger->logProfileEnd(prof.endSession());\
		}
	
public:


	/**
	 * Default constructor. You must provide the name of the test and optionally
	 * the class used to log the informations about the tests.
	 */
	xTest(xString name,xTestLogger* logger = NULL)
	{
		if(logger == NULL)
			m_logger = new xStandardTestLogger();
		else
			m_logger = logger;
			
		m_testNumber = 1;
		m_name = name;
	}
	
	/**
	 * Destructor.
	 */
	~xTest()
	{
		delete m_logger;
	}
	
	/**
	 * This method will start the execution of the test.
	 */
	void startTest()
	{
		try
		{
			m_logger->logTestStart(m_name);
			doTest();
		}
		catch(xException& ex)
		{
			m_logger->logTestFail(ex);
		}
		catch(TestResult tr)
		{
			if(tr == TEST_FAIL)
				m_logger->logTestFail();
		}
		//catch(...)
		//{
		//	m_logger->logTestFail("Unknown exception type");
		//}
	}
};



}//namespace


#endif /*XTK_TESTING_H*/
