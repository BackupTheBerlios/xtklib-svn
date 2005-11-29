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
* @file file.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/file.h"
#include "../../include/xtk/base/exception.h"
#include "../../include/xtk/base/system.h"
#include "../../include/xtk/base/datastructures.h"


#if defined(XTK_OS_WINDOWS)
	#include <windows.h>
	#include <direct.h>
	#include <io.h>

	//in VC seems some macro are not present
	#ifndef S_ISDIR
		#define	S_ISDIR(m)	(((m) & _S_IFMT) == _S_IFDIR)
	#endif

	#ifndef S_ISREG
		#define	S_ISREG(m)	(((m) & _S_IFMT) == _S_IFREG)
	#endif

#elif defined(XTK_OS_UNIX)
	#include <unistd.h>
	#include <dirent.h>

#endif

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


namespace xtk
{
	
#ifdef XTK_OS_WINDOWS
	const xchar xFile::separator = _T('\\');
#else
	const xchar xFile::separator = _T('/');
#endif

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xConcreteFileInputStream : public xFileInputStream
{
private:
	xFile			m_file;
	int 			m_fileDescriptor;
	xArray<xbyte>	m_buffer;
	xi64 			m_cacheOffBeg;
	xi64 			m_cacheOffEnd;
	xi64			m_position;
	xi64			m_mark;
	bool			m_bClosed;
	
	void fillBuffer();
	int	read0(xArray<xbyte>& a,int off, int len);
protected:
	xConcreteFileInputStream() : m_file(_T("")),m_buffer(0){}
	
public:
	/**
	 * Optimal value for buffer size is between 10k and 60k,default is 30k
	 * 
	 * @throw xFileNotFoundException,xIOException
	 */
	xConcreteFileInputStream(xFile& file,int bufferSize = 30000);
	virtual ~xConcreteFileInputStream();
	virtual void close() throw(xIOException);
	virtual bool isClosed(){return m_bClosed;}
	
	virtual int available();
	virtual int read(xArray<xbyte>& a,int off, int len) throw(xIOException,xIndexOutOfBoundsException);
	virtual int read(xArray<xbyte>& a) throw(xIOException);
	virtual int read() throw(xIOException);

	virtual void mark(int readAheadLimit) throw(xIOException);
	virtual void rewind() throw(xIOException);
	
	virtual xi64 getPosition();
	virtual void setPosition(xi64 pos);
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xConcreteFileOutputStream : public xFileOutputStream
{
private:
	xFile			m_file;
	int 			m_fileDescriptor;
	xArray<xbyte>	m_buffer;
	xi64 			m_cacheOffBeg;
	xi64 			m_cacheOffEnd;
	xi64			m_position;
	bool			m_bClosed;
	
protected:
	xConcreteFileOutputStream() : m_file(_T("")),m_buffer(0){}
	
public:
	/**
	 * Optimal value for buffer size is between 10k and 60k,default is 30k
	 * 
	 * @throw xFileNotFoundException,xIOException
	 */
	xConcreteFileOutputStream(xFile& file,int bufferSize = 30000);
	virtual ~xConcreteFileOutputStream();
	virtual void close() throw(xIOException);
	virtual bool isClosed(){return m_bClosed;}

 	virtual void flush() throw(xIOException);
	virtual void write(xArray<xbyte>& b) throw(xIOException);
	virtual void write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException);
	virtual void write(int b) throw(xIOException);
	
	virtual xi64 getPosition();
	virtual void setPosition(xi64 pos);
};

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
class xConcreteFileIOStream : public xFileIOStream
{
private:
	xFile			m_file;
	int 			m_fileDescriptor;
	bool			m_bClosed;
	xi64 			m_mark;
	
	xArray<xbyte>	m_buffer;
	bool 			m_bCacheWasWritten;
	bool 			m_bCacheValid;
	xi64 			m_iCacheOffBeg;
	xi64 			m_iCacheOffEnd;
	xi64 			m_iCachePointer;
	xi64 			m_iMaxCacheOffset;
	
	// Write back if necessary and read the data from the position of m_iCachePointer
	inline void __cacheUpdate();
	inline void __cacheWriteBack();
protected:
	xConcreteFileIOStream() : m_file(_T("")),m_buffer(0){}
	
public:
	/**
	 * Optimal value for buffer size is between 10k and 60k,default is 30k
	 * 
	 * @throw xFileNotFoundException,xIOException
	 */
	xConcreteFileIOStream(xFile& file,int bufferSize = 30000);
	virtual ~xConcreteFileIOStream();
	virtual void close() throw(xIOException);
	virtual bool isClosed(){return m_bClosed;}

 	virtual void flush() throw(xIOException);
	virtual void write(xArray<xbyte>& b) throw(xIOException);
	virtual void write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException);
	virtual void write(int b) throw(xIOException);
	
	virtual int available();
	virtual int read(xArray<xbyte>& a,int off, int len) throw(xIOException,xIndexOutOfBoundsException);
	virtual int read(xArray<xbyte>& a) throw(xIOException); 
	virtual int read() throw(xIOException);

	virtual void mark(int readAheadLimit) throw(xIOException);
	virtual void rewind() throw(xIOException);
	
