/** \file op_hue_saturation.cpp
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

#include "stdtex_gen.h"
#include "op_hue_saturation.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpHueSaturation::COpHueSaturation()
{
	// Parameters
	_Parameters.push_back (CParameter("hue", "correct", AngleSetup));
	_Parameters[Hue].Value = 0;
	_Parameters.push_back (CParameter("saturation", "correct", CenteredMT1_1Setup));
	_Parameters[Saturation].Value = 0;
	_Parameters.push_back (CParameter("luminosity", "correct", CenteredMT1_1Setup));
	_Parameters[Luminosity].Value = 0;
}

// ***************************************************************************

TChannel COpHueSaturation::modifyColor (CFloatBitmap &output, const CFloatBitmap &input, const CRenderParameter &renderParameters)
{
	// Blend the input and output
	output.resize (input.getWidth(), input.getHeight());
	float *out = output.getPixels();
	const float *in = input.getPixels();

	const float hue = _Parameters[Hue].Value * 6.f / 360.f;
	const float saturation = _Parameters[Saturation].Value;
	const float value = _Parameters[Luminosity].Value;

	uint i;
	const uint count = output.size();
	for (i=0; i<count; i++, out+=4, in+=4)
	{
		float h;
		float v;
		float s;
		RGB2HVS (in, h, v, s);

		// Add hue
		h += hue;
		if (h > 6.f)
			h -= 6.f;
		if (h < 0.f)
			h += 6.f;

		// Add saturation
		s += saturation;
		clamp (s, 0.f ,1.f);

		// Add value
		v += value;

		HVS2RGB (h, v, s, out);
	}

	return ChannelRGB;
}

// ***************************************************************************

/* End of op_hue_saturation.cpp */
