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
* @file smartptr.h
* @author Mario Casciaro (xshadow@email.it)
*/

#include "../globals.h"
#include "interfaces.h"

#ifndef XTK_SMARTPTR_H
#define XTK_SMARTPTR_H


namespace xtk
{
	
/**
 * Smart pointer permits to delete the referenced object when the current
 * scope ends or when a chance of the referenced object occurs.
 * So Smart pointers have the ownership by default.
 */
template <class TClass>
class smartPtr : public xOwnership
{
private:
	TClass* ptr;
	
public:
	/**
	 * 
	 */
	smartPtr(TClass* obj = NULL) 
	{
		ptr = obj;
		giveOwnership();
	}
	

	/**
	 * copy constructor
	 */
	smartPtr(const smartPtr<TClass>& obj) : xOwnership()
	{
		ptr = obj.ptr;
	}
	
	/**
	 * 
	 */
	virtual ~smartPtr() 
	{
		if (ptr != NULL && isOwner()) 
		{
			delete ptr;
		}
	}
	
	/**
	 * 
	 *
	TClass& operator=(const TClass& copy) 
	{
		if (this != &copy)
		{
        	if (ptr != NULL) 
        		delete ptr;
        		
        	ptr = copy.ptr;
    	}
    	return *this;
	}
	*/
	
	/**
	 * 
	 */
	TClass* operator=(TClass* copy) 
	{
		if (ptr != copy) 
		{
        	if (ptr != NULL && isOwner())
            	delete ptr;
            	
        	ptr = copy;
    	}
    	
    	return ptr;
	}
	
	TClass* operator&()
	{
		return ptr;
	}

	/**
	 * 
	 */
	TClass* operator->() 
	{
		return ptr;
	}
	
	/**
	 * 
	 */
	TClass& operator*() 
	{
		return (TClass&) *ptr;
	}
	

	int operator==(const smartPtr<TClass>& n) 
	{
		return this->ptr == n.ptr;
	}
	
	int operator!=(const smartPtr<TClass>& n) 
	{
		return this->ptr != n.ptr;
	}
	
	int operator==(TClass* n) 
	{
		return this->ptr == n;
	}
	
	int operator!=(TClass* n) 
	{
		return this->ptr != n;
	}
};


}//namespace

#endif //XTK_SMARTPTR_H
