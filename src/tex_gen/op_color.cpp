/** \file op_color.cpp
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

#include "stdtex_gen.h"
#include "op_color.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

IOpColor::IOpColor()
{
	// Two inputs
	_Inputs.push_back(CInput("source"));
	_Inputs.push_back(CInput("mask"));

	// Parameters
	_Parameters.push_back (CParameter("effect overlay", "effect", BlendModeName, NumBlendMode));
	_Parameters.push_back (CParameter("effect level", "effect", PercentSetup));
	_Parameters.push_back (CParameter("normalize (for normals)", "effect", false));
	_Parameters[Alpha].Value = 100.f;
}

// ***************************************************************************

TChannel IOpColor::evalInternal (CFloatBitmap &output, const CRenderParameter &renderParameters)
{
	TChannel channels;

	const float alpha = _Parameters[Alpha].Value / 100.f;

	// Alpha is 0 ?
	if (alpha <= 0.f)
	{
		// Eval the first entry
		channels = evalSubOp (output, 0, renderParameters);
	}
	else
	{
		// User mask ?
		bool userMask = outputMask ();

		// Alpha is 1 with Replace ?
		const ITexGenOperator *mask = const_cast<const IOpColor*>(this)->getBindedOp(Mask);
		if ( (alpha >= 1.f) && (_Parameters[BlendMode].Integer == Replace) &&
			(mask == NULL) && !userMask)
		{
			// Eval the first entry
			CFloatBitmap temp;
			channels = evalSubOp (temp, 0, renderParameters);
			channels = (TChannel)(channels | modifyColor (output, temp, renderParameters));
		}
		else
		{
			// Eval 1st argument
			channels = evalSubOp (output, 0, renderParameters);

			// Eval the mask
			CFloatBitmap maskBitmap;
			if (mask)
			{
				evalSubOp (maskBitmap, Mask, renderParameters);
				maskBitmap.resample (output.getWidth(), output.getHeight());
			}

			// Modify colors by the operator
			CFloatBitmap effect;
			channels = (TChannel)(channels | modifyColor (effect, output, renderParameters));
			nlassert (output.size() == effect.size());

			// Blend the input and output
			const float *in = const_cast<const CFloatBitmap*>(&effect)->getPixels();
			float *out = output.getPixels();

			// Blend the pixels
			if (userMask)
			{
				transformBlendSrc0Alpha (out, in, out, mask?const_cast<const CFloatBitmap*>(&maskBitmap)->getPixels():NULL, output.size()*4, (TBlendMode)_Parameters[BlendMode].Integer, 
					alpha);
			}
			else
			{
				transformBlend (out, in, out, mask?const_cast<const CFloatBitmap*>(&maskBitmap)->getPixels():NULL, output.size()*4, (TBlendMode)_Parameters[BlendMode].Integer, 
					channels, alpha);
			}
		}
	}

	// Normalize ?
	if (_Parameters[Normalize].Integer != 0)
	{
		float *out = output.getPixels();
		const uint count = output.size();
		uint i;
		for (i=0; i<count; i++, out+=4)
		{
			CVector v (out[0], out[1], out[2]);
			v.normalize();
			out[0] = v.x;
			out[1] = v.y;
			out[2] = v.z;
		}
	}

	// return the modified channels
	return channels;
}

// ***************************************************************************

/* End of op_color.cpp */
