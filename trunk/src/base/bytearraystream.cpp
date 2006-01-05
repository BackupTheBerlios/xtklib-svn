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
* @file bytearraystream.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/base/stream.h"

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xByteArrayInputStream::xByteArrayInputStream(xArray<xbyte>* buff) : m_buffer(*buff)
{
	m_length = buff->size();
	m_position = 0;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xByteArrayInputStream::xByteArrayInputStream(xArray<xbyte>* buff, int off, int len) 
throw(xIndexOutOfBoundsException) : m_buffer(*buff)
{
	if(len <= 0 || off+len > buff->size())
		throw xIndexOutOfBoundsException();
	
	m_length = len;
	m_position = off;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xByteArrayInputStream::~xByteArrayInputStream()
{
	if(isOwner())
		delete &m_buffer;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xByteArrayInputStream::available()
{
	return 	(m_length - m_position);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xByteArrayInputStream::read() throw(xIOException) 
{
	return (int) m_buffer[m_position++];
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xByteArrayInputStream::read(xArray<xbyte>& a) throw(xIOException) 
{
	return read(a,0,a.size());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xByteArrayInputStream::read(xArray<xbyte>& a,int off, int len)
throw(xIOException,xIndexOutOfBoundsException) 
{
	if(len <= 0 || off+len > a.size())
		throw xIndexOutOfBoundsException();
		
	int toCopy = 0;
	if(available() < len)
		toCopy = available();
	else
		toCopy = len;
		
	xArray<xbyte>::copy(a,off,m_buffer,m_position,toCopy);
	m_position += toCopy;
	
	return toCopy;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xByteArrayInputStream::setBuffer(xArray<xbyte>* buff)
{
	setBuffer(buff,0,buff->size());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xByteArrayInputStream::setBuffer(xArray<xbyte>* buff, int off, int len) throw(xIndexOutOfBoundsException)
{
	if(len <= 0 || off+len > buff->size())
		throw xIndexOutOfBoundsException();
		
	if(isOwner())
		delete &m_buffer;
	
	m_length = len;
	m_position = off;
	m_buffer = *buff;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xByteArrayOutputStream::xByteArrayOutputStream(int initialCapacity) 
: m_buffer(initialCapacity)
{
	m_position = 0;
	m_incrementMultiplier = 2;
	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xByteArrayOutputStream::size()
{
	return m_position;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xByteArrayOutputStream::write(xArray<xbyte>& b)
throw(xIOException) 
{
	write( b, 0, b.size());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xByteArrayOutputStream::write(xArray<xbyte>& b, int off, int len)
throw(xIOException,xIndexOutOfBoundsException) 
{
	if(len <= 0 || off+len > b.size())
		throw xIndexOutOfBoundsException();
	
	if(m_position + len >= m_buffer.size())
	{
		m_buffer.resize((m_position + len) * m_incrementMultiplier);
	}
	xArray<xbyte>::copy(m_buffer,m_position,b,off,len);
	m_position += len;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xByteArrayOutputStream::write(int b) throw(xIOException)
{
	if(m_position + 1 >= m_buffer.size())
	{
		m_buffer.resize((m_position + 1) * m_incrementMultiplier);
	}
	m_buffer[m_position++] = (xbyte) b;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xByteArrayOutputStream::writeTo(xOutputStream& out) throw(xIOException)
{
	out.write(m_buffer,0,m_position);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArray<xbyte>* xByteArrayOutputStream::toByteArray()
{
	return new xArray<xbyte>(m_buffer,0,m_position);
}


}//namespace
