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
#include "window.h"
#include "../base/string.h"

#ifdef XTK_USE_WIDGETS
namespace xtk
{

class xFrameInternal;

/**
 * Defines the interface for the various implementation of class xFrame.
 */
class XTKAPI xFrame : public xWindow
{
private:
	xFrameInternal* m_internal;
	
protected:
	xFrame(xWidget* parent,xString title,xLayoutManager* layout,xFrameInternal* i);
	
public:
	xFrame(xWidget* parent,xString title,
		xLayoutManager* layout = new xBoxLayout(xBoxLayout::BOX_ORIENTATION_X_AXIS,false,1));
		
	virtual ~xFrame();

	/**
	 * Gets the title of the frame.
	 */
	virtual xString getTitle();
	
	/**
	 * Indicates whether this frame is resizable by the user.
	 */
	virtual bool isResizable();
	
	/**
	 * Sets whether this frame is resizable by the user.
	 */
	virtual void setResizable(bool resizable);
		
	/**
	 * Sets the title for this frame to the specified string.
	 */
	virtual void setTitle(xString title);
};

}//namespace

#endif //XTK_USE_WIDGETS

#endif //XTK_FRAME_H
