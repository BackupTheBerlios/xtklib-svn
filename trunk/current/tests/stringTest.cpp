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
* @file stringTest.cpp
* @author Mario Casciaro (xshadow@email.it)
*/


#include "xtk/xtk.h"
using namespace xtk;

#include <stdio.h>
#include <wchar.h>


class xStringTest : public xTest
{
private:
	virtual void doTest()
	{
		xString string1(_T("Test String"));
		ensureTrue(string1.equals(_T("Test String")),"Constructor + Equals");
		
		xString string2(string1);
		ensureTrue(string2.equals(_T("Test String")),"Copy Constructor");
		
		xString string3(string1.c_str(),4);
		ensureTrue(string3.equals(_T("Test")),"Copy Constructor limited to 4 chars");
		
		string3.append(_T(" String"));
		ensureTrue(string3.equals(_T("Test String")),"Append");
		
		xString string4 = xString::getFormat(_T("Format %d Test string %s,ok"),5,_T("str"));
		ensureTrue(string4.equals(_T("Format 5 Test string str,ok")),"Format constructor");
		
		string1.insert(_T("insertString"),4);
		ensureTrue(string1.equals(_T("TestinsertString String")),"Insertion + Copy-on-Write");
		
		xString string5(_T("123456789"));
		string5.invert();
		ensureTrue(string5.equals(_T("987654321")),"Invert");
		
		string5.remove(5);
		ensureTrue(string5.equals(_T("98765321")),"Remove index");
		
		ensureTrue(string5.substring(1,4).equals(_T("876")),"Substring");
		
		string5 = _T("0123456789");
		string5.remove(3,7);
		ensureTrue(string5.equals(_T("012789")),"Remove portion");
		
		string5 = _T("Hello this is the little string to search for this -> asd <- Yes! asdasd!!!");
		ensureTrue(string5.find(_T("asd"),0) == 54,"Find");
		
		xString string7(_T("Test replace"));
		string7.replace(_T('e'),_T('a'));
		ensureTrue(string7.equals(_T("Tast raplaca")),"Replace chars");
		
		
		string5.replace(_T("asd"),_T("asd1"));
		ensureTrue(string5.equals(_T("Hello this is the little string to search for this -> asd1 <- Yes! asd1asd1!!!")),
			"Replace strings");
			
		string5 = _T("Hello for this -> ");
		xStringTokenizer st(string5,_T(" -"));
		ensureTrue(
			(st.countTokens() == 5) &&
			(st.nextToken().equals(_T("Hello"))) &&
			(st.nextToken().equals(_T("for"))) &&
			(st.nextToken().equals(_T("this"))) &&
			(st.nextToken().equals(_T(""))) &&
			(st.nextToken().equals(_T(">")))
			,"String tokenizer");

	}
	
public:
	xStringTest() : xTest("String Test"){}
};



