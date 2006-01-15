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
* @file tdatastructurestest.h
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#include "../include/xtk/xtk.h"
using namespace xtk;

class xTDataStructuresTest : public xTest
{
private:
	virtual void doTest()
	{
		xTLinkedList<int> list(&(xOperatorEquality<int>::getInstance()));
		list.add(1);
		list.add(2);
		list.add(3);
		ensureTrue(list.size() == 3,_T("xTLinkedList<int>::size()"));
		ensureTrue(list.get(0) == 1 && list.get(1) == 2 && list.get(2) == 3,_T("xTLinkedList<int>::get()"));
		ensureTrue(list.getFirst() == 1 && list.getLast() == 3 ,_T("xTLinkedList<int>::getFirst() and xTLinkedList<int>::getLast()"));
		ensureTrue(list.contains(2) && !list.contains(0) ,_T("xTLinkedList<int>::contains()"));

		list.set(1,0);
		ensureTrue(list.get(1) == 0,_T("xTLinkedList<int>::set()"));

		list.insert(1,5);
		ensureTrue(list.get(1) == 5,_T("xTLinkedList<int>::insert()"));

		xTListIterator<int>* liter = list.listIterator();
		ensureTrue(liter->next() == 1 && liter->next() == 5 && liter->next() == 0 && liter->next() == 3 && !liter->hasNext(),_T("xTLinkedListListIterator<int>::next()"));
		ensureTrue(liter->previous() == 0 && liter->previous() == 5 && liter->previous() == 1 && !liter->hasPrevious(),_T("xTLinkedListListIterator<int>::previous()"));

		liter->remove();
		ensureTrue(list.get(0) == 5 && list.get(1) == 0 && list.get(2) == 3 && list.size() == 3,_T("xTLinkedListListIterator<int>::remove()"));
		delete liter;

		list.remove(1);
		ensureTrue(list.get(0) == 5 && list.get(1) == 3 && list.size() == 2,_T("xTLinkedList<int>::removeIndex()"));

		list.clear();
		ensureTrue(list.size() == 0,_T("xTLinkedList<int>::clear()"));

		//-------------------------------------------------------------------------------

		xTArrayList<int> list2(&(xOperatorEquality<int>::getInstance()));
		list2.add(1);
		list2.add(2);
		list2.add(3);
		ensureTrue(list2.size() == 3,_T("xTArrayList<int>::size()"));
		ensureTrue(list2.get(0) == 1 && list2.get(1) == 2 && list2.get(2) == 3,_T("xTArrayList<int>::get()"));
		ensureTrue(list2.contains(2) && !list2.contains(0) ,_T("xTArrayList<int>::contains()"));

		list2.set(1,0);
		ensureTrue(list2.get(1) == 0,_T("xTArrayList<int>::set()"));

		list2.insert(1,5);
		ensureTrue(list2.get(1) == 5,_T("xTArrayList<int>::insert()"));

		liter = list2.listIterator();
		ensureTrue(liter->next() == 1 && liter->next() == 5 && liter->next() == 0 && liter->next() == 3 && !liter->hasNext(),_T("xTArrayListListIterator<int>::next()"));
		ensureTrue(liter->previous() == 0 && liter->previous() == 5 && liter->previous() == 1 && !liter->hasPrevious(),_T("xTArrayListListIterator<int>::previous()"));

		liter->remove();
		ensureTrue(list2.get(0) == 5 && list2.get(1) == 0 && list2.get(2) == 3 && list2.size() == 3,_T("xTArrayListListIterator<int>::remove()"));
		delete liter;

		list2.remove(1);
		ensureTrue(list2.get(0) == 5 && list2.get(1) == 3 && list2.size() == 2,_T("xTArrayList<int>::removeIndex()"));

		list2.clear();
		ensureTrue(list2.size() == 0,_T("xTArrayList<int>::clear()"));

		//-------------------------------------------------------------------------------
		
		xTAvlMap<int,xString*> map(&(xOperatorComparator<int>::getInstance()),&(xObjectEquality<xString*>::getInstance()));

		xString* str = new xString(_T("1"));
		map.put(1,str);
		str = new xString(_T("2"));
		map.put(2,str);
		str = new xString(_T("3"));
		map.put(3,str);
		str = new xString(_T("4"));
		map.put(4,str);
		str = new xString(_T("5"));
		map.put(5,str);

		ensureTrue(map.checkAVL(),_T("Check AVL"));
		ensureTrue(map.remove(4,&str) && map.checkAVL(),_T("xTAvlMap<int,xString*>::remove()"));
		delete str;

		xtkDeleteAll(map);
		map.clear();

		xRandom rand;
		int iteration = 1000;
		for(int i = 0;i < iteration;i++)
		{	
			int tmp = rand.nextInt();
			if(!map.containsKey(tmp))
				map.put(tmp,new xString(xString::getFormat(_T("%d"),tmp)));
		}
		ensureTrue(map.checkAVL() && map.size() == iteration,_T("xTAvlMap<int,xString*>::put() random inserts"));
		xtkDeleteAll(map);
		

		//-------------------------------------------------------------------------------
		xTHashMap<int,xString*> hmap(&(xNumberHashable<int>::getInstance()),&(xOperatorEquality<int>::getInstance()),
			&(xObjectEquality<xString*>::getInstance()));

		str = new xString(_T("1"));
		hmap.put(1,str);
		/*
		str = new xString(_T("2"));
		hmap.put(2,str);
		str = new xString(_T("3"));
		hmap.put(3,str);
		str = new xString(_T("4"));
		hmap.put(4,str);
		str = new xString(_T("5"));
		hmap.put(5,str);
		*/
		xtkDeleteAll(hmap);
	}

public:
	xTDataStructuresTest()
	: xTest(_T("Template Data Structures Test"))
	{}
};
