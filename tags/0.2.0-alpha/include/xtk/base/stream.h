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
* @file stream.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_STREAM_H
#define XTK_STREAM_H

#include "../globals.h"
#include "array.h"
#include "smartptr.h"

#define	xEOF	-1


namespace xtk
{
	
/**
 * This abstract class is the superclass of all classes representing an input 
 * stream of bytes.
 */
class XTKAPI xInputStream : public virtual xObject
{
public:
	virtual ~xInputStream(){}
	
	/**
	 * Returns the number of bytes that can be read 
	 * from this input stream without blocking by the next caller of a method 
	 * for this input stream.
	 */
	virtual int available() = 0;
	
	/**
	 * Reads the next byte of data from the input stream.
	 *  The value byte is returned as an int in the range 0 to 255. 
	 * If no byte is available because the end of the stream has been reached, 
	 * the value xEOF is returned. This method blocks until input data is 
	 * available, the end of the stream is detected, or an exception is thrown.
	 * 
	 * @exception xIOException
	 */
	virtual int read() throw(xIOException) = 0;
	
	/**
	 * Reads some number of bytes from the input stream and stores them into 
	 * the buffer array. The number of bytes actually read is returned or xEOF.
	 * This method blocks until input data is available, 
	 * end of file is detected, or an exception is thrown.
	 */
	virtual int read(xArray<xbyte>& a) throw(xIOException)
	{
		return read(a,0,a.size());
	}
	
	/**
	 * Same behavior of others read,reads up to len bytes of data from the 
	 * input stream and place them beginning from the a[off] byte.
	 */
	virtual int read(xArray<xbyte>& a,int off, int len) throw(xIOException,xIndexOutOfBoundsException);
	
	/**
	 * Close the stream and release its resources.
	 */
	virtual void close() throw(xIOException){}
	
	virtual bool isClosed() {return false;}
	
	/**
	 * Mark current position
	 */
	virtual void mark(int readAheadLimit = 1000) throw(xIOException){}
	
	/**
	 * Rewind the stream to the last created mark. This function fails if the
	 * read bytes from the last mark are superior of the buffer size.
	 * 
	 * @exception xIOException
	 */
	virtual void rewind() throw(xIOException){}
	
	/**
	 * 
	 */
	virtual bool isMarkSupported(){return false;}
	
	/**
	 * Skips over and discards n bytes of data from this input stream.
	 * @returns the actual number of bytes skipped.
	 */
	virtual int skip(int len)
	{
		int i;
		for( i = 0;i < len;i++)
		{
			if(read() == xEOF)
				break;
		}
		return i;
	}
};



/**
 * This abstract class is the superclass of all classes representing an 
 * output stream of bytes. 
 */
class XTKAPI xOutputStream : public virtual xObject
{
public:
	virtual ~xOutputStream(){}
	
	/**
	 * Closes this output stream and releases its system
	 */
	virtual void close() throw(xIOException) {flush();}
	
	virtual bool isClosed() {return false;}
	
	/**
	 * Flushes this output stream.
	 */
 	virtual void flush() throw(xIOException){}
    
    /**
     * Writes b.size() bytes from the specified byte array to this output stream.
     */
	virtual void write(xArray<xbyte>& b) throw(xIOException)
	{
		write(b,0,b.size());
	}
	
    /**
     * Writes len bytes from the specified byte array starting at offset off 
     * to this output stream.
     * 
     * @exception xIndexOutOfBoundsException
     */
	virtual void write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException);
    
    /**
     * Writes the specified byte to this output stream.
     */
	virtual void write(int b) throw(xIOException) = 0;
};

/**
 * This class represent a stream where you can perform both read and write,
 * the use of this class has sense only if the stream has a shared channel
 * for read and writing(for example a file and not for a socket).
 * 
 */
class XTKAPI xIOStream : public virtual xInputStream,public virtual xOutputStream
{
public:
	virtual ~xIOStream(){}
	
	virtual void close() throw(xIOException){flush();}
	virtual bool isClosed() {return false;}
};


/**
 * A xFilterInputStream uses another stream as its basic source of data, 
 * possibly transforming the data along the way or providing additional 
 * functionality.
 */
class XTKAPI xFilterInputStream : public xInputStream,public xOwnership
{
protected:
	xInputStream& 	m_in;

public: 
	xFilterInputStream(xInputStream* in) : m_in(*in){}
	
	virtual ~xFilterInputStream()
	{
		close();
		if(isOwner())
			delete &m_in;
	}
	
	
	virtual int available(){return m_in.available();}
	virtual int read() throw(xIOException) {return m_in.read();}
	virtual int read(xArray<xbyte>& a,int off, int len) throw(xIOException,xIndexOutOfBoundsException) 
	{return m_in.read(a,off,len);}
	virtual void close() throw(xIOException) {m_in.close();}
	virtual bool isClosed() {return m_in.isClosed();}
	virtual void mark(int readAheadLimit = 1000) throw(xIOException) {m_in.mark(readAheadLimit);}
	virtual bool isMarkSupported() {return m_in.isMarkSupported();}
	virtual void rewind() throw(xIOException) {m_in.rewind();}
};
	

