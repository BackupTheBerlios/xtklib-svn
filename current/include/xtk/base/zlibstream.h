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
* @file zlibstream.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_ZLIBSTREAM_H
#define XTK_ZLIBSTREAM_H

#include "../globals.h"
#include "array.h"
#include "stream.h"

//forward
struct z_stream_s;

namespace xtk
{

#ifdef XTK_USE_ZLIB

/**
 * @brief A class containing some constants for use in xZLibInputStream,xZLibOutputStream
 */
class XTKAPI xZLibConst
{
private:
	xZLibConst(){};
	
public:
	// Compression level
	enum CompressionLevel
	{
		COMPRESSION_DEFAULT = -1,
		COMPRESSION_STORE = 0,
		COMPRESSION_FAST = 1,
		COMPRESSION_NORMAL = 5,
		COMPRESSION_BEST = 9
	};

	// Header type
	enum HeaderType
	{
		HEADER_DISABLE = 0,    // raw deflate stream, no header or checksum
		HEADER_ZLIB = 1,         // zlib header and checksum
		HEADER_GZIP = 2,         // gzip header and checksum, requires zlib 1.2.1+
		HEADER_AUTO = 3          // autodetect header zlib or gzip
	};
};


/**
 * This filter stream decompresses a stream that is in zlib or gzip format.
 * It decompresses a single stream or a sequence of streams in that format.
 * For top efficiency, consider wrapping a xZLibInputStream within a xBufferedInputStream.
 *
 * @todo Riesce davvero a gestire stream consecutivi?Da migliorare.
 */
class XTKAPI xZLibInputStream : public xFilterInputStream
{
private:
	xZLibConst::HeaderType	m_headerType;
	struct z_stream_s*		m_inflateStream;
	xArray<xbyte>			m_inflateBuff;
	int						m_inflateBuffAvail;
	
public:
	xZLibInputStream(xInputStream* in,xZLibConst::HeaderType headerType = xZLibConst::HEADER_AUTO,int inflateBuffSize = 3000);
	virtual ~xZLibInputStream();
	
	/**
	* Return 0 if no bytes are available,returns 1 if some bytes are available
	*/
	virtual int available();
	virtual int read() throw(xIOException,xDataFormatException);
	virtual int read(xArray<xbyte>& a,int off, int len) throw(xIOException,xIndexOutOfBoundsException,xDataFormatException);
};


/**
 * This filter stream compresses a stream in zlib or gzip format.
 * For top efficiency, consider wrapping a xZLibOutputStream within a xBufferedOutputStream.
 */
class XTKAPI xZLibOutputStream : public xFilterOutputStream
{
private:
	int								m_compressionLevel;
	xZLibConst::HeaderType			m_headerType;
	xArray<xbyte>					m_deflateBuff;
	struct z_stream_s*				m_deflateStream;
	
public:
	xZLibOutputStream(xOutputStream* out,
		xZLibConst::CompressionLevel compressionLevel = xZLibConst::COMPRESSION_DEFAULT,
		xZLibConst::HeaderType headerType = xZLibConst::HEADER_ZLIB,int deflateBuffSize = 3000);

	virtual ~xZLibOutputStream();
	virtual void write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException);
	virtual void write(int b) throw(xIOException);
	virtual void close() throw(xIOException);
	virtual void flush() throw(xIOException);
};



#endif //XTK_USE_ZLIB
	
}//namespace

#endif //XTK_ZLIBSTREAM_H
