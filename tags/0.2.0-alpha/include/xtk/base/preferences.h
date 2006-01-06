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
* @file preferences.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#ifndef XTK_PREFERENCES_H
#define XTK_PREFERENCES_H

#include "../globals.h"
#include "datastructures.h"
#include "stream.h"
#include "file.h"

namespace xtk
{

/**
 * Represent an object for storing preference and configuration data.
 *
 * If you manually edit a preferences file will be preserved the header comment
 * the order of the params and the comment immediately up the params.
 */
class XTKAPI xPreferences : public virtual xObject,public xOwnership
{
private:
	class xPrefParam : public virtual xObject
	{
	public:
		enum ParamType
		{
			INT = 1,
			STRING = 2,
			BOOLEAN = 3,
			FLOAT = 5,
		};
		
		xString		name;
		xObject*	value;
		xString*	comment;
		int			paramOrder;
		ParamType	type;
		
		xPrefParam()
		{
			value = NULL;
			comment = NULL;
			paramOrder = -1;
			type = INT;
		}
		
		virtual ~xPrefParam()
		{
			if(value != NULL)
				delete value;
			if(comment != NULL)
				delete comment;
		}
	};

	
	class xParamsComparator : public xComparator<xPrefParam*>
	{
	public:

		virtual int compare(xPrefParam* o1,xPrefParam* o2)
		{
			if(o1->paramOrder < o2->paramOrder)
				return -1;
			if(o1->paramOrder > o2->paramOrder)
				return 1;
				
			return 0;
		}
	};
	
	xHashMap			m_params;
	xFile*				m_file;
	xInputStream*		m_defaultInput;
	xOutputStream*		m_defaultOutput;
	xCharset::Charset	m_charset;
	xString*			m_headerComment;
	bool				m_isPersistent;
	bool				m_autosave;
	
	bool				m_mustdeleteinput;
	bool				m_mustdeleteoutput;
	
	int getOrder(xString paramName) throw(xNotAvailableDataException);
	void setOrder(xString paramName,int ord);
	
public:
	/**
	 * Default constructor
	 */
	xPreferences(xInputStream* defaultInput = NULL,xOutputStream* defaultOutput = NULL,
		xCharset::Charset charset = xCharset::CS_UTF8) throw(xIOException);
	
	/**
	 * Constructor, set a file as default source/sink for preferences
	 */
	xPreferences(xFile file,xCharset::Charset charset = xCharset::CS_UTF8) throw(xIOException);
	
	virtual ~xPreferences();
	
	void setDefaultInput(xInputStream* defaultInput);
	xInputStream& getDefaultInput() throw(xNotAvailableDataException);
	
	void setDefaultOutput(xOutputStream* defaultOutput);
	xOutputStream& getDefaultOutput() throw(xNotAvailableDataException);
	
	
	void setAutosave(bool autosave){m_autosave = autosave;}
	
	/**
	 * By default autosave is on
	 */
	bool getAutosave(){return m_autosave;}
	
	/**
	 * Set the int value for the specified param name
	 */
	void setInt(xString paramName,int value);
	
	/**
	 * Set the string value for the specified param name
	 */
	void setString(xString paramName,xString value);
	
	/**
	 * Set the bool value for the specified param name
	 */
	void setBool(xString paramName,bool value);
	
	
	void setFloat(xString paramName,float value);
	
	/**
	 * Retrieve the int value associated with the given param name
	 */
	int getInt(xString paramName,int def) throw(xDataFormatException);
	
	/**
	 * Retrieve the string value associated with the given param name
	 */
	xString getString(xString paramName,xString def) throw(xDataFormatException);
	
	/**
	 * Retrieve the bool value associated with the given param name
	 */
	bool getBool(xString paramName,bool def) throw(xDataFormatException);
	
	float getFloat(xString paramName,float def) throw(xDataFormatException);
	
	void setComment(xString paramName,xString comment);
	bool hasComment(xString paramName);
	xString getComment(xString paramName) throw(xNotAvailableDataException);
	
	/**
	 * Remove the specified param
	 */
	void removeParam(xString paramName);
	
	/**
	 * Return true if the specified param maps to a value
	 */
	bool paramExists(xString paramName);
	
	/**
	 * Return true if the current object is persistent on the default output stream.
	 */
	bool isPersistent(){return m_isPersistent;}
	
	/**
	 * Load preferences data from the default input source
	 */
	void load() throw(xIOException,xDataFormatException);
	
	/**
	 * Load preference data from the specified input stream
	 */
	void load(xInputStream& source) throw(xIOException,xDataFormatException);
	
	/**
	 * Load preference data from the specified file
	 */
	void load(xFile file) throw(xIOException,xDataFormatException);
	
	/**
	 * Save preferences to default output stream
	 */
	void save() throw(xIOException);
	
	/**
	 * Save preferences to specified output stream
	 */
	void save(xOutputStream& sink) throw(xIOException);
	
	/**
	* Save preferences to specified file
	*/
	void save(xFile file) throw(xIOException);
};


}//namespace


#endif //XTK_PREFERENCES_H
