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
* @file file.h
* @author Mario Casciaro (xshadow@email.it)
*/


#ifndef XTK_FILE_H
#define XTK_FILE_H

#include "../globals.h"
#include "interfaces.h"
#include "string.h"
#include "stream.h"
#include "array.h"


namespace xtk
{

class xFileInputStream;
class xFileOutputStream;
class xFileIOStream;

/**
 * An object to manage file and directories pathnames.
 * 
 */	
class XTKAPI xFile : public virtual xObject,public virtual xComparable
{
friend class xConcreteFileInputStream;
friend class xConcreteFileOutputStream;
friend class xConcreteFileIOStream;

private:
	//____________WRAPPER_FUNCTIONS_FOR_STREM_USE________________________
	
	enum OpenMode{xlRead,xlWrite,xlReadWrite};
	enum SeekMode{xlFromBeginning,xlFromEnd,xlFromCurrent};
	/**
	* Open a file for access.
	*/
	
	#if !defined(XTK_OS_WINDOWS) || !defined(XTK_UNICODE)
		static int __open(const char* filename,OpenMode mode);
	#else
		static int __open(const xchar* filename,OpenMode mode);
	#endif

	/**
	* Wrapper around read primitive
	*/
	static int __read(int filedesc,const void* buffer,size_t buffer_size);

	/**
	* Wrapper around write primitive
	*/
	static int __write(int filedesc,const void* buffer,size_t buffer_size);

	/**
	* Wrapper around seek primitive
	*/
	static xi64 __seek(int filedesc,xi64 offset,SeekMode origin);

	/**
	* Wrapper around close primitive with flushing feature
	*/
	static void __close(int filedesc);
	
	//____________WRAPPER_FUNCTIONS_FOR_STREM_USE________________________END
	
protected:
	xString			m_abstractPathName;
	
	enum CheckAccess{xlCheckRead,xlCheckWrite,xlCheckReadWrite,xlCheckExistence};
	
	#if !defined(XTK_OS_WINDOWS) || !defined(XTK_UNICODE)
		bool __checkFileAccess(const char* filename,CheckAccess caccess);
	#else
		bool __checkFileAccess(const xchar* filename,CheckAccess caccess);
	#endif	
		
	void normalize(xString& str);
	
public:
	#ifdef XTK_OS_WINDOWS
		static const xchar separator;
	#else
		static const xchar separator;
	#endif
	
	/**
	 * Copy constructor
	 */
	xFile(const xFile& copy);
	
	
	/**
	 * Creates a new xFile instance from a given pathname string. Here
	 * will not be tested if the file exists or not.
	 */
	xFile(xString pathname);
	
	/**
	 * Creates a new xFile instance from a parent abstract pathname.
	 * and a child pathname string.Here
	 * will not be tested if the file exists or not.
	 */
	xFile(xFile& parent, xString child);
	
	/**
	 * Creates a new xFile instance from a parent pathname.
	 * and a child pathname string.Here
	 * will not be tested if the file exists or not.
	 */
	xFile(xString parent, xString child);
	
	/**
	 * Tests whether the application can read the file
	 */
	virtual bool canRead();
	
	/**
	 * Tests whether the application can write the file
	 */
	virtual bool canWrite();
	
	/**
	 * Creates a new, empty file named by this abstract pathname if a file with 
	 * this name does not yet exist. Return false if the file already exists
	 */
	virtual bool createNewFile();
	
	/**
	 * Tests whether the file or directory denoted by this abstract pathname 
	 * exists.
	 */
	virtual bool exists();
	
	/**
	 * Tests whether the file denoted by this abstract pathname is a directory.
	 */
	virtual bool isDirectory();
	
	/**
	 * Tests whether the file denoted by this abstract pathname is a file.
	 */
	virtual bool isFile();
	
	/**
	 * Returns the length of the file denoted by this abstract pathname.
	 */
	virtual xi64 size();
	
	/**
	 * Returns the absolute form of this abstract pathname.
	 * 
	 * @todo Da migliorare, deve gestire i "../"
	 */
	virtual xFile* getAbsoluteFile();
	
