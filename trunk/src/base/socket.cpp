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
* @file socket.cpp
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
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xSocketInputStream : public xInputStream
{
private:
	xSocket&	m_socket;
	
public:
	xSocketInputStream(xSocket* socket) : m_socket(*socket){}
	virtual ~xSocketInputStream(){}
	
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	virtual int available()
	{
		char* buffer = (char*) malloc(100000);
		int res = ::recv(m_socket.m_sockd, buffer,100000, MSG_PEEK);
		if(res <= 0)
		{
			return 0;
		}
		
		return res;
	}
	
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	virtual int read() throw(xIOException)
	{
		xArray<xbyte> arr(1);
		if(read(arr,0,1) == xEOF)
			return xEOF;
		return (int) *((xbyte*) arr.getRawData());
	}
	
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	virtual int read(xArray<xbyte>& a,int off, int len) throw(xIOException,xIndexOutOfBoundsException)
	{
		if(len <= 0 || off+len > a.size() || off < 0)
			throw xIndexOutOfBoundsException();
		if(m_socket.isClosed())
			throw xIOException(_T("Cannot read data, socket closed"));
		if(!m_socket.isConnected())
			throw xIOException(_T("Cannot read data, socket not connected"));
		if(m_socket.isInputShutdown())
			throw xIOException(_T("Cannot read data, socket has input shutdown"));

		int res = 0;
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(m_socket.m_sockd, &readfds);

		res = ::select(m_socket.m_sockd + 1,&readfds,NULL,NULL,NULL);

		if(res == -1)
		{
			throw xIOException(_T("Cannot read data from socket, unexpected error"));
		}
		else
		{
			if(FD_ISSET(m_socket.m_sockd, &readfds))
			{
				int res = ::recv(m_socket.m_sockd, (char*) a.getRawData() + off , len, 0);
				if(res < 0)
				{
					m_socket.m_isConnected = false;
					throw xIOException(_T("Cannot read data from socket, unexpected error"));
				}
				else if(res == 0)
					return xEOF;
				
				return res;
			}
			else
			{
				throw xIOException(_T("Cannot read data from socket, unexpected error"));
			}
		}
	}
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xSocketOutputStream : public xOutputStream
{
private:
	xSocket&	m_socket;

public:
	xSocketOutputStream(xSocket* socket) : m_socket(*socket){}
	virtual ~xSocketOutputStream(){}
	
	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	virtual void write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException)
	{
		if(len <= 0 || off+len > b.size())
			throw xIndexOutOfBoundsException();
		if(m_socket.isClosed())
			throw xIOException(_T("Cannot write data, socket closed"));
		if(!m_socket.isConnected())
			throw xIOException(_T("Cannot write data, socket not connected"));
		if(m_socket.isOutputShutdown())
			throw xIOException(_T("Cannot write data, socket has output shutdown"));

		int totalSent = 0;

		//loop until all data are sent
		while(totalSent < len)
		{
			fd_set readfds,writefds;
			FD_ZERO(&readfds);
			FD_SET(m_socket.m_sockd, &readfds);
			FD_ZERO(&writefds);
			FD_SET(m_socket.m_sockd, &writefds);

			int res = ::select(m_socket.m_sockd + 1,&readfds,&writefds,NULL,NULL);

			if(res == -1)
			{
				throw xIOException(_T("Unexpected error while writing on socket"));
			}
			else
			{
				if(FD_ISSET(m_socket.m_sockd, &readfds))
				{
					char c;
					if(::recv(m_socket.m_sockd, &c, 1, MSG_PEEK) <= 0)
					{
						m_socket.m_isConnected = false;
						throw xIOException(_T("Cannot write on socket, socket is not connected"));
					}
				}
				if(FD_ISSET(m_socket.m_sockd, &writefds))
				{
					int sent = ::send(m_socket.m_sockd,(char*)b.getRawData() + totalSent + off,len - totalSent,0);
					if(sent < 0)
						throw xIOException(_T("Unexpected error while writing on socket"));

					totalSent += sent;
				}
			}
		}
	}

	//##############################################################################
	//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	//##############################################################################
	virtual void write(int b) throw(xIOException)
	{
		xArray<xbyte> a((xbyte*) &b,0,1);
		write(a,0,1);
	}
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xSocket::initialize()
{
	#ifdef XTK_OS_WINDOWS
		WSADATA wsaData;   // if this doesn't work
		//WSAData wsaData; // then try this instead

		if(::WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) 
		{
			throw xSystemException(_T("Error calling WSAStartup(...)"));
		}
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xSocket::finalize()
{
	#ifdef XTK_OS_WINDOWS
		::WSACleanup();
	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################

xSocket::xSocket()
{
	m_isClosed = false;
	m_isConnected = false;
	m_isClosedRecv = true;
	m_isClosedSend = true;
	m_isBound = false;
	m_sockd = (XLSOCKET) ::socket(AF_INET,SOCK_STREAM,0);
	if(m_sockd == XTK_INVALID_SOCKET)
	{
		throw xSystemException(_T("Cannot retrieve a socket descriptor"));
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
xSocket::xSocket(XLSOCKET sock)
{
	m_isClosed = false;
	m_isConnected = true;
	m_isClosedRecv = false;
	m_isClosedSend = false;
	m_isBound = true;
	m_sockd = sock;

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
xSocket::~xSocket()
{
	if(!isClosed())
		this->close();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xSocket::connect(xString addr, unsigned short port,unsigned int timeout)
	throw(xIOException,xUnknownHostException,xIOTimeoutException)
{
	xInet4Address address(addr);
	connect(address,port,timeout);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xSocket::connect(xInetAddress& addr, unsigned short port,unsigned int timeout)
	throw(xIOException,xIOTimeoutException)
{
	if(isClosed())
		throw xIOException(_T("Cannot connect: Socket closed"));
	if(isConnected())
		throw xIOException(_T("Cannot connect: Socket already connected"));

	struct sockaddr_in address;

	address.sin_family = AF_INET;       // host byte order
	address.sin_port = htons(port);     // short, network byte order
	memcpy(&address.sin_addr.s_addr,addr.getAddress().getRawData(),4);
	memset(&(address.sin_zero), '\0', 8); // zero the rest of the struct


	fd_set writefds,exceptfds;
	struct timeval tv;
	FD_ZERO(&writefds);
	FD_SET(m_sockd, &writefds);
	FD_ZERO(&exceptfds);
	FD_SET(m_sockd, &exceptfds);

	tv.tv_sec = timeout;
	tv.tv_usec = 0;

	//connect not block
	::connect(m_sockd,(struct sockaddr *)&address, sizeof(address));

	int res = ::select(m_sockd + 1,NULL,&writefds,&exceptfds,&tv);
	if(res == 0)
	{
		//close, because after timeout socket can continue to connect
		close();
		throw xIOTimeoutException();
	}
	else if(res < 0)
	{
		throw xIOException(_T("Unexpected error while connecting"));
	}
	else
	{
		if(FD_ISSET(m_sockd, &exceptfds))
		{
			m_isConnected = false;
			m_isBound = false;
			m_isClosedRecv = true;
			m_isClosedSend = true;
			m_isClosed = false;

			throw xIOException(_T("Unable to connect to the specified host"));
		}    

		if(FD_ISSET(m_sockd, &writefds))
		{
			m_isConnected = true;
			m_isBound = true;
			m_isClosedRecv = false;
			m_isClosedSend = false;
			m_isClosed = false;
		}
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xSocket::bind(unsigned short port,xInetAddress& addr) throw(xIOException)
{
	if(isConnected() || m_isBound)
		throw xIOException(_T("Socket already bound"));
	if(isClosed())
		throw xIOException(_T("Socket closed"));

	struct sockaddr_in address;

	address.sin_family = AF_INET;         // host byte order
	address.sin_port = htons(port);     // short, network byte order
	memcpy(&address.sin_addr.s_addr,addr.getAddress().getRawData(),4);
	memset(&(address.sin_zero), '\0', 8); // zero the rest of the struct

	int res = ::bind(m_sockd,(struct sockaddr *)&address, sizeof(address));
	if(res != 0)
		throw xIOException(xString::getFormat(_T("Unable to bind on port %hu of host %s"),port,addr.toString().c_str()));

	m_isBound = true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xSocket::bind(unsigned short port) throw(xIOException)
{
	if(isConnected() || m_isBound)
		throw xIOException(_T("Socket already bound"));
	if(isClosed())
		throw xIOException(_T("Socket closed"));

	struct sockaddr_in address;

	address.sin_family = AF_INET;         // host byte order
	address.sin_port = htons(port);     // short, network byte order
	address.sin_addr.s_addr = INADDR_ANY;
	memset(&(address.sin_zero), '\0', 8); // zero the rest of the struct

	int res = ::bind(m_sockd,(struct sockaddr *)&address, sizeof(address));
	if(res != 0)
		throw xIOException(xString::getFormat(_T("Unable to bind on port %hu"),port));

	m_isBound = true;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xSocket::close()
{
	if(!isClosed())
	{
		#ifdef XTK_OS_WINDOWS
			::closesocket(m_sockd);
		#else 
			::close(m_sockd);
		#endif

		m_isConnected = false;
		m_isClosedRecv = true;
		m_isClosedSend = true;
		m_isClosed = true;   
	}    
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xSocket::shutdownInput()
{
	::shutdown(m_sockd,0); 
	m_isClosedRecv = true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xSocket::shutdownOutput()
{
	::shutdown(m_sockd,1); 
	m_isClosedSend = true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
unsigned short xSocket::getPeerPort()
{
	struct sockaddr address;
	int res;

	int size = sizeof(address);
	#ifdef XTK_OS_WINDOWS
		res = ::getpeername(m_sockd, &address,&size);
	#else
		res = ::getpeername(m_sockd, &address,(socklen_t*)&size);
	#endif

	if(res != 0)
	{
		return 0;
	}
	else
	{
		struct sockaddr_in* l_sock_addr_in =(struct sockaddr_in*)&address;
		return ntohs(l_sock_addr_in->sin_port); 
	}    
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xInetAddress* xSocket::getPeerAddress()
{
	struct sockaddr address;
	int res;

	int size = sizeof(address);

	#ifdef XTK_OS_WINDOWS
		res = ::getpeername(m_sockd, &address, &size);
	#else
		res = ::getpeername(m_sockd, &address, (socklen_t*)&size);
	#endif 

	if(res != 0)
	{
		return NULL;
	}
	else
	{
		struct sockaddr_in* l_sock_addr_in =(struct sockaddr_in*) &address;
		xArray<xbyte> a((xbyte*) & (l_sock_addr_in->sin_addr.s_addr),0,4);
		return new xInet4Address(a);  
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
unsigned short xSocket::getBindPort()
{
	struct sockaddr address;
	int res;

	int size = sizeof(address);

	#ifdef XTK_OS_WINDOWS
		res = ::getsockname(m_sockd,&address,&size);
	#else
		res = ::getsockname(m_sockd,&address,(socklen_t*) &size);
	#endif

	if(res != 0)
	{
		return 0;
	}
	struct sockaddr_in* l_sock_addr_in =(struct sockaddr_in*) &address;
	return ntohs(l_sock_addr_in->sin_port);    
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xInetAddress* xSocket::getBindAddress()
{
	struct sockaddr address;
	int res;

	int size = sizeof(address);

	#ifdef XTK_OS_WINDOWS
		res = ::getsockname(m_sockd,&address,&size);
	#else
		res = ::getsockname(m_sockd,&address, (socklen_t*)&size);
	#endif

	if(res != 0)
	{
		return false;
	}
	else
	{
		struct sockaddr_in* l_sock_addr_in =(struct sockaddr_in*) &address;
		xArray<xbyte> a( (xbyte*) & (l_sock_addr_in->sin_addr.s_addr),0,4);
		return new xInet4Address(a); 
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xOutputStream* xSocket::getOutputStream() throw(xIOException)
{
	return new xSocketOutputStream(this);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xInputStream* xSocket::getInputStream() throw(xIOException)
{
	return new xSocketInputStream(this);
}



}//namespace
