/** \file op_normalize.cpp
 * Convert RGB to normal
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
#include "op_normalize.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpNormalize::COpNormalize()
{
	// Parameters
	_Parameters.push_back (CParameter("height", "convert to normal", CenteredMT1_1Setup));
	_Parameters[Height].Value = 1;
	_Parameters[IOpColor::Normalize].Integer = 1;
}

// ***************************************************************************

TChannel COpNormalize::modifyColor (CFloatBitmap &output, const CFloatBitmap &input, const CRenderParameter &renderParameters)
{
	// Blend the input and output
	output.resize (input.getWidth(), input.getHeight());
	float *out = output.getPixels();
	const float *in = input.getPixels();

	const uint width = output.getWidth();
	const uint height = output.getHeight();
	const uint widthMask = width-1;
	const uint heightMask = height-1;

	const float heightMap = _Parameters[Height].Value*1024;
	const float mul = 1.f / (heightMap + 0.0001f);

	uint x, y, yWidth;
	for (y=0, yWidth=0; y<height; y++, yWidth+=width)
	for (x=0; x<width; x++, out+=4)
	{
		CVector tmp;
		// const uint left = (((x-1)&widthMask)+yWidth);
		const uint right = (((x+1)&widthMask)+yWidth);
		const uint top = (x+((y-1)&heightMask)*width);
		float cur = in[(x+y*width)*4];
		// const uint bottom = (x+((y+1)&heightMask)*width);
			
		out[0] = (cur - in[4*right]);
		out[1] = (cur - in[4*top]);
		out[2] = mul;
	}

	return ChannelRGB;
}

// ***************************************************************************

/* End of op_normalize.cpp */
