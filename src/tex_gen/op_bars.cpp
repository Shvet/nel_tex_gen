/** \file op_bars.cpp
 * Bars operator.
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
#include "op_bars.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

#define GRADIENT_COUNT 4

// ***************************************************************************

COpBars::COpBars()
{
	// Parameters
	_Parameters.push_back (CParameter(CParameter::TypeGradient, "gradient0", "bars"));
	_Parameters.push_back (CParameter("height0", "bars", PercentSetup));
	_Parameters.push_back (CParameter(CParameter::TypeGradient, "gradient1", "bars"));
	_Parameters.push_back (CParameter("height1", "bars", PercentSetup));
	_Parameters.push_back (CParameter(CParameter::TypeGradient, "gradient2", "bars"));
	_Parameters.push_back (CParameter("height2", "bars", PercentSetup));
	_Parameters.push_back (CParameter(CParameter::TypeGradient, "gradient3", "bars"));
	_Parameters.push_back (CParameter("height3", "bars", PercentSetup));

	// Default gradient
	CGradientColor	black;
	CGradientColor	white;
	NLTEXGEN::copy (black.Color, ColorBlack);
	black.Pos = 0;
	NLTEXGEN::copy (white.Color, ColorWhite);
	white.Pos = 0;
	_Parameters[Gradient0].Gradient.push_back (black);
	_Parameters[Height0].Value = 25;
	_Parameters[Gradient1].Gradient.push_back (white);
	_Parameters[Height1].Value = 25;
	_Parameters[Gradient2].Gradient.push_back (black);
	_Parameters[Height2].Value = 25;
	_Parameters[Gradient3].Gradient.push_back (white);
	_Parameters[Height3].Value = 25;
}

// ***************************************************************************

TChannel COpBars::modifyLayer (CFloatBitmap &output, const CRenderParameter &renderParameters)
{
	const uint width = output.getWidth();
	const uint height = output.getHeight();
	float *dst = output.getPixels();

	// Build the gradients
	float gradients[2048*4*GRADIENT_COUNT];
	uint i, j;
	for (j=0; j<GRADIENT_COUNT; j++)
	for (i=0; i<width; i++)
		gradient (gradients+i*4+2048*4*j, _Parameters[Gradient0+j*2].Gradient, (float)i/width);

	uint gradient = 0;
	float heightMaxFloat = _Parameters[Height0].Value / 100.f;
	uint heightMax = (uint)floor(0.5f + heightMaxFloat*(float)width);
	for (j=0; j<height; j++, dst+=width*4)
	{
		if ((j >= heightMax) && (gradient < (GRADIENT_COUNT-1)))
		{
			gradient++;
			heightMaxFloat += _Parameters[Height0+2*gradient].Value / 100.f;
			heightMax = (uint)floor(0.5f + heightMaxFloat*(float)width);
		}

		memcpy (dst, gradients + gradient*2048*4, width*4*sizeof(float));
	}

	return ChannelRGB;
}

// ***************************************************************************

/* End of op_bars.cpp */
