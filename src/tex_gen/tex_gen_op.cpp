/** \file tex_gen_op.cpp
 * Texture operator interface
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

#include "stdtex_gen.h"
#include "tex_gen_op.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

CFloatBitmap ITexGenOperator::DefaultBitmap;

// ***************************************************************************

void ITexGenOperator::operator=(const ITexGenOperator &other)
{
	clear ();

	// Copy
	// _Inputs = other._Inputs;
	_Parameters = other._Parameters;

	// Clone sub operators
	/*uint i;
	_BindedOperators.resize (other._BindedOperators.size(), NULL);
	for (i=0 ;i<_BindedOperators.size(); i++)
	{
		if(other._BindedOperators[i])
			_BindedOperators[i] = other._BindedOperators[i]->copy();
	}*/

	invalidate ();
}

// ***************************************************************************

ITexGenOperator::ITexGenOperator()
{
	_Enabled = true;
	invalidate ();
}

// ***************************************************************************

ITexGenOperator::~ITexGenOperator()
{
	clear ();
}

// ***************************************************************************

TChannel ITexGenOperator::eval(CFloatBitmap &dest, const CRenderParameter &renderParameters)
{
	// Operator invalidated ?
	if (_Invalidated)
	{
		// Operator enabled ?
		TChannel modifedChannels = ChannelNone;

		if (_Enabled)
		{
			// Eval the bitmap
			modifedChannels = evalInternal (dest, renderParameters);
		}
		else
		{
			// Eval the first child
			modifedChannels = evalSubOp (dest, 0, renderParameters);
		}

		// Cache ? 
		// Cache if cache is forced or if we have more than 1 parent : the evaluation can be called several times
		if (renderParameters.Cache || (_Parents.size()>1))
		{
			// Copy in the bitmap cache
			_Cache = dest;

			// Validate it
			_Invalidated = false;
		}

		// 
		return modifedChannels;
	}
	else
	{
		// Operator is valid, return the cache
		dest.ghostCopy (&_Cache);

		// Nothing modified
		return ChannelAll;
	}
}

// ***************************************************************************

void ITexGenOperator::clear()
{
	_BindedOperators.clear ();
}

// ***************************************************************************

void ITexGenOperator::bindOp(uint i, ITexGenOperator *op)
{
	if (i>=_BindedOperators.size ())
		_BindedOperators.resize (i+1, NULL);

	// Must be removed first
	nlassert(_BindedOperators[i] == NULL);

	_BindedOperators[i]=op;
	op->_Parents.push_back (this);
	invalidate ();
}

// ***************************************************************************

void ITexGenOperator::removeOp(uint i)
{
	uint j;
	for (j=0; j<_BindedOperators[i]->_Parents.size(); j++)
	{
		if (_BindedOperators[i]->_Parents[j] == this)
		{
			_BindedOperators[i]->_Parents.erase (_BindedOperators[i]->_Parents.begin()+j);
			break;
		}
	}
	_BindedOperators[i] = NULL;
	while (_BindedOperators.size() && (_BindedOperators[_BindedOperators.size()-1]==NULL))
		_BindedOperators.resize (_BindedOperators.size()-1);
	invalidate ();
}

// ***************************************************************************

TChannel ITexGenOperator::evalSubOp (CFloatBitmap &output, uint op, const CRenderParameter &cache)
{
	ITexGenOperator *op0 = getBindedOp(op);
	if (op0)
		return op0->eval(output, cache);
	else
		output.ghostCopy(&ITexGenOperator::DefaultBitmap);
	return ChannelAll;
}

// ***************************************************************************

void ITexGenOperator::setParameter(uint i, float value)
{
	if (i<_Parameters.size())
	{
		// Alpha value ?
		_Parameters[i].Value = value;
	}

	// Modify operator ?
	invalidate ();
}

// ***************************************************************************

void ITexGenOperator::setParameter(uint i, int value)
{
	if (i<_Parameters.size())
	{
		// Alpha value ?
		clamp (value, _Parameters[i].IntegerMin, _Parameters[i].IntegerMax);
		_Parameters[i].Integer = value;
	}

	// Modify operator ?
	invalidate ();
}

// ***************************************************************************

void ITexGenOperator::setParameter(uint i, const NLMISC::CRGBAF &value)
{
	if (i<_Parameters.size())
	{
		// Alpha value ?
		_Parameters[i].Color[0] = value.R;
		_Parameters[i].Color[1] = value.G;
		_Parameters[i].Color[2] = value.B;
		_Parameters[i].Color[3] = value.A;
	}

	// Modify operator ?
	invalidate ();
}

// ***************************************************************************

void ITexGenOperator::setParameter(uint i, const std::vector<NLTEXGEN::CGradientColor> &value)
{
	if (i<_Parameters.size())
	{
		// Alpha value ?
		_Parameters[i].Gradient = value;
	}

	// Modify operator ?
	invalidate ();
}

// ***************************************************************************

void ITexGenOperator::setParameter(uint i, const std::string &value)
{
	if (i<_Parameters.size())
	{
		_Parameters[i].StringValue = value;
	}

	// Modify operator ?
	invalidate ();
}

// ***************************************************************************

bool ITexGenOperator::isEnable() const
{
	return _Enabled;
}

// ***************************************************************************

void ITexGenOperator::setEnable(bool enable)
{
	_Enabled = enable;
	invalidate ();
}

// ***************************************************************************

void ITexGenOperator::serial(NLMISC::IStream &s)
{
	s.serial (_Enabled);

	// Serial parameters
	uint i;
	for (i=0; i<_Parameters.size(); i++)
	{
		sint toto = FileVersion;
		if ((FileVersion >= _Parameters[i].FirstVersion) && (FileVersion <= _Parameters[i].LastVersion))
			s.serial (_Parameters[i]);
	}

	// Serial the child operator
	s.serialContPolyPtr (_BindedOperators);

	// If reading, relink operators
	if (s.isReading())
	{
		uint i;
		for (i=0; i<_BindedOperators.size (); i++)
		{
			if (_BindedOperators[i])
			{
				// Backup
				ITexGenOperator *temp = _BindedOperators[i];
				_BindedOperators[i] = NULL;
				bindOp(i, temp);
			}
		}
	}
}

// ***************************************************************************

bool ITexGenOperator::canBindOp(const ITexGenOperator *op) const
{
	return !op->findChildren (this);
}

// ***************************************************************************

bool ITexGenOperator::findChildren (const ITexGenOperator *op) const
{
	if (this == op)
		return true;
	uint i;
	for (i=0; i<_BindedOperators.size(); i++)
	{
		if (_BindedOperators[i] && _BindedOperators[i]->findChildren (op))
			return true;
	}
	return false;
}

// ***************************************************************************

void ITexGenOperator::invalidate ()
{
	_Invalidated = true;
	uint i;
	for (i=0; i<_Parents.size(); i++)
	{
		_Parents[i]->invalidate();
	}
}

// ***************************************************************************

/* End of tex_gen_op.cpp */
