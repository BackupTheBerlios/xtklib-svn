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
* @file random.h
* @author Mario Casciaro (xshadow@email.it)
*/


#ifndef XTK_RANDOM_H
#define XTK_RANDOM_H

#include "../globals.h"
#include <time.h>

namespace xtk
{

/**
 * An object used for generate pseudorandom numbers.
 * The default version of this class use the "Tausworthe
   generator".
 */
class XTKAPI xRandom : public xObject
{
private:
	int m_seed1;
	int m_seed2;
	int m_seed3;
	
protected:
	
	/**
	 * Generates the next pseudorandom number.
	 */
	virtual int next();
	
public: 

	/**
	 * Creates a new random number generator using an optional seed
	 */
	xRandom(long seed = time(NULL));
	
	virtual ~xRandom(){}
	
	/**
	 * Returns the next pseudorandom boolean.
	 */
	virtual bool nextBool();
         
	/**
	 * Returns the next pseudorandom double value between 0.0 and 1.0
	 */
	virtual double nextDouble();
	
	/**
	 * Returns the next pseudorandom float value between 0.0 and 1.0
	 */
 	virtual float nextFloat();
    
    /**
     * Returns the next pseudorandom integer
     */
	virtual int nextInt();
         
    /**
     * Returns a pseudorandom, uniformly distributed int value between 0 
     * (inclusive) and the specified value (exclusive).
     */     
	virtual int nextInt(int n);
        
    /**
     * Sets the seed of this random number generator using a single long seed.
     */
	virtual void setSeed(long seed);
};
	
	
	
}//end namespace


#endif //XTK_RANDOM_H
