/** \file op_noise.cpp
 * Fill with a color operator
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
#include "op_noise.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpNoise::COpNoise()
{
	// Input
	_Inputs.push_back(CInput("density"));
	_Inputs.push_back(CInput("sprite"));

	// Parameters
	_Parameters.push_back (CParameter("density", "noise", PercentSetup));
	_Parameters[Density].Value = 20;
	_Parameters.push_back (CParameter("seed", "noise", RandomSetup));
	_Parameters[Seed].Value = 0;

	_Parameters.push_back (CParameter(CParameter::TypeGradient, "gradient", "noise"));
	CGradientColor	white;
	NLTEXGEN::copy (white.Color, ColorWhite);
	white.Pos = 1;
	_Parameters[Gradient].Gradient.push_back (white);
	white.Pos = 0;
	_Parameters[Gradient].Gradient.push_back (white);
	_Parameters.push_back (CParameter(CParameter::TypeColor, "background", "noise"));
	NLTEXGEN::copy(_Parameters[Background].Color, ColorBlack);
	_Parameters.push_back (CParameter("tile x", "sprite", true));
	_Parameters.push_back (CParameter("tile y", "sprite", true));
}

// ***************************************************************************

TChannel COpNoise::modifyLayer (CFloatBitmap &output, const CRenderParameter &renderParameters)
{
	// Blend the input and output
	float *out = output.getPixels();

	// Random
	TRandomValue random = getRandomValue (_Parameters[Seed].Value);
	const float threshold = _Parameters[Density].Value / 100;
	const float *background = _Parameters[Background].Color;
	const std::vector<CGradientColor> &gradient = _Parameters[Gradient].Gradient;

	// Eval the mask
	const float *maskPixels = NULL;
	CFloatBitmap densityBitmap;
	const ITexGenOperator *mask = const_cast<const COpNoise*>(this)->getBindedOp(DensityInput);
	if (mask)
	{
		evalSubOp (densityBitmap, DensityInput, renderParameters);
		densityBitmap.resample (output.getWidth(), output.getHeight());
		maskPixels = const_cast<const CFloatBitmap*>(&densityBitmap)->getPixels();
	}

	// Eval the sprite
	const float *spritePixels = NULL;
	CFloatBitmap spriteBitmap;
	const ITexGenOperator *sprite = const_cast<const COpNoise*>(this)->getBindedOp(SpriteInput);
	if (sprite)
	{
		evalSubOp (spriteBitmap, SpriteInput, renderParameters);
		spritePixels = const_cast<const CFloatBitmap*>(&spriteBitmap)->getPixels();
	}

	const bool tileU = _Parameters[TileU].Integer != 0;
	const bool tileV = _Parameters[TileV].Integer != 0;

	// Is a sprite is used, fill with background
	if (sprite)
	{
		replaceConstRGBA (out, background, output.size()*4);
		
		uint x, y, i;
		const uint width = output.getWidth();
		const uint height = output.getHeight();
		const uint srcWidth = spriteBitmap.getWidth();
		const uint srcHeight = spriteBitmap.getHeight();
		if ((srcWidth<=width) && (srcHeight<=height))
		{
			for (y=0, i=0; y<height; y++)
			for (x=0; x<width; x++, i++)
			{
				const float value = maskPixels?((1.f - maskPixels[i*4]) * getNextRandomValue (random)):getNextRandomValue (random);
				const float intensity = getNextRandomValue (random);
				
				if ( ( value < threshold) || ( (value <= threshold) && (threshold == 1.f) ) )
				{
					// Get a value from the gradient
					float color[4]; 
					NLTEXGEN::gradient (color, gradient, intensity);
				
					// Copy the bitmap here
					sint xOut = (sint)x - (srcWidth>>1);
					if (xOut < 0)
						xOut += srcWidth;
					sint yOut = (sint)y - (srcHeight>>1);
					if (yOut < 0)
						yOut += srcHeight;
					CMul mul;
					addPixels (out, width, height, xOut, yOut, spritePixels, srcWidth, srcHeight, tileU, tileV, color, mul);
				}
			}
		}
	}
	else
	{
		uint i;
		const uint count = output.size();
		for (i=0; i<count; i++)
		{
			const float value = maskPixels?((1.f - maskPixels[i*4]) * getNextRandomValue (random)):getNextRandomValue (random);
			const float intensity = getNextRandomValue (random);
			if (value < threshold)
			{
				// Get a value from the gradient
				NLTEXGEN::gradient (out+i*4, gradient, intensity);
			}
			else if (value > threshold)
			{
				// Copy the background
				NLTEXGEN::copy (out+i*4, background);
			}
			else if (threshold < 1.f)
			{
				// Copy the background
				NLTEXGEN::copy (out+i*4, background);
			}
			else
			{
				// Get a value from the gradient
				NLTEXGEN::gradient (out+i*4, gradient, intensity);
			}
		}
	}

	return ChannelRGB;
}

// ***************************************************************************

/* End of op_noise.cpp */
