/** \file op_clouds.cpp
 * Clouds operator.
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
#include "op_clouds.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpClouds::COpClouds()
{
	// Parameters
	_Parameters.push_back (CParameter("persistence", "clouds", PercentSetup));
	_Parameters[Persistence].Value = 0.f;
	_Parameters.push_back (CParameter("first harmonic", "clouds", 0, 10, 1));
	_Parameters[FirstHarmonic].Value = 0;
	_Parameters.push_back (CParameter("seed", "clouds", RandomSetup));
	_Parameters[Seed].Value = 0.f;
	CFloatValueSetup intensity (CFloatValueSetup::Linear, "times", 0, 5, 0.001f, false, false, NULL, 0, 0, 5);
	_Parameters.push_back (CParameter("intensity", "clouds", ScaleSetup, 1));
	_Parameters[Intensity].Value = 1;
}

// ***************************************************************************

TChannel COpClouds::modifyLayer (CFloatBitmap &output, const CRenderParameter &renderParameters)
{
	const uint width = output.getWidth();
	const uint height = output.getHeight();
	float *dst = output.getPixels();
	memset (dst, 0, output.size()*4*sizeof(float));

	// Parameters
	const float persistence = 1.f / (_Parameters[Persistence].Value / 100.f * 9.f + 1.f);

	// Random
	TRandomValue random = getRandomValue (_Parameters[Seed].Value);

	// Fill a first noise bitmap
	std::vector<float> noisePattern;
	std::vector<float> tempPattern;
	noisePattern.resize (width*height);
	tempPattern.resize (width*height);
	uint i;
	const uint count = output.size();
	for (i=0; i<count; i++)
		noisePattern[i] = getNextRandomValue (random);

	// Pattern size
	const uint patternWidth = width;
	const uint patternHeight = height;
	uint level = getPowerOf2 (std::min(patternWidth, patternHeight)) - 1;
	uint patternWidth2 = width>>(level-_Parameters[FirstHarmonic].Integer);
	uint patternHeight2 = height>>(level-_Parameters[FirstHarmonic].Integer);
	uint patternWidthMask = patternWidth2-1;
	uint patternHeightMask = patternHeight2-1;
	float normalize = 0;
	while (level >= (uint)_Parameters[FirstHarmonic].Integer)
	{
		uint exp = level;
		float expf = 1;
		while (exp--)
			expf *= persistence;
		normalize += expf;
		if (level <= (uint)_Parameters[FirstHarmonic].Integer)
			break;
		level--;
	}

	// Intensity
	normalize /= _Parameters[Intensity].Value;

	uint levelEnd = getPowerOf2 (std::min(patternWidth, patternHeight));
	level = _Parameters[FirstHarmonic].Integer;
	uint zoomFactor = 1 << (levelEnd-1-_Parameters[FirstHarmonic].Integer);
	float *src = &(tempPattern[0]);
	while (level < levelEnd)
	{
		// Zoom the noise pattern
		const float oo_zoomfactor = 1.f / (float)zoomFactor;
		float *patternNoise = &(noisePattern[0]);
		const uint lineSize = patternWidth*zoomFactor;
		uint x,y;
		for (y=0; y<patternHeight2; y++)
		for (x=0; x<patternWidth2; x++)
		{
			// 0 2
			// 1 3

			const uint right = (x+1)&patternWidthMask;
			const uint bottom = (y+1)&patternHeightMask;
			const uint y_patternWidth = y*patternWidth2;
			const uint bottom_patternWidth = bottom*patternWidth2;
			const float f0 = patternNoise[y_patternWidth+x];
			const float f1 = patternNoise[bottom_patternWidth+x];
			const float f2 = patternNoise[y_patternWidth+right];
			const float f3 = patternNoise[bottom_patternWidth+right];

			const float delta02 = oo_zoomfactor * (f2-f0);
			const float delta13 = oo_zoomfactor * (f3-f1);

			uint dx, dy;
			float yFactor = 0;
			for (dy=0; dy<zoomFactor; dy++, yFactor+=oo_zoomfactor)
			{
				float ytmp = easineasout(yFactor);
				const uint offset = y*lineSize+dy*patternWidth+x*zoomFactor;
				float xFactor = 0;
				for (dx=0; dx<zoomFactor; dx++, xFactor+=oo_zoomfactor)
				{
					float xtmp = easineasout(xFactor);
					src[offset+dx] = (f0 * (1.f - xtmp) + f2 * xtmp) * (1.f - ytmp) 
						+ (f1 * (1.f - xtmp) + f3 * xtmp) * ytmp;
				}
			}
		}

		// Copy the pattern
		uint exp = level;
		float expf = 1;
		while (exp--)
			expf *= persistence;
		const float factor = expf / normalize;
		float *tmp = dst;
		for (i=0; i<count; i++, tmp+=4)
		{
			float color = factor * src[i];			
			tmp[0] += color;
			tmp[1] += color;
			tmp[2] += color;
		}

		// Next pattern
		patternWidth2<<=1;
		patternHeight2<<=1;
		patternWidthMask<<=1;
		patternHeightMask<<=1;
		patternWidthMask|=1;
		patternHeightMask|=1;
		level++;
		zoomFactor>>=1;
	}

	return ChannelRGB;
}

// ***************************************************************************

/* End of op_clouds.cpp */
