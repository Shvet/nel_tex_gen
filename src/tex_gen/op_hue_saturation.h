/** \file op_hue_saturation.h
 * Correct hue saturation
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

#ifndef NL_OP_HUE_SATURATION
#define NL_OP_HUE_SATURATION

#include "tex_gen_op.h"
#include "op_color.h"


namespace NLTEXGEN
{

/**
 * Contrast luminosity operator.
 *
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class COpHueSaturation : public IOpColor
{
public:

	enum TInput
	{
		LastInput,
	};

	enum TParameter
	{
		Hue = IOpColor::LastParameter,			// The gradient color to use
		Saturation,
		Luminosity,
		LastParameter,
	};

	// Ctor. Init inputs and parameters
	COpHueSaturation();

	// \from IOpColor
	virtual TChannel modifyColor (CFloatBitmap &output, const CFloatBitmap &input, const CRenderParameter &renderParameters);

	// Streamable
	NLMISC_DECLARE_CLASS2(COpHueSaturation, HS);

	// Clone operator
	NL_DECLARE_COPY_OPERATOR(COpHueSaturation, ITexGenOperator);

protected:

};

// ***************************************************************************

} // NLTEXGEN


#endif // NL_OP_HUE_SATURATION

/* End of op_hue_saturation.h */
