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
* @file typewrappers.cpp
* @author Mario Casciaro (xshadow@email.it)
*/


#include "../../include/xtk/base/typewrappers.h"
#include "../../include/xtk/base/string.h"
#include "../../include/xtk/base/exception.h"

namespace xtk
{

float xFloat::MAX = FLT_MAX;
float xFloat::MIN = FLT_MIN;
	
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xInteger::xInteger(int value)
{
	m_value = value;
}
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xInteger::hashCode()
{
	return m_value;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################	
int xInteger::compareTo(xObject& o)
{
	xInteger* i = dynamic_cast<xInteger*>(&o);
	if(i == NULL)
		throw xClassCastException(_T("Can compare only against another xInteger"));
	
	if(this->m_value < i->m_value)
		return -1;
	if(this->m_value > i->m_value)
		return 1;
	
	return 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xInteger::equals(xObject& o)
{
	xInteger* i = dynamic_cast<xInteger*>(&o);
	if(i == NULL)
		return false;
		
	return i->m_value == this->m_value;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xInteger::toString()
{
	return xString::getFormat(_T("%d"),m_value);
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
int xCharacter::compareTo(xCharacter& o)
{
	xCharacter* i = dynamic_cast<xCharacter*>(&o);
	if(i = NULL)
		throw xClassCastException(_T("Can compare only against another xCharacter"));
	
	if(this->m_value < i->m_value)
		return -1;
	if(this->m_value > i->m_value)
		return 1;
	
	return 0;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
bool xCharacter::equals(xObject& o)
{
	xCharacter* i = dynamic_cast<xCharacter*>(&o);
	if(i == NULL)
		return false;
	
	return i->m_value == this->m_value;
}

//##############################################################################
//# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
//##############################################################################
xString xCharacter::toString()
{
	return xString(&m_value,1);
}

	
}//end namespace
