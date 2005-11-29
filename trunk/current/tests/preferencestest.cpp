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
* @file preferencestest.cpp
* @author Mario Casciaro (xshadow@email.it)
*/


#include "xtk/xtk.h"
using namespace xtk;

#include <stdio.h>


class xPreferencesTest : public xTest
{
private:
	virtual void doTest()
	{
		xPreferences pref(xFile(_T("prefstest.txt")));
		pref.load();
		pref.setAutosave(false);
		
		ensureTrue(pref.getString(_T("Parametro1"),_T("Default")).equals(_T("testString")),"Get string param");
		//ensureTrue(pref.getComment(_T("Parametro1")).equals(_T("commento al Parametro1\n")),"Param Comment");
		
		ensureTrue(pref.getInt(_T("Parametro2"),1) == 1000,"Get int param");
		
		ensureTrue(pref.getFloat(_T("Parametro3"),1.0) == 100.12f,"Get float param");
		
		pref.save(xFile(_T("preftest2.txt")));
	}

public:
	xPreferencesTest() : xTest("Preferences Test"){}
};



