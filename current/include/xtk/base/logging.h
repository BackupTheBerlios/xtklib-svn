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
* @file logging.h
* @author Mario Casciaro (xshadow@email.it)
*/


#ifndef XTK_LOGGING_H
#define XTK_LOGGING_H

#include "../globals.h"
#include "time.h"
#include "string.h"
#include "exception.h"
#include "file.h"
#include "datastructures.h"
#include "stream.h"
#include "system.h"
#include "thread.h"

namespace xtk
{

/**
 * Defines the importance of a log record
 */
class XTKAPI xLogLevel
{
private:
	xLogLevel(){}
public:
	enum Level
	{
		FATAL = 0,
		SEVERE = 1,
		WARNING = 2,
		INFO = 3,
		VERBOSE = 4,
		FINE = 5,
		FINER = 6,
		MAX = 6,
		MIN = 0,
	};
};

/**
 * 
 */
class XTKAPI xLogRecord : public virtual xObject
{
private:
	xLogLevel::Level	m_level;
	xString				m_message;
	xTime				m_time;
	xString				m_threadName;
	xException*			m_exception;
	xString				m_class;
	xString				m_method;

public:
	/**
	 * Default constructor
	 */
	xLogRecord(xLogLevel::Level level,xString& message);
	
	virtual ~xLogRecord();

	/**
	 * Set the level of the log record
	 */
	void setLevel(xLogLevel::Level level)
	{
		m_level = level;
	}
	
	/**
	 * Get the current level of the log record
	 */
	xLogLevel::Level getLevel()
	{
		return m_level;
	}
	
	/**
	 * Get the "raw" log message, before localization or formatting.
	 */
	xString	getMessage()
	{
		return m_message;
	}

	/**
	 * Set the "raw" log message, before localization or formatting.
	 */
	void setMessage(xString& message)
	{
		m_message = message;
	}

	/**
	 *  Get the name of the thread where the message originated.
	 */
	xString getThreadName()
	{
		return m_threadName;
	}

	/**
	* Get event time 
	*/
	xTime& getTime()
	{
		return m_time;
	}

	/**
	 * Set an exception associated with the log event.
	 */
	void setException(xException& ex)
	{
		m_exception = dynamic_cast<xException*>(ex.clone());
	}

	/**
	 * 
	 */
	bool haveException()
	{
		return m_exception != NULL;
	}
	
	/**
	 * Get an exception associated with the log event.
	 * 
	 * @throw xNotAvailableDataException if no exception are associated with the record
	 */
	xException& getException() throw(xNotAvailableDataException)
	{
		if(! haveException())
			throw xNotAvailableDataException(_T("No exception associated with record"));
			
		return *m_exception;
	}

	/**
	 * Set the name of the class that issued the logging request.
	 */
	void setSourceClassName(xString& sourceclass)
	{
		m_class = sourceclass;
	}
	
	/**
	 * Get the name of the class that issued the logging request
	 */
	xString getSourceClassName()
	{
		return m_class;
	}

	/**
	 * Set the name of the method that issued the logging request.
	 */
	void setSourceMethodName(xString& sourcemethod)
	{
		m_method = sourcemethod;
	}

	/**
	 * Get the name of the method that issued the logging request.
	 */
	xString getSourceMethodName()
	{
		return m_method;
	}
};


/**
 * A Formatter provides support for formatting xLogRecords.
 */
class XTKAPI xLogFormatter : public virtual xObject
{
public:
	virtual ~xLogFormatter(){}

	/**
	 *  Format the given log record and return the formatted string.
	 */
	virtual xString format(xLogRecord& record) = 0;
};


/**
 * A simple log formatter.
 */
class XTKAPI xLogFormatterSimple : public xLogFormatter
{
public:
	virtual xString format(xLogRecord& record);
	virtual ~xLogFormatterSimple(){}
};


/**
 * Handle the log records coming from a xLogger organize them in a specified
 * format and sends the resulting data to an output.
 */
class XTKAPI xLogHandler : public virtual xObject,public xOwnership
{
private:
	xLogLevel::Level	m_level;
	xLogFormatter*		m_formatter;

protected:
	xLogHandler(xLogLevel::Level level = xLogLevel::INFO,xLogFormatter* formatter = new xLogFormatterSimple())
	throw(xIllegalArgumentException);

public:
	virtual ~xLogHandler();