	/**
	 * Returns the name of the file or directory denoted by this 
	 * abstract pathname.
	 */
	virtual xString getName();
	
	/**
	 * Returns the xFile representing the parent of this xFile, 
	 * or null if this pathname does not name a parent directory.
	 */
	virtual xFile* getParent();
	
	/**
	 * Returns a pathname string representing this xFile
	 */
	virtual xString& getPath()
	{
		return m_abstractPathName;
	}
	
	/**
	 * Returns a string representing the extension of this xFile or null if
	 * the file does not have an extension.
	 */
	virtual xString getExtension();
	
	/**
	 * Deletes the file or directory denoted by this abstract pathname.
	 * 
	 * @exception xFileNotFoundException
	 * @exception xIOException
	 */
	virtual void remove();
	
	/**
	 * Tests whether this abstract pathname is absolute.
	 */
	virtual bool isAbsolute();
    
    /**
     * 
     */
	virtual void trim() throw(xIOException);
	
	/**
	 * Returns an array of strings naming the files and directories in the 
	 * directory denoted by this abstract pathname.
	 * 
	 * @return Returns null if this abstract pathname does not denote a 
	 * directory, or if an I/O error occurs.
	 */
	virtual xArray<xString*> list() throw(xIOException);
	
	/**
	 * Returns an array of abstract pathnames denoting the files in the 
	 * directory denoted by this abstract pathname.
	 */
	virtual xArray<xFile*> listFiles() throw(xIOException);
	
	/**
	 * Creates the directory named by this abstract pathname.Return false if the
	 * dir already exists
	 * 
	 * @xIOException
	 */
	virtual bool mkdir();
	
	/**
	 * Renames the file denoted by this abstract pathname.
	 */
	virtual bool renameTo(xFile dest);
	
	/**
	 * Optimal value for buffer size is between 10k and 60k,default is 30k
	 */
	virtual xFileInputStream* getInputStream(int bufferSize = 30000);
	
	/**
	 * Optimal value for buffer size is between 10k and 60k,default is 30k
	 */
	virtual xFileOutputStream* getOutputStream(int bufferSize = 30000);
	
	/**
	 * Optimal value for buffer size is between 10k and 60k,default is 30k
	 */
	virtual xFileIOStream* getIOStream(int bufferSize = 30000);
	
	/**
	 * List the available filesystem roots.
	 */
	static xArray<xFile*> listRoots();
	
	/**
	 * Creates a temporary file in the given directory.
	 * 
	 * @todo to implement
	 */
	static xFile* createTempFile(xString prefix,xString suffix,xFile& directory);
	
	/**
	 * Creates a temporary file in the Operating System default temp directory
	 * 
	 * @todo to implement
	 */
	static xFile* createTempFile(xString prefix,xString suffix);
	
	
	virtual int compareTo(xObject& o);
	virtual bool equals(xObject& o);
	virtual int hashCode();
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

/**
 * A stream for read files.This Stream is already buffered.
 */
class XTKAPI xFileInputStream : public virtual xInputStream
{
protected:
	xFileInputStream(){}
	
public:
	/**
	 * Returns true.
	 */
	virtual bool isMarkSupported(){return true;}
	virtual xi64 getPosition() = 0;
	virtual void setPosition(xi64 pos) = 0;
};

/**
 * A stream for write files.This Stream is already buffered.
 */
class XTKAPI xFileOutputStream : public virtual xOutputStream
{
protected:
	xFileOutputStream(){}
	
public:
	virtual xi64 getPosition() = 0;
	virtual void setPosition(xi64 pos) = 0;
};


/**
 * A stream for read-write files.This Stream is already buffered.
 */
class XTKAPI xFileIOStream : public virtual xIOStream,
public virtual xFileOutputStream,public virtual xFileInputStream
{
protected:
	xFileIOStream(){}
	
public:
	/**
	 * Returns true.
	 */
	virtual bool isMarkSupported(){return true;}
	
	virtual xi64 getPosition() = 0;;
	virtual void setPosition(xi64 pos) = 0;
};


}//namespace


#endif //XTK_FILE_H
