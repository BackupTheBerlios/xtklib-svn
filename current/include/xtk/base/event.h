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
* @file event.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_EVENT_H
#define XTK_EVENT_H


#include "../globals.h"


namespace xtk
{

/**
 * The root class from which all event state objects shall be derived.
 */
class XTKAPI xEvent : public xObject
{
protected:
	xObject* m_source;
	
public:
	xEvent(xObject* source){m_source = source;}
	
	/**
	 * The object on which the xEvent initially occurred.
	 */
	xObject& getSource(){return *m_source;}
};


}//namespace

#endif //XTK_EVENT_H
