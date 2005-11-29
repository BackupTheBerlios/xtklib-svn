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
* @file stream.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/stream.h"

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xInputStream::read(xArray<xbyte>& a,int off, int len)
throw(xIOException,xIndexOutOfBoundsException)
{
	if(len <= 0 || off+len > a.size()  || off < 0)
		throw xIndexOutOfBoundsException();
	
	int firstElement = read();
	if(firstElement == -1)
		return -1;
	
	a[off] = (xbyte) firstElement;
	int avail = available();
	int i = 1;
	for(;(i < avail) && (i < len);i++)
	{
		a[i + off] = (xbyte) read();
	}
	
	return i;
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xOutputStream::write(xArray<xbyte>& b, int off, int len)
throw(xIOException,xIndexOutOfBoundsException)
{
	if(len <= 0 || off+len > b.size())
		throw xIndexOutOfBoundsException();
		
	for(int i = off;i < off + len;i++)
	{
		write(b[i]);	
	}
	flush();
}
	
}//namespace


