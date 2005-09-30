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
* @file typewrappers.h
* @author Mario Casciaro (xshadow@email.it)
*/


#ifndef XTK_TYPEWRAPPERS_H
#define XTK_TYPEWRAPPERS_H

#include "../globals.h"
#include "interfaces.h"
#include "exception.h"
#include <limits.h>
#include <float.h>
#include <wchar.h>

namespace xtk
{

/**
 * The xInteger class wraps a value of the primitive type int in an object.
 */
class XTKAPI xInteger : public virtual xObject,public xComparable
{
protected:
	int		m_value;

public:
	const static int MAX = INT_MAX;
	const static int MIN = INT_MIN;
	
	xInteger(int value);
	virtual ~xInteger(){}
	
	int getValue(){return m_value;}
	
	/**
	 * Extract an int value from the given String.The characters in the string must all be valid for
	 * integer extraction.
	 * Returns true if the conversion goes well
	 */
	static bool parseInt(xString str,OUT int* res)
	{
		long tmp;
		xchar* remainder;
		
		#ifndef XTK_UNICODE
			tmp = ::strtol(str.c_str(),&remainder,10);
		#else
			tmp = ::wcstol(str.c_str(),&remainder,10);
		#endif
		
		if(
			(tmp == 0 && remainder == str.c_str()) ||
			(remainder != str.c_str() + str.length()) ||
			(tmp < xInteger::MIN || tmp > xInteger::MAX)
		  )
			return false;
		
		*res = (int) tmp;
		return true;
	}
	
	
	virtual int compareTo(xObject& o);
	virtual bool equals(xObject& o);
	virtual xString toString();
	virtual int hashCode();
};

/**
* The xLong class wraps a value of the primitive type long in an object.
*/
class XTKAPI xLong : public virtual xObject,public xComparable
{
protected:
	long		m_value;

public:
	const static long MAX = LONG_MAX;
	const static long MIN = LONG_MIN;

	xLong(long value) : m_value(value){}
	virtual ~xLong(){}

	long getValue(){return m_value;}

	int compareTo(xLong& o)
	{
		if(m_value < o.m_value)
			return -1;
		if(m_value > o.m_value)
			return 1;
		return 0;
	}
	
	virtual int compareTo(xObject& o)
	{
		xLong* l = dynamic_cast<xLong*>(&o);
		if(l == NULL)
			throw xClassCastException(_T("Can compare only against another xLong"));
			
		return compareTo(*l);
	}
	
	bool equals(xLong& o){return m_value == o.m_value;}
	virtual bool equals(xObject& o)
	{
		xLong* l = dynamic_cast<xLong*>(&o);
		if(l == NULL)
			return false;

		return equals(*l);
	}
	
	virtual xString toString(){return xString::getFormat(_T("%li"),m_value);}
	virtual int hashCode(){return (int) m_value;}
	
	/**
	* Extract a long value from the given String.The characters in the string must all be valid for
	* integer extraction.
	* Returns true if the conversion goes well
	*/
	static bool parseLong(xString str,OUT long* res)
	{
		long tmp;
		xchar* remainder;

		#ifndef XTK_UNICODE
			tmp = ::strtol(str.c_str(),&remainder,10);
		#else
			tmp = ::wcstol(str.c_str(),&remainder,10);
		#endif

		if(
			(tmp == 0 && remainder == str.c_str()) ||
			(remainder != str.c_str() + str.length())
			)
			return false;

		*res = tmp;
		return true;
	}
};

/**
* The xFloat class wraps a value of the primitive type float in an object.
*/
class XTKAPI xFloat : public virtual xObject,public xComparable
{
protected:
	float		m_value;

public:
	static float MAX;
	static float MIN;

	xFloat(float value) : m_value(value){}
	virtual ~xFloat(){}

	float getValue(){return m_value;}
	
	/**
	* Extract a float value from the given String.The characters in the string must all be valid for
	* float extraction.
	* Returns true if the conversion goes well
	*/
	static bool parseFloat(xString str,OUT float* res)
	{
		double tmp;
		xchar* remainder;

		#ifndef XTK_UNICODE
			tmp = ::strtod(str.c_str(),&remainder);
		#else
			tmp = ::wcstod(str.c_str(),&remainder);
		#endif

		if(
			(tmp == 0 && remainder == str.c_str()) ||
			(remainder != str.c_str() + str.length()) ||
			(tmp < xFloat::MIN || tmp > xFloat::MAX)
		  )
			return false;

		*res = (float) tmp;
		return true;
	}
	
	
	int compareTo(xFloat& o)
	{
		if(m_value < o.m_value)
			return -1;
		if(m_value > o.m_value)
			return 1;
		return 0;
	}

	virtual int compareTo(xObject& o)
	{
		xFloat* l = dynamic_cast<xFloat*>(&o);
		if(l == NULL)
			throw xClassCastException(_T("Can compare only against another xFloat"));

		return compareTo(*l);
	}

	bool equals(xFloat& o){return m_value == o.m_value;}
	virtual bool equals(xObject& o)
	{
		xFloat* l = dynamic_cast<xFloat*>(&o);
		if(l == NULL)
			return false;

		return equals(*l);
	}

	virtual xString toString(){return xString::getFormat(_T("%f"),m_value);}
	virtual int hashCode(){return (int) m_value;}
};


/**
* The xBoolean class wraps a value of the primitive type bool in an object.
*/
class XTKAPI xBoolean : public xObject
{
protected:
	bool		m_value;

public:
	xBoolean(bool value) : m_value(value) {};
	virtual ~xBoolean(){}

	virtual bool getValue(){return m_value;}

	virtual bool equals(xBoolean& o)
	{
		return m_value == o.m_value;
	}
	
	virtual bool equals(xObject& o)
	{
		xBoolean* b = dynamic_cast<xBoolean*>(&o);
		if(b == NULL)
			return false;
		
		return equals(*b);
	}
	
	virtual xString toString()
	{
		if(m_value)
			return _T("true");
		else
			return _T("false");
	}
	
	virtual int hashCode()
	{
		if(m_value)
			return 1;
		else
			return 0;
	}
};


/**
 * 
 * 
 */
class XTKAPI xCharacter : public virtual xObject,public xComparable
{
protected:
	xchar		m_value;

public:
	const static int MAX = CHAR_MAX;
	const static int MIN = CHAR_MIN;
	
	xCharacter(char value) : m_value(value){}
	virtual ~xCharacter(){}
	
	virtual xchar getValue(){return m_value;}
	
	virtual int compareTo(xCharacter& o);
	virtual bool equals(xObject& o);
	
	virtual xString toString();
	virtual int hashCode(){return (int) m_value;}
};
	
}//namespace



#endif //XTK_TYPEWRAPPERS_H
