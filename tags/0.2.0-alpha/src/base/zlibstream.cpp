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
* @file zlibstream.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#include "../../include/xtk/base/zlibstream.h"

#ifdef XTK_USE_ZLIB

#include "zlib.h"

namespace xtk
{

enum 
{
	ZSTREAM_GZIP        = 0x10,     // gzip header
	ZSTREAM_AUTO        = 0x20      // auto detect between gzip and zlib
};
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xZLibInputStream::xZLibInputStream(xInputStream* in,xZLibConst::HeaderType headerType,int inflateBuffSize)
: xFilterInputStream(in),m_inflateBuff(inflateBuffSize)
{
	m_headerType = headerType;
	m_inflateBuffAvail = 0;
	m_inflateStream = new z_stream_s;
	memset(m_inflateStream, 0, sizeof(z_stream_s));
	
	int windowBits = MAX_WBITS;
	switch(headerType) 
	{
		case xZLibConst::HEADER_DISABLE:	windowBits = -MAX_WBITS; break;
		case xZLibConst::HEADER_ZLIB:       windowBits = MAX_WBITS; break;
		case xZLibConst::HEADER_GZIP:       windowBits = MAX_WBITS | ZSTREAM_GZIP; break;
		case xZLibConst::HEADER_AUTO:       windowBits = MAX_WBITS | ZSTREAM_AUTO; break;
		default:							windowBits = MAX_WBITS | ZSTREAM_AUTO; break;
	}

	::inflateInit2(m_inflateStream, windowBits);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xZLibInputStream::~xZLibInputStream()
{
	::inflateEnd(m_inflateStream);
	delete m_inflateStream;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xZLibInputStream::read(xArray<xbyte>& a,int off, int len) 
throw(xIOException,xIndexOutOfBoundsException,xDataFormatException)
{
	if(len <= 0 || off+len > a.size() || off < 0)
		throw xIndexOutOfBoundsException();
		
	int err = Z_OK;
	m_inflateStream->next_out = (unsigned char*)a.getRawData() + off;
	m_inflateStream->avail_out = len;

	while(err == Z_OK && m_inflateStream->avail_out > 0) 
	{
		if(m_inflateStream->avail_in == 0) 
		{
			int rd = m_in.read(m_inflateBuff);
			if(rd == xEOF)
				return xEOF;
			m_inflateStream->next_in = (unsigned char*) m_inflateBuff.getRawData();
			m_inflateStream->avail_in = rd;
		}
		err = ::inflate(m_inflateStream, Z_SYNC_FLUSH);
	}

	switch (err)
	{
	case Z_OK:
		break;

	case Z_STREAM_END:
		//The stream ended
		//m_inflateBuffFill = m_inflateBuff.size() - m_inflateStream->avail_in;
		//m_inflateBuff.shiftLeft(m_inflateStream->avail_in);
		break;

	case Z_DATA_ERROR:
		throw xDataFormatException(_T("Corrupted or invalid ZStream"));
	case Z_BUF_ERROR:
		// Indicates that zlib was expecting more data, but the parent stream
		// has none.
		throw xDataFormatException(_T("Unexpected end of stream"));
	default:
		throw xIOException(_T("Unexpected error while inflating"));
	}

	m_inflateBuffAvail = m_inflateStream->avail_in;
	return len - m_inflateStream->avail_out;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xZLibInputStream::read() throw(xIOException,xDataFormatException)
{
	unsigned char buff;
	int err = Z_OK;
	m_inflateStream->next_out = &buff;
	m_inflateStream->avail_out = 1;

	if(m_inflateStream->avail_in == 0) 
	{
		int rd = m_in.read(m_inflateBuff,m_inflateBuffAvail,m_inflateBuff.size() - m_inflateBuffAvail);
		if(rd == xEOF)
			return xEOF;
		m_inflateStream->next_in = (unsigned char*) m_inflateBuff.getRawData();
		m_inflateStream->avail_in = rd;
	}
	err = ::inflate(m_inflateStream, Z_SYNC_FLUSH);

	switch (err)
	{
	case Z_OK:
		break;

	case Z_STREAM_END:
		//The stream ended
		break;

	case Z_DATA_ERROR:
		throw xDataFormatException(_T("Corrupted or invalid ZStream"));
	case Z_BUF_ERROR:
		// Indicates that zlib was expecting more data, but the parent stream
		// has none.
		throw xDataFormatException(_T("Unexpected end of stream"));
	default:
		throw xIOException(_T("Unexpected error while inflating"));
	}
	m_inflateBuffAvail = m_inflateStream->avail_in;
	return (int) buff;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xZLibInputStream::available()
{
	if(m_inflateBuffAvail > 0 || m_in.available() > 0)
		return 1;
		
	return 0;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xZLibOutputStream::xZLibOutputStream(xOutputStream* out, xZLibConst::CompressionLevel compressionLevel,
	xZLibConst::HeaderType headerType,int deflateBuffSize)
	: xFilterOutputStream(out),m_deflateBuff(deflateBuffSize)
{
	m_headerType = headerType;
	
	m_deflateStream = new z_stream_s;
	memset(m_deflateStream, 0, sizeof(z_stream_s));
	
	if(compressionLevel == xZLibConst::COMPRESSION_DEFAULT)
		m_compressionLevel = Z_DEFAULT_COMPRESSION;
	else
		m_compressionLevel = compressionLevel;
		
	int windowBits = MAX_WBITS;
	switch (headerType) 
	{
	case xZLibConst::HEADER_DISABLE:	windowBits = -MAX_WBITS; break;
	case xZLibConst::HEADER_ZLIB:       windowBits = MAX_WBITS; break;
	case xZLibConst::HEADER_GZIP:       windowBits = MAX_WBITS | ZSTREAM_GZIP; break;
	case xZLibConst::HEADER_AUTO:       windowBits = MAX_WBITS | ZSTREAM_AUTO; break;
	default:							windowBits = MAX_WBITS | ZSTREAM_AUTO; break;
	}
	
	::deflateInit2(m_deflateStream, m_compressionLevel, Z_DEFLATED, windowBits,
		8, Z_DEFAULT_STRATEGY);
		
	m_deflateStream->next_out = m_deflateBuff.getRawData();
	m_deflateStream->avail_out = m_deflateBuff.size();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xZLibOutputStream::~xZLibOutputStream()
{
	close();
	::deflateEnd(m_deflateStream);
	delete m_deflateStream;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xZLibOutputStream::close() throw(xIOException)
{
	if(!isClosed())
	{
		//call deflate with Z_FINISH to close the stream
		m_deflateStream->next_in = NULL;
		m_deflateStream->avail_in = 0;
		int err = Z_OK;
		
		while (err == Z_OK) 
		{
			if (m_deflateStream->avail_out == 0) 
			{
				m_out.write(m_deflateBuff);
				m_deflateStream->next_out = m_deflateBuff.getRawData();
				m_deflateStream->avail_out = m_deflateBuff.size();
			}

			err = ::deflate(m_deflateStream, Z_FINISH);
		}
		
		flush();
		m_out.close();
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xZLibOutputStream::flush() throw(xIOException)
{
	if(m_deflateStream->avail_in == 0 && (int)m_deflateStream->avail_out < m_deflateBuff.size())
		m_out.write(m_deflateBuff,0,m_deflateBuff.size() - m_deflateStream->avail_out);
	
	m_out.flush();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xZLibOutputStream::write(xArray<xbyte>& a, int off, int len) throw(xIOException,xIndexOutOfBoundsException)
{
	if(len <= 0 || off+len > a.size() || off < 0)
		throw xIndexOutOfBoundsException();

	int err = Z_OK;
	m_deflateStream->next_in = (unsigned char *)a.getRawData() + off;
	m_deflateStream->avail_in = len;
	
	while (err == Z_OK && m_deflateStream->avail_in > 0) 
	{
		if (m_deflateStream->avail_out == 0) 
		{
			m_out.write(m_deflateBuff);
			m_deflateStream->next_out = m_deflateBuff.getRawData();
			m_deflateStream->avail_out = m_deflateBuff.size();
		}

		err = ::deflate(m_deflateStream, Z_SYNC_FLUSH);
	}

	if(err != Z_OK)
		throw xIOException(_T("Error during deflating"));
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xZLibOutputStream::write(int b) throw(xIOException)
{
	int err = Z_OK;
	m_deflateStream->next_in = (unsigned char *)&b;
	m_deflateStream->avail_in = 1;

	while (err == Z_OK && m_deflateStream->avail_in > 0) 
	{
		if (m_deflateStream->avail_out == 0) 
		{
			m_out.write(m_deflateBuff);

			m_deflateStream->next_out = m_deflateBuff.getRawData();
			m_deflateStream->avail_out = m_deflateBuff.size();
		}

		err = ::deflate(m_deflateStream, Z_NO_FLUSH);
	}

	if(err != Z_OK)
		throw xIOException(_T("Error during deflating"));
}


}//namespace


#endif //XTK_USE_ZLIB
