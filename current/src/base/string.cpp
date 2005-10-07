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
* @file string.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/string.h"
#include "../../include/xtk/base/exception.h"
#include "../../include/xtk/base/math.h"
#include "../../include/xtk/base/array.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>

#if defined(XTK_OS_UNIX)
	#include <iconv.h>
#endif


namespace xtk
{

#define XTK_STRING_EXTRA_ALLOC			16
#define XTK_STRING_FORMAT_BASE_SIZE		16

#ifdef XTK_OS_WINDOWS
	const xchar* xString::newLine = _T("\r\n");
#elif defined(XTK_OS_UNIX)
	const xchar* xString::newLine = _T("\n");
#endif 

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xStringData : public virtual xObject
{
public:
	xchar* 	data;
	size_t 	capacity;
	size_t 	len;
	int		referenceCount;

	xStringData()
	{
		data = NULL;
		capacity = 0;
		len = 0;
		referenceCount = 0;	
	}

	xStringData(xchar* data,size_t capacity,size_t len,int referenceCount)
	{
		this->data = data;
		this->capacity = capacity;
		this->len = len;
		this->referenceCount = referenceCount;	
	}

	virtual ~xStringData(){}

	void attach()
	{
		synchronizeStart();
		if(!isEmpty())
			referenceCount++;
		synchronizeEnd();
	}

	void release()
	{
		synchronizeStart();
		if(!isEmpty() && --referenceCount == 0)
		{
			if(data != NULL && capacity != 0)
				free(data);
			synchronizeEnd();
			delete this;
		}
		else
			synchronizeEnd();
	}

	bool isEmpty(){return referenceCount == -10;}
	bool isShared(){return referenceCount > 1;}
};
	
static xStringData g_emptyString(_T('\0'),1,0,-10);
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
inline int xString::compareTo(const xchar* other)
{
	#ifndef XTK_UNICODE
		return ::strcmp(m_data->data,other);
	#else
		return ::wcscmp(m_data->data,other);
	#endif
}

inline size_t xString::xstrlen(const xchar* str)
{
	#ifndef XTK_UNICODE
		return ::strlen(str);
	#else
		return ::wcslen(str);
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString::xString()
{
	//init with blank string
	m_data = &g_emptyString;
	m_data->attach();
	
	#ifdef XTK_UNICODE
		mb_data = NULL;
		m_isMbUpdated = false;
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString::xString(size_t initialCapacity)
{
	//init with blank string
	m_data = new xStringData();
	m_data->attach();
	
	allocateNeededBuffer(initialCapacity);
	m_data->data[0] = _T('\0');
	#ifdef XTK_UNICODE
		mb_data = NULL;
		m_isMbUpdated = false;
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString::xString(const xString& copy)
{
	copy.m_data->attach();
	
	m_data = copy.m_data;
	
	#ifdef XTK_UNICODE
		mb_data = NULL;
		m_isMbUpdated = false;
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString::xString(const xchar* str)
{
	if(str == NULL)
		throw xNullPointerException();
		
	m_data = new xStringData();
	m_data->attach();
	
	m_data->len = xstrlen(str);

	allocateNeededBuffer(m_data->len);

	memcpy(m_data->data,str, m_data->len * sizeof(xchar));
	m_data->data[m_data->len] = _T('\0');
	
	#ifdef XTK_UNICODE
		mb_data = NULL;
		m_isMbUpdated = false;
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#ifdef XTK_UNICODE
xString::xString(const char* cstr,xCharset::Charset charset)
{
	//init with blank string
	m_data = &g_emptyString;
	m_data->attach();

	#ifdef XTK_UNICODE
		mb_data = NULL;
		m_isMbUpdated = false;
	#endif

	xCharsetDecoder dec(charset);
	dec.decode(*this,(const xbyte*) cstr,(int) strlen(cstr));
}
#endif

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString::xString(const xchar* copy,size_t numCopy)
{
	if(copy == NULL)
		throw xNullPointerException();

	m_data = new xStringData();
	m_data->attach();
	
	m_data->len = numCopy;
	allocateNeededBuffer(m_data->len);
	
	//copy the string
	memcpy(m_data->data,copy,m_data->len * sizeof(xchar));
	m_data->data[m_data->len] = _T('\0');
	m_data->len = xstrlen(m_data->data);
	
	#ifdef XTK_UNICODE
		mb_data = NULL;
		m_isMbUpdated = false;
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::allocateNeededBuffer(size_t newSize)
{
	if((newSize + 1) > m_data->capacity)
	{
		m_data->capacity = newSize + 1 + XTK_STRING_EXTRA_ALLOC;
		
		if(m_data->data == NULL)
			m_data->data = (xchar*) malloc(m_data->capacity * sizeof(xchar));
		else
			m_data->data = (xchar*) realloc(m_data->data,m_data->capacity * sizeof(xchar));
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::copyOnWrite()
{
	if(m_data->isShared() || m_data->isEmpty())
	{
		xStringData* oldData = m_data;
		m_data->release();
		
		m_data = new xStringData();
		m_data->attach();
		m_data->len = oldData->len;
		allocateNeededBuffer(m_data->len);
		
		memcpy(m_data->data,oldData->data,m_data->len * sizeof(xchar));
		m_data->data[m_data->len] = _T('\0');
	}

	
	#ifdef XTK_UNICODE
		m_isMbUpdated = false;
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::clear()
{
	m_data->release();
	m_data = &g_emptyString;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xString::isEmpty()
{
	return m_data->len == 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::ensureCapacity(size_t newCapacity)
{
	copyOnWrite();
	allocateNeededBuffer(newCapacity);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::trimToSize()
{
	copyOnWrite();
	m_data->data = (xchar*) realloc(m_data->data,(m_data->len + 1) * sizeof(xchar));
	m_data->capacity = m_data->len + 1;
	m_data->data[m_data->len] = _T('\0');
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString::~xString()
{
	m_data->release();
	
	#ifdef XTK_UNICODE
		if(mb_data != NULL)
			delete mb_data;
	#endif
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
size_t xString::length()
{
	return m_data->len;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
size_t xString::size()
{
	return length() * sizeof(xchar);	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
const xchar* xString::c_str()
{
	return (const xchar*) m_data->data;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
const char* xString::mb_str()
{
	#ifdef XTK_UNICODE
		if(isEmpty())
			return "\0";
	
		if(mb_data == NULL)
			mb_data = new xArray<xbyte>(0);
		
		if(!m_isMbUpdated)
		{
			xCharsetEncoder enc(xCharset::CS_SYSTEM);
			enc.encode(*mb_data,*this);
			mb_data->resize(mb_data->size() + 1);
			(*mb_data)[mb_data->size() - 1] = '\0';
			m_isMbUpdated = true;
		}
		return (const char*) mb_data->getRawData();
		
	#else
		return c_str();
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::append(const xchar* psz)
{
	append(psz,xstrlen(psz));
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::append(const xchar* psz,size_t lenToappend)
{
	copyOnWrite();
	
	size_t pastSize = m_data->len;
	
	m_data->len += lenToappend;
	allocateNeededBuffer(m_data->len);
	
	//copy the string
	memcpy(m_data->data + pastSize,psz,lenToappend * sizeof(xchar));
	m_data->data[m_data->len] = _T('\0');
	m_data->len = xstrlen(m_data->data);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::append(const xString& str)
{
	append(((xString&) str).c_str(),((xString&) str).length());
	
	/*
	copyOnWrite();
	
	int pastSize = m_data->size;
	
	m_data->size += str.m_data->size;
	allocateNeededBuffer(m_data->size);
	
	//copy the string
	memcpy(m_data->data + pastSize,str.m_data->data,str.m_data->size);
	m_data->data[m_data->size] = nullChar;	
	*/
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::formatV(const xchar* format,va_list ap)
{
	copyOnWrite();
	
	int ret = 0;
	size_t formatLen = xstrlen(format);
	for(int i = 1;;i++)
	{
		if(m_data->len < (formatLen + (XTK_STRING_FORMAT_BASE_SIZE * i)))
		{
			allocateNeededBuffer(formatLen + (XTK_STRING_FORMAT_BASE_SIZE * i));
		}
		#ifdef XTK_COMP_VISUAL_C
			#ifndef XTK_UNICODE
				ret = _vsnprintf(m_data->data,m_data->capacity,format,ap);
			#else
				ret = _vsnwprintf(m_data->data,m_data->capacity,format,ap);
			#endif
			
			if(ret >= 0 && ((size_t)ret < m_data->capacity))
			{
				m_data->len = ret;
				break;
			}
		#else //other compilers
			#ifndef XTK_UNICODE
				ret = vsnprintf(m_data->data,m_data->capacity,format,ap);
			#else
				//substitute any %s with %ls in format string
				xString strformat(format);
				strformat.replace(_T("%s"),_T("%ls"));
				ret = vswprintf(m_data->data,m_data->capacity,strformat.c_str(),ap);
			#endif

			if(ret != -1 && (((size_t)ret < m_data->capacity)))
			{
				m_data->len = ret;
				break;
			}
		#endif
	}
	
	m_data->data[m_data->len] = _T('\0');
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::set(const xchar* str)
{
	(*this) = str;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::format(const xchar* format,...)
{
	va_list ap;
	va_start(ap, format);
	formatV(format,ap);
	va_end(ap);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::appendFormat(const xchar* format,...)
{
	va_list ap;
	va_start(ap, format);
	appendFormatV(format,ap);
	va_end(ap);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::appendFormatV(const xchar* format,va_list ap)
{
	xString tmp;
	tmp.formatV(format,ap);
	this->append(tmp);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::trimSpaceLeft()
{
	copyOnWrite();
	
	size_t i = 0;
	size_t len = length();
	for(;i < len;i++)
		if(!isSpace(m_data->data[i]))
			break;
	
	//now shift left the string
	if(i != 0)
	{
		for(int j = 0;j + i < len;j++)
			m_data->data[j] = m_data->data[j + i];
	}
	
	m_data->len -= i;
	m_data->data[m_data->len] = _T('\0');
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::trimSpaceRight()
{
	copyOnWrite();

	size_t i = length() - 1;
	for(;i >= 0;i--)
		if(!isSpace(m_data->data[i]))
			break;

	m_data->len = i + 1;
	m_data->data[m_data->len] = _T('\0');
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xString::substring(size_t beginIndex)
{
	return substring(beginIndex,m_data->len);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xString::substring(size_t beginIndex,size_t endIndex)
{
	if((beginIndex >= length()) || endIndex > length() )
		throw xIndexOutOfBoundsException();
	if(beginIndex > endIndex)
		throw xIndexOutOfBoundsException(_T("Second index is greater than the first"));

	return xString(m_data->data + beginIndex,endIndex - beginIndex);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xchar xString::charAt(size_t index)
{
	if(index >= length())
		throw xIndexOutOfBoundsException();
					
	return m_data->data[index];
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::insert(const xchar* psz,size_t portion,size_t index)
{
	if(index > length() || index < 0)
		throw xIndexOutOfBoundsException();
	
	copyOnWrite();
	
	//allocate needed memory
	m_data->len += portion;
	allocateNeededBuffer(m_data->len);
	
	//shift right
	for(size_t i = m_data->len;i >= index + portion;i--)
		m_data->data[i] = m_data->data[i - portion];
	
	//insert new data
	for(size_t i = 0;i < portion;i++)
		m_data->data[index + i] = psz[i];
	
	m_data->data[m_data->len] = _T('\0');
	m_data->len = xstrlen(m_data->data);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::insert(const xchar* str,size_t index)
{
	insert(str,xstrlen(str),index);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::insert(xString& str,size_t index)
{
	insert(str.c_str(),index);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::invert()
{
	copyOnWrite();
	
	if(isEmpty())
		return;
	
	size_t i = 0;
	size_t j = m_data->len - 1;
	while(i < j)
	{
		xchar tmp = m_data->data[i];
		m_data->data[i] = m_data->data[j];
		m_data->data[j] = tmp;
		i++;
		j--;	
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::remove(size_t index)
{
	remove(index,index + 1);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::remove(size_t startIndex,size_t endIndex)
{
	if(endIndex > length() || endIndex <= startIndex)
		throw xIndexOutOfBoundsException();
	
	copyOnWrite();
	
	//shift left
	size_t diff = endIndex - startIndex;
	for(size_t i = startIndex;i + diff < length();i++)
		m_data->data[i] = m_data->data[i + diff];
		
	m_data->len -= diff;
	m_data->data[m_data->len] = _T('\0');
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xString::replace(xchar replace,size_t index)
{
	if(index >= m_data->len)
		throw xIndexOutOfBoundsException();
	
	copyOnWrite();
	
	m_data->data[index] = replace;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xString::replace(xchar find,xchar replacing)
{
	copyOnWrite();
	
	int occ = 0;
	for(size_t i = 0; i < m_data->len;i++)
	{
		if(m_data->data[i] == find)
		{
			m_data->data[i] = replacing;
			occ++;	
		}	
	}
	return occ;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
size_t xString::find(xString toFind, size_t startIndex)
{
	if(startIndex >= length())
		throw xIndexOutOfBoundsException();
	
	size_t index = startIndex;
	while(index < length())
	{
		bool found = true;
		for(size_t i = 0;(i < toFind.length());i++)
		{
			if(i + index > length())
			{
				found = false;
				break;
			}
			
			if(m_data->data[i + index] != toFind.m_data->data[i])
			{
				found = false;
				break;
			}
		}
		if(!found)
			index++;
		else
			return index;
	}
	
	return (size_t)-1;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xString::replace(xString toFind,xString toReplaceWith)
{
	copyOnWrite();
	
	size_t foundIndex = 0;
	int count = 0;
	while((foundIndex = find(toFind,foundIndex)) != (size_t)-1)
	{
		remove(foundIndex,foundIndex + toFind.length());
		insert(toReplaceWith,foundIndex);
		foundIndex += toReplaceWith.length();
		count++;
	}
	return count;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xString::hashCode()
{
	int hash = 0;
	for(register size_t i = 0;i < length();i++)
	{ 
		hash += (int)(m_data->data[i] * (int)(xMath::pow(2,(length() - i))));
	}
	return hash;
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xString::compareTo(xObject& other)
{
	xString* strOther = dynamic_cast<xString*>(&other);
	if(strOther == NULL)
		throw xClassCastException(_T("Can compare only against another xString object")); 
		
	return compareTo(strOther->m_data->data);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xString::equals(xObject& other)
{
	xString* strOther = dynamic_cast<xString*>(&other);
	if(strOther == NULL)
		return false;
		
	return compareTo(strOther->m_data->data) == 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xString::equals(const xchar* other)
{
	return compareTo(other) == 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xString::toString()
{
	return (*this);
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xObject* xString::clone()
{
	return new xString(*this);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString& xString::operator=(const xString& other)
{
	m_data->release();
	other.m_data->attach();
	
	m_data = other.m_data;
	return *this;
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString& xString::operator=(const xchar* other)
{
	copyOnWrite();
	m_data->len = xstrlen(other);
	allocateNeededBuffer(m_data->len);
	
	memcpy(m_data->data,other,m_data->len * sizeof(xchar));
	m_data->data[m_data->len] = _T('\0');
	
	return *this;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xString::getFormat(const xchar* format,...)
{
	xString tmp;

	va_list ap;
	va_start(ap, format);
	tmp.formatV(format,ap);
	va_end(ap);

	return tmp;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xString::getFormatV(const xchar* format,va_list ap)
{
	xString tmp;
	tmp.formatV(format,ap);

	return tmp;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#if defined(XTK_UNICODE) && defined(XTK_OS_WINDOWS)
	xString xString::getFromOS(const xchar* str)
	{
		return str;
	}
	
#else

	xString xString::getFromOS(const char* str)
	{
		#ifndef XTK_UNICODE
			return str;
		#else
			return xString(str,xCharset::CS_SYSTEM);
		#endif
	}

#endif
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xStringTokenizer::countTokens()
{
	if(m_str.isEmpty())
		return 0;
	
	//if not empty there is at least 1 token.
	int count = 1;
	for(size_t nextIndex = 0;nextIndex < m_str.length();nextIndex++)
	{
		for(size_t i = 0;i < m_delim.length();i++)
			if(m_str.charAt(nextIndex) == m_delim.charAt(i))
			{
				if(nextIndex != m_str.length() - 1)
				{
					count++;
					break;
				}
			}
	}
	return count;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xStringTokenizer::nextToken(xString separatorChars)
{
	if(!hasMoreTokens())
		throw xNoSuchElementException();
	
	//find the next delim
	size_t nextIndex = m_currentIndex;
	for(;nextIndex < m_str.length();nextIndex++)
	{
		bool found = false;
		for(size_t i = 0;i < separatorChars.length();i++)
			if(m_str.charAt(nextIndex) == separatorChars.charAt(i))
			{
				found = true;
				break;
			}
		
		if(found)
			break;
	}
	
	xString ret(m_str.substring(m_currentIndex,nextIndex));
	m_currentIndex = nextIndex + 1;
	return ret;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xStringTokenizer::hasMoreTokens()
{
	if(m_currentIndex < m_str.length() - 1)
		return true;
	
	return false;
}



}//namespace
