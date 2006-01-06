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
* @file geometry.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_GEOMETRY_H
#define XTK_GEOMETRY_H

#include "../globals.h"

namespace xtk
{

/**
 * A xRectangle specifies a rectangular area in a coordinate space.
 */
class XTKAPI xRectangle : public virtual xObject
{
private:
	int		m_x;
	int		m_y;
	int		m_height;
	int		m_width;
	
public:
	xRectangle(){set(0,0,0,0);}
	xRectangle(int x,int y,int width,int height){set(x,y,width,height);}
	
	virtual ~xRectangle(){}
	
	int getX(){return m_x;}
	int getY(){return m_y;}
	int getHeight(){return m_height;}
	int getWidth(){return m_width;}
	void set(int x,int y,int width,int height)
	{
		m_x = x;
		m_y = y;
		m_height = height;
		m_width = width;
	}
	
	bool equals(xRectangle& r)
	{return (m_y == r.m_y && m_x == r.m_x && m_height == r.m_height && m_width == r.m_width);}
	
	virtual bool equals(xObject& o)
	{
		xRectangle* r = dynamic_cast<xRectangle*>(&o);
		if(r == NULL)
			return false;
			
		return equals(*r);
	}
};


/**
 * Represent a point in a 2d space coordinate.
 */
class XTKAPI xPoint : public virtual xObject
{
private:
	int		m_x;
	int		m_y;

public:
	xPoint(){set(0,0);}
	xPoint(int x,int y){set(x,y);}
	virtual ~xPoint(){}

	int getX(){return m_x;}
	int getY(){return m_y;}
	void set(int x,int y)
	{
		m_x = x;
		m_y = y;
	}
	
	bool equals(xPoint& p)
	{return (m_y == p.m_y && m_x == p.m_x);}

	virtual bool equals(xObject& o)
	{
		xPoint* p = dynamic_cast<xPoint*>(&o);
		if(p == NULL)
			return false;

		return equals(*p);
	}
};


/**
* A class for defines dimensions
*/
class XTKAPI xDimension : public virtual xObject
{
private:
	int		m_height;
	int		m_width;

public:
	xDimension()
	{set(0,0);}
	
	xDimension(int width,int height)
	{set(width,height);}

	virtual ~xDimension(){}

	int getHeight()
	{return m_height;}
	
	int getWidth()
	{return m_width;}
	
	void setHeight(int height)
	{m_height = height;}
	
	void setWidth(int width)
	{m_width = width;}
	
	void set(int width,int height)
	{
		m_height = height;
		m_width = width;
	}

	bool equals(xDimension& d)
	{return (m_height == d.m_height && m_width == d.m_width);}

	virtual bool equals(xObject& o)
	{
		xDimension* d = dynamic_cast<xDimension*>(&o);
		if(d == NULL)
			return false;

		return equals(*d);
	}
};


}//namespace

#endif //XTK_GEOMETRY_H
