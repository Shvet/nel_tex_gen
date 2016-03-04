/** \file op_transform.cpp
 * Matrix transformation of a picture
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
#include "op_transform.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpTransform::COpTransform()
{
	// Parameters
	_Parameters.push_back (CParameter("rotate", "scale", AngleSetup));
	_Parameters.push_back (CParameter("x & y axis", "scale", ScaleSetup));
	_Parameters[Scale].Value = 1;
	_Parameters.push_back (CParameter("x axis", "scale", ScaleSetup));
	_Parameters[ScaleX].Value = 1;
	_Parameters.push_back (CParameter("y axis", "scale", ScaleSetup));
	_Parameters[ScaleY].Value = 1;
	_Parameters.push_back (CParameter("rotate", "shear", AngleSetup));
	_Parameters.push_back (CParameter("shear", "shear", CenteredMT1_1Setup));
	_Parameters.push_back (CParameter("angle", "rotate", AngleSetup));
	_Parameters.push_back (CParameter("x axis", "translation", PixelSetup));
	_Parameters.push_back (CParameter("y axis", "translation", PixelSetup));
}

// ***************************************************************************

TChannel COpTransform::modifyColor (CFloatBitmap &output, const CFloatBitmap &input, const CRenderParameter &renderParameters)
{
	// Blend the input and output
	output.resize (input.getWidth(), input.getHeight());
	float *out = output.getPixels();
	const float *in = input.getPixels();

	const uint width = output.getWidth();
	const uint height = output.getHeight();
	const uint widthMask = width-1;
	const uint heightMask = height-1;
	const uint width2 = width/2;
	const uint height2 = height/2;
	const uint widthShift = getPowerOf2 (width);

	// Parameters
	float scaleRotate = _Parameters[ScaleRotate].Value*(float)NLMISC::Pi/180;
	float scale = _Parameters[Scale].Value;
	float scaleX = _Parameters[ScaleX].Value;
	float scaleY = _Parameters[ScaleY].Value;
	float shearRotate = _Parameters[ShearRotate].Value*(float)NLMISC::Pi/180;
	float shear = _Parameters[Shear].Value;
	float rotate = _Parameters[Rotate].Value*(float)NLMISC::Pi/180;
	float translationX = _Parameters[TranslationX].Value*(float)width;
	float translationY = _Parameters[TranslationY].Value*(float)height;

	// Build the matrix
	CMatrix transform;
	transform.identity();
	transform.translate (CVector (translationX, translationY, 0));
	transform.translate (CVector ((float)width2, (float)height2, 0));
	transform.rotate (CVector (0, 0, rotate), CMatrix::XYZ);
	transform.rotate (CVector (0, 0, -shearRotate), CMatrix::XYZ);
	transform.shear (shear);
	transform.rotate (CVector (0, 0, shearRotate), CMatrix::XYZ);
	transform.rotate (CVector (0, 0, -scaleRotate), CMatrix::XYZ);
	transform.scale(CVector (scale*scaleX, scale*scaleY, 1));
	transform.rotate (CVector (0, 0, scaleRotate), CMatrix::XYZ);
	transform.translate (CVector (-(float)width2, -(float)height2, 0));
	transform.invert();

	// Use filter ?
	bool filter = _Parameters[Filter].Integer != 0;
	const bool tileU = _Parameters[TileU].Integer != 0;
	const bool tileV = _Parameters[TileV].Integer != 0;

	// Final matrix
	CVector I = transform.getI();
	CVector J = transform.getJ();
	CVector K = transform.getK();
	CVector P = transform.getPos();

	// For filter
	const float dX = (float)sqrt(fabs(I.x)*fabs(I.x)+fabs(J.x)*fabs(J.x));
	const float dY = (float)sqrt(fabs(I.y)*fabs(I.y)+fabs(J.y)*fabs(J.y));
	float delta = (float)(log (std::max(dX, dY)) / log (2.f));
	bool useMipMaps = (delta>0.f);
	float deltaFloor;
	float delta_minus;
	const float *mipMap0;
	const float *mipMap1;
	float mipXFactor0;
	float mipYFactor0;
	float mipXFactor1;
	float mipYFactor1;
	uint mipWidthMask0;
	uint mipHeightMask0;
	uint mipWidthShift0;
	uint mipWidthMask1;
	uint mipHeightMask1;
	uint mipWidthShift1;
	if (useMipMaps)
	{
		deltaFloor = (float)floor(delta);
		delta -= deltaFloor;
		delta_minus = 1.f - delta;

		// The mipmap
		const uint mipMapCount = input.getMipMapCount();
		const uint mipMapBaseLevel0 = std::min((uint)deltaFloor, mipMapCount);
		const uint mipMapBaseLevel1 = std::min(mipMapBaseLevel0+1, mipMapCount);
		
		// Pixels
		mipMap0 = input.getPixels(mipMapBaseLevel0);
		mipMap1 = input.getPixels(mipMapBaseLevel1);

		// Size
		mipWidthMask0 = std::max(width>>mipMapBaseLevel0, (uint)1) - 1;
		mipWidthShift0 = getPowerOf2 (mipWidthMask0+1);
		mipHeightMask0 = std::max(height>>mipMapBaseLevel0, (uint)1) - 1;
		mipWidthMask1 = std::max(width>>mipMapBaseLevel1, (uint)1) - 1;
		mipWidthShift1 = getPowerOf2 (mipWidthMask1+1);
		mipHeightMask1 = std::max(height>>mipMapBaseLevel1, (uint)1) - 1;

		// Factors
		mipXFactor0 = (float)(mipWidthMask0+1) / (float)width;
		mipYFactor0 = (float)(mipHeightMask0+1) / (float)height;
		mipXFactor1 = (float)(mipWidthMask1+1) / (float)width;
		mipYFactor1 = (float)(mipHeightMask1+1) / (float)height;
	}

	uint x, y;
	for (y=0; y<height; y++)
	for (x=0; x<width; x++, out+=4)
	{
		const float fx = (float)x;
		const float fy = (float)y;
		const float finalX = I.x * fx + J.x * fy + P.x;
		const float finalY = I.y * fx + J.y * fy + P.y;

		// Filter ?
		if (filter)
		{
			if (useMipMaps)
			{
				float mip0[4];
				float mip1[4];

				// Trilinear filtering

				bilinearSampleTileRGB (mip0, mipMap0, finalX*mipXFactor0, finalY*mipXFactor0, 
					mipWidthMask0, mipHeightMask0, mipWidthShift0, tileU, tileV);
				bilinearSampleTileRGB (mip1, mipMap1, finalX*mipXFactor1, finalY*mipXFactor1, 
					mipWidthMask1, mipHeightMask1, mipWidthShift1, tileU, tileV);

				out[0] = mip0[0] * delta_minus + mip1[0] * delta;
				out[1] = mip0[1] * delta_minus + mip1[1] * delta;
				out[2] = mip0[2] * delta_minus + mip1[2] * delta;
			}
			else
			{
				bilinearSampleTileRGB (out, in, finalX, finalY, widthMask, heightMask, widthShift,
					tileU, tileV);
			}
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
	return ChannelRGB;
}

// ***************************************************************************

/* End of op_transform.cpp */
