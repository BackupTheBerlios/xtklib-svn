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
* @file system.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_SYSTEM_H
#define XTK_SYSTEM_H

#include "../globals.h"
#include "charstream.h"

namespace xtk
{

/**
 * The xSystem class contains useful methods for manipulating the underlying system and obtain
 * useful informations about it. 
 * It cannot be instantiated.
 */
class XTKAPI xSystem
{
private:
	class xSTDWriter : public xWriter
	{
	public:
		
		virtual void writeChar(xchar c) throw(xIOException);
		virtual void write(xString s) throw(xIOException);
	};
	
	static xSTDWriter	s_stdo;
	
	xSystem(){}
public:
	/**
	* Retrieve the value for a named environment variable.
	* Return true if the given variable exists and could be get.
	*/
	static bool getEnv(xString name,OUT xString& value);

	/**
	 * Return the current working directory.
	 */
	static xString getWorkingDir();
	
	/**
	 * Return a xWriter associated with the standard output stream.Thread Safe.
	 */
	static xWriter& getStdout(){return s_stdo;}
	
	/**
	 * Get the temp directory of current user.
	 */
	static xString getTempDir();

	/**
	 * Get the home directory of current user.
	 */
	static xString getHomeDir();
	
	/**
	 * Returns the total physic memory installed in the system(in Megabyte).
	 *
	 * @todo to implement in unix
	 */
	int getTotalMemorySize();
	
	/**
	* Returns the free physic memory in the system(in Megabyte).
	*
	* @todo to implement in unix
	*/
	int getAvailableMemorySize();
};
	
	
}//namespace
#endif //XTK_SYSTEM_H
