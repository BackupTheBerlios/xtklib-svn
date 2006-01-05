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
* @file time.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/base/time.h"
#include <wchar.h>

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xTime::xTime()
{
	now();
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xTime::xTime(const xTime& copy)
{
	(*this) = copy;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xTime::xTime(int year,int month,int day,int hour,int minute,int second) throw(xIllegalArgumentException)
{
	set(year,month,day,hour,minute,second);	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xTime::now()
{
	setTime(::time(NULL));
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xTime::format(xString format)
{
	int formatBaseSize = 16;
	xchar* buffer;
	size_t bufferSize;
	for(int i = 1;;i++)
	{
		bufferSize = format.size() + (formatBaseSize * i);
		buffer = (xchar*) malloc(bufferSize * sizeof(xchar));
		buffer[0] = _T('\1');
		
		#ifndef XTK_UNICODE
			size_t ret = strftime(buffer,bufferSize,format.c_str(),&m_time);
		#else
			size_t ret = wcsftime(buffer,bufferSize,format.c_str(),&m_time);
		#endif
		
		if(((ret < bufferSize) && ret != 0) || (ret == 0 && buffer[0] == _T('\0')))
		{
			xString s(buffer,ret);
			free((void*) buffer);
			return s;
		}
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xTime::setYear(int year) throw(xIllegalArgumentException)
{
	m_time.tm_year = year - 1900;
	::mktime(&m_time);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xTime::setMonth(int month) throw(xIllegalArgumentException)
{
	if(month <= 0 || month > 12)
		throw xIllegalArgumentException(_T("Month must be an integer between 1-12"));
		
	m_time.tm_mon = month - 1;
	::mktime(&m_time);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xTime::setDay(int day) throw(xIllegalArgumentException)
{
	if(day <= 0 || day > 31)
		throw xIllegalArgumentException(_T("Day must be an integer between 1-31"));
		
	m_time.tm_mday = day;
	::mktime(&m_time);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xTime::setHour(int hour) throw(xIllegalArgumentException)
{
	if(hour < 0 || hour > 23)
		throw xIllegalArgumentException(_T("Hour must be an integer between 0-23"));
		
	m_time.tm_hour = hour;
	::mktime(&m_time);
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xTime::setMinute(int minute) throw(xIllegalArgumentException)
{
	if(minute < 0 || minute > 59)
		throw xIllegalArgumentException(_T("Minute must be an integer between 0-59"));
		
	m_time.tm_min = minute;
	::mktime(&m_time);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xTime::setSecond(int second) throw(xIllegalArgumentException)
{
	if(second < 0 || second > 59)
		throw xIllegalArgumentException(_T("Second must be an integer between 0-59"));
		
	m_time.tm_sec = second;
	::mktime(&m_time);
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xTime::set(int year,int month,int day,int hour,int minute,int second) throw(xIllegalArgumentException)
{
	if(month <= 0 || month > 12)
		throw xIllegalArgumentException(_T("Month must be an integer between 1-12"));
	if(day <= 0 || day > 31)
		throw xIllegalArgumentException(_T("Day must be an integer between 1-31"));
	if(hour < 0 || hour > 23)
		throw xIllegalArgumentException(_T("Hour must be an integer between 0-23"));
	if(minute < 0 || minute > 59)
		throw xIllegalArgumentException(_T("Minute must be an integer between 0-59"));
	if(second < 0 || second > 59)
		throw xIllegalArgumentException(_T("Second must be an integer between 0-59"));

	m_time.tm_year = year - 1900;
	m_time.tm_mon = month - 1;
	m_time.tm_mday = day;
	m_time.tm_hour = hour;
	m_time.tm_min = minute;
	m_time.tm_sec = second;

	::mktime(&m_time);
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
time_t xTime::getTime()
{
	return mktime(&m_time);	
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xTime::setTime(time_t time)
{
	memcpy(&m_time,::localtime(&time),sizeof(struct tm));
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xTime::compareTo(xTime& time)
{
	time_t diff = mktime(&m_time) - mktime(&(time.m_time));
	if(diff > 0)
		return 1;
	if(diff < 0)
		return -1;
	
	return 0;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xTime::compareTo(xObject& obj)
{
	xTime* comp = dynamic_cast<xTime*>(&obj);
	if(comp == NULL)
		throw xClassCastException(_T("Can compare only against another xTime object"));
	
	return compareTo(*comp);
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xTime::equals(xObject& obj)
{
	xTime* eq = dynamic_cast<xTime*>(&obj);
	if(eq == NULL)
		return false;
		
	return compareTo(*eq) == 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject* xTime::clone()
{
	xTime* t = new xTime();
	(*t) = (*this);
	return t;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xTime::toString()
{
	return format(_T("%c"));
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xTime::hashCode()
{
	return (int) mktime(&m_time);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xTime& xTime::operator=(const xTime& copy)
{
	memcpy(&m_time,&(copy.m_time),sizeof(struct tm));
	return *this;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xTime::operator==(const xTime& copy)
{
	return equals((xTime&)copy);	
}


}//namespace
