/** \file op_blur.h
 * Gaussian blur
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

#ifndef NL_OP_BLUR
#define NL_OP_BLUR

#include "tex_gen_op.h"
#include "op_color.h"


namespace NLTEXGEN
{

/**
 * Noise operator.
 * Fill the bitmap with a noisy color
 *
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class COpBlur : public IOpColor
{
public:

	enum TInput
	{
		LastInput = IOpColor::LastInput,			// The gradient color to use
	};

	enum TParameter
	{
		Radius = IOpColor::LastParameter,
		Quality,
		Intensity,
		Up,
		Down,
		Left,
		Right,
		LastParameter,
	};

	// Ctor. Init inputs and parameters
	COpBlur();

	// \from IOpColor
	virtual TChannel modifyColor (CFloatBitmap &output, const CFloatBitmap &input, const CRenderParameter &renderParameters);

	// Streamable
	NLMISC_DECLARE_CLASS2(COpBlur, Bl);

	// Clone operator
	NL_DECLARE_COPY_OPERATOR(COpBlur, ITexGenOperator);

protected:

};

// ***************************************************************************

} // NLTEXGEN


#endif // NL_OP_BLUR

/* End of op_blur.h */
