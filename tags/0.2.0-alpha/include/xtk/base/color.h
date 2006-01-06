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
* @file color.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_COLOR_H
#define XTK_COLOR_H

#include "../globals.h"

namespace xtk
{

/**
* The xColor class is used encapsulate colors in the RGB color space.
*/
class XTKAPI xColor : public virtual xObject
{
private:
	xui8 m_red;
	xui8 m_green;
	xui8 m_blue;
	
public:
	//const static xColor RED;
	//const static xColor GREEN;
	//const static xColor BLUE;
	//const static xColor WHITE;
	//const static xColor BLACK;


	xColor(){set(0,0,0);}
	xColor(xui8 red,xui8 green,xui8 blue){set(red,green,blue);}
	
	xui8 getRed(){return m_red;}
	xui8 getGreen(){return m_green;}
	xui8 getBlue(){return m_blue;}
	
	void set(xui8 red,xui8 green,xui8 blue)
	{
		m_red = red;
		m_green = green;
		m_blue = blue;
	}
	
	bool equals(xColor& c)
	{
		return (m_red == c.m_red && m_green == c.m_green && m_blue == c.m_blue);
	}
	
	virtual bool equals(xObject& o)
	{
		xColor* c = dynamic_cast<xColor*>(&o);
		if(c == NULL)
			return false;
			
		return equals(*c);
	}
	
	virtual int hashCode(){return ((int)m_red) | ((int)m_green << 8) ^ ((int)m_blue << 4);}
};





}//namespace

#endif //XTK_COLOR_H
