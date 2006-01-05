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
* @file functor.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/

#ifndef XTK_FUNCTOR_H
#define XTK_FUNCTOR_H

#include "../globals.h"

namespace xtk
{

/**
* Functors are objects used to encapsulate C++ function pointers, xFunctor is just 
* a tagging class for all functors.
*/
class XTKAPI xFunctor : public virtual xObject
{
protected:
	xFunctor(){}
public:
	virtual ~xFunctor(){}
};


}//namespace
#endif //XTK_FUNCTOR_H
