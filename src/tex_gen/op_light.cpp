/** \file op_light.cpp
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

#include "stdtex_gen.h"
#include "op_light.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpLight::COpLight()
{
	// Two inputs
	_Inputs.push_back(CInput("normal map"));

	// Parameters
	_Parameters.push_back (CParameter("horizontal angle", "light properties", AngleSetup));
	_Parameters.push_back (CParameter("vertical angle", "light properties", AngleSetup));
	_Parameters.push_back (CParameter(CParameter::TypeGradient, "gradient", "light properties"));
	_Parameters[ZAxis].Value = 0;
	_Parameters[XAxis].Value = 90;

	// Default gradient
	CGradientColor	black;
	CGradientColor	white;
	NLTEXGEN::copy (black.Color, ColorBlack);
	black.Pos = 0;
	_Parameters[Gradient].Gradient.push_back (black);
	black.Pos = 0.5f;
	_Parameters[Gradient].Gradient.push_back (black);
	NLTEXGEN::copy (white.Color, ColorWhite);
	white.Pos = 1;
	_Parameters[Gradient].Gradient.push_back (white);
}

// ***************************************************************************

TChannel COpLight::evalInternal (class CFloatBitmap &output, const CRenderParameter &renderParameters)
{
	// Get input
	
	// Eval 1st argument
	evalSubOp (output, NormalMap, renderParameters);

	// Blend the input and output
	float *out = output.getPixels();

	const uint width = output.getWidth();
	const uint height = output.getHeight();
	const uint widthMask = width-1;
	const uint heightMask = height-1;

	const float az = _Parameters[ZAxis].Value * (float)NLMISC::Pi / 180;
	const float ax = _Parameters[XAxis].Value * (float)NLMISC::Pi / 180;
	const float dx = (float)(sin(az) * sin(ax));
	const float dy = (float)(cos(az) * sin(ax));
	const float dz = (float)cos(ax);

	// Build a gradient
	float gradients[2048*4];
	uint i;
	for (i=0; i<2048; i++)
		gradient (gradients+i*4, _Parameters[Gradient].Gradient, (float)i/2048.f);

	const uint count = output.size();
	for (i=0; i<count; i++, out+=4)
	{
		uint index = (uint)((1.f + out[0]*dx + out[1]*dy + out[2]*dz) * 1024.f);
		clamp (index, (uint)0, (uint)2047);		
		out[0] = gradients[index*4];
		out[1] = gradients[index*4+1];
		out[2] = gradients[index*4+2];
	}

	return ChannelRGB;
}

// ***************************************************************************

/* End of op_light.cpp */
