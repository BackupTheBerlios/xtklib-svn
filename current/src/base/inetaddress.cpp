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
* @file inetaddress.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/net.h"
#include "../../include/xtk/base/typewrappers.h"

#ifdef XTK_OS_WINDOWS
	#include <winsock.h>

#elif defined(XTK_OS_UNIX)
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <fcntl.h>
	#include <sys/ioctl.h>
#endif


namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xInet4Address::xInet4Address(xString address) throw(xUnknownHostException) : m_ipv4Addr(4) 
{
	setByName(address);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xInet4Address::xInet4Address(xArray<xbyte>& address) throw(xUnknownHostException) : m_ipv4Addr(4)
{
	setByAddress(address);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xInet4Address::xInet4Address(xInet4Address& copy) : m_ipv4Addr(4)
{
	setByAddress(copy.m_ipv4Addr);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xInet4Address::inetAddr(const xchar* addr,unsigned long& inadd)
{
	//control the ip address passed
	bool valid = true;
	xchar num[4];
	int i = 0;
	int j = 0;
	int k = 0;

	while(1)
	{
		if(j > 3 || i > 15 )
		{
			valid = false;
			break;   
		}
		else
		{
			if(addr[i] != _T('.') && addr[i] != _T('\0'))
			{
				num[j] = addr[i];
				i++;
				j++;
			}
			else
			{
				num[j] = _T('\0');
				long result;
				if(!xLong::parseLong(num,&result))	
				{
					valid = false;
					break;
				}
					
				if(result < 0 || result > 255)
				{
					valid = false;
					break;
				}
				valid = true;
				j=0;
				k++;
				i++;
				if(k == 4)
					break;
			}     
		}//end else       
	}//end while

	//this is a valid ip address
	if((valid == true) && (k == 4))
	{
		inadd = ::inet_addr(xString(addr).mb_str());
		return true;
	}
	else
		return false;    
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xInet4Address::setByName(xString address) throw(xUnknownHostException)
{
	if(!inetAddr(address.c_str(),*((unsigned long*) m_ipv4Addr.getRawData())))
	{
		struct hostent* h;
		h = ::gethostbyname(address.mb_str());
		if(h == NULL)
		{
			throw xUnknownHostException( xString::getFormat(_T("Unknown host: %s"),address.c_str()) );
		}
		else
		{
			m_ipv4Addr.copyData( (xbyte*) &(((struct in_addr*)(h->h_addr_list[0]))->s_addr),0,4 );
		}
	}    
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xInet4Address::setByAddress(xArray<xbyte>& address)
{
	xArray<xbyte>::copy(m_ipv4Addr,0,address,0,4);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xInet4Address::getHostAddress()
{
	struct in_addr addr;
	memcpy(&(addr.s_addr),m_ipv4Addr.getRawData(),4);
	return xString(::inet_ntoa(addr),xCharset::CS_SYSTEM);
}

}//namespace
