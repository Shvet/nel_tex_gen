/** \file transform_float.cpp
 * Blend operations
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
#include "transform_float.h"

using namespace NLMISC;

namespace NLTEXGEN
{

// ***************************************************************************


// ***************************************************************************

void addPixels (float *dst, uint dstWidth, uint dstHeight, uint dstX, uint dstY, const float *src, uint srcWidth, 
				 uint srcHeight, bool tileX, bool tileY)
{
	// *********
	// * 0 * 2 *
	// *********
	// * 1 * 3 *
	// *********

	uint width_2_3 = (uint)std::min(std::max((sint)0, (sint)(dstX + srcWidth) - (sint)dstWidth), (sint)dstWidth);
	uint height_1_3 = (uint)std::min(std::max((sint)0, (sint)(dstY + srcHeight) - (sint)dstHeight), (sint)dstHeight);

	CAdd add;

	// Copy 0
	uint y;
	uint lastX = dstX + srcWidth - width_2_3;
	uint lastY = dstY + srcHeight - height_1_3;
	for (y=dstY; y<lastY; y++)
	{
		float *dstTemp = dst + (y*dstWidth + dstX) * 4;
		const float *srcTemp = src + ((y-dstY)*srcWidth) * 4;
		templateTransfoRGBA (dstTemp, dstTemp, srcTemp, (lastX-dstX)*4, add);
	}

	// Copy 1
	if (tileY && height_1_3)
	{
		lastX = dstX + srcWidth - width_2_3;
		lastY = height_1_3;
		for (y=0; y<lastY; y++)
		{
			float *dstTemp = dst + (y*dstWidth + dstX) * 4;
			const float *srcTemp = src + ((y+srcHeight-height_1_3)*srcWidth) * 4;
			templateTransfoRGBA (dstTemp, dstTemp, srcTemp, (lastX-dstX)*4, add);
		}
	}

	// Copy 2
	if (tileX && width_2_3)
	{
		lastY = dstY + srcHeight - height_1_3;
		for (y=dstY; y<lastY; y++)
		{
			float *dstTemp = dst + (y*dstWidth) * 4;
			const float *srcTemp = src + ((y-dstY)*srcWidth + srcWidth - width_2_3) * 4;
			templateTransfoRGBA (dstTemp, dstTemp, srcTemp, width_2_3*4, add);
		}

		// Copy 3
		if (tileY && height_1_3)
		{
			lastY = height_1_3;
			for (y=0; y<lastY; y++)
			{
				float *dstTemp = dst + (y*dstWidth) * 4;
				const float *srcTemp = src + ((y + srcHeight - height_1_3)*srcWidth + srcWidth - width_2_3) * 4;
				templateTransfoRGBA (dstTemp, dstTemp, srcTemp, width_2_3*4, add);
			}
		}
	}
}

// ***************************************************************************

void mulAddPixels (float *dst, uint dstWidth, uint dstHeight, uint dstX, uint dstY, const float *src, uint srcWidth, 
				 uint srcHeight, bool tileX, bool tileY, const float *color)
{
	// *********
	// * 0 * 2 *
	// *********
	// * 1 * 3 *
	// *********

	uint width_2_3 = (uint)std::min(std::max((sint)0, (sint)(dstX + srcWidth) - (sint)dstWidth), (sint)dstWidth);
	uint height_1_3 = (uint)std::min(std::max((sint)0, (sint)(dstY + srcHeight) - (sint)dstHeight), (sint)dstHeight);

	CMul mul;

	// Copy 0
	uint y;
	uint lastX = dstX + srcWidth - width_2_3;
	uint lastY = dstY + srcHeight - height_1_3;
	for (y=dstY; y<lastY; y++)
	{
		float *dstTemp = dst + (y*dstWidth + dstX) * 4;
		const float *srcTemp = src + ((y-dstY)*srcWidth) * 4;
		templateConstAddRGBA (dstTemp, srcTemp, color, (lastX-dstX)*4, mul);
	}

	// Copy 1
	if (tileY && height_1_3)
	{
		lastX = dstX + srcWidth - width_2_3;
		lastY = height_1_3;
		for (y=0; y<lastY; y++)
		{
			float *dstTemp = dst + (y*dstWidth + dstX) * 4;
			const float *srcTemp = src + ((y+srcHeight-height_1_3)*srcWidth) * 4;
			templateConstAddRGBA (dstTemp, srcTemp, color, (lastX-dstX)*4, mul);
		}
	}

	// Copy 2
	if (tileX && width_2_3)
	{
		lastY = dstY + srcHeight - height_1_3;
		for (y=dstY; y<lastY; y++)
		{
			float *dstTemp = dst + (y*dstWidth) * 4;
			const float *srcTemp = src + ((y-dstY)*srcWidth + srcWidth - width_2_3) * 4;
			templateConstAddRGBA (dstTemp, srcTemp, color, width_2_3*4, mul);
		}

		// Copy 3
		if (tileY && height_1_3)
		{
			lastY = height_1_3;
			for (y=0; y<lastY; y++)
			{
				float *dstTemp = dst + (y*dstWidth) * 4;
				const float *srcTemp = src + ((y + srcHeight - height_1_3)*srcWidth + srcWidth - width_2_3) * 4;
				templateConstAddRGBA (dstTemp, srcTemp, color, width_2_3*4, mul);
			}
		}
	}
}

// ***************************************************************************

void HVS2RGB (float h, float v, float s, float *color)
{
	if (s == 0.f)
	{
		color[0] = v;
		color[1] = v;
		color[2] = v;
	}
	else
	{
		float fi = (float)floor(h);
		float f = h - fi;
		const float p = v*(1.f - s);
		const float q = v*(1.f - s*f);
		const float t = v*(1.f - s*(1.f-f));
		switch ((uint)fi)
		{
		case 0:
			color[0] = v;
			color[1] = t;
			color[2] = p;
			break;
		case 1:
			color[0] = q;
			color[1] = v;
			color[2] = p;
			break;
		case 2:
			color[0] = p;
			color[1] = v;
			color[2] = t;
			break;
		case 3:
			color[0] = p;
			color[1] = q;
			color[2] = v;
			break;
		case 4:
			color[0] = t;
			color[1] = p;
			color[2] = v;
			break;
		case 5:
			color[0] = v;
			color[1] = p;
			color[2] = q;
		}
	}
}

// ***************************************************************************

void RGB2HVS (const float *color, float &h, float &v, float &s)
{
	const float minVal = std::min(std::min(color[0], color[1]), color[2]);
	v = std::max(std::max(color[0], color[1]), color[2]);
	const float delta = v-minVal;

	// Calculate saturation: saturation is 0 if r, g and b are all 0 
	if (v==0.f)
		s=0.0;
	else
		s=delta / v;

	if (s==0.f)
		h=0.0;
	else
	{
		if (color[0]==v)
			h=(color[1]-color[2])/delta;
		else
		{
			if (color[1]==v)
				h=2.f+(color[2]-color[0])/delta;
			else
				h=4.f+(color[0]-color[1])/delta;
		}
	}
	while (h < 0)
		h += 6;
	while (h > 6)
		h -= 6;
}

// ***************************************************************************

void gradient (float *dst, float *gradientPixels, uint width, uint height, const std::vector<CGradientColor> &_gradient, bool filter, bool tile, bool mirror, bool invert, bool wrapFilter)
{
	const uint count = width*height;
	const double log2 = log (2.f);
	if (filter)
	{
		const uint widthMask = width-1;
		const uint heightMask = height-1;

		// Build the gradient mipmaps
		float gradients0[2048*4];
		float gradients1[1024*4];
		float gradients2[512*4];
		float gradients3[256*4];
		float gradients4[128*4];
		float gradients5[64*4];
		float gradients6[32*4];
		float gradients7[16*4];
		float gradients8[8*4];
		float gradients9[4*4];
		float gradients10[2*4];
		float gradients11[1*4];
		float *gradients[12]=
		{
			gradients0,
			gradients1,
			gradients2,
			gradients3,
			gradients4,
			gradients5,
			gradients6,
			gradients7,
			gradients8,
			gradients9,
			gradients10,
			gradients11,
		};

		// Build the first gradient
		uint i;
		for (i=0; i<2048; i++)
			gradient (gradients0+i*4, _gradient, (float)i/2048.f);

		// Build the mipmaps
		uint j;
		for (j=1; j<12; j++)
		{
			float *_prev = gradients[j-1];
			float *_cur = gradients[j];
			const uint size = 2048>>j;
			for (i=0; i<size; i++, _cur+=4, _prev+=8)
			{
				_cur[0] = (_prev[0]+_prev[4])/2.f;
				_cur[1] = (_prev[1]+_prev[5])/2.f;
				_cur[2] = (_prev[2]+_prev[6])/2.f;
			}
		}
			
		// Gradient the image to get the final gradient value
		float *tempDst = gradientPixels;
		for (i=0; i<count; i++)
		{
			float tmp = tempDst[i];
			if (tile)
				tmp -= (float)floor(tmp);
			if (mirror)
			{
				if (tmp>=0.5f)
					tmp = 1.f - tmp;
				tmp *= 4196.f;
			}
			else
			{
				tmp *= 2048.f;
			}
			clamp (tmp, 0.f, 2047.9f);
			tempDst[i] = tmp;
		}

		// Gradient the image to get the final gradient value
		tempDst = gradientPixels;
		uint x, y;
		for (y=0; y<height; y++)
		for (x=0; x<width; x++, dst+=4)
		{
			float dg = 0;
			const float cur = tempDst[x+y*width];
			if (wrapFilter)
			{
				dg = std::max ((float)fabs(tempDst[(x-1)%widthMask+y*width]-cur),
						std::max ((float)fabs(tempDst[(x+1)%widthMask+y*width]-cur),
							std::max ((float)fabs(tempDst[x+((y-1)&heightMask)*width]-cur),
								(float)fabs(tempDst[x+((y+1)&heightMask)*width]-cur))));
			}
			else
			{
				if (x > 0)
					dg = (float)fabs(tempDst[(x-1)%widthMask+y*width]-cur);
				if (x < (width-1))
					dg = std::max ((float)fabs(tempDst[(x-1)%widthMask+y*width]-cur), dg);
				if (y > 0)
					dg = std::max ((float)fabs(tempDst[x+((y-1)&heightMask)*width]-cur), dg);
				if (y < (height-1))
					dg = std::max ((float)fabs(tempDst[x+((y+1)&heightMask)*width]-cur), dg);
			}

			// Compute the delta
			float delta = (float)(log (dg) / log2);
			if (delta>0.f)
			{
				float deltaFloor = (float)floor(delta);
				delta -= deltaFloor;
				float delta_minus = 1.f - delta;

				// The mipmap
				const uint mipMapBaseLevel0 = std::min((uint)deltaFloor, (uint)11);
				const uint mipMapBaseLevel1 = std::min(mipMapBaseLevel0+1, (uint)11);
				
				// Pixels
				float *mipMap0 = gradients[mipMapBaseLevel0];
				float *mipMap1 = gradients[mipMapBaseLevel1];

				// Size
				uint mipWidthMask0 = std::max((uint)(2048>>mipMapBaseLevel0), (uint)1) - 1;
				uint mipWidthMask1 = std::max((uint)(2048>>mipMapBaseLevel1), (uint)1) - 1;

				// Factors
				float mipXFactor0 = (float)(mipWidthMask0+1) / (float)2048;
				float mipXFactor1 = (float)(mipWidthMask1+1) / (float)2048;

				float mip0[4];
				float mip1[4];

				// Trilinear filtering
				bilinearSampleTileRGB (mip0, mipMap0, cur*mipXFactor0, mipWidthMask0, tile);
				bilinearSampleTileRGB (mip1, mipMap1, cur*mipXFactor1, mipWidthMask1, tile);

				dst[0] = mip0[0] * delta_minus + mip1[0] * delta;
				dst[1] = mip0[1] * delta_minus + mip1[1] * delta;
				dst[2] = mip0[2] * delta_minus + mip1[2] * delta;
			}
			else
			{
				bilinearSampleTileRGB (dst, gradients0, cur, widthMask, tile);
			}
		}
	}
	else
	{
		// Build a gradient
		float gradients[2048*4];
		uint i;
		for (i=0; i<2048; i++)
			gradient (gradients+i*4, _gradient, (float)i/2048.f);

		// Gradient the image
		for (i=0; i<count; i++, dst+=4)
		{
			float tmp = gradientPixels[i];
			int index;
			if (tile)
				tmp -= (float)floor(tmp);
			if (mirror)
			{
				if (tmp>=0.5f)
					tmp = 1.f - tmp;
				index = (int)(4196.f * tmp);
				clamp (index, 0, 2047);
			}
			else
			{
				index = (int)(2048.f * (tmp));
				clamp (index, 0, 2047);
			}
			if (invert)
				index = 2047 - index;
			const uint tmpI = index<<2;
			dst[0] = gradients[tmpI];
			dst[1] = gradients[tmpI+1];
			dst[2] = gradients[tmpI+2];
		}
	}
}

// ***************************************************************************

void drawQuad (float *dst, sint dstX, sint dstY, uint width, uint height, uint quadwidth, uint quadheight, const float *color, bool tileU, bool tileV)
{
	const uint widthShift = getPowerOf2 (width);
	const uint widthMask = width-1;
	const uint heightMask = height-1;

	sint y;
	for (y=dstY; y<dstY+(sint)quadheight; y++)
	{
		if (tileV || ((y>=0) && (y<(sint)height)))
		{
			sint x;
			const uint limitX = dstX+quadwidth;
			for (x=dstX; x<(sint)limitX; x++)
			{
				if (tileU || ((x>=0) && (x<(sint)width)))
				{
					const uint index = ((x&widthMask)+((y&heightMask)<<widthShift))<<2;
					dst[index+0] = color[0];
					dst[index+1] = color[1];
					dst[index+2] = color[2];
					dst[index+3] = color[3];
				}
			}
		}
	}
}

// ***************************************************************************

} // NLTEXGEN

/* End of transform_float.cpp */
