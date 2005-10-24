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
* @file string.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_STRING_H
#define XTK_STRING_H

#include "../globals.h"
#include "interfaces.h"
//#include "exception.h"
#include "charset.h"
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <wctype.h>

namespace xtk
{

//forward
class xStringData;

/**
 * A String class. Use copy-on-write method.
 */
class XTKAPI xString : public virtual xComparable,public virtual xObject
{
private:
	xStringData*	m_data;
	
	#ifdef XTK_UNICODE
		//convenience buffers for not recalculate mb_data
		xArray<xbyte>*		mb_data;
		bool				m_isMbUpdated;
	#endif
	
	
	void allocateNeededBuffer(size_t newSize);
	void copyOnWrite();
	
	
	inline int compareTo(const xchar* other);
	
	inline static size_t xstrlen(const xchar* str);
public:
	static const xchar* newLine; 
	static const xchar nullChar = _T('\0');

	/**
	 * Create an empty string
	 */
	xString();
	
	/**
	 * Create an empty string
	 */
	xString(size_t initialCapacity);
	
	/**
	 * Create a string by copying the content of another one. Use copy on write.
	 */
	xString(const xString& copy);
	
	/**
	 * Create a string from a char array.
	 */
	xString(const xchar* format);
	
	/**
	 *
	 */
	xString(const char* cstr,xCharset::Charset charset);

	
	/**
	 * Create a string by copying at numCopy characters from a given 
	 * char array.
	 */
	xString(const xchar* copy,size_t charToCopy);
	
	virtual ~xString();
	
	
	/**
	 * Ensure that the capacity of the string is at least the value specified.
	 * 
	 */
	void ensureCapacity(size_t newCapacity);
	
	
	/**
	 * Returns the length in characters of this string
	 */
	size_t length();
	
	/**
	 * Return the size in bytes of this string
	 */
	size_t size();
	
	/**
	 * Returns a pointer to the string data.
	 */
	const xchar* c_str();
	
	/**
	 * Returns multibyte representation of the string encoded 
	 * in the system default charset
	 */
	const char* mb_str(xCharset::Charset charset = xCharset::CS_UTF8);

	#if !defined(XTK_OS_WINDOWS) || !defined(XTK_UNICODE)
		const char* fn_str(){return mb_str();}
	#else
		const xchar* fn_str()
		{
			#ifdef XTK_OS_WINDOWS
				return c_str();
			#elif defined(XTK_OS_UNIX)
				return mb_str();
			#endif
		}
	#endif	
	
	
	/**
	 * Trim the capacity of the String to its effective size
	 */
	void trimToSize();
	
	void trimSpaceLeft();
	void trimSpaceRight();
	
	/**
	 * Append a string at the end of this.
	 */
	void append(const xchar* psz);
	
	/**
	 * Append a string at the end of this.
	 */
	void append(const xchar* psz,size_t lenToappend);
	
	/**
	 * Append a string at the end of this.
	 */
	void append(const xString& str);

	/**
	 * Format a string and assign the result to this
	 */
	void formatV(const xchar* format,va_list ap);
	
	/**
	 * Format a string and assign the result to this
	 */
	void format(const xchar* format,...);
	
	/**
	 * Format a string and append it to this.
	 */
	void appendFormatV(const xchar* format,va_list ap);
	
	/**
	 * Format a string and append it to this.
	 */
	void appendFormat(const xchar* format,...);
	
	/**
	 * Returns a new string that is a substring of this string.
	 * @exception xIndexOutOfBoundsException
	 */
	xString substring(size_t beginIndex);
	
	/**
	 * Returns a new string that is a substring of this string.
	 * @exception xIndexOutOfBoundsException
	 */
	xString substring(size_t beginIndex,size_t endIndex);
	
	/**
	 * Gets the char a the given position
	 * @exception xIndexOutOfBoundsException
	 */
	xchar charAt(size_t index);
	
