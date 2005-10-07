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
* @file charstream.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_CHARSTREAM_H
#define XTK_CHARSTREAM_H

#include "../globals.h"
#include "interfaces.h"
#include "charset.h"
#include "stream.h"

namespace xtk
{

/**
 * Abstract class for reading character streams.
 */
class XTKAPI xReader : public virtual xObject
{
public:
	virtual ~xReader(){}
	
	/**
	 * Closes the char stream
	 */
	virtual void close() {}

	virtual bool isClosed() {return false;}
	
	/**
	 * Test if the end of stream is reached
	 */
	virtual bool eof() = 0;
	
	/**
	 * Read a single char from the stream. Before use this function
	 * test eof().
	 */
	virtual xchar readChar() throw(xIOException) = 0;
	
	/**
	 * Read a specified amount of character from the stream.
	 * Before use this function test eof().
	 */
	virtual xString read(int len) throw(xIOException);
	
	/**
	 * 
	 */
	virtual int read(xArray<xchar> a,int off,int len) throw(xIOException,xIndexOutOfBoundsException);
	
	/**
	* Mark current position
	*/
	virtual void mark(int readAheadLimit = 1000) throw(xIOException){}

	/**
	* Rewind the stream to the last created mark. This function fails if the
	* read chars from the last mark are superior of the buffer size.
	* 
	* @exception xIOException
	*/
	virtual void rewind() throw(xIOException){}

	/**
	* 
	*/
	virtual bool isMarkSupported(){return false;}
};



/**
 * Abstract class for writing to character streams.
 */
class XTKAPI xWriter : public virtual xObject
{
public:
	virtual ~xWriter(){}
	
	/**
	* Closes the char stream
	*/
	virtual void close() {}

	virtual bool isClosed() {return false;}
	
	virtual void flush() throw(xIOException){}
	
	/**
	 * Write a single char to the stream
	 */
	virtual void writeChar(xchar c) throw(xIOException) = 0;
	
	/**
	 * Write a string into the stream
	 */
	virtual void write(xString s) throw(xIOException);
	
	virtual void write(xArray<xchar> a,int off,int len) throw(xIOException,xIndexOutOfBoundsException);
};


/**
 * Abstract class for reading filtered character streams.
 */
class XTKAPI xFilterReader : public xReader,public xOwnership
{
protected:
	xReader&	m_in;

public:
	xFilterReader(xReader* in) : m_in(*in){};

	virtual ~xFilterReader()
	{
		close();
		if(isOwner())
			delete &m_in;
	}

	virtual void close() {m_in.close();}
	virtual bool isClosed() {return m_in.isClosed();}
	virtual bool eof() {return m_in.eof();}
	virtual xchar readChar() throw(xIOException){return m_in.readChar();}
	virtual xString read(int len) throw(xIOException){return m_in.read(len);}
	virtual int read(xArray<xchar> a,int off,int len) throw(xIOException,xIndexOutOfBoundsException)
	{return m_in.read(a,off,len);}
	virtual void mark(int readAheadLimit = 1000) throw(xIOException){m_in.mark(readAheadLimit);}
	virtual void rewind() throw(xIOException){m_in.rewind();}
	virtual bool isMarkSupported(){return m_in.isMarkSupported();}
};



/**
 * Abstract class for writing filtered character streams.
 */
class XTKAPI xFilterWriter : public xWriter,public xOwnership
{
protected:
	xWriter&	m_out;

public:
	xFilterWriter(xWriter* out) : m_out(*out){}
	
	virtual ~xFilterWriter()
	{
		flush();
		close();
		if(isOwner())
			delete &m_out;
	}


	virtual void close() {m_out.close();}
	virtual bool isClosed() {return m_out.isClosed();}
	virtual void flush() throw(xIOException){m_out.flush();}
	virtual void writeChar(xchar c) throw(xIOException){m_out.writeChar(c);}
	virtual void write(xString s) throw(xIOException){m_out.write(s);}
	virtual void write(xArray<xchar> a,int off,int len) throw(xIOException,xIndexOutOfBoundsException)
	{m_out.write(a,off,len);}
}; 
 

/**
 * Read text from a character-input stream, buffering characters.
 */
class XTKAPI xBufferedReader : public xFilterReader
{
private:
	xArray<xchar>	m_buffer;
	int				m_fill;
	int 			m_mark;
	int				m_position;
	int				m_markReadAhead;

