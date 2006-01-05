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
* @file datastream.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/base/stream.h"
#include "../../include/xtk/base/charset.h"

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xi8 xDataInputStream::readI8() throw(xIOException,xEOFException)
{
	int c = m_in.read();
	if(c == -1)
		throw xEOFException();
	
	return (xi8) c;	
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xi16 xDataInputStream::readI16() throw(xIOException,xEOFException)
{
	xArray<xbyte> buf(2);
	readFully(buf);
	
	return *((xi16*) buf.getRawData());	
}	

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xi32 xDataInputStream::readI32() throw(xIOException,xEOFException)
{
	xArray<xbyte> buf(4);
	readFully(buf);
	
	return *((xi32*) buf.getRawData());	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xi64 xDataInputStream::readI64() throw(xIOException,xEOFException)
{
	xArray<xbyte> buf(8);
	readFully(buf);
	
	return *((xi64*) buf.getRawData());	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#ifdef XTK_UNICODE
	xString xDataInputStream::readUTF8() throw(xIOException,xEOFException)
	{
		xi32 strSize = readI32();
		xArray<xbyte> buf(strSize);
		readFully(buf,0,strSize);
		xString out;
		xCharsetDecoder dec(xCharset::CS_UTF8);
		dec.decode(out,buf);
		
		return out;
	}
#endif

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xDataInputStream::readFully(xArray<xbyte>& b, int off, int len)
throw(xIOException,xEOFException,xIndexOutOfBoundsException)
{
	if(len <= 0 || off+len > b.size())
		throw xIndexOutOfBoundsException();
		
	int totRead = 0;
	int read = 0;
	do
	{
		read = m_in.read(b,off + totRead,len - totRead);
		if(read == -1)
			throw xEOFException();
		
		totRead += read;
	}
	while(read < len);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xDataInputStream::readFully(xArray<xbyte>& b)
throw(xIOException,xEOFException)
{
	readFully(b,0,b.size());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArray<xbyte>* xDataInputStream::readData()
throw(xIOException,xEOFException)
{
	xi32 dataSize = readI32();
	xArray<xbyte>* buf = new xArray<xbyte>(dataSize);
	readFully(*buf);
	return buf;
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
void xDataOutputStream::writeI8(xi8 d) throw(xIOException)
{
	m_out.write(d);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xDataOutputStream::writeI16(xi16 d) throw(xIOException)
{
	xArray<xbyte> buf((xbyte*) &d,0,2);
	m_out.write(buf);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xDataOutputStream::writeI32(xi32 d) throw(xIOException)
{
	xArray<xbyte> buf((xbyte*) &d,0,4);
	m_out.write(buf);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xDataOutputStream::writeI64(xi64 d) throw(xIOException)
{ 
	xArray<xbyte> buf((xbyte*) &d,0,8);
	m_out.write(buf);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#ifdef XTK_UNICODE
	void xDataOutputStream::writeUTF8(xString str) throw(xIOException)
	{
		xArray<xbyte> outBuf;
		xCharsetEncoder enc(xCharset::CS_UTF8);
		enc.encode(outBuf,str);
		
		writeData(outBuf);
	}
#endif

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xDataOutputStream::writeData(xArray<xbyte>& b, int off, int len)
throw(xIOException,xIndexOutOfBoundsException)
{
	if(len <= 0 || off+len > b.size() || off < 0)
		throw xIndexOutOfBoundsException();
		
	writeI32((xi32) b.size());
	m_out.write(b,off,len);
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xDataOutputStream::writeData(xArray<xbyte>& b)
throw(xIOException)
{
	writeData(b,0,b.size());
}

}//namespace