	/**
	 * Publish a LogRecord.
	 * The Handler is responsible for formatting the message, when and if necessary.
	 */
	virtual void publish(xLogRecord& record) = 0;

	/**
	 * Specifies that log records with level > logLevel should be ignored 
	 * and not sent to the output.
	 */
	void setLogLevel(xLogLevel::Level logLevel)
	{
		m_level = logLevel;
	}
	
	/**
	 * Return the current log level limit
	 */
	xLogLevel::Level getLogLevel()
	{
		return m_level;
	}
	
	xLogFormatter& getFormatter()
	{
		return *m_formatter;
	}
};


/**
 * A Log handler that write the logs to the standard output.
 */
class XTKAPI xLogHandlerSTD : public xLogHandler
{
public:
	xLogHandlerSTD(xLogLevel::Level level = xLogLevel::INFO,xLogFormatter* formatter = new xLogFormatterSimple())
		: xLogHandler(level,formatter){}

	virtual ~xLogHandlerSTD(){}

	virtual void publish(xLogRecord& record)
	{
		if(record.getLevel() <= getLogLevel())
			xSystem::getStdout().write(getFormatter().format(record));
	}
};


/**
* A Log handler that write the logs to a file.
*/
class XTKAPI xLogHandlerFile : public xLogHandler
{
private:
	xFile					m_file;
	xOutputStreamWriter*	m_out;
	int						m_fileSizeLimit;

public:

	/**
	 * Default constructor. 
	 *
	 * @param fileSizeLimit if 0 no limits,if < 1000 the parameter is set to secure value
	 * of 1000
	 */
	xLogHandlerFile(xString filename,
					int fileSizeLimit = 0,
					xLogLevel::Level level = xLogLevel::INFO,
					xLogFormatter* formatter = new xLogFormatterSimple(),
					xCharset::Charset charset = xCharset::CS_SYSTEM
				   );

	virtual ~xLogHandlerFile();
	
	/**
	 * @todo By now file size limit is ignored, implement it!!!
	 */
	virtual void publish(xLogRecord& record);
};


/**
 * A class for logging messages. Thread Safe.
 */
class XTKAPI xLogger : public virtual xObject,public xOwnership
{

private:
	xArrayList			m_handlers;
	xString				m_name;
	xLogLevel::Level	m_level;
	
	
	//a convenience default logger
	static xLogger		s_defaultLogger;

	void publishRecord(xLogRecord& record);
	
	virtual void ownershipChangedHandler()
	{
		synchronizeStart();

		if(isOwner())
			m_handlers.giveOwnership();
		else
			m_handlers.rescindOwnership();

		synchronizeEnd();
	}
	
public:
	xLogger(xString name) : m_name(name),m_level(xLogLevel::MAX){}

	/**
	 * Get the name of the logger
	 */
	xString getName()
	{
		return m_name;
	}

	/**
	 * Add a log handler
	 */
	void addHandler(xLogHandler* handler)
	{
		synchronizeStart();
		m_handlers.add(handler);
		synchronizeEnd();
	}

	/**
	 * Remove a log handler
	 */
	void removeHandler(xLogHandler& handler)
	{
		synchronizeStart();
		m_handlers.removeObject(handler);
		synchronizeEnd();
	}

	/**
	* Specifies that log records with level > logLevel should be ignored 
	* and not sent to the handlers.
	*/
	void setLogLevel(xLogLevel::Level logLevel)
	{
		synchronizeStart();
		m_level = logLevel;
		synchronizeEnd();
	}

	/**
	* Return the current log level limit
	*/
	xLogLevel::Level getLogLevel()
	{
		xSynchronized s(*this);
		return m_level;
	}

	/**
	 * 
	 */
	void log(xLogLevel::Level level,xString message);
	void log(xLogLevel::Level level,xString message,xException& ex);
	void log(xLogLevel::Level level,xString message,xString sourceclass,xString sourcemethod);
	void log(xLogLevel::Level level,xString message,xString sourceclass,xString sourcemethod,xException& ex);

	/**
	 * Get a default global logger that can be used for your convenience.
	 */
	static xLogger& getDefaultLogger(){return s_defaultLogger;}
};



/**
 * A class for maintain a global set of shared Loggers.
 */
/*
class xLogManager
{
private:
	static xLogManager	s_logManager;

	xHashMap			m_loggers;


	xLogManager(){}
public:
	xLogger& getLogger(xString name);
	xLogManager& getInstance();
};
*/


}//namespace

#endif /*XTK_LOGGING_H*/
