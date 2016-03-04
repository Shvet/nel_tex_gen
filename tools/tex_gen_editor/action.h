/** \file action.h
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

#ifndef NL_ACTION
#define NL_ACTION

// ***************************************************************************

#include "tex_gen/basics.h"

namespace NLTEXGEN
{
	class ITexGenOperator;
};

/**
 * Action interface
 *
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class IAction
{
public:
	
	virtual ~IAction();
	
	// Return true if something has been modified
	virtual void undo(class CNelTexGenDoc *doc) = 0;
	virtual bool redo(CNelTexGenDoc *doc) = 0;

	// Return true if the modification are accumulative (move), false if they are absolute (set a value)
	virtual bool accum(const IAction &action) { return false; }
};

// Action to change the enable flag of an operator
class CActionEnable : public IAction
{
public:
	CActionEnable (uint opId, bool enable);

protected:
	
	uint _OperatorId;
	bool _Enable;

	virtual void undo(CNelTexGenDoc *doc);
	virtual bool redo(CNelTexGenDoc *doc);
};

// Action to add an operator
class CActionAddOperator : public IAction
{
public:
	CActionAddOperator (const class COperatorDesc	*operatorDesc, sint x, sint y);

protected:
	
	const COperatorDesc	*_OperatorDesc;
	uint	_OperatorId;
	sint	_X;
	sint	_Y;

	virtual void undo(CNelTexGenDoc *doc);
	virtual bool redo(CNelTexGenDoc *doc);
};

// Action to delete an operator
class CActionDeleteOperator : public IAction
{
public:
	CActionDeleteOperator (uint operatorId, CNelTexGenDoc *doc);

protected:
	
	std::string	_ClassName;
	uint	_OperatorId;
	sint	_X;
	sint	_Y;
	NLTEXGEN::ITexGenOperator	*_Clone;

	virtual void undo(CNelTexGenDoc *doc);
	virtual bool redo(CNelTexGenDoc *doc);
};

// Action to link two operator
class CActionLink : public IAction
{
public:
	CActionLink (uint inputId, uint input, uint outputId);

protected:
	
	uint	_InputId;
	uint	_Input;
	uint	_OutputId;

	virtual void undo(CNelTexGenDoc *doc);
	virtual bool redo(CNelTexGenDoc *doc);
};

// Action to unlink two operators
class CActionUnlink : public IAction
{
public:
	CActionUnlink (uint inputId, uint input, uint outputId);

protected:
	
	uint	_InputId;
	uint	_Input;
	uint	_OutputId;

	virtual void undo(CNelTexGenDoc *doc);
	virtual bool redo(CNelTexGenDoc *doc);
};

// Action to set an operators parameter
template <class _A>
class CActionSetParameter : public IAction
{
public:
	CActionSetParameter (uint opId, uint paramId, const _A &oldValue, const _A &newValue)
	{
		_OperatorId = opId;
		_ParamId = paramId;
		_OldValue = oldValue;
		_NewValue = newValue;
	}

protected:
	
	uint	_OperatorId;
	uint	_ParamId;
	_A		_OldValue;
	_A		_NewValue;

	virtual void undo(CNelTexGenDoc *doc)
	{
		NLTEXGEN::ITexGenOperator *op = doc->getOperator(_OperatorId);
		op->setParameter(_ParamId, _OldValue);
		doc->modifyBitmap ();
		doc->modifyOperator(doc->getOperator(_OperatorId), true);
	}
	virtual bool redo(CNelTexGenDoc *doc)
	{
		if (_OldValue != _NewValue)
		{
			NLTEXGEN::ITexGenOperator *op = doc->getOperator(_OperatorId);
			op->setParameter(_ParamId, _NewValue);
			doc->modifyBitmap ();
			doc->modifyOperator(doc->getOperator(_OperatorId), true);
			return true;
		}
		else
			return false;
	}

	virtual bool accum(const IAction &action)
	{
		const CActionSetParameter *other = dynamic_cast<const CActionSetParameter*>(&action);
		if (other)
		{
			if ((_OperatorId == other->_OperatorId) &&
				(_ParamId == other->_ParamId))
			{
				_OldValue = other->_OldValue;
				return true;
			}
		}
		return false;
	}
};

typedef CActionSetParameter<int> CActionSetParameterInt;
typedef CActionSetParameter<std::string> CActionSetParameterString;
typedef CActionSetParameter<float> CActionSetParameterFloat;
typedef CActionSetParameter<NLMISC::CRGBAF> CActionSetParameterCRGBAF;
typedef CActionSetParameter<std::vector<NLTEXGEN::CGradientColor> > CActionSetParameterGradient;

// ***************************************************************************

#endif // NL_ACTION

/* End of action.h */
