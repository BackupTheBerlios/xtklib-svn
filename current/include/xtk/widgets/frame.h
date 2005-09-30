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
* @file frame.h
* @author Mario Casciaro (xshadow@email.it)
*/

#ifndef XTK_FRAME_H
#define XTK_FRAME_H

#include "../globals.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{


class XTKAPI xAbstractFrame
{
protected:
	xAbstractFrame(){}
	
public:
	virtual ~xAbstractFrame(){}

	/**
	 * Gets the title of the frame.
	 */
	virtual xString getTitle() = 0;
	
	/**
	 * Indicates whether this frame is resizable by the user.
	 */
	virtual bool isResizable() = 0;
	
	/**
	 * Sets whether this frame is resizable by the user.
	 */
	virtual void setResizable(bool resizable) = 0;
		
	/**
	 * Sets the title for this frame to the specified string.
	 */
	virtual void setTitle(xString title) = 0;
};



}//namespace
#endif //XTK_USE_WIDGETS

#endif //XTK_FRAME_H