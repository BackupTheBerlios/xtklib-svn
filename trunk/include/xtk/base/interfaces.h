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
* @file interfaces.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#ifndef XTK_INTERFACES_H
#define XTK_INTERFACES_H

#include "../globals.h"

namespace xtk
{
	
/**
 * This interface impose a total ordering on the objects that implements it.
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
 * A function that test if two elements are equals.
 */
template<class TClass>
class xEquality : public virtual xObject
{
public:
	virtual ~xEquality()
	{}

	virtual bool equals(TClass o1,TClass o2) = 0;
};

/**
 * A convenience xEquality class that uses the equals() method of xObject class.
 * TClass must be a subclass of xObject.
 */
template <class TClass>
class xObjectEquality : public xEquality<TClass>,private xDefaultInstance
{
public:
	static xObjectEquality& getInstance()
	{
		static xObjectEquality s;
		return s;
	}

	virtual ~xObjectEquality()
	{}

	virtual bool equals(TClass o1,TClass o2)
	{return o1->equals(*o2);}
};

/**
 * A convenience xEquality class that uses the operator==.
 * TClass must define a valid == operator.
 */
template <class TClass>
class xOperatorEquality : public xEquality<TClass>,private xDefaultInstance
{
public:
	static xOperatorEquality& getInstance()
	{
		static xOperatorEquality s;
		return s;
	}

	virtual ~xOperatorEquality()
	{}

	virtual bool equals(TClass o1,TClass o2)
	{return o1==o2;}
};


/**
 * A comparison function, which imposes a total ordering on two objects.
 */
template<class TClass>
class xComparator : public xEquality<TClass>
{
public:
	virtual ~xComparator()
	{}
	
	virtual int compare(TClass o1,TClass o2) = 0;

	virtual bool equals(TClass o1,TClass o2)
	{return compare(o1,o2) == 0;}
};

/**
 * A convenience implementation of xComparator that uses operators <,==,>.
 */
template<class TClass>
class xOperatorComparator : public xComparator<TClass>,private xDefaultInstance
{
public:
	static xOperatorComparator& getInstance()
	{
		static xOperatorComparator s;
		return s;
	}

	virtual ~xOperatorComparator()
	{}
	
	virtual int compare(TClass o1,TClass o2)
	{
		if(o1 < o2)
			return -1;
		if(o1 > o2)
			return 1;
		return 0;
	}

	virtual bool equals(TClass o1,TClass o2)
	{return o1 == o2;}
};


/**
 * A convenience implementation of xComparator that uses operators ->compareTo().
 */
template<class TClass>
class xObjectComparator : public xComparator<TClass>,private xDefaultInstance
{
public:
	static xObjectComparator& getInstance()
	{
		static xObjectComparator s;
		return s;
	}

	virtual ~xObjectComparator()
	{}
	
	virtual int compare(TClass o1,TClass o2)
	{return o1->compareTo(*o2);}
};

/**
 * A function that compute the hash of an element.
 */
template<class TClass>
class xHashable : public virtual xObject
{
public:
	virtual ~xHashable()
	{}
	
	virtual int hashOf(TClass o) = 0;
};


/**
 * A convenience implementation of xHashable that uses operator ->hashCode().
 */
template<class TClass>
class xObjectHashable : public xHashable<TClass>,private xDefaultInstance
{
public:
	static xObjectHashable& getInstance()
	{
		static xObjectHashable s;
		return s;
	}

	virtual ~xObjectHashable()
	{}
	
	virtual int hashOf(TClass o)
	{return o->hashCode();}
};


/**
 * A convenience implementation of xHashable that uses a generic number(int,char,int64,etc.) for the hash.
 */
template<class TClass>
class xNumberHashable : public xHashable<TClass>,private xDefaultInstance
{
public:
	static xNumberHashable& getInstance()
	{
		static xNumberHashable s;
		return s;
	}

	virtual ~xNumberHashable()
	{}
	
	virtual int hashOf(TClass o)
	{
		return (int)o;
	}
};


}//namespace

#endif //XTK_INTERFACES_H
