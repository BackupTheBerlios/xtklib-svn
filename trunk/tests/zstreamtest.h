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
* @file zstreamtest.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#include "xtk/xtk.h"
using namespace xtk;

#include <stdio.h>
#include <wchar.h>


class xZStreamTest : public xTest
{
private:
	virtual void doTest()
	{
		xFile fin(_T("testzlib.txt"));
		xFile fout(_T("testzlib.txt.gz"));
		fout.trim();
		xArray<xbyte> buff(50000);
		int read = 0;
		
		xZLibOutputStream zout(fout.getOutputStream(50000),xZLibConst::COMPRESSION_BEST,xZLibConst::HEADER_GZIP);
		smartPtr<xFileInputStream> fins = fin.getInputStream(50000);
		profile(
		while((read = fins->read(buff)) != xEOF )
		{
			zout.write(buff,0,read);
		}
		zout.close();
		,1,"profile compression");
		
		testManualStart("Start decompression");
		//now decompress
		xZLibInputStream zin(fout.getInputStream(),xZLibConst::HEADER_GZIP);
		xFile fout2(_T("testzlib2.txt"));
		smartPtr<xFileOutputStream> fos = fout2.getIOStream();
		read = 0;
		while((read = zin.read(buff,0,buff.size())) != xEOF )
		{
			fos->write(buff,0,read);
		}
		testOK();
		
	}

public:
	xZStreamTest() : xTest("xZStreamTest Test"){}
};
