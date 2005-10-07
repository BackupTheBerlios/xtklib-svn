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
* @file charset.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_CHARSET_H
#define XTK_CHARSET_H

#include "../globals.h"


namespace xtk
{
template<class TClass> class xArray;
class xString;

/**
 * A named mapping between sequences of sixteen-bit Unicode characters 
 * and sequences of bytes.
 */
class XTKAPI xCharset
{
private:
	xCharset(){}
	
public:
	enum Charset
	{
		CS_WCHAR_T = -1,
		CS_SYSTEM = 0,
		
		CS_ISO8859_1 = 1,
		CS_ISO8859_2 = 2,
		CS_ISO8859_3 = 3,
		CS_ISO8859_4 = 4,
		CS_ISO8859_5 = 5,
		CS_ISO8859_6 = 6,
		CS_ISO8859_7 = 7,
		CS_ISO8859_8 = 8,
		CS_ISO8859_9 = 9,
		CS_ISO8859_10 = 10,
		CS_ISO8859_11 = 11,
		CS_ISO8859_12 = 12,
		CS_ISO8859_13 = 13,
		CS_ISO8859_14 = 14,
		CS_ISO8859_15 = 15,
		CS_UTF7 = 16,
		CS_UTF8 = 18,
	};
	
	/**
	 * Return the canonical name of the charset usable 
	 * in the os function calling
	 */
	static NODELETE const char* getName(Charset cs);
	
	/**
	 * Return the codepage for the given charset usable in
	 * the Windows os function calling
	 */
	static unsigned int getCodepage(Charset cs);
	
	/**
	 * Return the description of the given charset
	 */
	static xString getDescription(Charset cs);
};

#ifdef XTK_UNICODE

/**
 * An engine that can transform a sequence of wide Unicode characters into a 
 * sequence of bytes in a specific charset. This class is available only with
 * the flag XTK_USE_UNICODE defined.
 */
class XTKAPI xCharsetEncoder : public virtual xObject
{
private:
	xCharset::Charset	m_charset;
	
public:
	xCharsetEncoder(xCharset::Charset charset)
	: m_charset(charset)
	{}
	
	virtual ~xCharsetEncoder(){}
	
	/**
	 * Encode the given xchar buffer in an array of bytes.
	 * The output buffer will be resized by the function to fit 
	 * its space needs.
	 * Returns true if function succeeded to convert the buffer.
	 *
	 * @warning The output buffer is not guaranteed null terminated
	 *
	 * @todo resolve the problem with WC_NO_BEST_FIT_CHARS and Win95 or UTF8
	 */
	bool encode(OUT xArray<xbyte>& outBuf,const xchar* inBuf,int inLen);
	
	
	/**
	 * Encode the given xchar buffer in an array of bytes.
	 * The output buffer will be resized by the function to fit 
	 * its space needs.
	 * Returns true if function succeeded to convert the buffer
	 *
	 * @warning The output buffer is not guaranteed null terminated
	 */
	bool encode(OUT xArray<xbyte>& outBuf,xArray<xchar>& inBuf);
	
	/**
	 * Encode the given xString in an array of bytes.
	 * The output buffer will be resized by the function to fit 
	 * its space needs.
	 * Returns true if function succeeded to convert the buffer.
	 *
	 * @warning The output buffer is not guaranteed null terminated
	 */
	bool encode(OUT xArray<xbyte>& outBuf,xString inBuf);
}; 
 
 
/**
 * An engine that can transform a sequence of bytes in a specific charset into 
 * a sequence of wide Unicode characters.This class is available only with
 * the flag XTK_USE_UNICODE defined.
 */
class XTKAPI xCharsetDecoder : public virtual xObject
{
private:
	xCharset::Charset	m_charset;

public:
	xCharsetDecoder(xCharset::Charset charset)
	: m_charset(charset)
	{}

	virtual ~xCharsetDecoder(){}
	
	/**
	 * Decode the given byte array in an array of xchars.
	 * The output buffer will be resized by the function to fit 
	 * its space needs.
	 * Returns false if the Unicode translation failed.
	 *
	 * @warning The output buffer is not guaranteed null terminated
	 */
	bool decode(OUT xArray<xchar>& outBuf,const xbyte* inBuf,int inLen);
	
	
	/**
	 * Decode the given byte array in an array of xchars.
	 * The output buffer will be resized by the function to fit 
	 * its space needs.
	 * Returns false if the Unicode translation failed.
	 *
	 * @warning The output buffer is not guaranteed null terminated
	 */
	bool decode(OUT xArray<xchar>& outBuf,xArray<xbyte>& inBuf);

	/**
	 * Decode the given byte array in a xString.
	 * The output buffer will be resized by the function to fit 
	 * its space needs.
	 * Returns false if the Unicode translation failed.
	 */
	bool decode(OUT xString& outBuf,xArray<xbyte>& inBuf);
	
	/**
	 Decode the given byte array in a xString.
	 * The output buffer will be resized by the function to fit 
	 * its space needs.
	 * Returns false if the Unicode translation failed.
	 */
	bool decode(OUT xString& outBuf,const xbyte* inBuf,int inLen);
};

#endif //#ifdef XTK_UNICODE

}//namespace

#endif //XTK_CHARSET_H
