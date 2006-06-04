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
* @file document.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_DOCUMENT_H
#define XTK_DOCUMENT_H

#include "../globals.h"
#include "interfaces.h"
#include "charset.h"
#include "stream.h"

namespace xtk
{

/**
 * The xDocument is a container for text that serves as the model for text visualization components.
 * It is part of the Model/View framework implemented in xtklib.
 */
class XTKAPI xDocument : public virtual xObject
{
public:

	/**
	 * Registers the given observer to begin receiving notifications when changes are made to the document.
	 */
	virtual void addDocumentListener(xDocumentListener listener) = 0;

	/**
	 * Returns the root element that views should be based upon.
	 */
	virtual xDocumentElement getDefaultRootElement() = 0;
        
	/**
	 * Returns number of characters of content currently in the document.
	 */
	virtual int getLength() = 0;

	/**
	 * Gets the properties associated with the document.
	 */
	virtual xObject getProperty(xObject key) = 0;
         
	/**
	 *  Returns all of the root elements that are defined.
	 */
	virtual xArray<MYOWNERSHIP xDocumentElement*> getRootElements() = 0;
    
	/**
	 * Fetches the text contained within the given portion of the document.
	 */
	virtual xString getText(int offset, int length) = 0;

    /**
	 * Inserts a string of content.
	 */
	virtual void insertString(int offset, String str, AttributeSet a) = 0;

	/**
	 * Associates a property with the document.
	 */
	virtual void putProperty(xObject key, xObject value) = 0;
    
	/**
	 * Removes a portion of the content of the document.
	 */
	virtual void remove(int offs, int len) = 0;
    
	/**
	 *  Unregisters the given observer from the notification list so it will no longer receive change updates.
	 */
	virtual void removeDocumentListener(xDocumentListener listener) = 0;
};



}//namespace

#endif //XTK_DOCUMENT_H
