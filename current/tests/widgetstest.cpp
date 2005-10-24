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
		xFrame* frame = new xFrame(NULL,_T("Frame title"));
		//frame->setLayout(new xBoxLayout(xBoxLayout::Y_AXIS));
		frame->setDefaultCloseAction(xWindow::XTK_EXIT_ON_CLOSE);
		frame->setVisible(true);
		
		xButton* butt1 = new xButton(frame,_T("Button1"));
		frame->getLayout().setConstraints(*butt1,new xBoxConstraint(0,true,true,1));
		butt1->addActionPerformedHandler(new xWidgetEventHandler<xWidgetsTest>(this,&xWidgetsTest::actionPerformed));
		xButton* butt2 = new xButton(frame,_T("Button2"));
		
		xtkUIEventLoop();
	}

public:
	xWidgetsTest() : xTest(_T("Widgets Test")){}
	
	virtual void actionPerformed(xWidgetEvent& e)
	{
		xSystem::getStdout().write(_T("Button 1 activated\n"));
	}
	
	virtual void mousePressed(xWidgetEvent& ev)
	{
		xSystem::getStdout().write(_T("Mouse button pressed\n"));
	}
	
	virtual void mouseReleased(xWidgetEvent& ev)
	{
		xSystem::getStdout().write(_T("Mouse button released\n"));
	}
	
	virtual void mouseDblClicked(xWidgetEvent& ev)
	{
		xSystem::getStdout().write(_T("Mouse double clicked\n"));
	}
	
	virtual void focusGained(xFocusEvent& e)
	{
		xSystem::getStdout().write(_T("Button focus gained\n"));
	}
};
