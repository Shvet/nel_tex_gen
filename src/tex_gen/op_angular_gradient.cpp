/** \file op_angular_gradient.cpp
 * Angular gradient operator
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
#include "op_angular_gradient.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpAngularGradient::COpAngularGradient()
{
	// One input
	_Inputs.push_back(CInput("offset"));

	// Parameters
	_Parameters.push_back (CParameter("mirror", "gradient", false));
	_Parameters.push_back (CParameter("tile", "gradient", true));
	_Parameters.push_back (CParameter("invert", "gradient", false));
	_Parameters.push_back (CParameter("scale", "gradient", ScaleSetup, 1));
	_Parameters.push_back (CParameter("rotation", "gradient", AngleSetup));
	_Parameters.push_back (CParameter(CParameter::TypeGradient, "gradient", "gradient"));

	// Default gradient
	CGradientColor	black;
	CGradientColor	white;
	NLTEXGEN::copy (black.Color, ColorBlack);
	black.Pos = 0;
	NLTEXGEN::copy (white.Color, ColorWhite);
	white.Pos = 1;
	_Parameters[Gradient].Gradient.push_back (black);
	_Parameters[Gradient].Gradient.push_back (white);
	_Parameters[Scale].Value = 1;
}

// ***************************************************************************

TChannel COpAngularGradient::modifyLayer (CFloatBitmap &output, const CRenderParameter &renderParameters)
{
	const uint width = output.getWidth();
	const uint height = output.getHeight();
	const float fWidth = (float)width;
	const float fHeight = (float)height;

	// Eval the mask
	const float *offsetPixels = NULL;
	CFloatBitmap offsetBitmap;
	if (const_cast<const COpAngularGradient*>(this)->getBindedOp(OffsetInput))
	{
		evalSubOp (offsetBitmap, OffsetInput, renderParameters);
		offsetBitmap.resample (output.getWidth(), output.getHeight());
		offsetPixels = const_cast<const CFloatBitmap*>(&offsetBitmap)->getPixels();
	}

	const float dY = 2.f / fHeight;
	const float dX = 2.f / fWidth;
	const float offset = -_Parameters[Offset].Value/360;
	const bool tile = _Parameters[Tile].Integer != 0;
	const bool invert = _Parameters[Invert].Integer != 0;
	float *dst = output.getPixels();
	const float oo2pi = 1.f / (_Parameters[Scale].Value * 2.f * (float)NLMISC::Pi);
	if (dst)
	{
		// Build a gradient
		float gradients[2048*4];
		uint i;
		for (i=0; i<2048; i++)
			gradient (gradients+i*4, _Parameters[Gradient].Gradient, (float)i/2048.f);

		if (_Parameters[Mirror].Integer)
		{
			// Mirror

			uint x, y;
			float fy = -1.f + dY / 2.f;
			for (y=0; y<height; y++, fy += dY)
			{
				float fx = -1.f + dX / 2.f;
				for (x=0; x<width; x++, fx += dX, dst+=4)
				{
					float tmp = ((float)NLMISC::Pi + (float)atan2(fx, fy)) * oo2pi - offset;
					if (offsetPixels)
					{
						tmp -= *offsetPixels;
						offsetPixels += 4;
					}
					if (tile)
						tmp -= (float)floor(tmp);
					if (tmp>=0.5f)
						tmp = 1.f - tmp;
					int index = (int)(4196.f * tmp);
					clamp (index, 0, 2047);
					if (invert)
						index = 2047 - index;
					const uint tmpI = index*4;
					dst[0] = gradients[tmpI];
					dst[1] = gradients[tmpI+1];
					dst[2] = gradients[tmpI+2];
				}
			}
		}
		else
		{
			// No mirror

			uint x, y;
			float fy = -1.f + dY / 2.f;
			for (y=0; y<height; y++, fy += dY)
			{
				float fx = -1.f + dX / 2.f;
				for (x=0; x<width; x++, fx += dX, dst+=4)
				{
					float tmp = ((float)NLMISC::Pi + (float)atan2(fx, fy)) * oo2pi - offset;
					if (offsetPixels)
					{
						tmp -= *offsetPixels;
						offsetPixels += 4;
					}
					if (tile)
						tmp -= (float)floor(tmp);
					int index = (int)(2048.f * (tmp));
					clamp (index, 0, 2047);
					if (invert)
						index = 2047 - index;
					const uint tmpI = index*4;
					dst[0] = gradients[tmpI];
					dst[1] = gradients[tmpI+1];
					dst[2] = gradients[tmpI+2];
				}
			}
		}
	}
	return ChannelRGB;
}

// ***************************************************************************

void COpAngularGradient::serial(NLMISC::IStream &s)
{
	ITexGenOperator::serial (s);
}

// ***************************************************************************

/* End of op_angular_gradient.cpp */