/**
 * A xFilterOutputStream uses another stream as its basic sink of data, 
 * possibly transforming the data along the way or providing additional 
 * functionality.
 */
class XTKAPI xFilterOutputStream : public xOutputStream,public xOwnership
{
protected:
	xOutputStream& 	m_out;

public: 
	xFilterOutputStream(xOutputStream* out) : m_out(*out){}
	virtual ~xFilterOutputStream()
	{
		close();
		if(isOwner())
			delete &m_out;
	}
	
	virtual void close() throw(xIOException){m_out.flush(); m_out.close();}
	virtual bool isClosed() {return m_out.isClosed();}
 	virtual void flush() throw(xIOException){m_out.flush();}
	virtual void write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException) 
	{m_out.write(b,off,len);}
	virtual void write(int b) throw(xIOException) {m_out.write(b);}
};


/**
 * A xFilterIOStream uses another IO stream as its basic source and sink of data
 * possibly transforming the data along the way or providing additional 
 * functionality.
 *
 * @bug found a bug on __writeBack when called from dtor, consider also removing this class
 */
class XTKAPI xFilterIOStream : virtual public xIOStream,virtual public xOwnership
{
protected:
	xIOStream&		m_io;

public:
	xFilterIOStream(xIOStream* io) : m_io(*io){}
	virtual ~xFilterIOStream()
	{
		close();
		if(isOwner())
			delete &m_io;
	}
	
	virtual void close() throw(xIOException) {m_io.flush(); m_io.close();}
	virtual bool isClosed() {return m_io.isClosed();}
 	virtual void flush() throw(xIOException) {m_io.flush();}
 	virtual int available(){return m_io.available();}
	virtual int read() throw(xIOException) {return m_io.read();}
	virtual int read(xArray<xbyte>& a) throw(xIOException) {return m_io.read(a);}
	virtual int read(xArray<xbyte>& a,int off, int len) throw(xIOException,xIndexOutOfBoundsException) 
	{return m_io.read(a,off,len);}
	virtual void write(xArray<xbyte>& b) throw(xIOException) {m_io.write(b);}
	virtual void write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException) 
	{m_io.write(b,off,len);}
	virtual void write(int b) throw(xIOException) {m_io.write(b);}
};


/**
 * A data input stream lets an application read primitive data types from an 
 * underlying input stream
 * 
 */
class XTKAPI xDataInputStream : public xFilterInputStream
{
public:
	xDataInputStream(xInputStream* in) : xFilterInputStream(in){}
	virtual ~xDataInputStream(){}
	
	/**
	 * @exception EOFException
     * @exception IOException
	 */
	virtual xi8 readI8() throw(xIOException,xEOFException);
	
	/**
	 * @exception EOFException
     * @exception IOException
	 */
	virtual xi16 readI16() throw(xIOException,xEOFException);
	
	/**
	 * @exception EOFException
     * @exception IOException
	 */
	virtual xi32 readI32() throw(xIOException,xEOFException);
	
	/**
	 * @exception EOFException
     * @exception IOException
	 */
	virtual xi64 readI64() throw(xIOException,xEOFException);
	
	#ifdef XTK_UNICODE
		/**
		* @exception EOFException
		* @exception IOException
		*/
		virtual xString readUTF8() throw(xIOException,xEOFException);
	#endif

	/**
	 * @exception EOFException
     * @exception IOException
	 */
	virtual void readFully(xArray<xbyte>& b) throw(xIOException,xEOFException);
	
	/**
	 * @exception EOFException
     * @exception IOException
     * @xIndexOutOfBoundsException
	 */
	virtual void readFully(xArray<xbyte>& b, int off, int len) 
	throw(xIOException,xEOFException,xIndexOutOfBoundsException);
	
	/**
	 * @exception EOFException
     * @exception IOException
     * 
	 */
	virtual xArray<xbyte>* readData() throw(xIOException,xEOFException);
};


/**
 * A data output stream lets an application write primitive data types to an 
 * underlying input stream
 * 
 */
class XTKAPI xDataOutputStream : public xFilterOutputStream
{
public:
	xDataOutputStream(xOutputStream* out) : xFilterOutputStream(out){}
	virtual ~xDataOutputStream(){}
	
	/**
     * @exception IOException
	 */
	virtual void writeI8(xi8 d) throw(xIOException);
	
	/**
     * @exception IOException
	 */
	virtual void writeI16(xi16 d) throw(xIOException);
	
	/**
     * @exception IOException
	 */
	virtual void writeI32(xi32 d) throw(xIOException);
	
	/**
     * @exception IOException
	 */
	virtual void writeI64(xi64 d) throw(xIOException);
	
	#ifdef XTK_UNICODE
		/**
		* @exception IOException
		*/	
		virtual void writeUTF8(xString str) throw(xIOException);
	#endif
	
	/**
     * @exception IOException
	 */	
	virtual void writeData(xArray<xbyte>& b) throw(xIOException);
	
