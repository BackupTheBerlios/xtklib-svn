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
* @file serversocket.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/net.h"

#ifdef XTK_OS_WINDOWS
	#include <winsock.h>
	#define XTK_INVALID_SOCKET INVALID_SOCKET

#elif defined(XTK_OS_UNIX)
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <fcntl.h>
	#include <sys/ioctl.h>

	#define XTK_INVALID_SOCKET -1
#endif


namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xServerSocket::xServerSocket() throw(xSystemException)
{
	createSocket();
}
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xServerSocket::xServerSocket(unsigned short port,xInetAddress& addr,int backlog) 
throw(xIOException,xSystemException)
{
	createSocket();
	bind(port,addr,backlog);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xServerSocket::xServerSocket(unsigned short port,int backlog) throw(xIOException,xSystemException)
{
	createSocket();
	bind(port,backlog);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
 void xServerSocket::createSocket()
 {
	 m_closed = false;
	 m_listening = false;
	 m_bound = false;

	 m_sockd = (XLSOCKET) ::socket(AF_INET,SOCK_STREAM,0);
	 if(m_sockd == XTK_INVALID_SOCKET)
	 {
		 throw xSystemException(_T("Unable to create socket"));
	 }

	 //Switch socket to non-blocking
	#ifdef XTK_OS_WINDOWS
		unsigned long arg = 1; //nonzero, nonblocking enabled
		::ioctlsocket(m_sockd,FIONBIO,&arg);
	#else //unix
		int flags = ::fcntl(m_sockd,F_GETFL,0);
		::fcntl(m_sockd, F_SETFL, flags | O_NONBLOCK);
	#endif
 }
 
 //##############################################################################
 //# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
 //##############################################################################
xServerSocket::~xServerSocket()
{
	if(!isClosed())
		close();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xServerSocket::bind(unsigned short port,int backlog) throw(xIOException)
{
	if(isBound())
		throw xIOException(_T("Socket already bound"));
	if(isClosed())
		throw xIOException(_T("Cannot bind socket, socket closed"));

	struct sockaddr_in address;

	address.sin_family = AF_INET;         // host byte order
	address.sin_port = htons(port);     // short, network byte order
	address.sin_addr.s_addr = INADDR_ANY; 
	memset(&(address.sin_zero), '\0', 8); // zero the rest of the struct
	
	if(backlog > 0)
		m_backlog = backlog;
	else
		m_backlog = 10;

	int res = ::bind(m_sockd,(struct sockaddr*)&address, sizeof(address));
	if(res != 0)
	{
		m_bound = false;
		throw xIOException(xString::getFormat(_T("Cannot bind socket on port %hu"),port));
	}    

	m_bound = true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xServerSocket::bind(unsigned short port,xInetAddress& addr,int backlog) throw(xIOException)
{
	if(isBound())
		throw xIOException(_T("Socket already bound"));
	if(isClosed())
		throw xIOException(_T("Cannot bind socket, socket closed"));

	struct sockaddr_in address;

	address.sin_family = AF_INET;         // host byte order
	address.sin_port = htons(port);     // short, network byte order
	memcpy(&address.sin_addr.s_addr,addr.getAddress().getRawData(),4);
	memset(&(address.sin_zero), '\0', 8); // zero the rest of the struct

	if(backlog > 0)
		m_backlog = backlog;
	else
		m_backlog = 10;

	int res = ::bind(m_sockd,(struct sockaddr*)&address, sizeof(address));
	if(res != 0)
	{
		m_bound = false;
		throw xIOException(xString::getFormat(_T("Cannot bind socket on port %hu"),port));
	}    

	m_bound = true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xSocket* xServerSocket::accept(int timeout) throw(xIOException,xIOTimeoutException)
{
	if(!isBound())
		throw xIOException(_T("Cannot listen for new connections, socket not bound"));
	if(isClosed())
		throw xIOException(_T("Cannot listen for new connections, socket is closed"));

	if(!m_listening)
	{//if the connection queue is not already created
		if(::listen(m_sockd,m_backlog) != 0)
		{
			throw xIOException(_T("Cannot listen for new connections, unknown error"));
		}
		else
		{
			m_listening = true;   
		}
	}

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(m_sockd, &readfds);

	int res = 0;
	if(timeout == 0)
	{
		//when a connection is ready, readfds is set and 
		//then accept will be successful
		res = ::select(m_sockd + 1,&readfds,NULL,NULL,NULL);
	}
	else
	{
		struct timeval tv;
		tv.tv_sec = timeout;
		tv.tv_usec = 0;
		//when a connection is ready, readfds is set and 
		//then accept will be successful
		res = ::select(m_sockd + 1,&readfds,NULL,NULL,&tv);
	}


	switch(res)
	{
	case 0:
		throw xIOTimeoutException();
	default:
		if(FD_ISSET(m_sockd, &readfds))
		{
			//a connection is pending so we accept it
			struct sockaddr address;
			#ifdef XTK_OS_WINDOWS
				int size = sizeof(address);
				XLSOCKET new_sockd = (XLSOCKET) ::accept(m_sockd, &address, &size);
			#else 
				XLSOCKET new_sockd = ::accept(m_sockd, &address, (socklen_t*)sizeof(address));
			#endif

			if(new_sockd == XTK_INVALID_SOCKET)
				throw xIOException(_T("Cannot accept new connections, unknown error"));
			else
				return new xSocket(new_sockd);
		}
		else
		{
			throw xIOException(_T("Cannot accept new connections, unknown error"));
		}
		break;
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xServerSocket::close()
{
	if(!isClosed())
	{
		#ifdef XTK_OS_WINDOWS
			::closesocket(m_sockd);
		#else 
			::close(m_sockd);
		#endif

		m_closed = true;   
	}
}


}//namespace