	void refillBuffer();
public:
	xBufferedReader(xReader* in, int buffSize = 1000);
	virtual ~xBufferedReader(){}

	virtual bool eof();
	virtual xchar readChar() throw(xIOException);
	virtual xString read(int len) throw(xIOException);
	virtual int read(xArray<xchar> a,int off,int len) throw(xIOException,xIndexOutOfBoundsException);
	virtual void mark(int readAheadLimit = 1000) throw(xIOException);
	virtual void rewind() throw(xIOException);

	/**
	* Return true for this class.
	*/
	virtual bool isMarkSupported(){return true;}
	
	/**
	* Reads the next line of text from the input stream. The accepted lf are
	* the \\n,\\r,\\r\\n.Before use this function test eof().
	*/
	virtual xString readLine() throw(xIOException);

	/**
	* Reads the text from the input stream until the string limit is not found,
	* then returns the read string without the limit string.
	* Before use this function test eof().
	*/
	virtual xString readToken(xString separatorChars) throw(xIOException);
};




/**
 * Write text to a character-output stream, buffering characters so as to provide for 
 * the efficient writing
 */
class XTKAPI xBufferedWriter : public xFilterWriter
{
private:
	xArray<xchar>		m_buffer;
	int					m_position;

public:
	xBufferedWriter(xWriter* out,int buffSize = 1000);

	virtual ~xBufferedWriter(){close();}

	virtual void close();
	virtual void flush() throw(xIOException);
	virtual void writeChar(xchar c) throw(xIOException);
	virtual void write(xString s) throw(xIOException);
	virtual void write(xArray<xchar> a,int off,int len) throw(xIOException,xIndexOutOfBoundsException);
};
 
/**
 * An InputStreamReader is a bridge from byte streams to character streams: 
 * It reads bytes and decodes them into characters using a specified charset.
 */
class XTKAPI xInputStreamReader : public xReader,public xOwnership
{
protected:
	xInputStream&		m_in;
	xCharsetDecoder		m_dec;

private:
	int		m_eofTestByte;
	bool	m_isEofTestByteValid;
	
public:
	xInputStreamReader(xInputStream* in,xCharset::Charset charset) : m_in(*in),m_dec(charset)
	{
		m_isEofTestByteValid = false;
	}
	
	virtual ~xInputStreamReader()
	{
		close();
		if(isOwner())
			delete &m_in;
	}
	
	virtual void close() {m_in.close();}
	virtual bool isClosed() {return m_in.isClosed();}
	virtual bool eof();
	virtual xchar readChar() throw(xIOException);
};


/**
 * An OutputStreamWriter is a bridge from character streams to byte streams: 
 * Characters written to it are encoded into bytes using a specified charset.
 */
class XTKAPI xOutputStreamWriter : public xWriter,public xOwnership
{
protected:
	xOutputStream&		m_out;
	xCharsetEncoder		m_enc;
	
public:
	xOutputStreamWriter(xOutputStream* out,xCharset::Charset charset) : m_out(*out),m_enc(charset)
	{}
	
	virtual ~xOutputStreamWriter()
	{
		close();
		if(isOwner())
			delete &m_out;
	}
	

	virtual void close() {m_out.flush();m_out.close();}
	virtual bool isClosed() {return m_out.isClosed();}
	virtual void flush() throw(xIOException){m_out.flush();}


	virtual void writeChar(xchar c) throw(xIOException);
	virtual void write(xString s) throw(xIOException);
	virtual void write(xArray<xchar> a,int off,int len) throw(xIOException,xIndexOutOfBoundsException);
};



}//namespace

#endif //XTK_CHARSTREAM_H
