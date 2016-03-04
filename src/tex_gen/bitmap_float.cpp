/** \file bitmap_f.cpp
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

void CFloatBitmap::export(NLMISC::CBitmap &bitmap) const
{
	// Erase ghost copy
	const CFloatBitmap *copy;
	if (_GhostCopy)
		copy = _GhostCopy;
	else
		copy = this;
	
	// Resize the dest bitmap
	bitmap.resize(copy->_Width, copy->_Height, CBitmap::RGBA);

	// Copy the pixels
	const uint count = copy->_Width*copy->_Height*4;
	uint i;
	if (count)
	{
		const float *src = &(copy->_Pixels[0]);
		uint8 *dest = &(bitmap.getPixels()[0]);
		for (i=0; i<count; i++)
		{
			float f = src[i] * 255.f;
			clamp(f, 0.f, 255.f);
			dest[i] = (uint8)f;
		}
	}
}

// ***************************************************************************

void CFloatBitmap::exportAlpha(NLMISC::CBitmap &bitmap) const
{
	// Erase ghost copy
	const CFloatBitmap *copy;
	if (_GhostCopy)
		copy = _GhostCopy;
	else
		copy = this;

	// Copy the pixels
	const uint count = copy->_Width*copy->_Height*4;
	uint i;
	if (count)
	{
		const float *src = &(copy->_Pixels[0]);
		uint8 *dest = &(bitmap.getPixels()[0]);
		for (i=0; i<count; i+=4)
		{
			float f = src[i] * 255.f;
			clamp(f, 0.f, 255.f);
			dest[i+3] = (uint8)f;
		}
	}
}

// ***************************************************************************

CFloatBitmap::CFloatBitmap()
{
	_Width = 0;
	_Height = 0;
	_GhostCopy = NULL;
	_MipMapsBuilded = false;
}

// ***************************************************************************

void CFloatBitmap::resize(uint width, uint height)
{
	_GhostCopy = NULL;
	_Width = width;
	_Height = height;
	_Pixels.resize (_Width*_Height*4);
	_MipMapsBuilded = false;
}

// ***************************************************************************

void CFloatBitmap::operator=(const NLMISC::CBitmap &bitmap)
{
	// Erase ghost copy
	_GhostCopy = NULL;
	_MipMapsBuilded = false;

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
	const CFloatBitmap *copy;
	if (bitmap._GhostCopy)
		copy = bitmap._GhostCopy;
	else
		copy = &bitmap;

	_Width = copy->_Width;
	_Height = copy->_Height;
	_Pixels = copy->_Pixels;
	_MipMapsBuilded = copy->_MipMapsBuilded;
	if (_MipMapsBuilded)
		_MipMaps = copy->_MipMaps;

	// Erase ghost copy
	_GhostCopy = NULL;
}

// ***************************************************************************

void CFloatBitmap::ghostCopy(const CFloatBitmap *bitmap)
{
	if (bitmap->_GhostCopy)
		_GhostCopy = bitmap->_GhostCopy;
	else
		_GhostCopy = bitmap;
}

// ***************************************************************************

void CFloatBitmap::resample (uint width, uint height)
{
	// Erase ghost copy
	const CFloatBitmap *copy;
	if (_GhostCopy)
		copy = _GhostCopy;
	else
		copy = this;
	if ((copy->_Width != width) || (copy->_Height != height))
	{
		_MipMapsBuilded = false;

		uint originalWidth = copy->_Width;
		uint originalHeight = copy->_Height;

		bool bXMag=(width>=originalWidth);
		bool bYMag=(height>=originalHeight);
		bool bXEq=(width==originalWidth);
		bool bYEq=(height==originalHeight);
		std::vector<NLMISC::CRGBAF> pIterm (width*originalHeight);
		CFloatBitmap temp;
		temp = *this;

		// Erase ghost copy
		_GhostCopy = NULL;

		float *pSrc = &(temp._Pixels[0]);
		resize(width, height);
		float *pDest = &(_Pixels[0]);

		if (bXMag)
		{
			float fXdelta=(float)(originalWidth)/(float)(width);
			NLMISC::CRGBAF *pItermPtr=&*pIterm.begin();
			uint nY;
			for (nY=0; nY<originalHeight; nY++)
			{
				const float *pSrcLine=pSrc;
				float fX=0.f;
				uint nX;
				for (nX=0; nX<width; nX++)
				{
					float fVirgule=fX-(float)floor(fX);
					nlassert (fVirgule>=0.f);
					NLMISC::CRGBAF vColor;
					const uint index = 4*(uint)floor(fX);
					if (fVirgule>=0.5f)
					{
						if (fX<(float)(originalWidth-1))
						{
							NLMISC::CRGBAF vColor1 (pSrcLine[index], pSrcLine[index+1], 
								pSrcLine[index+2], pSrcLine[index+3]);
							NLMISC::CRGBAF vColor2 (pSrcLine[index+4], pSrcLine[index+5], 
								pSrcLine[index+6], pSrcLine[index+7]);
							vColor=vColor1*(1.5f-fVirgule)+vColor2*(fVirgule-0.5f);
						}
						else
							vColor.set(pSrcLine[index], pSrcLine[index+1], 
								pSrcLine[index+2], pSrcLine[index+3]);
					}
					else
					{
						if (fX>=1.f)
						{
							NLMISC::CRGBAF vColor1 (pSrcLine[index], pSrcLine[index+1], 
								pSrcLine[index+2], pSrcLine[index+3]);
							NLMISC::CRGBAF vColor2 (pSrcLine[index-4], pSrcLine[index-3], 
								pSrcLine[index-2], pSrcLine[index-1]);
							vColor=vColor1*(0.5f+fVirgule)+vColor2*(0.5f-fVirgule);
						}
						else
							vColor.set(pSrcLine[index], pSrcLine[index+1], 
								pSrcLine[index+2], pSrcLine[index+3]);
					}
					*(pItermPtr++)=vColor;
					fX+=fXdelta;
				}
				pSrc+=4*originalWidth;
			}
		}
		else if (bXEq)
		{
			NLMISC::CRGBAF *pItermPtr=&*pIterm.begin();
			for (uint nY=0; nY<originalHeight; nY++)
			{
				const float *pSrcLine=pSrc;
				uint nX;
				for (nX=0; nX<width; nX++)
				{
					const uint x4 = 4*nX;
					(pItermPtr++)->set (pSrcLine[x4], pSrcLine[x4+1], pSrcLine[x4+2], pSrcLine[x4+3]);
				}
				pSrc+=4*originalWidth;
			}
		}
		else
		{
			double fXdelta=(double)(originalWidth)/(double)(width);
			nlassert (fXdelta>1.f);
			NLMISC::CRGBAF *pItermPtr=&*pIterm.begin();
			uint nY;
			for (nY=0; nY<originalHeight; nY++)
			{
				const float *pSrcLine=pSrc;
				double fX=0.f;
				uint nX;
				for (nX=0; nX<width; nX++)
				{
					NLMISC::CRGBAF vColor (0.f, 0.f, 0.f, 0.f);
					double fFinal=fX+fXdelta;
					while (fX<fFinal)
					{
						double fNext=(double)floor (fX)+1.f;
						if (fNext>fFinal)
							fNext=fFinal;
						const uint index = 4*(uint)floor(fX);
						vColor+=((float)(fNext-fX))*NLMISC::CRGBAF (pSrcLine[index], pSrcLine[index+1], pSrcLine[index+2], pSrcLine[index+3]);
						fX=fNext;
					}
					nlassert (fX==fFinal);
					vColor/=(float)fXdelta;
					*(pItermPtr++)=vColor;
				}
				pSrc+=4*originalWidth;
			}
		}
					
		if (bYMag)
		{
			double fYdelta=(double)(originalHeight)/(double)(height);
			uint nX;
			for (nX=0; nX<width; nX++)
			{
				double fY=0.f;
				uint nY;
				for (nY=0; nY<height; nY++)
				{
					double fVirgule=fY-(double)floor(fY);
					nlassert (fVirgule>=0.f);
					NLMISC::CRGBAF vColor;
					if (fVirgule>=0.5f)
					{
						if (fY<(double)(originalHeight-1))
						{
							NLMISC::CRGBAF vColor1=pIterm[((uint)floor(fY))*width+nX];
							NLMISC::CRGBAF vColor2=pIterm[(((uint)floor(fY))+1)*width+nX];
							vColor=vColor1*(1.5f-(float)fVirgule)+vColor2*((float)fVirgule-0.5f);
						}
						else
							vColor=pIterm[((uint)floor(fY))*width+nX];
					}
					else
					{
						if (fY>=1.f)
						{
							NLMISC::CRGBAF vColor1=pIterm[((uint)floor(fY))*width+nX];
							NLMISC::CRGBAF vColor2=pIterm[(((uint)floor(fY))-1)*width+nX];
							vColor=vColor1*(0.5f+(float)fVirgule)+vColor2*(0.5f-(float)fVirgule);
						}
						else
							vColor=pIterm[((uint)floor(fY))*width+nX];
					}
					const uint index = 4*(nX+nY*width);
					pDest[index]=vColor.R;
					pDest[index+1]=vColor.G;
					pDest[index+2]=vColor.B;
					pDest[index+3]=vColor.A;
					fY+=fYdelta;
				}
			}
		}
		else if (bYEq)
		{
			for (uint nX=0; nX<width; nX++)
			{
				uint nY;
				for (nY=0; nY<height; nY++)
				{
					const CRGBAF &src = pIterm[nY*width+nX];
					const uint index = 4*(nX+nY*width);
					pDest[index] = src.R;
					pDest[index+1] = src.G;
					pDest[index+2] = src.B;
					pDest[index+3] = src.A;
				}
			}
		}
		else
		{
			double fYdelta=(double)(originalHeight)/(double)(height);
			nlassert (fYdelta>1.f);
			uint nX;
			for (nX=0; nX<width; nX++)
			{
				double fY=0.f;
				uint nY;
				for (nY=0; nY<height; nY++)
				{
					NLMISC::CRGBAF vColor (0.f, 0.f, 0.f, 0.f);
					double fFinal=fY+fYdelta;
					while ((fY<fFinal)&&((uint)fY!=originalHeight))
					{
						double fNext=(double)floor (fY)+1.f;
						if (fNext>fFinal)
							fNext=fFinal;
						vColor+=((float)(fNext-fY))*pIterm[((uint)floor(fY))*width+nX];
						fY=fNext;
					}
					vColor/=(float)fYdelta;
					const uint index = 4*(nX+nY*width);
					pDest[index]=vColor.R;
					pDest[index+1]=vColor.G;
					pDest[index+2]=vColor.B;
					pDest[index+3]=vColor.A;
				}
			}
		}
		
	}
}

// ***************************************************************************

void CFloatBitmap::buildMipMaps () const
{
	// Should not be a ghost copy
	nlassert (_GhostCopy==NULL);

	// Number of mipmaps
	if (_Width && _Height)
	{
		uint mipMapCount = getMipMapCount() - 1;

		// Resize the array, never destroy bitmaps
		_MipMaps.resize (mipMapCount);

		// Previous pixels
		const float *previous = &(_Pixels[0]);
		uint previousWidth = _Width;
		uint previousHeight = _Height;
		uint i;
		for (i=0; i<mipMapCount; i++)
		{
			// Destination size
			uint width = std::max (previousWidth>>1, (uint)1);
			uint height = std::max (previousHeight>>1, (uint)1);
			_MipMaps[i].resize (width*height*4);

			// Destination pointer
			float *dst = &(_MipMaps[i][0]);

			uint x, y;
			uint previousLineSize = previousWidth << 2;
			for (y=0; y<height; y++, previous += previousLineSize)
			for (x=0; x<width; x++, dst += 4, previous += 8)
			{
				dst[0] = (previous[0] + previous[4] + previous[previousLineSize] + previous[previousLineSize+4]) / 4.f;
				dst[1] = (previous[1] + previous[5] + previous[previousLineSize+1] + previous[previousLineSize+5]) / 4.f;
				dst[2] = (previous[2] + previous[6] + previous[previousLineSize+2] + previous[previousLineSize+6]) / 4.f;
				dst[3] = (previous[3] + previous[7] + previous[previousLineSize+3] + previous[previousLineSize+7]) / 4.f;
			}

			// Next bitmap
			previous = &(_MipMaps[i][0]);
			previousWidth = width;
			previousHeight = height;
		}
	}

	_MipMapsBuilded = true;
}

// ***************************************************************************

/* End of bitmap_f.cpp */
