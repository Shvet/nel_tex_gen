/** \file bitmap_float.h
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

#ifndef NL_FLOAT_BITMAP
#define NL_FLOAT_BITMAP

namespace NLMISC
{
class CBitmap;
}

namespace NLTEXGEN
{

// A float bitmap class
class CFloatBitmap
{
public:

	// Constructor
	CFloatBitmap();

	// Get the pixels
	float *getPixels()
	{
		// Erase ghost copy
		if (_GhostCopy)
		{
			// Hard copy
			*this = *_GhostCopy;
		}

		// No more mipmaps
		_MipMapsBuilded = false;

		if (_Pixels.empty())
			return NULL;
		return &(_Pixels[0]);
	}

	// Get the pixels
	const float *getPixels(uint mipMap = 0) const
	{
		const CFloatBitmap *bitmap = _GhostCopy ? _GhostCopy : this;
		if (mipMap == 0)
		{
			if (bitmap->_Pixels.empty())
				return NULL;
			return &(bitmap->_Pixels[0]);
		}
		else
		{
			// Mipmaps builded ?
			if (!bitmap->_MipMapsBuilded)
				bitmap->buildMipMaps();
			
			nlassert ((mipMap-1) < bitmap->_MipMaps.size ());
			return &(bitmap->_MipMaps[mipMap-1][0]);
		}
	}

	// Resize the bitmap
	void resize(uint width, uint height);

	// Get the width
	uint getWidth() const
	{
		if (_GhostCopy)
			return _GhostCopy->_Width;
		else
			return _Width;
	}

	// Get the height
	uint getHeight() const
	{
		if (_GhostCopy)
			return _GhostCopy->_Height;
		else
			return _Height;
	}

	// Get the size of the bitmap in pixels.
	uint size() const
	{
		return getWidth()*getHeight();
	}

	// Get number of mipmap
	uint getMipMapCount() const
	{
		const CFloatBitmap *bitmap = _GhostCopy ? _GhostCopy : this;
		return std::max(NLMISC::getPowerOf2 (bitmap->_Width), NLMISC::getPowerOf2(bitmap->_Height)) + 1;
	}

	// Ghost copy
	void ghostCopy(const CFloatBitmap *bitmap);
	
	// Export to a NLMISC::CBitmap
	void export(NLMISC::CBitmap &bitmap) const;
	
	// Export alpha to a NLMISC::CBitmap : warning you should have ccalled export before call this
	void exportAlpha(NLMISC::CBitmap &bitmap) const;

	// Copy from a NLMISC::CBitmap
	void operator=(const NLMISC::CBitmap &bitmap);

	// Hard copy
	void operator=(const CFloatBitmap &bitmap);

	// Resample
	void resample (uint width, uint height);

protected:

	// Build the mipmaps
	void buildMipMaps () const;

	// The pixels
	const CFloatBitmap		*_GhostCopy;

	// The pixels
	std::vector<float>		_Pixels;

	// The mipmaps
	mutable std::vector<std::vector<float> >		_MipMaps;

	// The size
	uint	_Width;
	uint	_Height;

	// Mipmaps builded ?
	mutable bool	_MipMapsBuilded;
};

// ***************************************************************************

} // NLTEXGEN

#endif // NL_FLOAT_BITMAP

/* End of bitmap_float.h */