	/**
	 * Insert a portion of a string at the given index in the current string
	 * @exception xIndexOutOfBoundsException
	 */
	void insert(const xchar* psz,size_t portion,size_t index);
	
	void clear();
	
	/**
	 * Insert a string at the given index in the current string
	 * @exception xIndexOutOfBoundsException
	 */
	void insert(const xchar* str,size_t index);
	
	/**
	 * Insert a string at the given index in the current string
	 * @exception xIndexOutOfBoundsException
	 */
	void insert(xString& str,size_t index);
	
	/**
	 * Invert the position of characters in the string, so that the first char
	 * become the last and so on.
	 */
	void invert();
	
	/**
	 * erase the char a the given position
	 * @exception xIndexOutOfBoundsException
	 */
	void remove(size_t index);
	
	/**
	 * Remove the portion of the string beginning ad startIndex (included) and
	 * ending at endIndex (excluded)
	 * @exception xIndexOutOfBoundsException
	 */
	void remove(size_t startIndex,size_t endIndex);
	
	void set(const xchar* str);
	
	bool isEmpty();
	
	/**
	 * Replace the char at the given position
	 * @exception xIndexOutOfBoundsException
	 */
	void replace(xchar replace,size_t index);
	
	/**
	 * Find the given string into this string starting at startIndex
	 * and returns the index of the first char of the first occurrence found.
	 * Returns (size_t)-1 if no occurrences where found.
	 * 
	 * @exception xIndexOutOfBoundException
	 */
	size_t find(xString toFind,size_t startIndex = 0);
	
	/**
	 * Replace all occurrences of a given char with another char.
	 * @return the number of replaced chars
	 */
	int replace(xchar find,xchar replacing);
	
	/**
	 * Replace all occurrences of a given string pattern with another string.
	 * @return the number of replaced items
	 */
	int replace(xString toFind,xString toReplaceWith);
	
	
	static bool isAlpha(xchar c)
	{
		#ifndef XTK_UNICODE
			if((unsigned)(c < 0) <= 256) //if not a valid char
				return false;
			return ::isalpha(c) != 0;
		#else
			return ::iswalpha(c) != 0;
		#endif
	}	
	
	static bool isSpace(xchar c)
	{
		#ifndef XTK_UNICODE
			if(c < 0) //if not a valid char
				return false;
			return ::isspace(c) != 0;
		#else
			return ::iswspace(c) != 0;
		#endif
	}
	
	virtual int hashCode();
	virtual int compareTo(xObject& compare);
	virtual bool equals(xObject& equals);
	virtual bool equals(const xchar* other);
	virtual xString toString();
	virtual xObject* clone();
	
	xString& operator=(const xString& other);
	xString& operator=(const xchar* other);

	//---------------------------------------
	static xString getFormat(const xchar* format,...);
	static xString getFormatV(const xchar* format,va_list ap);
	
	#if defined(XTK_UNICODE) && defined(XTK_OS_WINDOWS)
		/**
		 * Build a xString object from a string given from the underlying os.
		 */
		static xString getFromOS(const xchar* str);
	#else
	
		/**
		* Build a xString object from a string given from the underlying os.
		*/
		static xString getFromOS(const char* str);
	#endif
};

/**
 * The string tokenizer class allows to break a string into tokens.
 */
class XTKAPI xStringTokenizer : public virtual xObject
{
protected:
	xString		m_str;
	xString		m_delim;
	size_t		m_currentIndex;
	
public:
	xStringTokenizer(xString str,xString delim = _T(" ")) 
	: m_str(str),m_delim(delim),m_currentIndex(0){}
	
	/**
	 * Count the total number of  tokens.
	 */
	int countTokens();
	
	/**
	 * 
	 */
	bool hasMoreTokens();
	
	/**
	 * @exception xNoSuchElementException
	 */
	xString nextToken()
	{
		return nextToken(m_delim);
	}
	
	/**
	 * @exception xNoSuchElementException
	 */
	xString nextToken(xString separatorChars);
};

}//namespace

#endif //XTK_STRING_H
