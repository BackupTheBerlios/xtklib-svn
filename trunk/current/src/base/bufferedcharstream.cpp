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
* @file bufferedcharstream.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/charstream.h"
#include "../../include/xtk/base/stream.h"

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xBufferedReader::xBufferedReader(xReader* in, int buffSize) : xFilterReader(in),m_buffer(buffSize)
{
	m_mark = -1;
	m_position = 0;
	m_fill = 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedReader::refillBuffer()
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
			xArray<xchar>::copy(m_buffer,0,m_buffer,m_mark,sz);
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
int xBufferedReader::read(xArray<xchar> a,int off,int len) throw(xIOException,xIndexOutOfBoundsException)
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
	xArray<xchar>::copy(a, off, m_buffer, m_position, cnt);
	m_position += cnt;
	return cnt;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xBufferedReader::eof()
{
	int avail = m_fill - m_position;
	if(avail <= 0)
	{
		refillBuffer();
		avail = m_fill - m_position;
		if(avail <= 0) 
			return true;
	}
	
	return false;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xchar xBufferedReader::readChar() throw(xIOException)
{
	if(m_position >= m_fill) 
	{
		refillBuffer();
		if(m_position >= m_fill)
			return 0;
	}

	return m_buffer[m_position++];
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xBufferedReader::read(int len) throw(xIOException)
{
	int avail = m_fill - m_position;
	if (avail <= 0) 
	{
		//if the len is > of the buffer size
		if (len >= m_buffer.size() && m_mark < 0)
			return read(len);

		refillBuffer();
		avail = m_fill - m_position;
		if(avail <= 0) 
			return _T("");
	}

	int cnt = (avail < len) ? avail : len;
	xString ret(m_buffer.getRawData() + m_position,cnt);
	m_position += cnt;
	return ret;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedReader::mark(int readAheadLimit) throw(xIOException)
{
	m_mark = m_position;
	m_markReadAhead = readAheadLimit;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedReader::rewind() throw(xIOException)
{
	if(m_mark < 0)
		throw xIOException(_T("Illegal mark position"));	

	m_position = m_mark;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xBufferedReader::readLine() throw(xIOException)
{	
	xString line;
	
	while(!eof())
	{
		xchar c = readChar();
		
		if(c == _T('\n')) 
			break; // UNIX lf

		if(c == _T('\r')) //Mac or DOS lf ?
		{
			if(eof())
				break;
				
			xchar c2 = readChar();
			if (c2 != _T('\n'))  // is a mac lf
			{
				if(m_position > 0)
					m_position--;
				else
				{
					m_buffer.resize(m_buffer.size() + 1);
					
					//insert at left the last read char
					for(int i = m_buffer.size() - 1;i > 0;i--)
						m_buffer[i] = m_buffer[i - 1];
						
					m_buffer[0] = c2;
					m_position = 0;
					m_fill++;
					if(m_mark >= 0)
						m_mark++;
				}
			}
				
			break;
		}

		line.append(&c,1);
	}
	return line;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xBufferedReader::readToken(xString separatorChars) throw(xIOException)
{
	xString ret;
	while(!eof())
	{
		bool found = false;
		xchar c = readChar();
		for(int i = 0;i < (int)separatorChars.length();i++)
			if(c == separatorChars.charAt(i))
			{
				found = true;
				break;
			}

		if(found)
			break;
		else
			ret.append(&c,1);
	}
	return ret;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xBufferedWriter::xBufferedWriter(xWriter* out,int buffSize)
: xFilterWriter(out),m_buffer(buffSize)
{
	m_position = 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedWriter::close()
{
	flush();
	m_out.close();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedWriter::flush() throw(xIOException)
{
	m_out.write(m_buffer,0,m_position);
	m_position = 0;
	m_out.flush();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedWriter::write(xArray<xchar> a,int off,int len) throw(xIOException,xIndexOutOfBoundsException)
{
	if(len <= 0 || off+len > a.size() || off < 0)
		throw xIndexOutOfBoundsException();

	if(m_position + len >= m_buffer.size())
	{
		flush();
		//write directly, bypass the buffer
		m_out.write(a,off,len);
	}
	else
	{
		xArray<xchar>::copy(m_buffer,m_position,a,off,len);
		m_position += len;
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedWriter::writeChar(xchar c) throw(xIOException)
{
	if(m_position >= m_buffer.size())
		flush();

	m_buffer[m_position++] = c;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xBufferedWriter::write(xString s) throw(xIOException)
{
	if(m_position + (int)s.length() >= m_buffer.size())
	{
		flush();
		//write directly, bypass the buffer
		m_out.write(s);
	}
	else
	{
		m_buffer.copyData(s.c_str(),m_position,(int)s.length());
		m_position += (int)s.length();
	}
}


}//namespace
