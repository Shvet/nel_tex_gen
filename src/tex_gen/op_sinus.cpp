/** \file op_transform.cpp
 * Sinus transformation of a picture
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
#include "op_sinus.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpSinus::COpSinus()
{
	// Parameters
	_Parameters.push_back (CParameter("length", "sinus", PixelSetup));
	_Parameters.push_back (CParameter("period", "sinus", CenteredMT1_1Setup));
	_Parameters[Period].Value = 1;
}

// ***************************************************************************

TChannel COpSinus::modifyColor (CFloatBitmap &output, const CFloatBitmap &input, const CRenderParameter &renderParameters)
{
	// Blend the input and output
	output.resize (input.getWidth(), input.getHeight());
	float *out = output.getPixels();
	const float *in = input.getPixels();

	const uint width = output.getWidth();
	const uint height = output.getHeight();
	const uint widthMask = width-1;
	const uint heightMask = height-1;
	const uint widthShift = getPowerOf2 (width);

	// Use filter ?
	bool filter = _Parameters[Filter].Integer != 0;
	const bool tileU = _Parameters[TileU].Integer != 0;
	const bool tileV = _Parameters[TileV].Integer != 0;

	// Parameters
	const float length = (float)width * _Parameters[Length].Value;
	const float period = 2 * (float)Pi * _Parameters[Period].Value / height;

	uint x, y;
	for (y=0; y<height; y++)
	{
		float deltaX = length * (float) sin (period * y);
		const float finalY = (float)y;
		for (x=0; x<width; x++, out+=4)
		{
			const float finalX = (float)x + deltaX;

			// Filter ?
			if (filter)
			{
				bilinearSampleTileRGB (out, in, finalX, finalY, widthMask, heightMask, widthShift,
					tileU, tileV);
			}
			else
			{
				sint iX = ((sint)floor(finalX+0.5f));
				sint iY = ((sint)floor(finalY+0.5f));
				if (tileU)
					 iX &= widthMask;
				else
					clamp (iX, (sint)0, (sint)widthMask);
				if (tileV)
					 iY &= heightMask;
				else
					clamp (iY, (sint)0, (sint)heightMask);

				NLTEXGEN::copy(out, in+(iX+iY*width)*4);
			}
		}
	}
	return ChannelRGB;
}

// ***************************************************************************

/* End of op_transform.cpp */