	/**
     * @exception IOException
     * @xIndexOutOfBoundsException
	 */
	virtual void writeData(xArray<xbyte>& b, int off, int len) 
	throw(xIOException,xIndexOutOfBoundsException);
};


/**
 * An object that permits to read from an internal buffer of bytes.
 */
class XTKAPI xByteArrayInputStream : public xInputStream,public xDefaultRescindedOwnership
{
protected:
	xArray<xbyte>& 	m_buffer;
	int				m_length;
	int				m_position;
	
public:
	xByteArrayInputStream(xArray<xbyte>* buff);
	
	/**
	 * @exception xIndexOutOfBoundsException
	 */
	xByteArrayInputStream(xArray<xbyte>* buff, int off, int len) throw(xIndexOutOfBoundsException);
	
	virtual ~xByteArrayInputStream();
	

	virtual int available();
	virtual int read() throw(xIOException);
	virtual int read(xArray<xbyte>& a) throw(xIOException);
	virtual int read(xArray<xbyte>& a,int off, int len) throw(xIOException,xIndexOutOfBoundsException);
	
	virtual void setBuffer(xArray<xbyte>* buff);
	
	/**
	 * @exception xIndexOutOfBoundsException
	 */
	virtual void setBuffer(xArray<xbyte>* buff, int off, int len) throw(xIndexOutOfBoundsException);
};


/**
 * An object that permits to write to an internal buffer of bytes.
 */
class XTKAPI xByteArrayOutputStream : public xOutputStream
{
protected:
	xArray<xbyte> 	m_buffer;
	int				m_position;
	int				m_incrementMultiplier;
	
public:
	xByteArrayOutputStream(int initialCapacity = 32);
	virtual ~xByteArrayOutputStream(){}
	
	virtual int size();

	virtual void write(xArray<xbyte>& b) throw(xIOException);
	virtual void write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException);
	virtual void write(int b) throw(xIOException);
	
	virtual void writeTo(xOutputStream& out) throw(xIOException);
	
	/**
	 * Creates a newly allocated byte array and the contents of the buffer 
	 * have been copied into it.
	 */
	virtual xArray<xbyte>* toByteArray();
};


/**
 * A xBufferedInputStream permits to buffer the data for fast access and
 * for supporting mark-rewind operations
 */
class XTKAPI xBufferedInputStream : public xFilterInputStream
{
private:
	xArray<xbyte>		m_buffer;
	int					m_fill;
	int 				m_mark;
	int					m_position;
	int					m_markReadAhead;

	void refillBuffer();
	int read0(xArray<xbyte>& a,int off, int len);
public:
	xBufferedInputStream(xInputStream* in,int bufferSize = 3000) ;
	virtual ~xBufferedInputStream(){}

	virtual int available();
	virtual int read() throw(xIOException);
	virtual int read(xArray<xbyte>& a) throw(xIOException);
	virtual int read(xArray<xbyte>& a,int off, int len) throw(xIOException,xIndexOutOfBoundsException);
	
	virtual void mark(int readAheadLimit = 1000) throw(xIOException);
	virtual void rewind() throw(xIOException);
	
	/**
	 * Returns true.
	 */
	virtual bool isMarkSupported(){return true;}
};

/**
 * A xBufferedOutputStream permits to buffer the data for fast write
 */
class XTKAPI xBufferedOutputStream : public xFilterOutputStream
{
private:
	xArray<xbyte>		m_buffer;
	int					m_position;

	void close0();
public:
	xBufferedOutputStream(xOutputStream* out,int bufferSize = 3000) ;
	virtual ~xBufferedOutputStream();

	virtual void close() throw(xIOException);
 	virtual void flush() throw(xIOException);
	virtual void write(xArray<xbyte>& b) throw(xIOException);
	virtual void write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException) ;
	virtual void write(int b) throw(xIOException);
};

/**
 * Permits to read data from an xInputStream in an atomic mode. Every atom
 * is called Message and represent an amount of data.
 */
class XTKAPI xMessageInputStream : public xDataInputStream
{
protected:
	smartPtr<xDataInputStream>			m_dataIn;
	xInputStream&						m_rawIn;
	
public:
	xMessageInputStream(xInputStream* in);
	virtual ~xMessageInputStream();
	
	/**
	 * Receive the next message. Returns false if the EOF is reached and
	 * so no new message will be read.
	 */
	virtual bool receive() throw(xIOException);
};

/**
 * Permits to send data in an atomic mode.
 */
class XTKAPI xMessageOutputStream : public xDataOutputStream
{
protected:
	smartPtr<xDataOutputStream>			m_dataOut;
	xOutputStream&						m_rawOut;
	
	
public:
	xMessageOutputStream(xOutputStream* out);
	virtual ~xMessageOutputStream();
	
	/**
	 * Send the data as a Message and reset the buffer
	 */
	virtual void send() throw(xIOException);
	
	/**
	 * Cancel the writing of the current message and reset the buffer
	 */
	virtual void cancel();
};

}//namespace



#endif //XTK_STREAM_H
