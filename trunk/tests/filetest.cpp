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
* @file filetest.cpp
* @author Mario Casciaro xshadow[AT]email(DOT)it
*/


#include "xtk/xtk.h"
using namespace xtk;

#include <stdio.h>



class xFileTest : public xTest
{
private:
	virtual void doTest()
	{
		smartPtr<xFile> file = new xFile("testfiletest.extn");
		
		file->createNewFile();
		ensureTrue(file->exists(),"Create file");
		
		file->renameTo(xFile("testfiletestnewname.extn"));
		ensureTrue(file->getPath().equals("testfiletestnewname.extn"),"Rename");
		file->renameTo(xFile("testfiletest.extn"));
		
		file->remove();
		ensureTrue(!file->exists(),"Delete");
		file->createNewFile();
		
		ensureTrue(file->isFile(),"isFile");
		
		ensureTrue(!file->isAbsolute(),"isAbsolute");
		
		smartPtr<xFile> abs = file->getAbsoluteFile();
		ensureTrue(
			abs->isAbsolute() && 
			abs->getPath().equals("D:\\Development\\xtklib\\tests\\filetest\\testfiletest.extn")
			,"getAbsolute");
			
		
		smartPtr<xString> ext = file->getExtension();
		ensureTrue(ext->equals("extn"),"getExtension");
		
		file = new xFile("D:\\\\Development\\xtklib\\tests\\\\filetest\\testfiletest.extn");
		ensureTrue(file->getPath().equals("D:\\Development\\xtklib\\tests\\filetest\\testfiletest.extn"),"Normalize");
		
		xString parent = smartPtr<xFile>((file->getParent()))->getPath();
		ensureTrue(parent.equals("D:\\Development\\xtklib\\tests\\filetest\\"),"getParent");
		
		smartPtr<xString> name = file->getName();
		ensureTrue(name->equals("testfiletest.extn"),"getName");
		
		file = new xFile("testdir");
		file->mkdir();
		ensureTrue(file->exists(),"Create Dir");
		
		ensureTrue(file->isDirectory() && !file->isFile(),"isDir");
		
		testManualStart("List Dir");
		smartPtr<xFile> dir1 = new xFile(*file,"testdir1");
		dir1->mkdir();
		smartPtr<xFile> dir2 = new xFile(*file,"testdir2");
		dir2->mkdir();
		smartPtr<xArray<xString*> > dirs = file->list();
		for(int i = 0;i < dirs->size();i++)
			testLog("%s",(*dirs)[i]->c_str());
		dirs->deleteData();
		testOK();
		
	}
public:
	xFileTest() : xTest("File Test"){}
};


	
	/*
		
		smartPtr<xFile> dir1 = new xFile(*file,"testdir1");
		dir1->mkdir();
		smartPtr<xFile> dir2 = new xFile(*file,"testdir2");
		dir2->mkdir();
		printf("Test List dir...:");
		smartPtr<xArray<xString*> > dirs = file->list();
		for(int i = 0;i < dirs->size();i++)
			printf("%s, ",(*dirs)[i]->c_str());
		dirs->deleteData();
		printf("\n");
		
		printf("Test delete dir...");
		dir1->remove();
		dir2->remove();
		file->remove();
		if(!file->exists())
			printf("OK\n");
		else
			printf("FAILED!\n");
		file->mkdir();
			
		printf("Test list roots...:");
		smartPtr<xArray<xFile*> > roots = xFile::listRoots();
		for(int i = 0;i < roots->size();i++)
			printf("%s, ",(*roots)[i]->getPath().c_str());
		roots->deleteData();
		printf("OK\n");
		
	}
	
	{
		smartPtr<xFile> file = new xFile("testfiletest1.extn");
		if(file->exists())
			file->remove();
		file->createNewFile();
		
		
		printf("Test write file...");
		smartPtr<xFileOutputStream> fos = file->getOutputStream();
		smartPtr<xOutputStreamWriter> osw = new xOutputStreamWriter(&fos);
		osw->writeLine("Test!!!");
		osw->writeLine("TEEEEEEEEEEEEEEEEEEEEEEEEEEEESTTTT");
		osw->close();
		fos->close();
		printf("OK\n");
		
		printf("Test read file...:\n");
		smartPtr<xFileInputStream> fis = file->getInputStream();
		smartPtr<xInputStreamReader> isr = new xInputStreamReader(&fis);
		smartPtr<xString> line;
		while((line = isr->readLine()) != NULL)
			printf("%s\n",line->c_str());
		isr->close();
		fis->close();
		printf("OK\n");
		
		file->remove();
		file->createNewFile();
		printf("Test read write file...:\n");
		smartPtr<xFileIOStream> fios = file->getIOStream();
		smartPtr<xOutputStreamWriter> ioosr = new xOutputStreamWriter(&fios);
		smartPtr<xInputStreamReader> ioisr = new xInputStreamReader(&fios);
		
		ioosr->writeLine("TestSeLeggiSoloQuestoèOK!!!");
		ioosr->flush();
		
		fios->setPosition(4);
		
		line = ioisr->readToken("!!!");
		
		printf("%s",line->c_str());
		
		printf("\nOK\n");
	}
	}
	catch(xException& ex)
	{
		printf("Exception: Type %s,Message: %s",ex.getType()->c_str(),ex.getDescription().c_str());
	}
	*/



