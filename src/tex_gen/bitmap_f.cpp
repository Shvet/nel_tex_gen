/** \file bitmap_float.cpp
 * Float bitmap
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
#include "bitmap_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

friend void operator=(NLMISC::CBitmap &bitmap, const CFloatBitmap &bitmapF)
{
	// Erase ghost copy
	_GhostCopy = NULL;
	
	// Resize the dest bitmap
	bitmap.resize(bitmapF._Width, bitmapF._Height, CBitmpa::RGBA);

	// Copy the pixels
	const uint count = bitmapF._Width*bitmapF._Height*4;
	uint i;
	const float *src = &(bitmapF._Pixels[0]);
	uint8 *dest = &(bitmap.getPixels()[0]);
	for (i=0; i<count; i++)
	{
		float f = src[i] * 255.f;
		clamp(f, 0.f, 255.f);
		dest[i] = (uint8)f;
	}
}

// ***************************************************************************

CFloatBitmap::CFloatBitmap()
{
	_Width = 0;
	_Height = 0;
	_GhostCopy = NULL;
}

// ***************************************************************************

void CFloatBitmap::resize(uint width, uint height)
{
	_GhostCopy = NULL;
	_Width = width;
	_Height = height;
	_Pixels.resize (_Width*_Height*4);
}

// ***************************************************************************

void CFloatBitmap::operator=(const NLMISC::CBitmap &bitmap)
{
	// Erase ghost copy
	_GhostCopy = NULL;

	// Only RGBA
	if (bitmap.getPixelFormat() == CBitmap::RGBA)
	{
		// Resize the dest bitmap
		resize(bitmap.getWidth(), bitmap.getHeight());

		// Copy the pixels
		const uint count = _Width*_Height*4;
		uint i;
		float *dest = &(_Pixels[0]);
		const uint8 *src = &(bitmap.getPixels()[0]);
		for (i=0; i<count; i++)
		{
			dest[i] = ((float)src[i]) / 255.f;
		}
	}
}

// ***************************************************************************

void CFloatBitmap::operator=(const CFloatBitmap &bitmap)
{
	// Erase ghost copy
	_GhostCopy = NULL;

	_Width = bitmap._Width;
	_Height = bitmap._Height;
	_Pixels = bitmap._Pixels;
}

// ***************************************************************************

void CFloatBitmap::ghostCopy(const CFloatBitmap *bitmap)
{
	_GhostCopy = bitmap;
}

/* End of bitmap_float.cpp */
