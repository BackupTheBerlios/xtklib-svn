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
* @file time.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_TIME_H
#define XTK_TIME_H

#include "../globals.h"
#include "interfaces.h"
#include "string.h"
#include "exception.h"
#include <time.h>

namespace xtk
{

//GetSystemTime,GetTimeFormat


/**
 * The class xTime represents a specific instant in time, with second precision.
 */
class XTKAPI xTime : public virtual xObject,public virtual xComparable
{
private:
	struct tm m_time;
	
public:
	/**
	 * Initializes the xTime object so that it represents the time at which 
	 * it was allocated
	 */
	xTime();
	
	/**
	 * Copy constructor
	 */
	xTime(const xTime& copy);
	
	/**
	 * Initializes the xTime with the given values.
	 */
	xTime(int year,int month,int day,int hour,int minute,int second) throw(xIllegalArgumentException);
	
	virtual ~xTime(){}
	
	/**
	 * Format the current time in a string representation\n
	 *
	 * The following table lists all conversion specifiers\n
	 * 
	 * %%a    abbreviated weekday name (Sun)\n
	 * %%A    full weekday name (Sunday)\n
	 * %%b    abbreviated month name (Dec)\n
	 * %%B    full month name (December)\n
	 * %%c    date and time (Dec  2 06:55:15 1979)\n
	 * %%d    day of the month (02)\n
	 * %%H    hour of the 24-hour day (06)\n
	 * %%I    hour of the 12-hour day (06)\n
	 * %%j    day of the year, from 001 (335)\n
	 * %%m    month of the year, from 01 (12)\n
	 * %%M    minutes after the hour (55)\n
	 * %%p    AM/PM indicator (AM)\n
	 * %%S    seconds after the minute (15)\n
	 * %%U    Sunday week of the year, from 00 (48)\n
	 * %%w    day of the week, from 0 for Sunday (6)\n
	 * %%W    Monday week of the year, from 00 (47)\n
	 * %%x    date (Dec  2 1979)\n
	 * %%X    time (06:55:15)\n
	 * %%y    year of the century, from 00 (79)\n
	 * %%Y    year (1979)\n
	 * %%Z    time zone name, if any (EST)\n
	 * %%    percent character %\n
	 */
	xString format(xString format);
	
	void now();

	int getYear(){return (m_time.tm_year + 1900);}
	int getMonth(){return (m_time.tm_mon + 1);}
	int getDay(){return (m_time.tm_mday);}
	int getHour(){return (m_time.tm_hour);}
	int getMinute(){return (m_time.tm_min);}
	int getSecond(){return (m_time.tm_sec);}
	
	void setYear(int year) throw(xIllegalArgumentException);
	void setMonth(int month) throw(xIllegalArgumentException);
	void setDay(int day) throw(xIllegalArgumentException);
	void setHour(int hour) throw(xIllegalArgumentException);
	void setMinute(int minute) throw(xIllegalArgumentException);
	void setSecond(int second) throw(xIllegalArgumentException);
	
	void set(int year,int month,int day,int hour,int minute,int second) throw(xIllegalArgumentException);
	
	time_t getTime();
	void setTime(time_t time);
	
	int compareTo(xTime& time);
	virtual int compareTo(xObject& obj);
	virtual bool equals(xObject& obj);
	virtual xObject* clone();
	
	/**
	 * Is equivalent to format("%%c")
	 */
	virtual xString toString();
	virtual int hashCode();
	
	xTime& operator=(const xTime& copy);
	bool operator==(const xTime& copy);
};
	
	
	
	
	
}//namespace


#endif /*XTK_DATE_H*/
