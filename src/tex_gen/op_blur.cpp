/** \file op_blur.cpp
 * Gaussian blur
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
#include "op_blur.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpBlur::COpBlur()
{
	// Parameters
	CFloatValueSetup radius (CFloatValueSetup::Linear, "pixels", 0.01f, 10, 0.001f, false, false, NULL, 0, 0.01f, 10);
	_Parameters.push_back (CParameter("radius", "blur", radius));
	CFloatValueSetup quality (CFloatValueSetup::Linear, "times the radius", 0.01f, 5, 0.001f, false, false, NULL, 0, 0.01f, 5);
	_Parameters.push_back (CParameter("quality", "blur", quality));
	CFloatValueSetup intensity (CFloatValueSetup::Linear, "times", 0, 5, 0.001f, false, false, NULL, 0);
	_Parameters.push_back (CParameter("intensity", "blur", intensity));
	_Parameters.push_back (CParameter("up", "blur", true, 2));
	_Parameters.push_back (CParameter("down", "blur", true, 2));
	_Parameters.push_back (CParameter("left", "blur", true, 2));
	_Parameters.push_back (CParameter("right", "blur", true, 2));
	_Parameters[Intensity].Value = 1;
	_Parameters[Radius].Value = 0.01f;
	_Parameters[Quality].Value = 1;
}

// ***************************************************************************

TChannel COpBlur::modifyColor (CFloatBitmap &output, const CFloatBitmap &input, const CRenderParameter &renderParameters)
{
	// Blend the input and output
	output.resize (input.getWidth(), input.getHeight());
	float *out = output.getPixels();
	const float *in = input.getPixels();

	// Random
	const float intensity = _Parameters[Intensity].Value;
	const float radius = _Parameters[Radius].Value;
	const float quality = _Parameters[Quality].Value;
	const uint iradius = 2*((uint)(quality*radius) + 1);

	// Init the gaussian
	vector<float> gaussian;
	gaussian.resize (iradius);
	sint i;
	for (i=0; i<(sint)iradius; i++)
		gaussian[i] = (float)(exp(-(double)(i*i)/(2*(double)radius*(double)radius)) * ((double)radius * sqrt(2.0*NLMISC::Pi)));
	float sum = 0;
	for (i=1-iradius; i<(sint)iradius; i++)
		sum += gaussian[abs(i)];
	for (i=0; i<(sint)iradius; i++)
	{
		gaussian[i] *= intensity;
		gaussian[i] /= sum;
	}
	float *gaussianPixels = &(gaussian[0]);

	CFloatBitmap tempBitmap;
	tempBitmap.resize (output.getWidth(), output.getHeight());
	float *temp = tempBitmap.getPixels();
	memset (temp, 0, tempBitmap.size()*4*sizeof(float));

	const uint width = output.getWidth();
	const uint height = output.getHeight();
	const uint widthMask = width-1;
	const uint heightMask = height-1;

	const sint up = _Parameters[Up].Integer ? iradius:1;
	const sint down = _Parameters[Down].Integer ? 1-iradius:0;
	const sint left = _Parameters[Left].Integer ? iradius:1;
	const sint right = _Parameters[Right].Integer ? 1-iradius:0;

	uint x, y;
	for (y=0; y<height; y++)
	{
		for (x=0; x<width; x++, temp+=4)
		{
			sint dy;
			for (dy = down; dy < up; dy++)
			{
				float blur = gaussianPixels[abs(dy)];
				const uint blurIndex = (((y+dy)&heightMask)*width + x)*4;
				temp[0] += blur * in[blurIndex];
				temp[1] += blur * in[blurIndex+1];
				temp[2] += blur * in[blurIndex+2];
			}
		}
	}

	temp = tempBitmap.getPixels();
	memset (out, 0, tempBitmap.size()*4*sizeof(float));
	for (y=0; y<height; y++)
	{
		for (x=0; x<width; x++, out+=4)
		{
			sint dx;
			for (dx = right; dx < left; dx++)
			{
				// Blur intensity
				float blur = gaussianPixels[abs(dx)];
				const uint blurIndex = (y*width + ((x+dx)&widthMask))*4;
				out[0] += blur * temp[blurIndex];
				out[1] += blur * temp[blurIndex+1];
				out[2] += blur * temp[blurIndex+2];
			}
		}
	}

	return ChannelRGB;
}

// ***************************************************************************

/* End of op_blur.cpp */
