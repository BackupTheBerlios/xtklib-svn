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
* @file Utilities.h
* @author Mario Casciaro (xshadow@email.it)
*/


#ifndef XTK_UTILITIES_H
#define XTK_UTILITIES_H

#include "../globals.h"
#include <stdarg.h>
#include <time.h>


//----------------------------------------------
//CONSTANTS
//----------------------------------------------
#define XTK_FORMAT_BASE_SIZE		300


namespace xtk
{

/** 
* Initializes xtklib components
*/
void xlibStart();

/** 
* Free xtklib components
*/
void xlibEnd();

/**
 * Return a pseudo random integer
 */
int xlRandomInt();

/** 
* Generates the hexadecimal representation of data
*
* @param rawData [in] data to format
* @param rawData_size [in] size of rawData
* @param formattedString [out] string to fill with formatted data
*/
//void xlFormatHexData(const ui8* rawData,size_t rawData_size,std::string& formattedString);

/**
 * Break into the debugger
 */
void xlTrap();

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------

/**
* @brief Implements some basic timer functios.
* @note this is not a high precision timer!
*/
class XTKAPI xTimer
{
protected:
	bool m_bStarted;
	clock_t m_ticksAtStart;

public:

	xTimer(bool autostart = true);
	~xTimer();

	/**
	* Start the timer
	*/
	void start();

	/**
	* Reset the timer
	*/
	void reset();

	/**
	* Restart the timer
	*/
	void restart();

	/**
	* Get the seconds elapsed from start.If not started return 0.
	*/
	long getSecs();

	/**
	* Get the milliseconds elapsed from start.If not started return 0.
	*/
	long getMsecs();
};

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//----------------------------------------------------------------- 
    
    
}//end namespace


#endif //XTK_UTILITIES_H
