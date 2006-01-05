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
* @file random.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#include "../../include/xtk/base/random.h"

namespace xtk
{

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xRandom::xRandom(long seed)
{
	setSeed(seed);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
void xRandom::setSeed(long seed)
{
	m_seed1 = ((69069 * seed) & 0xffffffffL);
	m_seed2 = ((69069 * seed) & 0xffffffffL);
	m_seed3 = ((69069 * seed) & 0xffffffffL);
  
	next();
	next();
	next();
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xRandom::next()
{
	#define MASK 0xffffffffUL
	#define TAUSWORTHE(s,a,b,c,d) (((s & c) <<d) & MASK) ^ ((((s << a) & MASK)^s) >> b)

	m_seed1 = TAUSWORTHE (m_seed1, 13, 19, 4294967294UL, 12);
	m_seed2 = TAUSWORTHE (m_seed2, 2, 25, 4294967288UL, 4);
	m_seed3 = TAUSWORTHE (m_seed3, 3, 11, 4294967280UL, 17);

	return (m_seed1 ^ m_seed2 ^ m_seed3);
}


//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xRandom::nextBool()
{
	return (((unsigned int)next()) % 2) == 1;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
double xRandom::nextDouble()
{
	return (((unsigned int)next()) / 4294967296.0);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
float xRandom::nextFloat()
{
	return (float)(((unsigned int)next()) / 4294967296.0);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xRandom::nextInt()
{
	return next();	
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xRandom::nextInt(int n)
{
	return ((unsigned int)next() % n);
}


}//end namespace

