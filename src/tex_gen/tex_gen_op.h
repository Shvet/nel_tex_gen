/** \file tex_gen_op.h
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

#ifndef NL_TEX_GEN_OP
#define NL_TEX_GEN_OP

#include "basics.h"
#include "bitmap_float.h"

namespace NLTEXGEN
{

/**
 * Texture operator interface.
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class ITexGenOperator : public NLMISC::IStreamable
{
public:

	/**
	  * Eval the operator in the destination bitmap. Returns the channels that have been modified.
	  * If renderParameters.Cash is true, the result will be saved by the operator.
	  */
	TChannel	eval(class CFloatBitmap &dest, const CRenderParameter &renderParameters);

	/// \name Operator modification methods

	/**
	  * This method returns the number of bitmap needed by the operator to work
	  */
	uint getNumInputBitmap () const
	{
		return _Inputs.size();
	}

	/**
	  * This method returns an input descriptor
	  */
	const CInput &getInputBitmapDescriptor (uint i) const
	{
		return _Inputs[i];
	}

	/// \name Parameters methods

	/**
	  * This method returns the number of parameters used by this operator
	  */
	uint	getNumParameter() const
	{
		return _Parameters.size();
	}

	/**
	  * This method returns the number of parameters used by this operator
	  */
	const CParameter &getParameter(uint i) const
	{
		return _Parameters[i];
	}

	/**
	  * This method modify a parameter used by this operator
	  */
	void setParameter(uint i, float value);
	void setParameter(uint i, int value);
	void setParameter(uint i, const NLMISC::CRGBAF &value);
	void setParameter(uint i, const std::vector<NLTEXGEN::CGradientColor> &value);
	void setParameter(uint i, const std::string &value);

	/// \name Binding input operators

	// Get the binded operators
	const std::vector<ITexGenOperator*>	&getBindedOp() const
	{
		return _BindedOperators;
	}

	// Get the binded operators
	const ITexGenOperator	*getBindedOp(uint i) const
	{
		if (i<_BindedOperators.size())
			return _BindedOperators[i];
		else
			return NULL;
	}

	// Get the parent operators
	const std::vector<ITexGenOperator*> &getParents () const
	{
		return _Parents;
	}

	// Get the parent operators
	std::vector<ITexGenOperator*> &getParents ()
	{
		return _Parents;
	}

	// Can bind an operator ?
	bool canBindOp(const ITexGenOperator *op) const;

	// Bind an operator
	void bindOp(uint i, ITexGenOperator *op);

	// Remove an operator
	void removeOp(uint i);

	// Where is bind the operator
	uint whereIsBindOp(ITexGenOperator *op) const
	{
		uint i;
		for (i=0; i<_BindedOperators.size(); i++)
		{
			if (_BindedOperators[i] == op)
				return i;
		}
		// Not found !
		nlstop;
		return 0;
	}

	// Eval an operator
	TChannel evalSubOp (CFloatBitmap &output, uint op, const CRenderParameter &cache);

	/// \name Gloabal properties

	// The operator is enable or not. If not enable, it returns it's first entry as output.
	bool	isEnable() const;

	// The operator is enable or not. If not enable, it returns it's first entry as output.
	void	setEnable(bool enable);

	// The operator name (not serialized)
	// std::string	Name;

	// Selection flag (not serialized)
	bool Selected;

	// The operator position (not serialized)
	sint32	X, Y;

	/// \name Class management

	/// Constructor
	ITexGenOperator();

	/// Destructor
	virtual ~ITexGenOperator();

	/// Copy
	void operator=(const ITexGenOperator &other);

	/// Clone
	virtual ITexGenOperator *copy() const = 0;

	// Default bitmap
	static CFloatBitmap			DefaultBitmap;

protected:

	/// Clear the content
	void clear();

	/**
	  * Internal eval method.
	  *
	  * This method must modify the input bitmaps and put the result in the output bitmap
	  */
	virtual TChannel evalInternal (CFloatBitmap &output, const CRenderParameter &renderParameters) = 0;

	// Serial this operator
	virtual void serial(NLMISC::IStream &s);

	// Inputs
	std::vector<CInput>			_Inputs;

	// Parameters
	std::vector<CParameter>		_Parameters;

private:

	// Invaliate operator
	void invalidate ();

	// Find a child operator
	bool findChildren (const ITexGenOperator *op) const;

	// Get the binded operators
	ITexGenOperator	*getBindedOp(uint i)
	{
		if (i<_BindedOperators.size())
			return _BindedOperators[i];
		else
			return NULL;
	}

	// The cash bitmap
	CFloatBitmap				_Cache;

	// Invalidate flag
	bool						_Invalidated;

	// Enabled
	bool						_Enabled;

	// Parent operator
	std::vector<ITexGenOperator*>	_Parents;

	// Inputs
	std::vector<ITexGenOperator*>	_BindedOperators;
};

#define	NLMISC_DECLARE_CLASS2(_class_, name)					\
	virtual std::string	getClassName() {return #name;}		\
	static	NLMISC::IClassable	*creator() {return new _class_;}

// ***************************************************************************

} // NLTEXGEN

#endif // NL_TEX_GEN_OP

/* End of tex_gen_op.h */