	virtual xi64 getPosition();
	virtual void setPosition(xi64 pos);
};


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#if !defined(XTK_OS_WINDOWS) || !defined(XTK_UNICODE)
	int xFile::__open(const char* filename,OpenMode mode)
#else
	int xFile::__open(const xchar* filename,OpenMode mode)
#endif
{
	int openFlags = 0;
	int fileDescriptor = 0;
	switch(mode)
	{
	case xlRead:
		#ifdef XTK_OS_WINDOWS
			openFlags = _O_RDONLY | _O_BINARY | _O_SEQUENTIAL;
			
		#elif defined( XTK_OS_UNIX )
			openFlags = O_RDONLY ;

		#endif
		break;
	case xlWrite:
		#ifdef XTK_OS_WINDOWS
			openFlags = _O_WRONLY | _O_BINARY | _O_SEQUENTIAL | _O_CREAT | _O_APPEND;
		#elif defined(  XTK_OS_UNIX )
			openFlags = O_WRONLY | O_CREAT | O_APPEND;
		#endif
		break;
	case xlReadWrite:
		#ifdef XTK_OS_WINDOWS
			openFlags = _O_RDWR | _O_BINARY | _O_SEQUENTIAL | _O_CREAT;
		#elif defined( XTK_OS_UNIX )
			openFlags = O_RDWR | O_CREAT ;
		#endif
		break;
	default:
		break;
	}
	
	#ifdef XTK_OS_WINDOWS
		#ifndef XTK_UNICODE
			fileDescriptor = ::_open(filename,openFlags,_S_IREAD | _S_IWRITE);
		#else
			fileDescriptor = ::_wopen(filename,openFlags,_S_IREAD | _S_IWRITE);
		#endif

	#elif defined( XTK_OS_UNIX )
		fileDescriptor = ::open64(filename,openFlags,S_IREAD | S_IWRITE);

	#endif

	if(fileDescriptor == -1)
	{
		//now see which error is
		switch(errno)
		{
		case EACCES:
			throw xIOException(xString::getFormat(_T("File %s: access denied"),filename));
		case ENOENT:
			throw xFileNotFoundException(xString::getFormat(_T("File %s not found"),filename)); 
		
		default:
			#ifdef XTK_OS_WINDOWS
				throw xIOException(
					xString::getFormat(_T("Error while opening file %s"),filename),
					::GetLastError()
					); 
			#elif defined(XTK_OS_UNIX)
				throw xIOException(
					xString::getFormat(_T("Error while opening file %s"),filename),
					errno
					); 
			#endif
		}
	}
	
	return fileDescriptor;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xFile::__read(int filedesc,const void* buffer,size_t buffer_size)
{
	#ifdef XTK_OS_WINDOWS
		int res = ::_read(filedesc,(void*) buffer,(unsigned int) buffer_size);
		if(res == -1)
			throw xIOException(_T("Cannot read from file!"),::GetLastError());
			
	#elif defined( XTK_OS_UNIX )
		ssize_t res = ::read(filedesc,(void*) buffer, buffer_size);
		if(res == -1)
			throw xIOException(_T("Cannot read from file!"),errno);
			
	#endif
	
	if(res == 0 && buffer_size!= 0)
		return xEOF;

	return (int) res;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xFile::__write(int filedesc,const void* buffer,size_t buffer_size)
{
	size_t totalWritten = 0;
	while(totalWritten != buffer_size)
	{
		#ifdef XTK_OS_WINDOWS
			int res = ::_write(filedesc,(const void*) buffer,(unsigned int) buffer_size);
			if(res < 0)
				throw xIOException(_T("Cannot write on file!"),::GetLastError());
		#elif defined( XTK_OS_UNIX )
			ssize_t res = ::write(filedesc,(const void*) buffer, buffer_size);
			if(res < 0)
				throw xIOException(_T("Cannot write on file!"),errno);
		#endif
		
		totalWritten += (size_t)res;
	}
	
	return (int) totalWritten;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xi64 xFile::__seek(int filedesc,xi64 offset,SeekMode origin)
{
	int iOrigin = 0;
	switch(origin)
	{
	case xlFromBeginning:
		iOrigin = SEEK_SET;
		break;
	case xlFromEnd:
		iOrigin = SEEK_END;
		break;
	case xlFromCurrent:
		iOrigin = SEEK_CUR;
		break;
	default:
		break;
	}

	#ifdef XTK_OS_WINDOWS
		__int64 res = ::_lseeki64(filedesc,(__int64)offset,iOrigin);
		if(res == -1)
			throw xIOException(_T("Cannot seek!"),::GetLastError());
	#elif defined( XTK_OS_UNIX )
		off64_t res = ::lseek64(filedesc,(off64_t) offset,iOrigin);
		if(res == -1)
			throw xIOException(_T("Cannot seek!"),errno);
	#endif

	

	return (xi64) res;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xFile::__close(int filedesc)
{
	#ifdef XTK_OS_WINDOWS
		::_close(filedesc);

	#elif defined( XTK_OS_UNIX )
		::close(filedesc);
	#endif	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFile::xFile(const xFile& copy) : m_abstractPathName(copy.m_abstractPathName)
{
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFile::xFile(xString pathname) : m_abstractPathName(pathname)
{
	normalize(m_abstractPathName);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFile::xFile(xFile& parent, xString child)
{
	xString& str = parent.getPath();
	m_abstractPathName.append(str);
	m_abstractPathName.append(&separator,1);
	m_abstractPathName.append(child);
	normalize(m_abstractPathName);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFile::xFile(xString parent, xString child)
{
	m_abstractPathName.append(parent);
	m_abstractPathName.append(&separator,1);
	m_abstractPathName.append(child);
	normalize(m_abstractPathName);	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xFile::normalize(xString& str)
{
	#ifdef XTK_OS_WINDOWS
		str.replace(_T('/'),_T('\\'));
	#elif defined (XTK_OS_UNIX)
		str.replace(_T('\\'),_T('/'));
	#endif 
	
	//replace double separator
	xString doubleSep;
	doubleSep.append(&separator,1);
	doubleSep.append(&separator,1);
	
	xString singleSep;
	singleSep.append(&separator,1);
	
	str.replace(doubleSep,singleSep);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
#if !defined(XTK_OS_WINDOWS) || !defined(XTK_UNICODE)
	bool xFile::__checkFileAccess(const char* filename,CheckAccess caccess)
#else
	bool xFile::__checkFileAccess(const xchar* filename,CheckAccess caccess)
#endif
{
	int res = 0;
	int mode = 0;
	switch(caccess)
	{
	case xlCheckExistence:
		#ifdef XTK_OS_WINDOWS
			mode = 0;
		#elif defined (XTK_OS_UNIX)
			mode = F_OK;
		#endif
		break;
	case xlCheckRead:
		#ifdef XTK_OS_WINDOWS
			mode = 4;
		#elif defined (XTK_OS_UNIX)
			mode = R_OK;
		#endif
		break;
	case xlCheckWrite:
		#ifdef XTK_OS_WINDOWS
			mode = 2;
		#elif defined (XTK_OS_UNIX)
			mode = W_OK;
		#endif
		break;
	case xlCheckReadWrite:
		#ifdef XTK_OS_WINDOWS
			mode = 6;
		#elif defined (XTK_OS_UNIX)
			mode = W_OK | R_OK;
		#endif
		break;
	default:
		return false;
	}


	#ifdef XTK_OS_WINDOWS
		#ifndef  XTK_UNICODE
			res = _access(filename, mode);
		#else
			res = _waccess(filename, mode);
		#endif
		
	#elif defined( XTK_OS_UNIX )
		res = access(filename, mode);
	#endif

	if (res != 0)
	{
		return false;
	}

	return true;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xFile::canRead()
{
	return __checkFileAccess(m_abstractPathName.fn_str(),xlCheckRead);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xFile::canWrite()
{
	return __checkFileAccess(m_abstractPathName.fn_str(),xlCheckWrite);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xFile::createNewFile()
{
	int iNewFileDescriptor = 0;
	//try to open the new file
	#ifdef XTK_OS_WINDOWS
		int flags = _O_WRONLY | _O_CREAT | _O_EXCL;
		int pmode = _S_IREAD | _S_IWRITE;
		
		#ifndef XTK_UNICODE
			iNewFileDescriptor = ::_open(m_abstractPathName.c_str(),flags,pmode);
		#else
			iNewFileDescriptor = ::_wopen(m_abstractPathName.c_str(),flags,pmode);
		#endif
		
		if(iNewFileDescriptor == -1)
			throw xIOException(
				xString::getFormat(_T("Cannot create file %s"),m_abstractPathName.c_str()),
				::GetLastError()
				);
	#elif defined( XTK_OS_UNIX )
		int flags = O_WRONLY | O_CREAT | O_EXCL;
		int pmode = S_IREAD | S_IWRITE;
		
		iNewFileDescriptor = ::open64(m_abstractPathName.mb_str(),flags,pmode);

		if(iNewFileDescriptor == -1)
			throw xIOException(
				xString::getFormat(_T("Cannot create file %s"),m_abstractPathName.c_str()),
				errno
				);
	#endif

	::close(iNewFileDescriptor);
	return true;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xFile::exists()
{
	return __checkFileAccess(m_abstractPathName.fn_str(),xlCheckExistence);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xFile::isDirectory()
{
	#ifdef XTK_OS_WINDOWS
		struct _stati64 stats;
		#ifndef XTK_UNICODE
			int res = ::_stati64(m_abstractPathName.c_str(),&stats);
		#else
			int res = ::_wstati64(m_abstractPathName.c_str(),&stats);
		#endif
	

	#elif defined( XTK_OS_UNIX )
		struct stat64 stats;
		int res = ::stat64(m_abstractPathName.mb_str(),&stats);

	#endif

	if(res == -1)
		return false;
	
	return S_ISDIR(stats.st_mode);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xFile::isFile()
{
	#ifdef XTK_OS_WINDOWS
		struct _stati64 stats;
		#ifndef XTK_UNICODE
			int res = ::_stati64(m_abstractPathName.c_str(),&stats);
		#else
			int res = ::_wstati64(m_abstractPathName.c_str(),&stats);
		#endif

	#elif defined( XTK_OS_UNIX )
		struct stat64 stats;
		int res = ::stat64(m_abstractPathName.mb_str(),&stats);

	#endif

	if(res == -1)
		return false;
	
	return S_ISREG(stats.st_mode);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xi64 xFile::size()
{
	#ifdef XTK_OS_WINDOWS
		struct _stati64 stats;
		#ifndef XTK_UNICODE
			int res = ::_stati64(m_abstractPathName.c_str(),&stats);
		#else
			int res = ::_wstati64(m_abstractPathName.c_str(),&stats);
		#endif

	#elif defined( XTK_OS_UNIX )
		struct stat64 stats;
		int res = ::stat64(m_abstractPathName.mb_str(),&stats);

	#endif

	if(res == -1)
	{
		return 0;
	}

	return (xi64) stats.st_size;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFile* xFile::getAbsoluteFile()
{
	if(isAbsolute())
	{
		return dynamic_cast<xFile*>(this->clone());
	}
	else
	{
		xString str = xSystem::getWorkingDir();
		str.append(&separator,1);
		str.append(getPath());
		normalize(str);
		return new xFile(str);
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xFile::getName()
{
	xString str;
	int i;
	for(i = (int)m_abstractPathName.length() - 1;i >= 0; i--)
	{
		if(m_abstractPathName.charAt(i) == separator)
		{
			if(i != (int) m_abstractPathName.length())
				return str;
		}
		else
		{
			xchar tmp = m_abstractPathName.charAt(i);
			str.insert(&tmp, 1,0);
		}
	}
	
	return str;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xFile::trim() throw(xIOException)
{
	#ifdef XTK_OS_WINDOWS
		#ifndef XTK_UNICODE
			int fileDescriptor = ::_open(m_abstractPathName.c_str(),_O_WRONLY | _O_CREAT | _O_TRUNC,_S_IREAD | _S_IWRITE);
		#else
			int fileDescriptor = ::_wopen(m_abstractPathName.c_str(),_O_WRONLY | _O_CREAT | _O_TRUNC,_S_IREAD | _S_IWRITE);
		#endif

		if(fileDescriptor == -1)
			throw xIOException(xString::getFormat(_T("Cannot trim file %s"),m_abstractPathName.mb_str()),::GetLastError());
	#elif defined( XTK_OS_UNIX )
		int fileDescriptor = ::open64(m_abstractPathName.mb_str(),O_WRONLY | O_CREAT | O_TRUNC,S_IREAD | S_IWRITE);

		if(fileDescriptor == -1)
			throw xIOException(xString::getFormat(_T("Cannot trim file %s"),m_abstractPathName.mb_str()),errno);
	#endif

	
	
	__close(fileDescriptor);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFile* xFile::getParent()
{
	xString parent;
	int i = (int)m_abstractPathName.length() - 1;
	
	//skip the first '/' if exists
	if(m_abstractPathName.charAt(i) == separator)
	{
		i--;
	}

	//find the first '/' 
	for(;i >= 0; i--)
	{
		if(m_abstractPathName.charAt(i) == separator)
		{
			break;
		}
	}

	//now extract the updir
	for(int j = 0;j <= i; j++)
	{
		xchar tmp = m_abstractPathName.charAt(j);
		parent.append(&tmp,1);
	}	
	
	
	if(!parent.isEmpty())
	{
		return new xFile(parent);
	}
	else
		return NULL;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xFile::getExtension()
{
	xString extension;
	for(int i = (int)m_abstractPathName.length() - 1;i >= 0; i--)
	{
		if(m_abstractPathName.charAt(i) == separator)
		{
			return _T("");
		}
		else if(m_abstractPathName.charAt(i) == _T('.'))
		{
			if(!extension.isEmpty())
			{
				return extension;	
			}
			else
				return _T("");
		}

		xchar tmp = m_abstractPathName.charAt(i);
		extension.insert(&tmp, 1, 0);
	}
	
	return _T("");
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xFile::remove()
{
	if(exists())
	{
		if(isFile())
		{
			#ifdef XTK_OS_WINDOWS
				#ifndef XTK_UNICODE
					int res = ::_unlink(m_abstractPathName.c_str());
				#else
					int res = ::_wunlink(m_abstractPathName.c_str());
				#endif
		
			#elif defined( XTK_OS_UNIX )
				int res = ::unlink(m_abstractPathName.mb_str());
		
			#endif
		
			if(res == -1)
			{
				switch(errno)
				{
				case EACCES:
					throw xIOException(
					xString::getFormat(_T("Access Denied: file %s cannot be deleted"),m_abstractPathName.c_str())
					);
				default:
					throw xIOException(
					xString::getFormat(_T("Unspecified error while deleting %s"),m_abstractPathName.c_str())
					);
				}
			}	
		}
		else if(isDirectory())
		{
			#ifdef XTK_OS_WINDOWS
				#ifndef XTK_UNICODE
					int res = ::_rmdir(m_abstractPathName.c_str());
				#else
					int res = ::_wrmdir(m_abstractPathName.c_str());
				#endif
		
			#elif defined( XTK_OS_UNIX )
				int res = ::rmdir(m_abstractPathName.mb_str());
		
			#endif
		
			if(res == -1)
			{
				switch(errno)
				{
				case EACCES:
					throw xIOException(
					xString::getFormat(_T("Access Denied: directory %s cannot be deleted"),m_abstractPathName.c_str())
					);
				case ENOTEMPTY:
					throw xFileNotFoundException(
					xString::getFormat(_T("Directory %s cannot be deleted cause it is not empty"),m_abstractPathName.c_str())
					);
				default:
					throw xIOException(
					xString::getFormat(_T("Unspecified error while deleting %s"),m_abstractPathName.c_str())
					);
				}
			}	
		}
	}//exists
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xFile::isAbsolute()
{
	#ifdef XTK_OS_WINDOWS
		return (
			xString::isAlpha(m_abstractPathName.charAt(0)) && 
			m_abstractPathName.charAt(1) == ':'
			);

	#elif defined( XTK_OS_UNIX )
		return m_abstractPathName.charAt(0) == separator;

	#endif
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArray<xString*> xFile::list() throw(xIOException)
{
	xArrayList entlist;
	entlist.rescindOwnership();
	
	#if defined (XTK_OS_WINDOWS)
		xString strWinDir = m_abstractPathName;
		strWinDir.append(_T("\\*"));
		WIN32_FIND_DATA FileData;
		HANDLE hScanDir;
		

		// Get the first file
		hScanDir = ::FindFirstFile(strWinDir.c_str(), &FileData);
		if (hScanDir == INVALID_HANDLE_VALUE)
		{
			throw xIOException(_T("Cannot open directory"),::GetLastError());
		}
		else
		{
			while( ::FindNextFile(hScanDir, &FileData))
			{
				//set the name
				entlist.add(new xString(FileData.cFileName));
			}
			
			if ( ::GetLastError() != ERROR_NO_MORE_FILES)
			{
				::FindClose(hScanDir);
				return xArray<xString*>();
			}
		}
		::FindClose(hScanDir);

	#elif defined (XTK_OS_UNIX)
		DIR* dir = ::opendir(m_abstractPathName.mb_str());

		if(dir == NULL)
			throw xIOException(_T("Cannot open directory"),errno);
		
		struct dirent dirData;
		struct dirent* pDirData;
		while( ::readdir_r(dir, &dirData, &pDirData) == 0)
		{ //success
			//set the name
			entlist.add(new xString(xString::getFromOS(pDirData->d_name)));
		}
		
		::closedir(dir);
	#endif
	
	return entlist.toArray().castTo<xString*>();
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArray<xFile*> xFile::listFiles() throw(xIOException)
{
	xArray<xString*> strList = list();
	xArray<xFile*> fileList(strList.size());
	for(int i = 0;i < strList.size();i++)
		fileList[i] = new xFile(m_abstractPathName,*(strList[i]));
	
	strList.deleteData();
	
	return fileList;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xFile::mkdir()
{
	#if defined (XTK_OS_WINDOWS)
		#ifndef XTK_UNICODE
			int res = _mkdir(m_abstractPathName.c_str());
		#else
			int res = _wmkdir(m_abstractPathName.c_str());
		#endif
	#elif defined (XTK_OS_UNIX)
		int res = ::mkdir(m_abstractPathName.mb_str(),0666);
	#endif
	
	if(res != 0)
	{
		switch(errno)
		{
			case EEXIST:
				return false;
			case ENOENT:
				throw xIOException(xString::getFormat(_T("Path to directory %s not found"),m_abstractPathName.c_str()));
			default:
				throw xIOException(xString::getFormat(_T("Cannot create directory %s"),m_abstractPathName.c_str()));
		}	
	}
	
	return true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xFile::renameTo(xFile dest)
{
	#if !defined(XTK_UNICODE) || !defined(XTK_OS_WINDOWS)
		int res = ::rename(m_abstractPathName.mb_str(),dest.getPath().mb_str());
	#else
		int res = ::_wrename(m_abstractPathName.c_str(),dest.getPath().c_str());
	#endif

	if(res == -1)
		return false;
		
	m_abstractPathName = dest.m_abstractPathName;
	return true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xArray<xFile*> xFile::listRoots()
{
	xArrayList roots;
	roots.rescindOwnership();
	
	#ifdef XTK_OS_WINDOWS
		#define nBufferLength 1000
		TCHAR lpBuffer[nBufferLength];
		
		DWORD res = GetLogicalDriveStrings(nBufferLength,lpBuffer);
		
		//Now format the data
		unsigned int i = 0;
		unsigned int j = 0;
		while(i < res)
		{
		    while(lpBuffer[j] != _T('\0'))
			{
		        j += sizeof(TCHAR);
		    }
		    xString currentRoot(&lpBuffer[i],j - i);
		    roots.add(new xFile(currentRoot));
		    
		    j += sizeof(TCHAR);
		    i = j;
		}
		

	#elif defined( XTK_OS_UNIX )
		roots.add(new xFile(_T("/")));

	#endif
	
	return roots.toArray().castTo<xFile*>();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xFile::compareTo(xObject& o)
{
	xFile* f = dynamic_cast<xFile*>(&o);
	if(f == NULL)
		throw xClassCastException(_T("Can compare only against another xFile"));
		
	return m_abstractPathName.compareTo(f->m_abstractPathName);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xFile::equals(xObject& o)
{
	xFile* f = dynamic_cast<xFile*>(&o);
	if(f == NULL)
		return false;
		
	return m_abstractPathName.equals(f->m_abstractPathName);	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xFile::hashCode()
{
	return 	m_abstractPathName.hashCode();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFileInputStream* xFile::getInputStream(int bufferSize)
{
	return new xConcreteFileInputStream(*this,bufferSize);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFileIOStream* xFile::getIOStream(int bufferSize)
{
	return new xConcreteFileIOStream(*this,bufferSize);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xFileOutputStream* xFile::getOutputStream(int bufferSize)
{
	return new xConcreteFileOutputStream(*this,bufferSize);
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
xConcreteFileInputStream::xConcreteFileInputStream(xFile& file,int bufferSize)
: m_file(file),m_buffer(bufferSize)
{
	m_cacheOffEnd = 0;
	m_cacheOffBeg = 0;
	m_position = 0;
	m_bClosed = false;
	m_mark = -1;
	m_fileDescriptor = xFile::__open(m_file.getPath().fn_str(),xFile::xlRead);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xConcreteFileInputStream::~xConcreteFileInputStream()
{
	close();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileInputStream::close() throw(xIOException)
{
	if(!isClosed())
		xFile::__close(m_fileDescriptor);
	m_bClosed = true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xConcreteFileInputStream::available()
{
	if(isClosed())
		return 0;
		
	return (int)(m_file.size() - m_position);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileInputStream::fillBuffer()
{
	xFile::__seek(m_fileDescriptor,m_position,xFile::xlFromBeginning);
	
	int read = xFile::__read(m_fileDescriptor,m_buffer.getRawData(),m_buffer.size());
	m_cacheOffBeg = m_position;
	m_cacheOffEnd = m_position + read;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int	xConcreteFileInputStream::read0(xArray<xbyte>& a,int off, int len)
{
	int avail = (int) (m_cacheOffEnd - m_position);
	if (avail <= 0 || avail >= m_buffer.size()) 
	{
		//if the len is > of the buffer size
	    if (len >= m_buffer.size())
	    {
			int n = xFile::__read(m_fileDescriptor,a.getRawData() + off, len);
			if(n >= 0)
				m_position += n;
			return n;
	    }
		
	    fillBuffer();
	    avail = (int) (m_cacheOffEnd - m_position);
	    if(avail <= 0)
	    	return xEOF;
	}
	int cnt = (avail < len) ? avail : len;
	xArray<xbyte>::copy(a, off, m_buffer, (int)(m_position - m_cacheOffBeg), cnt);
	m_position += cnt;
	return cnt;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xConcreteFileInputStream::read(xArray<xbyte>& a,int off, int len) throw(xIOException,xIndexOutOfBoundsException)
{
	if(len <= 0 || off+len > a.size())
		throw xIndexOutOfBoundsException();
	
	if(isClosed())
		throw xIOException(_T("Cannot read,stream closed"));
	
	int n = read0(a, off, len);
	if(n <= 0) 
		return n;
	while(n < len) 
	{
	    int n1 = read0(a, off + n, len - n);
	    if (n1 <= 0) 
	    	break;
	    n += n1;
	}
	return n;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xConcreteFileInputStream::read(xArray<xbyte>& a) throw(xIOException)
{
	return read(a,0,a.size());
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xConcreteFileInputStream::read() throw(xIOException)
{
	if(isClosed())
		throw xIOException(_T("Cannot read,stream closed"));
	
	if(m_position >= m_cacheOffEnd || m_position < m_cacheOffBeg) 
	{
	    fillBuffer();
	    if(m_position >= m_cacheOffEnd)
			return xEOF;
	}
	
	return (int) m_buffer[(int)((m_position++) - m_cacheOffBeg)];
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileInputStream::mark(int readAheadLimit) throw(xIOException)
{
	m_mark = m_position;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileInputStream::rewind() throw(xIOException)
{
	if(m_mark < 0)
		throw xIOException(_T("Invalid mark position"));
		
	m_position = m_mark;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xi64 xConcreteFileInputStream::getPosition()
{
	return m_position;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileInputStream::setPosition(xi64 pos)
{
	xFile::__seek(m_fileDescriptor,pos,xFile::xlFromBeginning);
	m_position = pos;
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
xConcreteFileOutputStream::xConcreteFileOutputStream(xFile& file,int bufferSize)
: m_file(file),m_buffer(bufferSize)
{
	m_cacheOffEnd = 0;
	m_cacheOffBeg = 0;
	m_position = 0;
	m_bClosed = false;
	m_fileDescriptor = xFile::__open(m_file.getPath().fn_str(),xFile::xlWrite);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xConcreteFileOutputStream::~xConcreteFileOutputStream()
{
	close();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileOutputStream::close() throw(xIOException)
{ 
	if(!isClosed())
	{
		flush();
		xFile::__close(m_fileDescriptor);
	}
	
	m_bClosed = true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileOutputStream::flush() throw(xIOException)
{
	if(m_position - m_cacheOffBeg > 0)
	{
		xFile::__write(m_fileDescriptor,m_buffer.getRawData(),(size_t) (m_position - m_cacheOffBeg));
		m_cacheOffBeg = m_position;
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileOutputStream::write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException)
{
	if(len < 0 || off+len > b.size() || off < 0)
		throw xIndexOutOfBoundsException();
	if(isClosed())
		throw xIOException(_T("Cannot write,stream closed"));
		
	if(m_position < m_cacheOffBeg)
		flush();
		
	int avail = (int) (m_cacheOffBeg + m_buffer.size() - m_position);
	if(len >= avail)
	{
		//write directly
		if(len > (m_buffer.size() / 2))
		{
			flush();
			xFile::__write(m_fileDescriptor,m_buffer.getRawData() + off,len);
			m_position += len;
			return;
		}
		else
		{
			xArray<xbyte>::copy(m_buffer,(int) (m_position - m_cacheOffBeg),b,off,avail);
			m_position += avail;
			flush();
			xArray<xbyte>::copy(m_buffer,(int) (m_position - m_cacheOffBeg),b,avail,len - avail);
			m_position += len - avail;
		}
	}
	else
	{
		xArray<xbyte>::copy(m_buffer,(int) (m_position - m_cacheOffBeg),b,off,len);
		m_position += len;
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileOutputStream::write(xArray<xbyte>& b) throw(xIOException)
{
	write(b,0,b.size());	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileOutputStream::write(int b) throw(xIOException)
{
	if(isClosed())
		throw xIOException(_T("Cannot write,stream closed"));
		
	if((m_position + 1 >= m_cacheOffBeg + m_buffer.size()) || (m_position < m_cacheOffBeg))
		flush();
	
	m_buffer[(int)((m_position++) - m_cacheOffBeg)] = (xbyte) b;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xi64 xConcreteFileOutputStream::getPosition()
{
	return m_position;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileOutputStream::setPosition(xi64 pos)
{
	flush();
	xFile::__seek(m_fileDescriptor,pos,xFile::xlFromBeginning);
	m_position = pos;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xConcreteFileIOStream::xConcreteFileIOStream(xFile& file,int bufferSize)
: m_file(file),m_buffer(bufferSize)
{
	m_bClosed = false;
	m_bCacheWasWritten = false;
	m_bCacheValid = false;
	m_iCacheOffBeg = 0;
	m_iCacheOffEnd = 0;
	m_iCachePointer = 0;
	m_iMaxCacheOffset = m_file.size();
	m_fileDescriptor = xFile::__open(m_file.getPath().fn_str(),xFile::xlReadWrite);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xConcreteFileIOStream::~xConcreteFileIOStream()
{
	close();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileIOStream::close() throw(xIOException)
{
	if(!isClosed())
	{
		flush();
		xFile::__close(m_fileDescriptor);
	}
	
	m_bClosed = true;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileIOStream::__cacheUpdate()
{	
	//first write back
	__cacheWriteBack();

	if(m_iMaxCacheOffset > 0)
	{
		//align the cache pointer and the file pointer
		xFile::__seek(m_fileDescriptor,m_iCachePointer,xFile::xlFromBeginning);

		if(m_iCachePointer < m_iMaxCacheOffset)
			xFile::__read(m_fileDescriptor,m_buffer.getRawData(),m_buffer.size());
	}

	m_iCacheOffBeg = m_iCachePointer;
	m_iCacheOffEnd = m_iCacheOffBeg + m_buffer.size();
	m_bCacheWasWritten = false;
	m_bCacheValid = true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileIOStream::__cacheWriteBack()
{
	if(m_bCacheValid && m_bCacheWasWritten)
	{
		if(m_iCacheOffEnd > m_iMaxCacheOffset)
			xFile::__write(m_fileDescriptor,m_buffer.getRawData(),(size_t) (m_buffer.size() - (m_iCacheOffEnd - m_iMaxCacheOffset)));
		else
			xFile::__write(m_fileDescriptor,m_buffer.getRawData(),m_buffer.size());
	}
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileIOStream::flush() throw(xIOException)
{
	__cacheWriteBack();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileIOStream::write(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException)
{
	if(isClosed())
		throw xIOException(_T("Cannot write,stream closed"));
		
	if(len <= 0 || off+len > b.size() || off < 0)
		throw xIndexOutOfBoundsException();

	//first see if it's useful to use cache.
	//the requested size should be at maximum 1/3 of the cache size
	if(len < (m_buffer.size() / 3))
	{
		for(;;)
		{
			//now see if we can write the data in one piece
			if(((m_iCachePointer + len) < m_iCacheOffEnd || m_iCachePointer < m_iCacheOffBeg) && m_bCacheValid)
			{
				//yes we can write the data in one piece
				xArray<xbyte>::copy(m_buffer,(int) (m_iCachePointer - m_iCacheOffBeg),b,0,len);
				m_iCachePointer += len;

				//update maxOffset if needed
				if(m_iCachePointer > m_iMaxCacheOffset)
					m_iMaxCacheOffset = m_iCachePointer;

				m_bCacheWasWritten = true;
				
				break;
			}
			else
			{
				//no we can't write the data in one piece
				__cacheUpdate();
			}
		}//end for
	}
	else //read normally from the disk
	{
		//first writeback the cache
		__cacheWriteBack();

		//align the cache pointer and the file pointer
		xFile::__seek(m_fileDescriptor,m_iCachePointer,xFile::xlFromBeginning);

		xFile::__write(m_fileDescriptor,b.getRawData(),len);

		//sync cache pointer
		m_iCachePointer += len;
		m_bCacheValid = false;
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileIOStream::write(xArray<xbyte>& b) throw(xIOException)
{
	write(b,0,b.size());	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileIOStream::write(int b) throw(xIOException)
{
	if(isClosed())
		throw xIOException(_T("Cannot write,stream closed"));
		
	for(;;)
	{
		//now see if we can write the data in one piece
		if(((m_iCachePointer + 1) < m_iCacheOffEnd || m_iCachePointer < m_iCacheOffBeg) && m_bCacheValid)
		{
			//yes we can write the data in one piece
			m_buffer[(int) (m_iCachePointer - m_iCacheOffBeg)] = (xbyte) b;
			m_iCachePointer++;

			//update maxOffset if needed
			if(m_iCachePointer > m_iMaxCacheOffset)
				m_iMaxCacheOffset = m_iCachePointer;

			m_bCacheWasWritten = true;
			
			break;
		}
		else
		{
			//no we can't write the data in one piece
			__cacheUpdate();
		}
	}//end for
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xConcreteFileIOStream::available()
{
	if(isClosed())
		return 0;
		
	return (int)(m_iMaxCacheOffset - m_iCachePointer);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xConcreteFileIOStream::read(xArray<xbyte>& b, int off, int len) throw(xIOException,xIndexOutOfBoundsException)
{
	if(isClosed())
		throw xIOException(_T("Cannot read,stream closed"));
		
	if(len <= 0 || off+len > b.size())
		throw xIndexOutOfBoundsException();
		
	//first see if it's useful to use cache.
	//the requested size should be at maximum 1/3 of the cache size
	if(len < (m_buffer.size() / 3))
	{
		//see if eof
		if(m_iCachePointer >= m_iMaxCacheOffset)
			return xEOF;

		for(;;)
		{
			//now see if we have the necessary data
			if(((m_iCachePointer + len) < m_iCacheOffEnd || m_iCachePointer < m_iCacheOffBeg) && m_bCacheValid)
			{
				//yes we have the data
				
				xi64 bytesToRead = (xi64) len;
				if((m_iCachePointer + len) > m_iMaxCacheOffset)
					bytesToRead = m_iMaxCacheOffset - m_iCachePointer;

				xArray<xbyte>::copy(b,0,m_buffer,(int)(m_iCachePointer - m_iCacheOffBeg),(int) bytesToRead);
				m_iCachePointer += bytesToRead;

				return (int) bytesToRead;
			}
			else
			{
				//no we don't have the data
				__cacheUpdate();
			}
		}//end for
	}
	else //read normally from the disk
	{
		//first writeback the cache
		__cacheWriteBack();

		//align the cache pointer and the file pointer
		xFile::__seek(m_fileDescriptor, m_iCachePointer,xFile::xlFromBeginning);

		int res = xFile::__read(m_fileDescriptor,b.getRawData(),len);
		
		//sync cache pointer
		m_iCachePointer += res;
		
		if(res == 0 && len != 0)
			return xEOF;
			
		return res;
	}	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xConcreteFileIOStream::read(xArray<xbyte>& b) throw(xIOException)
{
	return read(b,0,b.size());	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xConcreteFileIOStream::read() throw(xIOException)
{
	if(isClosed())
		throw xIOException(_T("Cannot read,stream closed"));
		

	//see if eof
	if(m_iCachePointer >= m_iMaxCacheOffset)
		return xEOF;

	for(;;)
	{
		//now see if we have the necessary data
		if(((m_iCachePointer + 1) < m_iCacheOffEnd || m_iCachePointer < m_iCacheOffBeg) && m_bCacheValid)
		{
			//yes we have the data
			int c = (int) m_buffer[(int)(m_iCachePointer - m_iCacheOffBeg)];
			m_iCachePointer++;
			return c;
		}
		else
		{
			//no we don't have the data
			__cacheUpdate();
		}
	}//end for
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xi64 xConcreteFileIOStream::getPosition()
{
	return m_iCachePointer;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileIOStream::setPosition(xi64 pos)
{
	m_iCachePointer = pos;
	xFile::__seek(m_fileDescriptor,pos,xFile::xlFromBeginning);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileIOStream::mark(int readAheadLimit) throw(xIOException)
{
	m_mark = m_iCachePointer;	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xConcreteFileIOStream::rewind() throw(xIOException)
{
	if(m_mark < 0)
		throw xIOException(_T("Invalid mark position"));
		
	setPosition(m_mark);
}

}//namespace
