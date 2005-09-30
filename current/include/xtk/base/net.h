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
* @file net.h
* @author Mario Casciaro (xshadow@email.it)
*/



#ifndef XTK_NET_H
#define XTK_NET_H

#include "../globals.h"
#include "string.h"
#include "exception.h"
#include "stream.h"

#ifdef XTK_OS_WINDOWS
	typedef unsigned int XLSOCKET;
#elif defined(XTK_OS_UNIX)
	typedef int XLSOCKET;
#endif

namespace xtk
{

/**
 * This is the superclass for all Internet addresses representations
 */
class XTKAPI xInetAddress : public xObject
{
public:
	virtual ~xInetAddress(){}

    /**
    * Set the address for this object.
	* This function permits to do an automatic check of given address
	* and recognize if it's an ip address or a hostname.
	*
    * @param addr [in] ip address or hostname
    */
    virtual void setByName(xString address) throw(xUnknownHostException) = 0;

	/**
	 * Set the address providing an already translated in network order address.
	 * @exception xUnknownHostException
	 */
	virtual void setByAddress(xArray<xbyte>& address) = 0;
	
    /** 
    * Get the network representation of the address
    */
    virtual xArray<xbyte>& getAddress() = 0;
    
    /**
     * Gets the host name for this IP address.
     */
    virtual xString getHostName() = 0;
    
    /**
     * Returns the IP address string in textual presentation.
     */
	virtual xString getHostAddress() = 0;
	
	/**
	 * Set the current IP address to localhost address.
	 */
	virtual void setLocalHost() = 0;
};


/**
 * @brief Represent an IPv4 address
 */
class XTKAPI xInet4Address : public xInetAddress
{
protected:
    //unsigned long m_ipv4Addr;
    xArray<xbyte> m_ipv4Addr;
    
    /** 
	* Convert a string representation of the address, in network data
	* representation
	*
    * @param addr [in] ip address
    * @param inadd [out] inet address read
    * @return True if successful, False if fails
    */
    static bool inetAddr(const xchar* addr,unsigned long& inadd);
    
public:
	/**
	 * Initializes the IP address with the given hostname or textual
	 * representation of IP address
	 */
	xInet4Address(xString address) throw(xUnknownHostException);
	
	/**
	 * Initializes the IP address with the given raw IP address 
	 */
	xInet4Address(xArray<xbyte>& address) throw(xUnknownHostException);
	
	xInet4Address(xInet4Address& copy);
	
	virtual ~xInet4Address(){}
	
	virtual void setByName(xString address) throw(xUnknownHostException);
	virtual void setByAddress(xArray<xbyte>& address);
	
	virtual xArray<xbyte>& getAddress()
	{
		return m_ipv4Addr;
	}
	
	/**
	 * @todo to implement
	 */
	virtual xString getHostName()
	{
		throw xNotImplementedException();
	}
	
	virtual xString getHostAddress();
	
	virtual void setLocalHost()
	{
		setByName(_T("127.0.0.1"));
	}
	
	virtual int hashCode()
	{
		return *((int*) m_ipv4Addr.getRawData());
	}
	
	virtual xString toString()
	{
		return getHostAddress();
	}
	
	virtual bool equals(xInet4Address& obj)
	{
		return m_ipv4Addr.equals(obj.m_ipv4Addr);
	}
	
	virtual bool equals(xObject& obj)
	{
		xInet4Address* a = dynamic_cast<xInet4Address*>(&obj);
		if(a == NULL)
			return false;
			
		return equals(*a);
	}
};


/** 
* @brief This class implements client sockets
*/
class XTKAPI xSocket : public xObject
{
friend class xServerSocket;
friend class xSocketInputStream;
friend class xSocketOutputStream;
private:
    bool 				m_isClosed;
    bool 				m_isClosedRecv;
    bool 				m_isClosedSend;
    bool 				m_isConnected;
    bool 				m_isBound;
    XLSOCKET 			m_sockd;
    
	/**
	 * Create the object with an already connected socket descriptor.This is
	 * generally used by the xServerSocket class.
	 */
	xSocket(XLSOCKET sock);
public:
    /**
    * Creates an unconnected socket.
    * @exception xSystemException
    */
    xSocket();
    

    virtual ~xSocket();
    
    /**  
    * Connect the socket to a peer
    *
    * @param addr [in] The connection endpoint
    * @param port [in] connection port of the endpoint
    * @param timeout [in] connection timeout in seconds
    */
    virtual void connect(xInetAddress& addr, unsigned short port,unsigned int timeout = 3)
    throw(xIOException,xIOTimeoutException);
    
