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
* @file interfaces.h
* @author Mario Casciaro (xshadow@email.it)
*/


#ifndef XTK_INTERFACES_H
#define XTK_INTERFACES_H

#include "../globals.h"

namespace xtk
{
	
/**
 * @brief This interface impose a total ordering on the objects that implements
 * it
 */
class XTKAPI xComparable : public virtual xObject
{
public:
	virtual ~xComparable(){}
	
	/**
	 * Compare this object with the specified object and return a negative 
	 * integer, zero, or a positive integer as this object 
	 * is less than, equal to, or greater than the specified object.
	 * 
	 * @exception xClassCastException
	 */
	virtual int compareTo(xObject& compare) = 0;
	
	
	virtual int compareTo(xComparable& compare)
	{
		return compareTo(dynamic_cast<xObject&>(compare));	
	}
};


/**
 * Every class that contains data (as member variables) 
 * provided by the outside world in the form of heap-allocated objects
 * should inherits xOwnership, this abstract class in fact, defines the behavior
 * to manage such objects. If you give the Ownership to an object your are saying that 
 * such object will delete every heap-allocated data coming from the outside 
 * when not needed.\n
 * By default an xOwnership object as the ownership.
 */
class XTKAPI xOwnership
{
private:
	bool owner;

protected:
	xOwnership() : owner(true){}
	
	virtual void ownershipChangedHandler(){}
	
public:

	void rescindOwnership(){owner = false; ownershipChangedHandler();}
	void giveOwnership(){owner = true; ownershipChangedHandler();}
	bool isOwner(){return owner;}
};

/**
 * This class is the same as xOwnership, the only difference is that 
 * xDefaultOwnership as the ownership rescinded by default.
 */
class XTKAPI xDefaultRescindedOwnership : public xOwnership
{
protected:
	xDefaultRescindedOwnership()
	{
		rescindOwnership();
	}
};




/**
 * A comparison function, which imposes a total ordering on two objects.
 */
template<class TClass>
class xComparator : public xObject
{
public:
	virtual ~xComparator(){}
	
	virtual int compare(TClass o1,TClass o2) = 0;
};


}//namespace

#endif //XTK_INTERFACES_H
