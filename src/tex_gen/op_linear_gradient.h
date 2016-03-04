/** \file op_linear_gradient.h
 * Linear gradient operator
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

#ifndef NL_OP_LINEAR_GRADIENT
#define NL_OP_LINEAR_GRADIENT

#include "tex_gen/tex_gen_op.h"
#include "tex_gen/op_layer.h"


namespace NLTEXGEN
{

/**
 * Linear gradient operator.
 *
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class COpLinearGradient : public IOpLayer
{
public:

	enum TInput
	{
		OffsetInput = IOpLayer::LastInput,
		LastInput,
	};

	enum TParameter
	{
		Mirror = IOpLayer::LastParameter,
		Tile,
		Invert,
		Scale,
		Rotation,
		Offset,
		Gradient,
		LastParameter,
	};

	// Ctor. Init inputs and parameters
	COpLinearGradient();

	// \from IOpLayer
	virtual TChannel modifyLayer (CFloatBitmap &output, const CRenderParameter &renderParameters);

	// Streamable
	NLMISC_DECLARE_CLASS2(COpLinearGradient, LG);

	// Clone operator
	NL_DECLARE_COPY_OPERATOR(COpLinearGradient, ITexGenOperator);
};

// ***************************************************************************

} // NLTEXGEN


#endif // NL_OP_LINEAR_GRADIENT

/* End of op_linear_gradient.h */
