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
		
		frame->addMousePressedHandler(new xWidgetEventHandler<xWidgetsTest>(this,&xWidgetsTest::mousePressed));
		frame->addMouseReleasedHandler(new xWidgetEventHandler<xWidgetsTest>(this,&xWidgetsTest::mouseReleased));
		frame->addMouseDoubleClickedHandler(new xWidgetEventHandler<xWidgetsTest>(this,&xWidgetsTest::mouseDblClicked));
		
		xPanel* panel1 = new xPanel(frame,xPanel::BORDER_TITLED,_T("Panel1"));
		xPanel* panel2 = new xPanel(frame,xPanel::BORDER_TITLED,_T("Panel2"));
		
		frame->getLayout().setConstraints(*panel1,new xBoxConstraint(1,1,xBoxConstraint::FILL_BOTH));
		frame->getLayout().setConstraints(*panel2,new xBoxConstraint(2,1,xBoxConstraint::FILL_BOTH));

		xButton* butt = new xButton(panel1,_T("|"));
		butt->addActionPerformedHandler(new xWidgetEventHandler<xWidgetsTest>(this,&xWidgetsTest::actionPerformed));
		xButton* butt2 = new xButton(panel1,_T("13"),103,0);
		xButton* butt3 = new xButton(panel1,_T("aaaaaa"),103,0);
		
		xLabel* label1 = new xLabel(panel2,_T("Label1"));
		xLabel* label2 = new xLabel(panel2,_T("Label2"));

		frame->setSize(500,500);
		frame->doLayout();
		
		while(xtkProcessNextUIMessage()){}
	}

public:
	xWidgetsTest() : xTest("Widgets Test"){}
	
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
