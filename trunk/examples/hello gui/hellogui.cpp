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
* @file hellogui.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
* 
* Basic "hello world" application using xtklib "widgets" module.
*/

#include "xtk/xtk.h"
#include "xtk/widgets.h"
using namespace xtk;


class HelloGuiApp : public xApplication
{
private:
	xFrame* m_frame;
	
public:
	virtual void onInit()
	{
		//create the topmost window
		m_frame = new xFrame(NULL,_T("Hello world application"));
		//change its layout
		m_frame->setLayout(new xBoxLayout(xBoxLayout::BOX_ORIENTATION_Y_AXIS,true,3));
		//set the default close operation
		m_frame->setDefaultCloseAction(xWindow::XTK_EXIT_ON_CLOSE);
		
		//create a label
		xLabel* lab1 = new xLabel(m_frame,_T("Hello xtklib!!!"));
		
		//create a button
		xButton* butt1 = new xButton(m_frame,_T("Exit"));
		//attach an event handler to button
		butt1->addActionPerformedHandler(new xWidgetEventHandler<HelloGuiApp>(this,&HelloGuiApp::actionPerformed));

		m_frame->setSize(250,90);
		m_frame->setVisible(true);
	}

	void actionPerformed(xtk::xWidgetEvent& e)
	{
		m_frame->destroy();
	}
};

SET_APPLICATION(HelloGuiApp);
