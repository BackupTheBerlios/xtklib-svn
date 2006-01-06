// Copyright(C) 2003-2006 Mario Casciaro xshadow[AT]email(DOT)it
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
* @file logging.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#include "../../include/xtk/base/logging.h"

namespace xtk
{

xLogger		xLogger::s_defaultLogger(_T("DefaultLogger"));

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xLogRecord::xLogRecord(xLogLevel::Level level,xString& message) 
: m_level(level),m_message(message)
{
	m_exception = NULL;
	
	xThread* t = xThread::getSelfThread();
	if(t == NULL)
		m_threadName = _T("MainThread");
	else
		m_threadName = t->getName();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xLogRecord::~xLogRecord()
{
	if(m_exception != NULL)
		delete m_exception;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xLogFormatterSimple::format(xLogRecord& record)
{
	xString formatted;
	
	switch(record.getLevel())
	{
	case xLogLevel::FATAL :
		formatted.append(_T("[FATAL  ]"));
		break;
	case xLogLevel::SEVERE :
		formatted.append(_T("[SEVERE ]"));
		break;
	case xLogLevel::WARNING :
		formatted.append(_T("[WARNING]"));
		break;
	case xLogLevel::INFO :
		formatted.append(_T("[INFO   ]"));
		break;
	case xLogLevel::VERBOSE :
		formatted.append(_T("[VERBOSE]"));
		break;
	case xLogLevel::FINE :
		formatted.append(_T("[FINE   ]"));
		break;
	case xLogLevel::FINER :
		formatted.append(_T("[FINER  ]"));
		break;
	}
	
	formatted.append(record.getTime().format(_T("[%x - %X]")));
	
	if(!record.getSourceClassName().isEmpty() || !record.getSourceMethodName().isEmpty())
	{
		formatted.append(_T("{"));
		formatted.append(record.getSourceClassName());
		formatted.append(_T("::"));
		formatted.append(record.getSourceMethodName());
		formatted.append(_T("}"));
	}
	
	formatted.append(_T(" "));
	formatted.append(record.getMessage());
	formatted.append(_T(" "));
	
	
	if(record.haveException())
	{
		formatted.append(_T("(Exception thrown, type:"));
		formatted.append(record.getException().getType());
		formatted.append(_T(" , Message:"));
		formatted.append(record.getException().getDescription());
		formatted.append(_T(")"));
	}
	
	formatted.append(_T("\n"));
	return formatted;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xLogHandler::xLogHandler(xLogLevel::Level level,xLogFormatter* formatter) throw(xIllegalArgumentException)
{
	if(formatter == NULL)
		throw xIllegalArgumentException(_T("No NULL argument"));
	
	m_formatter = formatter;
	m_level = level;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xLogHandler::~xLogHandler()
{
	if(isOwner())
		delete m_formatter;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xLogHandlerFile::xLogHandlerFile(xString filename,int fileSizeLimit,xLogLevel::Level level
	,xLogFormatter* formatter,xCharset::Charset charset)
	: xLogHandler(level,formatter),m_file(filename)
{
	if(m_fileSizeLimit < 1000 && m_fileSizeLimit != 0)
		m_fileSizeLimit = 1000;
	else
		m_fileSizeLimit = fileSizeLimit;
	
	try
	{
		m_out = new xOutputStreamWriter(m_file.getOutputStream(),charset);
	}
	catch(xException&)
	{
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xLogHandlerFile::~xLogHandlerFile()
{
	m_out->close();
	delete m_out;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLogHandlerFile::publish(xLogRecord& record)
{
	if(record.getLevel() <= getLogLevel())
		m_out->write(getFormatter().format(record));
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLogger::publishRecord(xLogRecord& record)
{
	synchronizeStart();
	
	if(record.getLevel() <= getLogLevel())
	{
		xIterator* iter = m_handlers.iterator();
		while(iter->hasNext())
		{
			xLogHandler* lh = dynamic_cast<xLogHandler*>(&(iter->next()));
			if(lh != NULL)
				lh->publish(record);
		}	
		delete iter;
	}
	
	synchronizeEnd();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLogger::log(xLogLevel::Level level,xString message)
{
	xLogRecord record(level,message);
	publishRecord(record);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLogger::log(xLogLevel::Level level,xString message,xException& ex)
{
	xLogRecord record(level,message);
	record.setException(ex);
	publishRecord(record);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLogger::log(xLogLevel::Level level,xString message,xString sourceclass,xString sourcemethod)
{
	xLogRecord record(level,message);
	record.setSourceClassName(sourceclass);
	record.setSourceMethodName(sourcemethod);
	publishRecord(record);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xLogger::log(xLogLevel::Level level,xString message,xString sourceclass,xString sourcemethod,xException& ex)
{
	xLogRecord record(level,message);
	record.setSourceClassName(sourceclass);
	record.setSourceMethodName(sourcemethod);
	record.setException(ex);
	publishRecord(record);
}

}//namespace
