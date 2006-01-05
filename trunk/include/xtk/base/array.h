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
* @file array.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#ifndef XTK_ARRAY_H
#define XTK_ARRAY_H

#include "../globals.h"
#include "string.h"
#include "exception.h"
#include "interfaces.h"
#include "math.h"

namespace xtk
{

/**
 * @brief An array with automatic memory management.
 * 
 * You can dynamically define the size of this array. This array class use
 * shared data, with copyOnWrite support.
 *
 * @warning You must pay particular attention with the use of operator [] and 
 * the function getRawData(). If you use this to modify the content of the array
 * you should manually call the function copyOnWrite().
 * 
 * @remark If you want to create an array of complex objects you should take care 
 * of providing an adequate operator "=" for them because xArray will use it when
 * resizing or copying.
 */
template <class TClass>
class xArray : public virtual xObject
{
private:
	class xArrayData : public virtual xObject
	{
	public:
		TClass*			m_array;
		int 			m_size;
		int				m_referenceCount;
		
		xArrayData()
		{
			m_array = NULL;
			m_size = 0;
			m_referenceCount = 0;
		}
	};

	xArrayData*	m_data;
	
	/**
	 * 
	 */
	void resizeArray(int newSize) throw(xIllegalArgumentException)
	{
		if(newSize < 0)
			throw xIllegalArgumentException(_T("Size cannot be negative"));
		
		if(newSize == 0)
		{
			if(m_data->m_array != NULL && m_data->m_size > 0)
			{
				delete[] (m_data->m_array);
				m_data->m_array = NULL;
			}
		}
		else if(m_data->m_array == NULL || m_data->m_size == 0)
		{
			m_data->m_array = new TClass[newSize];
		}
		else if(m_data->m_size != newSize)
		{
			TClass* tmp = m_data->m_array;
			m_data->m_array = new TClass[newSize];
			for(register int i = 0;i < MIN(newSize,m_data->m_size);i++)
			{
				m_data->m_array[i] = tmp[i];
			}
			delete[] tmp;
		}
		m_data->m_size = newSize;
	}
	
public:
	/**
	 * @brief Create an empty array
	 */
	xArray(int init_size = 0) throw(xIllegalArgumentException)
	{
		m_data = new xArrayData();
		m_data->m_referenceCount++;
		resizeArray(init_size);
	}
	
	/**
	 * @brief Create an empty array
	 */
	xArray(const TClass* data,int off,int len) throw(xIndexOutOfBoundsException)
	{
		m_data = new xArrayData();
		m_data->m_referenceCount++;
		
		resizeArray(len + off);
		copyData(data,off,len);
	}
	
	/**
	 * Copy constructor
	 */
	xArray(const xArray<TClass>& copy) : xObject()
	{
		xArray<TClass>& copy2 = (xArray<TClass>&) copy;
		copy2.synchronizeStart();
		
		copy2.m_data->m_referenceCount++;
		m_data = copy2.m_data;
		
		copy2.synchronizeEnd();
	}
	
	/**
	 * Copy constructor 2
	 * 
	 */
	xArray(xArray<TClass>& copy,int off,int len) throw(xIndexOutOfBoundsException)
	{
		if(len <= 0 || off+len > copy.size())
			throw xIndexOutOfBoundsException();
		
		m_data = new xArrayData();
		m_data->m_referenceCount++;
		resizeArray(off + len);
		
		copyData(copy.getRawData() + off,0,len);
	}
	
	virtual ~xArray()
	{
		m_data->synchronizeStart();
		
		m_data->m_referenceCount--;
		if(m_data->m_referenceCount <= 0)
		{
			resizeArray(0);
			
			m_data->synchronizeEnd();
			delete m_data;
		}
		else
			m_data->synchronizeEnd();
		
	}
	
	/**
	 * 
	 */
	void copyOnWrite()
	{
		m_data->synchronizeStart();

		if(m_data->m_referenceCount > 1)
		{
			m_data->m_referenceCount--;
			xArrayData* oldData = m_data;

			m_data = new xArrayData();
			m_data->m_referenceCount++;
			resizeArray(oldData->m_size);

			for(register int i = 0;i < m_data->m_size;i++)
				m_data->m_array[i] = oldData->m_array[i];
		}

		m_data->synchronizeEnd();
	}
	
	/**
	 * Resize the array to 0 and delete all its elements.
	 */
	void clear()
	{
		m_data->synchronizeStart();
		
		if(m_data->m_referenceCount <= 1)
			resizeArray(0);
		else
		{
			m_data->m_referenceCount--;
			m_data = new xArrayData();
		}
		
		m_data->synchronizeEnd();
		
	}
	
	/**
	 * Reserve space in the array
	 */
	void resize(int size) 
	throw(xIllegalArgumentException)
	{
		copyOnWrite();
		resizeArray(size);
	}
	
	/**
	 * Returns the raw data of the array contents
	 */
	TClass* getRawData()
	throw()
	{
		return m_data->m_array;
	}
	
