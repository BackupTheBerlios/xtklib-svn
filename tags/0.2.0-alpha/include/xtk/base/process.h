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
* @file process.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_PROCESS_H
#define XTK_PROCESS_H

#include "../globals.h"
#include "string.h"
#include "stream.h"
#include "thread.h"

namespace xtk
{

#ifdef XTK_OS_WINDOWS
	typedef DWORD	xpid_t;
#elif defined(XTK_OS_UNIX)
	typedef int		xpid_t;
#endif

/**
 * Represent a running or death child process and in addition contains
 * static methods for run processes.
 * The xProcess class is not insatiable directly,but can be obtained 
 * from the static methods execute(...).
 */
class XTKAPI xProcess : public virtual xObject
{
	friend class xProcessWaitThread;
private:
	bool		m_isRunning;
	long			m_exitStatus;
	bool		m_detatched;
	xThread*	m_processWaitThread;
	
	#ifdef XTK_OS_WINDOWS
		HANDLE		m_hProcess;
		HANDLE		m_hReadPipeStdOut;
		HANDLE		m_hReadPipeStdErr;
		HANDLE		m_hWritePipeStdIn;
		DWORD		m_pid;
		
		xProcess(HANDLE hProcess,DWORD pid,HANDLE hWritePipeStdIn,
			HANDLE hReadPipeStdOut,HANDLE hReadPipeStdErr);
	#elif defined(XTK_OS_UNIX)
		int			m_pid;
		int			m_fdReadPipeStdOut;
		int			m_fdReadPipeStdErr;
		int			m_fdWritePipeStdIn;
		
		xProcess(int pid,int fdReadPipeStdOut,int fdReadPipeStdErr,int fdWritePipeStdIn);
	#endif
	
public:

	virtual ~xProcess();
	
	/**
	 * Kill the process represented by this object.
	 */
	bool kill();
	
	/**
	 * Wait until this process terminate its execution.
	 */
	void wait();
	
	/**
	 * Retrieve the exit status of the terminated process.
	 */
	long getExitStatus() throw(xIllegalStateException);
	
	/**
	 * Get a xInputStream which corresponds to the standard output 
	 * of the process.It returns NULL if an error occurs or if redirection was not 
	 * turned on.
	 */
	xInputStream* getStdOut();
	
	/**
	 * Get a xInputStream which corresponds to the standard error output 
	 * of the process.It returns NULL if an error occurs or if redirection was not 
	 * turned on.
	 */
	xInputStream* getStdErr();
	
	/**
	 * Get a xOutputStream which corresponds to the standard input 
	 * of the process.It returns NULL if an error occurs or if redirection was not 
	 * turned on.
	 */
	xOutputStream* getStdIn();
	
	/**
	 * The xProcess object will be automatically deleted when the process exits.
	 * Obviously all operation on current xProcess object should be avoided
	 * because this could lead to memory error or to invalid results.
	 */
	void detatch();
	
	/**
	 * Tells if the current process actually is in execution.
	 */
	bool isRunning(){return m_isRunning;}
	
	
	//-------------STATIC----------------
	
	/**
	 * Execute a child process and returns an object representing that process.
	 */
	static xProcess* execute(xString filename,xArray<xString*> arguments,bool redirect) 
	throw(xSystemException);
	
	/**
	 * 
	 */
	static bool kill(xpid_t pid);
};



}//namespace

#endif //XTK_PROCESS_H