    /**  
    * Connect the socket to a peer
    *
    * @param addr [in] The connection endpoint in ip address or hostname
    * @param port [in] connection port of the endpoint
    * @param timeout [in] connection timeout in seconds
    */
    virtual void connect(xString addr, unsigned short port,unsigned int timeout = 3)
    throw(xIOException,xUnknownHostException,xIOTimeoutException);
    
    /**  
    * Binds the socket to a local address.
    *
    * @param port [in] the local port
    */
    virtual void bind(unsigned short port) throw(xIOException);
    
	/**  
	* Binds the socket to a local address.
	*
	* @param port [in] the local port.
	* @param addr [in] The local address.
	*/
	virtual void bind(unsigned short port,xInetAddress& addr) throw(xIOException);
	
    /**  
    * Close this socket
    */
    virtual void close();
    
    /**  
    * Further receives are disallowed.
    * If you read from a socket input stream after invoking 
    * shutdownInput() on the socket, the stream will return EOF.
    */
    virtual void shutdownInput();
    
    /**  
    * Further sends are disallowed
    */
    virtual void shutdownOutput();
    
    /**  
    * Get endpoint connection port. Returns 0 if the socket is not connected
    */
    virtual unsigned short getPeerPort();
    
    /**  
    * Get endpoint address. Returns NULL if the socket is not connected
    */
    virtual xInetAddress* getPeerAddress();
    
    /**  
    * Get the binding port of current socket.Returns 0 if the socket is not boud
    */
    virtual unsigned short getBindPort();
    
    /**  
    * Get the binding address of current socket
    */
    virtual xInetAddress* getBindAddress();
    
    /**
     * 
     */
    virtual xOutputStream* getOutputStream() throw(xIOException);
    
	/**
	 * 
	 */
	virtual xInputStream* getInputStream() throw(xIOException);
       
    bool isClosed()
	{
        return m_isClosed;
	}    
    
    bool isConnected()
    {
        return m_isConnected;
    }    
    
    bool isInputShutdown()
    {
        return m_isClosedRecv;
    }    
    
    bool isOutputShutdown()
    {
        return m_isClosedSend;
    }
    
    
	static void initialize();
	static void finalize();
};

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################

/** 
* @brief This class represents a socket for sending 
* and receiving datagram packets.
* @todo To Implement
*/
class XTKAPI xDatagramSocket
{
    
};    

//###################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # #
//###################################################

/** 
* @brief This class implements server sockets
*
* If you want to use this class you must call the static xSocket::start() 
* function before, and the xSocket::end() function after the use.
*/
class XTKAPI xServerSocket : public xObject
{
private:
    bool m_closed;
    bool m_listening;
    bool m_bound;
    int m_backlog;
    
    XLSOCKET m_sockd;
    
    void createSocket();
    
public:
    
    /** 
    * Creates an unbound server socket
    */
    xServerSocket() throw(xSystemException);

	/** 
 	* Creates and bind a server socket.
    */
	xServerSocket(unsigned short port,xInetAddress& addr,int backlog = 10) throw(xIOException,xSystemException);
    xServerSocket(unsigned short port,int backlog = 10) throw(xIOException,xSystemException);
    
    virtual ~xServerSocket();
    
    /**  
 	* Binds the socket to a local address.
 	*
    * @param port [in] the specified port, or 0 to use any free port.
    * @param backlog [in] the maximum length of the queue.
    */
    virtual void bind(unsigned short port,int backlog = 10) throw(xIOException);
    
    /**
     * Binds the socket to a local address.
     *
	 * @param port [in] the specified port, or 0 to use any free port.
	 * @param addr [in] The local address.
	 * @param backlog [in] the maximum length of the queue.
     */
    virtual void bind(unsigned short port,xInetAddress& addr,int backlog = 10) throw(xIOException);
    
    /**  
    * Listens for a connection to be made to this socket and accepts it. 
    * The method blocks until a connection is made.
    *
	* @param timeout [in] timeout in seconds after witch the function return,
	* or 0 for infinite wait.
    */
    virtual xSocket* accept(int timeout = 0) throw(xIOException,xIOTimeoutException);
    
    /**  
    * Close this socket
    */
    virtual void close();
    
    
    bool isClosed()
    {
        return m_closed; 
    }    
    
    bool isBound()
    {
        return m_bound;   
    }
};
	
}//namespace

#endif //XTK_NET_H
