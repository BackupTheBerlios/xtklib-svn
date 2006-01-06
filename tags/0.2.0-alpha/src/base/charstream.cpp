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
* @file charstream.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/base/charstream.h"

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xReader::read(int len) throw(xIOException)
{
	xString ret;
	for(int i = 0;i < len && !eof();i++)
	{
		xchar c = readChar();
		ret.append(&c,1);
	}
	
	return ret;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xReader::read(xArray<xchar> a,int off,int len) throw(xIOException,xIndexOutOfBoundsException)
{
	if(off < 0 || len < 0 || off+len > a.size())
		throw xIndexOutOfBoundsException();
	
	int i;
	for(i = off;i < a.size() && !eof();i++)
		a[i] = readChar();
		
	return i;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWriter::write(xString s) throw(xIOException)
{
	for(size_t i = 0; i < s.length();i++)
		writeChar(s.charAt(i));
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xWriter::write(xArray<xchar> a,int off,int len) throw(xIOException,xIndexOutOfBoundsException)
{
	if(off < 0 || len < 0 || off+len > a.size())
		throw xIndexOutOfBoundsException();
		
	for(int i = off; i < off+len;i++)
		writeChar(a[i]);
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xInputStreamReader::eof()
{
	if(!m_isEofTestByteValid)
		m_eofTestByte = m_in.read();
	if(m_eofTestByte == xEOF)
		return true;
		
	m_isEofTestByteValid = true;
	return false;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xchar xInputStreamReader::readChar() throw(xIOException)
{
#ifdef XTK_UNICODE
	xbyte tbuff[9];
	for(int i = 0; i < 9;i++)
	{
		int b;
		if(m_isEofTestByteValid)
		{
			b = m_eofTestByte;
			m_isEofTestByteValid = false;
		}
		else
			b = m_in.read();
		
		if(b == xEOF && i == 0)
			return 0;
		else if(b == xEOF)
			throw xIOException(_T("Cannot decode the stream of byte"));
		
		tbuff[i] = b;
		xArray<xchar> outBuf;
		if(!m_dec.decode(outBuf,tbuff,i + 1))
			continue;
		else
		{
			//This character seems to be a tag.
			if(outBuf[0] == 65279)
			{	
				i = 0;
				continue;
			}
			
			return outBuf[0];
		}
	}
	
	//cannot decode char
	throw xIOException(_T("Cannot decode the stream of byte"));
	
#else
	if(m_isEofTestByteValid)
	{
		m_isEofTestByteValid = false;
		return m_eofTestByte;
	}
	else
		return (xbyte) m_in.read();

#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xOutputStreamWriter::writeChar(xchar c) throw(xIOException)
{
	#ifdef XTK_UNICODE
		xArray<xbyte> out;
		if(!m_enc.encode(out,&c,1))
			throw xIOException(_T("Cannot encode the data"));
		m_out.write(out);
	#else
		m_out.write(c);
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xOutputStreamWriter::write(xString s) throw(xIOException)
{
	#ifdef XTK_UNICODE
		xArray<xbyte> out;
		if(!m_enc.encode(out,s))
			throw xIOException(_T("Cannot encode the data"));
		m_out.write(out);
	#else
		xArray<xbyte> out((const xbyte*) s.c_str(),0,s.length());
		m_out.write(out);
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xOutputStreamWriter::write(xArray<xchar> a,int off,int len) throw(xIOException,xIndexOutOfBoundsException)
{
	#ifdef XTK_UNICODE
		xArray<xbyte> out;
		if(!m_enc.encode(out,a.getRawData() + off,len))
			throw xIOException(_T("Cannot encode the data"));
		m_out.write(out);
	#else
		m_out.write((xArray<xbyte>&)a,off,len);
	#endif
}


}//namespace

