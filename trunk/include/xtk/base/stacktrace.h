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
* @file stacktrace.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#ifndef XTK_STACKTRACE_H
#define XTK_STACKTRACE_H

#include "../globals.h"
#include "string.h"


namespace xtk
{

/**
 * Represent a single stack frame.
 */
class XTKAPI xStackFrame : public virtual xObject
{
friend class xStackFrameInternal;
friend class xStackTrace;

private:

	void*		m_address;
	xString		m_name;
	int			m_level;
	xString		m_module;
	
	
public:
	xStackFrame()
	{
		m_address = NULL;
		m_level = -1;
		
	}
	
	virtual ~xStackFrame(){}
	
	void* getAddress(){return m_address;}
	xString getName(){return m_name;}
	int getLevel(){return m_level;}
	xString getModule(){return m_module;}
};


/**
 * Contains methods for manipulation of the stack.
 * This class works correctly only on Windows+VC and Unix+RecentLibc+GCC.
 */
class XTKAPI xStackTrace : public virtual xObject
{
private:
	xStackTrace(){}

public:
	
	/**
	 * Retrieve the current stack trace as an array of xStackFrames.
	 * The method can fail silently by returning an empty array.
	 */
	static xArray<xStackFrame*> getCurrentStackTrace(size_t skip = 0);
};


}//namespace

#endif //XTK_STACKTRACE_H
