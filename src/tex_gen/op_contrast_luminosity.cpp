/** \file op_constrast_luminosity.cpp
 * Correct contrast luminosity
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
#include "op_contrast_luminosity.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpContrastLuminosity::COpContrastLuminosity()
{
	// Parameters
	_Parameters.push_back (CParameter("contrast", "correct", CenteredMT1_1Setup));
	_Parameters[Contrast].Value = 0;
	_Parameters.push_back (CParameter("luminosity", "correct", CenteredMT1_1Setup));
	_Parameters[Luminosity].Value = 0;
}

// ***************************************************************************

TChannel COpContrastLuminosity::modifyColor (CFloatBitmap &output, const CFloatBitmap &input, const CRenderParameter &renderParameters)
{
	// Blend the input and output
	output.resize (input.getWidth(), input.getHeight());
	float *out = output.getPixels();
	const float *in = input.getPixels();

	const float mul = (float)pow(20.f, _Parameters[Contrast].Value);
	const float add = 0.5f + _Parameters[Luminosity].Value;

	uint i;
	const uint count = output.size();
	for (i=0; i<count; i++, out+=4, in+=4)
	{
		out[0] = (in[0] - 0.5f) * mul + add;
		out[1] = (in[1] - 0.5f) * mul + add;
		out[2] = (in[2] - 0.5f) * mul + add;
	}

	return ChannelRGB;
}

// ***************************************************************************

/* End of op_constrast_luminosity.cpp */
