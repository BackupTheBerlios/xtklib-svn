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
* @file preferences.cpp
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../../include/xtk/base/preferences.h"
#include "../../include/xtk/base/typewrappers.h"
#include "../../include/xtk/base/charstream.h"
#include "../../include/xtk/base/interfaces.h"

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xPreferences::xPreferences(xInputStream* defaultInput,xOutputStream* defaultOutput,xCharset::Charset charset) 
throw(xIOException)
{
	m_defaultInput = defaultInput;
	m_defaultOutput = defaultOutput;
	m_isPersistent = false;
	m_mustdeleteinput = false;
	m_mustdeleteoutput = false;
	m_autosave = true;
	m_headerComment = NULL;
	m_file = NULL;
	m_charset = charset;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xPreferences::xPreferences(xFile file,xCharset::Charset charset) throw(xIOException)
{
	m_file = new xFile(file);
	m_defaultOutput = m_file->getOutputStream();
	m_defaultInput = m_file->getInputStream();
	m_isPersistent = false;
	m_mustdeleteinput = true;
	m_mustdeleteoutput = true;
	m_autosave = true;
	m_headerComment = NULL;
	m_charset = charset;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xPreferences::~xPreferences()
{
	if(m_autosave && m_defaultOutput != NULL)
		save();
		
	if((isOwner() || m_mustdeleteinput) && m_defaultInput != NULL)
		delete m_defaultInput;
	
	if((isOwner() || m_mustdeleteoutput) && m_defaultOutput != NULL)
		delete m_defaultOutput;
		
	if(m_headerComment != NULL)
		delete m_headerComment;
	
	if(m_file != NULL)
		delete m_file;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::setDefaultInput(xInputStream* defaultInput)
{
	if(isOwner() || m_mustdeleteinput)
		delete m_defaultInput;
	
	m_defaultInput = defaultInput;
	m_mustdeleteinput = false;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xInputStream& xPreferences::getDefaultInput() throw(xNotAvailableDataException)
{
	if(m_defaultInput == NULL)
		throw xNotAvailableDataException(_T("Default input not available"));
		
	return *m_defaultInput;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::setDefaultOutput(xOutputStream* defaultOutput)
{
	if(isOwner() || m_mustdeleteoutput)
		delete m_defaultOutput;

	m_defaultOutput = defaultOutput;
	m_mustdeleteoutput = false;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xOutputStream& xPreferences::getDefaultOutput() throw(xNotAvailableDataException)
{
	if(m_defaultOutput == NULL)
		throw xNotAvailableDataException(_T("Default output not available"));

	return *m_defaultOutput;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::setInt(xString paramName,int value)
{
	xObject& o = m_params.get(paramName);
	if(!o.isNull())
	{
		xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
		if(p->value != NULL)
			delete p->value;
		p->type = xPrefParam::INT;
		p->value = new xInteger(value);
	}
	else
	{
		//insert new object
		xPrefParam* p = new xPrefParam();
		p->name = paramName;
		p->type = xPrefParam::INT;
		p->value = new xInteger(value);
		m_params.put(new xString(paramName),p);
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::setString(xString paramName,xString value)
{
	xObject& o = m_params.get(paramName);
	if(!o.isNull())
	{
		xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
		if(p->value != NULL)
			delete p->value;
		p->type = xPrefParam::STRING;
		p->value = new xString(value);
	}
	else
	{
		//insert new object
		xPrefParam* p = new xPrefParam();
		p->type = xPrefParam::STRING;
		p->value = new xString(value);
		p->name = paramName;
		m_params.put(new xString(paramName),p);
	}
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::setBool(xString paramName,bool value)
{
	xObject& o = m_params.get(paramName);
	if(!o.isNull())
	{
		xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
		if(p->value != NULL)
			delete p->value;
		p->type = xPrefParam::BOOLEAN;
		p->value = new xBoolean(value);
	}
	else
	{
		//insert new object
		xPrefParam* p = new xPrefParam();
		p->type = xPrefParam::BOOLEAN;
		p->value = new xBoolean(value);
		p->name = paramName;
		m_params.put(new xString(paramName),p);
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::setFloat(xString paramName,float value)
{
	xObject& o = m_params.get(paramName);
	if(!o.isNull())
	{
		xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
		if(p->value != NULL)
			delete p->value;
		p->type = xPrefParam::FLOAT;
		p->value = new xFloat(value);
	}
	else
	{
		//insert new object
		xPrefParam* p = new xPrefParam();
		p->type = xPrefParam::FLOAT;
		p->value = new xFloat(value);
		p->name = paramName;
		m_params.put(new xString(paramName),p);
	}
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xPreferences::getInt(xString paramName,int def) throw(xDataFormatException)
{
	xObject& o = m_params.get(paramName);
	if(o.isNull())
		return def;
	
	xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
	if(p->type != xPrefParam::INT)
		throw xDataFormatException();
	
	return (dynamic_cast<xInteger*>(p->value))->getValue();
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xPreferences::getString(xString paramName,xString def) throw(xDataFormatException)
{
	xObject& o = m_params.get(paramName);
	if(o.isNull())
		return def;

	xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
	if(p->type != xPrefParam::STRING)
		throw xDataFormatException();

	return *(dynamic_cast<xString*>(p->value));
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xPreferences::getBool(xString paramName,bool def) throw(xDataFormatException)
{
	xObject& o = m_params.get(paramName);
	if(o.isNull())
		return def;

	xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
	if(p->type != xPrefParam::BOOLEAN)
		throw xDataFormatException();

	return (dynamic_cast<xBoolean*>(p->value))->getValue();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
float xPreferences::getFloat(xString paramName,float def) throw(xDataFormatException)
{
	xObject& o = m_params.get(paramName);
	if(o.isNull())
		return def;

	xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
	if(p->type != xPrefParam::FLOAT)
		throw xDataFormatException();

	return (dynamic_cast<xFloat*>(p->value))->getValue();
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::setComment(xString paramName,xString comment)
{
	xObject& o = m_params.get(paramName);
	if(o.isNull())
		return;

	xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
	if(p->comment != NULL)
		delete p->comment;
	
	p->comment = new xString(comment);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xPreferences::hasComment(xString paramName)
{
	xObject& o = m_params.get(paramName);
	if(o.isNull())
		return false;
		
	xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
	return p->comment != NULL;

}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xPreferences::getComment(xString paramName) throw(xNotAvailableDataException)
{
	xObject& o = m_params.get(paramName);
	if(o.isNull())
		throw xNotAvailableDataException();

	xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
	if(p->comment == NULL)
		throw xNotAvailableDataException();
		
	return *(p->comment);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xPreferences::getOrder(xString paramName) throw(xNotAvailableDataException)
{
	xObject& o = m_params.get(paramName);
	if(o.isNull())
		throw xNotAvailableDataException();

	xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
	return p->paramOrder;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::setOrder(xString paramName,int ord)
{
	xObject& o = m_params.get(paramName);
	if(o.isNull())
		return;

	xPrefParam* p = dynamic_cast<xPrefParam*>(&o);
	p->paramOrder = ord;
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::removeParam(xString paramName)
{
	m_params.remove(paramName);
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xPreferences::paramExists(xString paramName)
{
	xObject& o = m_params.get(paramName);
	if(o.isNull())
		return false;
		
	return true;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::load() throw(xIOException,xDataFormatException)
{
	if(m_defaultInput == NULL)
		throw xIOException(_T("Default Input stream not set"));
		
	load(*m_defaultInput);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::load(xInputStream& source) throw(xIOException,xDataFormatException)
{
	xInputStreamReader* tmp = new xInputStreamReader(&source,m_charset);
	tmp->rescindOwnership();
	xBufferedReader isr(tmp);
	
	xString paramComment;
	bool isHeaderComment = true;
	bool isParamComment = false;
	int numParamsRead = 0;
	//parse data from the stream and fill the hash map
	
	while(!isr.eof())
	{
		xString line = isr.readLine();
		line.trimSpaceRight();
		line.trimSpaceLeft();
		
		if(line.isEmpty())
		{
			isParamComment = false;
			paramComment = _T("");
		}
		else if(line.charAt(0) == _T('#'))
		{
			line.remove(0,1);
			
			if(isHeaderComment)
			{
				if(m_headerComment != NULL)
				{
					m_headerComment->append(xString::newLine);
					m_headerComment->append(line);
				}
				else
				{
					m_headerComment = new xString(line);
				}
				isHeaderComment = false;
			}
			else //is a param comment
			{
				if(!paramComment.isEmpty())
					paramComment.append(xString::newLine);
				paramComment.append(line);
				isParamComment = true;
			}
		}
		else
		{
			//parse param name
			size_t i = 0;
			for(; i < line.length();i++)
				if(line.charAt(i) == _T('='))
					break;
			
			xString paramName = line.substring(0,i);
			paramName.trimSpaceRight();
			paramName.trimSpaceLeft();
			i++;
			
			//parse value
			xString value;
			if(i >= line.length())
			{
				//the param will be ignored, no value present
				paramComment = _T("");
				isParamComment = false;
				continue;
			}
			else
			{
				value = line.substring(i);
				value.trimSpaceRight();
				value.trimSpaceLeft();
				
				if(value.isEmpty())
				{
					//the param will be ignored, no value present
					paramComment = _T("");
					isParamComment = false;
					continue;
				}
			}
			
			//try to see if it is a string
			if(value.charAt(0) == _T('\"'))
			{
				xString str;
				bool ok = false;
				for(size_t i = 1;i < value.length();i++)
				{
					if(value.charAt(i) == _T('\"'))
					{
						ok = true;
						break;
					}
					str.append(&(value.c_str()[i]),1);
				}
				
				if(!ok)
					throw xDataFormatException();
					
				setString(paramName,str);
			}
			//is a boolean?
			else if(value.equals(_T("true")) || value.equals(_T("false")))
			{
				if(value.equals(_T("true")))
					setBool(paramName,true);
				else
					setBool(paramName,false);
			}
			else
			{
				int ival;
				float fval;
				if(xInteger::parseInt(value,&ival))
					setInt(paramName,ival);
				else if(xFloat::parseFloat(value,&fval))
					setFloat(paramName,fval);
				else
					throw xDataFormatException(_T("Cannot parse correctly a param value"));
			}
			
			setOrder(paramName, numParamsRead++);
			
			if(isParamComment)
				setComment(paramName,paramComment);
			paramComment = _T("");
			isParamComment = false;
		}
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::load(xFile file) throw(xIOException,xDataFormatException)
{
	xInputStream* is = file.getInputStream();
	load(*is);
	delete is;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::save() throw(xIOException)
{
	if(m_defaultOutput == NULL)
		throw xIOException(_T("Default output not set"));
	if(m_mustdeleteoutput && m_file != NULL)
		m_file->trim();
	
	save(*m_defaultOutput);
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::save(xOutputStream& sink) throw(xIOException)
{
	xCollection* paramscoll = m_params.values();
	xArray<xPrefParam*> params = paramscoll->toArray().castTo<xPrefParam*>();
	
	paramscoll->rescindOwnership();
	delete paramscoll;
	
	xParamsComparator comp;
	params.sort(comp);
	
	xOutputStreamWriter wr(&sink,m_charset);
	wr.rescindOwnership();
	
	
	//write header if there is one
	if(m_headerComment != NULL)
	{
		xStringTokenizer tokens(*m_headerComment,_T("\n"));
		while(tokens.hasMoreTokens())
		{
			wr.write(_T("#"));
			wr.write(tokens.nextToken());
		}
		
		wr.write(_T("\n"));
		wr.write(_T("\n"));
	}
	
	
	for(int i = 0;i < params.size();i++)
	{
		//write first comment
		if(params[i]->comment != NULL)
		{
			xStringTokenizer tokens(*(params[i]->comment),_T("\n"));
			while(tokens.hasMoreTokens())
			{
				wr.write(_T("#"));
				wr.write(tokens.nextToken());
			}
			wr.write(_T("\n"));
		}
		
		//now write param name
		wr.write(params[i]->name);
		wr.write(_T(" = "));
		
		//now write value
		switch(params[i]->type)
		{
			case xPrefParam::STRING :
				wr.write(_T("\""));
				wr.write(*(dynamic_cast<xString*>(params[i]->value)));
				wr.write(_T("\""));
				break;
			case xPrefParam::INT :
				wr.write(dynamic_cast<xInteger*>(params[i]->value)->toString());
				break;
			case xPrefParam::BOOLEAN :
				wr.write(dynamic_cast<xBoolean*>(params[i]->value)->toString());
				break;
			case xPrefParam::FLOAT :
				wr.write(dynamic_cast<xFloat*>(params[i]->value)->toString());
				break;
			default:
				break;
		}
		
		//leave a free line
		wr.write(_T("\n"));
		wr.write(_T("\n"));
	}
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xPreferences::save(xFile file) throw(xIOException)
{
	file.trim();
	xOutputStream* o = file.getOutputStream();

	save(*o);
	
	delete o;
}


}//namespace
