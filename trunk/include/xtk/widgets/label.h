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
* @file label.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_LABEL_H
#define XTK_LABEL_H

#include "../globals.h"
#include "widget.h"
#include "../base/string.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

class xLabelInternal;

/**
 * A label is a static widget that cannot interact with the user,used to display texts 
 * other drawings
 */
class XTKAPI xLabel : public xWidget
{
private:
	xLabelInternal* m_internal;
	
protected:
	xLabel(xWidget* parent,xString text,xLabelInternal* i);
		
public:
	xLabel(xWidget* parent,xString text);
	
	virtual ~xLabel();

	/**
	* Gets the text of this label.
	*/
	virtual xString getText();

	/**
	* Set the text of this label.
	*/
	virtual void setText(xString label);
};


}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_LABEL_H
