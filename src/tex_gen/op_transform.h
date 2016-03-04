/** \file op_transform.h
 * Matrix transformation of a picture
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

#ifndef NL_OP_TRANSFORM
#define NL_OP_TRANSFORM

#include "tex_gen_op.h"
#include "op_deform.h"


namespace NLTEXGEN
{

/**
 * Matrix transformation of a picture
 *
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class COpTransform : public COpDeform
{
public:

	enum TInput
	{
		LastInput,
	};

	enum TParameter
	{
		ScaleRotate = COpDeform::LastParameter,	// Scale rotation
		Scale,				// 2d scale
		ScaleX,				// Scale on X axis
		ScaleY,				// Scale on Y axis
		ShearRotate,		// Shear rotation
		Shear,				// Shear
		Rotate,				// Post scale rotation
		TranslationX,		// X translation
		TranslationY,		// X translation
		LastParameter,
	};

	// Ctor. Init inputs and parameters
	COpTransform();

	// \from COpDeform
	virtual TChannel modifyColor (CFloatBitmap &output, const CFloatBitmap &input, const CRenderParameter &renderParameters);

	// Streamable
	NLMISC_DECLARE_CLASS2(COpTransform, Tr);

	// Clone operator
	NL_DECLARE_COPY_OPERATOR(COpTransform, ITexGenOperator);

protected:

};

// ***************************************************************************

} // NLTEXGEN


#endif // NL_OP_TRANSFORM

/* End of op_transform.h */
