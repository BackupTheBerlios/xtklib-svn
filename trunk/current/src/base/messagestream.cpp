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
* @file messagestream.cpp
* @author Mario Casciaro (xshadow@email.it)
*/


#include "../../include/xtk/base/stream.h"
#include "../../include/xtk/base/thread.h"

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xMessageInputStream::xMessageInputStream(xInputStream* in) 
: xDataInputStream(new xByteArrayInputStream(0)),m_rawIn(*in)
{
	m_dataIn = new xDataInputStream(in);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xMessageInputStream::~xMessageInputStream()
{
	if(isOwner())
		delete &m_rawIn;
		
	//this for let parent object to delete its xByteArrayInputStream
	giveOwnership();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xMessageInputStream::receive() throw(xIOException)
{
	try
	{
		xi32 size = m_dataIn->readI32();
		xArray<xbyte>* array = new xArray<xbyte>((int)size);
		m_dataIn->readFully(*array);
		delete &m_in;
		xByteArrayInputStream* bais = new xByteArrayInputStream(array);
		bais->giveOwnership();
		m_in = *(bais);
		return true;
	}
	catch(xEOFException&)
	{
		return false;
	}
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
xMessageOutputStream::xMessageOutputStream(xOutputStream* out)
 : xDataOutputStream(new xByteArrayOutputStream()),m_rawOut(*out)
{
	m_dataOut = new xDataOutputStream(&m_rawOut);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xMessageOutputStream::~xMessageOutputStream()
{
	if(isOwner())
		delete &m_rawOut;
	
	//to let parent class to delete its xByteArrayOutputStream
	giveOwnership();	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xMessageOutputStream::send() throw(xIOException)
{
	xSynchronized s(m_out);
	
	xByteArrayOutputStream* baos = dynamic_cast<xByteArrayOutputStream*>(&m_out);
	m_dataOut->writeI32(baos->size());
	baos->writeTo(m_rawOut);
	m_rawOut.flush();
	
	baos = new xByteArrayOutputStream();
	m_out = *(baos);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xMessageOutputStream::cancel()
{
	m_out = *(new xByteArrayOutputStream());
}



}//namespace
