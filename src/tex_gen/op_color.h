/** \file op_color.h
 * Color operator
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

#ifndef NL_OP_COLOR
#define NL_OP_COLOR

#include "tex_gen/tex_gen_op.h"


namespace NLTEXGEN
{

/**
 * Simple color modificator operator.
 * Derived operator must use only one input and must return a bitmap of same size than the input.
 *
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class IOpColor : public ITexGenOperator
{
public:

	enum TInput
	{
		Source = 0,			// The gradient color to use
		Mask,
		LastInput,
	};

	enum TParameter
	{
		BlendMode = 0,		// The blend mode
		Alpha,				// The blend alpha
		Normalize,			// Normalize flag
		LastParameter,
	};

	// Ctor. Init inputs and parameters
	IOpColor();

	// \from ITexGenOperator
	virtual TChannel evalInternal (class CFloatBitmap &output, const CRenderParameter &renderParameters);

	/** 
	  * Interface to implement by the color operator.
	  * This operator must change the size of output with the size of input.
	  * output and input can be the same object sometimes.
	  *
	  * This method must returns the modifed channels.
	  */
	virtual TChannel modifyColor (CFloatBitmap &output, const CFloatBitmap &input, const CRenderParameter &renderParameters) = 0;

	// Output a mask in alpha channel ?
	virtual bool outputMask () const
	{
		return false;
	};
};

// ***************************************************************************

} // NLTEXGEN


#endif // NL_OP_COLOR

/* End of op_color.h */
