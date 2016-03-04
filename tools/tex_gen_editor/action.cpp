/** \file action.cpp
 * Action definition
 *
 * $Id: $
 */

/* Copyright, 2003 DIGITAL MURDER.
 *
 * This file is part of DIGITAL MURDER NEL.
 * DIGITAL MURDER NEL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * DIGITAL MURDER NEL is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with DIGITAL MURDER NEL; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#include "stdafx.h"
#include "action.h"
#include "nel_tex_gen.h"
#include "nel_tex_gen_doc.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

IAction::~IAction()
{
}
	
// ***************************************************************************

CActionEnable::CActionEnable (uint opId, bool enable)
{
	_Enable = enable;
	_OperatorId = opId;
}

// ***************************************************************************

void CActionEnable::undo(CNelTexGenDoc *doc)
{
	redo(doc);
}

// ***************************************************************************

bool CActionEnable::redo(CNelTexGenDoc *doc)
{
	NLTEXGEN::ITexGenOperator *op = doc->getOperator(_OperatorId);
	bool enable = op->isEnable();
	if (enable != _Enable)
	{
		std::swap (enable, _Enable);
		op->setEnable(enable);
		doc->modifyOperator(op, true);
		return true;
	}
	return false;
}

// ***************************************************************************

CActionAddOperator::CActionAddOperator (const COperatorDesc	*operatorDesc, sint x, sint y)
{
	_OperatorDesc = operatorDesc;
	_OperatorId = (uint)-1;
	_X = x;
	_Y = y;
}

// ***************************************************************************
	
void CActionAddOperator::undo(CNelTexGenDoc *doc)
{
	_X = doc->getOperator(_OperatorId)->X;
	_Y = doc->getOperator(_OperatorId)->Y;
	doc->remove (doc->getOperator(_OperatorId));
}

// ***************************************************************************

bool CActionAddOperator::redo(CNelTexGenDoc *doc)
{
	// Create a new class
	ITexGenOperator* op = (ITexGenOperator*)CClassRegistry::create(_OperatorDesc->ClassName);
	op->X = _X;
	op->Y = _Y;
	_OperatorId = doc->add (op, _OperatorId);
	return true;
}

// ***************************************************************************

CActionDeleteOperator::CActionDeleteOperator (uint operatorId, CNelTexGenDoc *doc)
{
	// Get the operator
	_OperatorId = operatorId;
	ITexGenOperator *op = doc->getOperator(_OperatorId);
	_ClassName = op->getClassName().c_str();
	_X = op->X;
	_Y = op->Y;
	_Clone = op->copy();
}

// ***************************************************************************
	
void CActionDeleteOperator::undo(CNelTexGenDoc *doc)
{
	// Create a new class
	ITexGenOperator* op = (ITexGenOperator*)CClassRegistry::create(_ClassName);
	op->X = _X;
	op->Y = _Y;
	_OperatorId = doc->add (op, _OperatorId);
	*(doc->getOperator(_OperatorId)) = *_Clone;
}

// ***************************************************************************

bool CActionDeleteOperator::redo(CNelTexGenDoc *doc)
{
	doc->remove (doc->getOperator(_OperatorId));
	return true;
}

// ***************************************************************************

CActionLink::CActionLink (uint inputId, uint input, uint outputId)
{
	_InputId = inputId;
	_OutputId = outputId;
	_Input = input;
}

// ***************************************************************************

void CActionLink::undo(CNelTexGenDoc *doc)
{
	doc->getOperator(_InputId)->removeOp(_Input);
	doc->modifyGraph ();
	doc->modifyOperator(doc->getOperator(_InputId), true);
	doc->modifyOperator(doc->getOperator(_OutputId), true);
}

// ***************************************************************************

bool CActionLink::redo(CNelTexGenDoc *doc)
{
	if (doc->getOperator(_InputId)->canBindOp(doc->getOperator(_OutputId)))
	{
		doc->getOperator(_InputId)->bindOp(_Input, doc->getOperator(_OutputId));
		doc->modifyGraph ();
		doc->modifyOperator(doc->getOperator(_InputId), true);
		doc->modifyOperator(doc->getOperator(_OutputId), true);
		return true;
	}
	return false;
}

// ***************************************************************************

CActionUnlink::CActionUnlink (uint inputId, uint input, uint outputId)
{
	_InputId = inputId;
	_OutputId = outputId;
	_Input = input;
}

// ***************************************************************************

void CActionUnlink::undo(CNelTexGenDoc *doc)
{
	nlassert(doc->getOperator(_InputId)->canBindOp(doc->getOperator(_OutputId)));
	doc->getOperator(_InputId)->bindOp(_Input, doc->getOperator(_OutputId));
	doc->modifyGraph ();
	doc->modifyOperator(doc->getOperator(_InputId), true);
	doc->modifyOperator(doc->getOperator(_OutputId), true);
}

// ***************************************************************************

bool CActionUnlink::redo(CNelTexGenDoc *doc)
{
	doc->getOperator(_InputId)->removeOp(_Input);
	doc->modifyGraph ();
	doc->modifyOperator(doc->getOperator(_InputId), true);
	doc->modifyOperator(doc->getOperator(_OutputId), true);
	return true;
}

// ***************************************************************************

/* End of action.cpp */
