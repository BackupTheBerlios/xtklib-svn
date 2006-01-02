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
* @file hellogui.cpp
* @author Mario Casciaro (xshadow@email.it)
* 
* Basic "hello world" application using xtklib "widgets" module.
*/

#include "xtk/xtk.h"
#include "xtk/widgets.h"
using namespace xtk;


class HelloGui
{
private:
	xFrame* m_frame;
	
public:
	void initgui()
	{
		//create the topmost window
		m_frame = new xFrame(NULL,_T("Hello world application"));
		//change its layout
		m_frame->setLayout(new xBoxLayout(xBoxLayout::BOX_ORIENTATION_Y_AXIS,true,3));
		//set the default close operation
		m_frame->setDefaultCloseAction(xWindow::XTK_EXIT_ON_CLOSE);
		
		//create the label
		xLabel* lab1 = new xLabel(m_frame,_T("Hello xtklib!!!"));
		//frame->getLayout().setConstraints(*lab1,new xBoxConstraint(true,true));
		
		xButton* butt1 = new xButton(m_frame,_T("Exit"));
		//m_frame->getLayout().setConstraints(*butt1,new xBoxConstraint(true,true));
		butt1->addActionPerformedHandler(new xWidgetEventHandler<HelloGui>(this,&HelloGui::actionPerformed));

		m_frame->setSize(250,90);
	}

	void startgui()
	{
		m_frame->setVisible(true);
		xtkUIEventLoop();
	}
	
	void actionPerformed(xtk::xWidgetEvent& e)
	{
		m_frame->destroy();
	}
};


int xApplication::entryPoint()
{
	HelloGui myapp;
	myapp.initgui();
	myapp.startgui();
	return 0;
}


