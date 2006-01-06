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
* @file math.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_MATH_H
#define XTK_MATH_H


#include "../globals.h"
#include <math.h>


namespace xtk
{
/**
* @brief A set of math functions.
*/
class XTKAPI xMath
{
private:	
	xMath();
	
public:

	/**
	* 
	*/
	static double pow(double x, double y)
	{
		return ::pow(x,y);
	}
	
	/**
	* 
	*/
	static int abs(int x)
	{
		return ::abs(x);
	}
	
	/**
	* 
	*/
	static double floor(double x)
	{
		return ::floor(x);
	}
};


}//namespace

#endif //XTK_MATH_H
