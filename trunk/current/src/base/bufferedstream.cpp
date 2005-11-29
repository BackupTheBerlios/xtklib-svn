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
* @file bufferedstream.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/stream.h"
#include "../../include/xtk/base/utilities.h"

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xBufferedInputStream::xBufferedInputStream(xInputStream* in,int bufferSize) 
: xFilterInputStream(in),m_buffer(bufferSize)
{
	m_mark = -1;
	m_position = 0;
	m_fill = 0;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedInputStream::refillBuffer()
{
	if(m_mark < 0)
	{
	    m_position = 0;
	}
	else if (m_position >= m_buffer.size())
	{
	    if(m_mark > 0)
	    {
			int sz = m_position - m_mark;
			xArray<xbyte>::copy(m_buffer,0,m_buffer,m_mark,sz);
			m_position = sz;
			m_mark = 0;
	    } 
	    else if(m_buffer.size() >= m_markReadAhead) 
	    {
			m_mark = -1;
			m_position = 0;
	    } 
	    else 
	    {
			int newsz = m_position * 2;
			if(newsz > m_mark)
			    newsz = m_markReadAhead;
			    
			m_buffer.resize(newsz);
	    }
	}
    m_fill = m_position;
    
	int n = m_in.read(m_buffer, m_position, m_buffer.size() - m_position);
	if(n > 0)
		m_fill = n + m_position;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xBufferedInputStream::read() throw(xIOException)
{
	if(m_position >= m_fill) 
	{
	    refillBuffer();
	    if(m_position >= m_fill)
			return xEOF;
	}
	
	return m_buffer[m_position++];
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xBufferedInputStream::available()
{
	return m_fill - m_position + m_in.available();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xBufferedInputStream::read(xArray<xbyte>& a) throw(xIOException)
{
	return read(a,0,a.size());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xBufferedInputStream::read0(xArray<xbyte>& a,int off, int len) 
{
	int avail = m_fill - m_position;
	if (avail <= 0) 
	{
		//if the len is > of the buffer size
	    if (len >= m_buffer.size() && m_mark < 0)
			return m_in.read(a, off, len);
			
	    refillBuffer();
	    avail = m_fill - m_position;
	    if(avail <= 0) 
	    	return xEOF;
	}
	int cnt = (avail < len) ? avail : len;
	xArray<xbyte>::copy(a, off, m_buffer, m_position, cnt);
	m_position += cnt;
	return cnt;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xBufferedInputStream::read(xArray<xbyte>& a,int off, int len)
throw(xIOException,xIndexOutOfBoundsException) 
{
	if(len <= 0 || off+len > a.size())
		throw xIndexOutOfBoundsException();
		
	int n = read0(a, off, len);
	if(n <= 0) 
		return n;
	while((n < len) && (m_in.available() > 0)) 
	{
	    int n1 = read0(a, off + n, len - n);
	    if (n1 <= 0) 
	    	break;
	    n += n1;
	}
	return n;
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedInputStream::mark(int readAheadLimit) throw(xIOException)
{
	m_mark = m_position;
	m_markReadAhead = readAheadLimit;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedInputStream::rewind() throw(xIOException)
{
	if(m_mark < 0)
		throw xIOException(_T("Illegal mark position"));	
		
	m_position = m_mark;
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
xBufferedOutputStream::xBufferedOutputStream(xOutputStream* out,int bufferSize)
: xFilterOutputStream(out),m_buffer(bufferSize)
{
	m_position = 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xBufferedOutputStream::~xBufferedOutputStream()
{
	close0();
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedOutputStream::close0()
{
	flush();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedOutputStream::close() throw(xIOException)
{
	flush();
	m_out.flush();
	m_out.close();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedOutputStream::flush() throw(xIOException)
{
	m_out.write(m_buffer,0,m_position);
	m_position = 0;
	m_out.flush();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedOutputStream::write(xArray<xbyte>& b, int off, int len) 
throw(xIOException,xIndexOutOfBoundsException)
{
	if(len <= 0 || off+len > b.size() || off < 0)
		throw xIndexOutOfBoundsException();
		
	if(m_position + len >= m_buffer.size())
	{
		flush();
		//write directly
		m_out.write(b,off,len);
	}
	else
	{
		xArray<xbyte>::copy(m_buffer,m_position,b,off,len);
		m_position += len;
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedOutputStream::write(xArray<xbyte>& b) throw(xIOException)
{
	write(b,0,b.size());	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedOutputStream::write(int b) throw(xIOException)
{
	if(m_position >= m_buffer.size())
		flush();
	
	m_buffer[m_position++] = (xbyte) b;
}
	
}//namespace

