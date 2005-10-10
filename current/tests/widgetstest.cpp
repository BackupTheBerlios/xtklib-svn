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

class xWidgetsTest : public xTest,public xActionListener,public xMouseListener,public xFocusListener
{
private:
	virtual void doTest()
	{
		xFrame* frame = new xFrame(_T("Frame title"));
		//frame->setLayout(new xBoxLayout(xBoxLayout::Y_AXIS));
		frame->setDefaultCloseAction(xWindow::XTK_EXIT_ON_CLOSE);
		frame->setVisible(true);
		frame->addMouseListener(this);
		
		xPanel* panel1 = new xPanel(frame,xPanel::BORDER_TITLED,_T("This is a panel"));
		frame->getLayout().setConstraints(*panel1,new xBoxConstraint(1,1,xBoxConstraint::FILL_BOTH));
		xPanel* panel2 = new xPanel(frame,xPanel::BORDER_TITLED,_T("This is a panel2"));
		frame->getLayout().setConstraints(*panel2,new xBoxConstraint(2,1,xBoxConstraint::FILL_BOTH));
		
		xButton* butt = new xButton(panel1,_T("|"));
		butt->addActionListener(this);
		butt->addFocusListener(this);
		xButton* butt2 = new xButton(panel1,_T("13"),103,0);
		xButton* butt3 = new xButton(panel1,_T("aaaaaa"),103,0);
		//butt2->setLabel(_T("Hello"));
		frame->setSize(500,500);
		frame->doLayout();
		
		while(xtkProcessNextUIMessage()){}
	}

public:
	xWidgetsTest() : xTest("Widgets Test"){}
	
	virtual void actionPerformed(xActionEvent& e)
	{
		xSystem::getStdout().write(_T("Button 1 activated\n"));
	}
	
	virtual void mousePressed(xMouseEvent& ev)
	{
		xSystem::getStdout().write(_T("Mouse button pressed\n"));
	}
	
	virtual void mouseReleased(xMouseEvent& ev)
	{
		xSystem::getStdout().write(_T("Mouse button released\n"));
	}
	
	virtual void mouseDoubleClicked(xMouseEvent& ev)
	{
		xSystem::getStdout().write(_T("Mouse button double clicked\n"));
	}
	
	virtual void focusGained(xFocusEvent& e)
	{
		xSystem::getStdout().write(_T("Button focus gained\n"));
	}
};