	/**
	 * Returns the element at the specified position
	 * 
	 * @exception xIndexOutOfBoundsException
	 */
	TClass& operator[](int position)
	throw(xIndexOutOfBoundsException)
	{
		if(position < 0 || (position >= m_data->m_size))
			throw xIndexOutOfBoundsException();
				
		return m_data->m_array[position];
	}
	
	
	int size()
	throw()
	{
		return m_data->m_size;	
	}
	
	
	/**
	 * Copy the content of the given buffer into this array
	 */
	void copyData(const TClass* data,int arrayoff,int len)
	throw(xIndexOutOfBoundsException)
	{
		if(arrayoff < 0 || arrayoff+len > size() || len < 0)
			throw xIndexOutOfBoundsException();
			
		copyOnWrite();
		
		for(register int i = 0;i < len;i++)
			m_data->m_array[i+arrayoff] = data[i];
	}
	
	
	/** 
	 * Call delete over EVERY non-NULL object in the array.
	 * If there are some other references on the current array
	 * no deletion will occur.
	 */
	void deleteData()
	throw()
	{
		m_data->synchronizeStart();
		if(m_data->m_referenceCount == 1)
		{
			for(int i = 0;i < size();i++)
			{
				if(m_data->m_array[i] != NULL)
					delete m_data->m_array[i];
			}
		}
		m_data->synchronizeEnd();
	}
	
	
	/**
	 * 
	 */
	void initElements(TClass data)
	throw()
	{
		copyOnWrite();
		
		for(register int i = 0;i < size();i++)
			m_data->m_array[i] = data;
	}
	
	
	/**
	 * Sorts this array according to the order induced by the specified comparator.
	 * Uses merge sort algorithm with guaranteed n*log(n) performance.
	 */
	void sort(xComparator<TClass>& comparator,int begin,int end)
	{
		int mid = (int) xMath::floor((end + begin) / 2);
		int i,j,k;
		if(end - begin < 1)
			return;
		sort(comparator,begin,mid);
		sort(comparator,mid+1,end);

		//init fusion
		xArray<TClass> arrTmp(end - begin + 1);
		for(i = begin,j = mid+1,k = 0;i <= mid && j <= end;)
		{
			if(comparator.compare(m_data->m_array[j],m_data->m_array[i]) < 0)
			{
				arrTmp[k] = m_data->m_array[j];
				k++;
				j++;
			}
			else
			{
				arrTmp[k] = m_data->m_array[i];
				k++;
				i++;
			}//end else
		}
		//copy the remained elements
		while(i <= mid)
		{
			arrTmp[k] = m_data->m_array[i];
			i++;
			k++;
		}
		while(j <= end)
		{
			arrTmp[k] = m_data->m_array[j];
			j++;
			k++;
		}
		
		//copy array
		for(i = begin , k = 0;k < arrTmp.size(); k++,i++)
			m_data->m_array[i] = arrTmp[k];
	}
	
	
	/**
	 * Sorts this array according to the order induced by the specified comparator.
	 * Uses merge sort algorithm with guaranteed n*log(n) performance.
	 */
	void sort(xComparator<TClass>& comparator)
	{
		sort(comparator,0,size() - 1);
	}
	
	/**
	 * Shifts elements left of len positions
	 */
	void shiftLeft(int len)
	{
		for(register int i = 0;i < size() - len;i++)
			m_data->m_array[i] = m_data->m_array[i + len];
	}
	
	/**
	 * Copy an array into another
	 */
	static void copy(xArray<TClass>& dst,int dstoff,xArray<TClass>& src,int srcoff,int len)
	throw(xIndexOutOfBoundsException)
	{
		if(dstoff < 0 || srcoff < 0 || dstoff + len > dst.size() || srcoff + len > src.size() )
			throw xIndexOutOfBoundsException();
			
		dst.copyOnWrite();
		
		int i = dstoff;
		int j = srcoff;
		for(int k = 0;k < len;k++)
			dst[i++] = src[j++];
	}
	
	
	template<class TClass2>
	xArray<TClass2> castTo() throw(xClassCastException)
	{
		try
		{
			xArray<TClass2> temp(size());
			for(int i = 0;i < size();i++)
			{
				temp[i] = dynamic_cast<TClass2>(m_data->m_array[i]);
				if(temp[i] == NULL)
					throw xClassCastException();
			}
			
			return temp;
		}
		catch(...) //catch bad cast exception in case of reference cast
		{
			throw xClassCastException();
		}
	}
	
	
	virtual xObject* clone() 
	throw()
	{
		return new xArray<TClass>(*this);
	}
	
	
	virtual xString toString()
	throw()
	{
		return xString::getFormat(_T("Array of size %d"),m_data->m_size);
	}
	
	virtual bool equals(xArray<TClass>& other)
	{
		if(m_data == other.m_data)
			return true;
		if(size() != other.size())
			return false;
		
		for(int i = 0;i < size();i++)
		{
			if(m_data->m_array[i] != other[i])
				return false;
		}
		
		return true;
	}
	
	
	virtual bool equals(xObject& obj)
	{
		xArray<TClass>* a = dynamic_cast<xArray<TClass>* >(&obj);
		if(a == NULL)
			return false;
			
		return equals(*a);
	}
	
	const xArray<TClass>& operator=(const xArray<TClass>& copy)
	{
		xArray<TClass>& copy2 = (xArray<TClass>&) copy;
		m_data->synchronizeStart();

		m_data->m_referenceCount--;
		if(m_data->m_referenceCount <= 0)
		{
			resizeArray(0);

			m_data->synchronizeEnd();
			delete m_data;
		}
		else
			m_data->synchronizeEnd();
		
		copy2.synchronizeStart();
		copy2.m_data->m_referenceCount++;
		m_data = copy2.m_data;
		copy2.synchronizeEnd();
		
		return *this;
	}
	
};
	
	
}//namespace



#endif //XTK_ARRAY_H
