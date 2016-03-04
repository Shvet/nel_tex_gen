/** \file op_light.h
 * Light a normal map
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

#ifndef NL_OP_LIGHT
#define NL_OP_LIGHT

#include "tex_gen_op.h"


namespace NLTEXGEN
{

/**
 * Light a normal map
 *
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class COpLight : public ITexGenOperator
{
public:

	enum TInput
	{
		NormalMap = 0,
		LastInput,
	};

	enum TParameter
	{
		ZAxis = 0,		// The Rotation around Z axis
		XAxis,			// The Rotation around X axis
		Gradient,		// The gradient used for lighting
		LastParameter,
	};

	// Ctor. Init inputs and parameters
	COpLight();

	// \from ITexGenOperator
	virtual TChannel evalInternal (class CFloatBitmap &output, const CRenderParameter &renderParameters);

	// Streamable
	NLMISC_DECLARE_CLASS2(COpLight, Li);

	// Clone operator
	NL_DECLARE_COPY_OPERATOR(COpLight, ITexGenOperator);

protected:

};

// ***************************************************************************

} // NLTEXGEN


#endif // NL_OP_LIGHT

/* End of op_light.h */
