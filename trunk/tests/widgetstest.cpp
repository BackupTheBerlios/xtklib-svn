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
#include "../include/xtk/widgets/widgetevent.h"
using namespace xtk;

class xWidgetsTest : public xTest
{
private:
	virtual void doTest()
	{
		xFrame* frame = new xFrame(NULL,_T("Frame title"));
		frame->setLayout(new xBoxLayout(xBoxLayout::BOX_ORIENTATION_X_AXIS,true,3));
		
		frame->setDefaultCloseAction(xWindow::XTK_EXIT_ON_CLOSE);
		frame->setVisible(true);
		
		xPanel* panel1 = new xPanel(frame,xPanel::BORDER_TITLED,new xBoxLayout(xBoxLayout::BOX_ORIENTATION_X_AXIS,true,3),_T("Titled panel"));
		frame->getLayout().setConstraints(*panel1,new xBoxConstraint(true,true));

		xButton* butt1 = new xButton(panel1,_T("Button1"));
		panel1->getLayout().setConstraints(*butt1,new xBoxConstraint(true,true));
		butt1->addActionPerformedHandler(new xWidgetEventHandler<xWidgetsTest>(this,&xWidgetsTest::actionPerformed));
		
		xButton* butt2 = new xButton(panel1,_T("Button2"));
		panel1->getLayout().setConstraints(*butt2,new xBoxConstraint(true,true));
		
		xLabel* lab1 = new xLabel(frame,_T("Label1asdfasdfasdfqwecdls,ol232311"));
		frame->getLayout().setConstraints(*lab1,new xBoxConstraint(true,true));
		
		xButton* butt4 = new xButton(frame,_T("Button4"));
		frame->getLayout().setConstraints(*butt4,new xBoxConstraint(true,true));
		
		frame->setSize(200,200);

		xtkUIEventLoop();
	}

public:
	xWidgetsTest() 
	: xTest(_T("Widgets Test"))
	{}
	
	virtual void actionPerformed(xtk::xWidgetEvent& e)
	{
		xSystem::getStdout().write(_T("Button 1 activated\n"));
	}
};
