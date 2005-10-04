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


#include "../include/xtk/xtk.h"
#include "../include/xtk/widgets.h"
using namespace xtk;

class xWidgetsTest : public xTest
{
private:
	virtual void doTest()
	{
		xFrame* frame = new xFrame(_T("Frame title"));
		frame->setDefaultCloseAction(xWindow::XTK_EXIT_ON_CLOSE);
		frame->setVisible(true);
		xButton* butt = new xButton(frame,_T("|"));
		xButton* butt2 = new xButton(frame,_T("13"),103,0);
		xButton* butt3 = new xButton(frame,_T("aaaaaa"),103,0);
		//butt2->setLabel(_T("Hello"));
		frame->setSize(500,500);
		frame->doLayout();
		
		while(xtkProcessNextUIMessage()){}
	}

public:
	xWidgetsTest() : xTest("Widgets Test"){}
};


