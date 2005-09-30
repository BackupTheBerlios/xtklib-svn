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
* @file charset.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/charset.h"
#include "../../include/xtk/base/array.h"
#include <wchar.h>

#ifdef XTK_OS_WINDOWS
	#include <windows.h>
#elif defined(XTK_OS_UNIX)
	#include <iconv.h>
#endif

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
static xCharset::Charset gs_charsets[] = 
{	
	xCharset::CS_WCHAR_T,
	xCharset::CS_SYSTEM,
	
	xCharset::CS_ISO8859_1,
	xCharset::CS_ISO8859_2,
	xCharset::CS_ISO8859_3,
	xCharset::CS_ISO8859_4,
	xCharset::CS_ISO8859_5,
	xCharset::CS_ISO8859_6,
	xCharset::CS_ISO8859_7,
	xCharset::CS_ISO8859_8,
	xCharset::CS_ISO8859_9,
	xCharset::CS_ISO8859_10,
	xCharset::CS_ISO8859_11,
	xCharset::CS_ISO8859_12,
	xCharset::CS_ISO8859_13,
	xCharset::CS_ISO8859_14,
	xCharset::CS_ISO8859_15,
	xCharset::CS_UTF7,
	xCharset::CS_UTF8,
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
static const xchar* gs_charsetsDesc[] = 
{
	_T("wchar_t pseudo charset"),
	_T("System default charset"),
	
	_T("Western European (ISO-8859-1)"),
	_T("Central European (ISO-8859-2)" ),
	_T("Esperanto (ISO-8859-3)" ),
	_T("Baltic (old) (ISO-8859-4)" ),
	_T("Cyrillic (ISO-8859-5)" ),
	_T("Arabic (ISO-8859-6)" ),
	_T("Greek (ISO-8859-7)" ),
	_T("Hebrew (ISO-8859-8)" ),
	_T("Turkish (ISO-8859-9)" ),
	_T("Nordic (ISO-8859-10)" ),
	_T("Thai (ISO-8859-11)" ),
	_T("Indian (ISO-8859-12)" ),
	_T("Baltic (ISO-8859-13)" ),
	_T("Celtic (ISO-8859-14)" ),
	_T("Western European with Euro (ISO-8859-15)" ),
	_T("Unicode 7 bit (UTF-7)"),
	_T("Unicode 8 bit (UTF-8)"),
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
static const char* gs_charsetsName[] =
{
	"wchar_t",
	"System charset",
	
	"iso-8859-1",
	"iso-8859-2",
	"iso-8859-3",
	"iso-8859-4",
	"iso-8859-5",
	"iso-8859-6",
	"iso-8859-7",
	"iso-8859-8",
	"iso-8859-9",
	"iso-8859-10",
	"iso-8859-11",
	"iso-8859-12",
	"iso-8859-13",
	"iso-8859-14",
	"iso-8859-15",
	"utf-7",
	"utf-8",
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
static unsigned int gs_charsetsCP[] =
{
	0,
	28591,
	
	28591,
	28592,
	28593,
	28594,
	28595,
	28596,
	28597,
	28598,
	28599,
	28600,
	28601,
	28602,
	28603,
	28604,
	28605,
	65000,
	65001,
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int findCharsetIndex(xCharset::Charset cs)
{
	//find charset index
	bool found = false;
	int i = 0;
	for(;i < (int)sizeof(gs_charsets);i++)
	{
		if(gs_charsets[i] == cs)
		{
			found = true;
			break;
		}
	}
	if(!found)
		return -1;
		
	return i;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
const char* xCharset::getName(Charset cs)
{
	int index = findCharsetIndex(cs);
	//if not found return SYSTEM by default
	if(index == -1)
		return gs_charsetsName[1];
	
	return gs_charsetsName[index];
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
unsigned int xCharset::getCodepage(Charset cs)
{
	int index = findCharsetIndex(cs);
	//if not found return SYSTEM by default
	if(index == -1)
		return gs_charsetsCP[1];
		
	return gs_charsetsCP[index];
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xCharset::getDescription(Charset cs)
{
	int index = findCharsetIndex(cs);
	//if not found return SYSTEM by default
	if(index == -1)
		return gs_charsetsDesc[1];

	return gs_charsetsDesc[index];
}

#ifdef XTK_UNICODE
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xCharsetEncoder::encode(OUT xArray<xbyte>& outBuf,const xchar* inBuf,int inLen)
{
#ifdef XTK_UNICODE
	outBuf.copyOnWrite();
	#define	RESIZE_INCREMENT_LEN 64
	
	if(m_charset == xCharset::CS_WCHAR_T) //symply copy the data
	{
		int len = (int)(inLen * sizeof(xchar));
		outBuf.resize(len);
		outBuf.copyData((const xbyte*)inBuf,0,len);
		return true;
	}
	else if(m_charset == xCharset::CS_SYSTEM) //use the libc functions
	{
		mbstate_t state;
		memset(&state,0,sizeof(state));
		//::mbsinit(&state);
		size_t written = 0;
		for(register int i = 0; i < inLen;i++)
		{
			//if not enough space left, resize the array
			if((outBuf.size() - (int) written) < (int)MB_CUR_MAX)
				outBuf.resize(outBuf.size() + RESIZE_INCREMENT_LEN);
				
			size_t res = wcrtomb((char*) (outBuf.getRawData() + written),inBuf[i],&state);
			if(res == (size_t) -1)
				return false;
			
			written += res;
		}
		outBuf.resize((int)written);
	}
	else //not CS_SYSTEM
	{
		#ifdef XTK_OS_WINDOWS
			//BOOL usedDefChar;
			int len = WideCharToMultiByte(xCharset::getCodepage(m_charset),0,
				inBuf,inLen,NULL,0,NULL,NULL);
			
			if(inLen > 0 && len == 0)
			{	
				//DWORD err = GetLastError();
				return false;
			}
				
			outBuf.resize(len);
			len = WideCharToMultiByte(xCharset::getCodepage(m_charset),0,
				inBuf,inLen,(LPSTR) outBuf.getRawData(),outBuf.size(),
				NULL,NULL);
			
			/*
			if(usedDefChar || (inLen > 0 && len == 0))
			{	
				DWORD err = GetLastError();

				return false;
			}
			*/
			
			return true;
			
		#elif defined(XTK_OS_UNIX)
			iconv_t cd = iconv_open(xCharset::getName(m_charset),"WCHAR_T");

			if(cd == (iconv_t) -1)
			{
				//try with another charset
				cd = iconv_open(xCharset::getName(m_charset),"UTF16");
				if(cd == (iconv_t) -1)
					return false;
			}

			//perform the conversion
			#define ICONV_BUFF_SIZE 100
			char tbuf[ICONV_BUFF_SIZE];
			size_t len = 0;
			char* pszPtr = (char*) inBuf;
			size_t inLeftSize = inLen * sizeof(xchar);

			for(;;)
			{
				char* bufPtr = tbuf;
				size_t outSize = ICONV_BUFF_SIZE;

				size_t cres = iconv(cd,&pszPtr, &inLeftSize,&bufPtr,&outSize);
				
				if(cres == (size_t)-1 && errno != E2BIG)
					return false; 
				
				
				outBuf.resize(len + ICONV_BUFF_SIZE - outSize);
				outBuf.copyData((const xbyte*) tbuf,len,ICONV_BUFF_SIZE - outSize);
				len += ICONV_BUFF_SIZE - outSize;

				if(cres != (size_t)-1)
					break;
			}
		#endif
	}
#endif// XTK_UNICODE
	return true;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xCharsetEncoder::encode(OUT xArray<xbyte>& outBuf,xArray<xchar>& inBuf)
{
	return encode(outBuf,inBuf.getRawData(),inBuf.size());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xCharsetEncoder::encode(OUT xArray<xbyte>& outBuf,xString inBuf)
{
	return encode(outBuf,inBuf.c_str(),(int)inBuf.length());
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xCharsetDecoder::decode(OUT xArray<xchar>& outBuf,const xbyte* inBuf,int inLen)
{
#ifdef XTK_UNICODE
	outBuf.copyOnWrite();
	#define	RESIZE_INCREMENT_LEN 64
	
	if(m_charset == xCharset::CS_WCHAR_T)
	{
		//if the len is not a multiple of sizeof(xchar) 
		if((inLen % sizeof(xchar)) != 0)
			return false;
			
		int len = (int)inLen / sizeof(xchar);
		outBuf.resize(len);
		outBuf.copyData((const xchar*)inBuf,0,len);
		return true;
	}
	else if(m_charset == xCharset::CS_SYSTEM)
	{
		//use the libc functions
		mbstate_t state;
		memset(&state,0,sizeof(state));
		//mbsinit(&state);
		int consumed = 0;
		int i;
		for(i = 0;i < inLen;i++)
		{
			xchar wbuff;
			size_t res = mbtowc(&wbuff,(const char*)inBuf + consumed,inLen - consumed);
			if(res >= (size_t) -2)
				return false;
			
			if(i >= outBuf.size())
				outBuf.resize(outBuf.size() + RESIZE_INCREMENT_LEN);
			
			outBuf[i] = wbuff;	
			consumed += (int) res;
		}
		outBuf.resize(i);
	}
	else //not CS_SYSTEM
	{
		#ifdef XTK_OS_WINDOWS
			int len = MultiByteToWideChar(xCharset::getCodepage(m_charset),MB_ERR_INVALID_CHARS,
				(LPCSTR) inBuf,inLen,NULL,0);

			//error in translation
			if(inLen > 0 && len == 0)
				return false;

			outBuf.resize(len);
			MultiByteToWideChar(xCharset::getCodepage(m_charset),MB_ERR_INVALID_CHARS,
				(LPCSTR) inBuf,inLen,outBuf.getRawData(),outBuf.size());

		#elif defined(XTK_OS_UNIX)
			iconv_t cd = iconv_open(xCharset::getName(m_charset),"WCHAR_T");

			if(cd == (iconv_t) -1)
			{
				//try with another charset
				cd = iconv_open(xCharset::getName(m_charset),"UTF16");
				if(cd == (iconv_t) -1)
					return false;
			}

			//perform the conversion
			#define ICONV_BUFF_SIZE 100
			xchar tbuf[ICONV_BUFF_SIZE];
			size_t len = 0;
			char* pszPtr = (char*) inBuf;
			size_t inLeftSize = inLen;

			for(;;)
			{
				char* bufPtr = (char*) tbuf;
				size_t outSize = ICONV_BUFF_SIZE * sizeof(xchar);

				size_t cres = iconv(cd,&pszPtr, &inLeftSize,&bufPtr,&outSize);
				
				if(cres == (size_t)-1 && errno != E2BIG)
					return false; 
					
				int wcharWritten = (outSize / sizeof(xchar));
				outBuf.resize(len + ICONV_BUFF_SIZE - wcharWritten);
				outBuf.copyData(tbuf,len,ICONV_BUFF_SIZE - wcharWritten);
				len += ICONV_BUFF_SIZE - wcharWritten;

				if(cres != (size_t)-1)
					break;
			}
		#endif
	}
#endif // XTK_UNICODE
	return true;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xCharsetDecoder::decode(OUT xArray<xchar>& outBuf,xArray<xbyte>& inBuf)
{
	return decode(outBuf,inBuf.getRawData(),inBuf.size());
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xCharsetDecoder::decode(OUT xString& outBuf,const xbyte* inBuf,int inLen)
{
	xArray<xchar> t(0);
	if(!decode(t,inBuf,inLen))
		return false;
		
	outBuf.clear();
	outBuf.append(t.getRawData(),t.size());
	
	return true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xCharsetDecoder::decode(OUT xString& outBuf,xArray<xbyte>& inBuf)
{
	xArray<xchar> t(0);
	if(!decode(t,inBuf))
		return false;

	outBuf.clear();
	outBuf.append(t.getRawData(),t.size());

	return true;
}




#endif //XTK_UNICODE

}//namespace
