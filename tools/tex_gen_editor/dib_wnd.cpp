// dib_wnd.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "dib_wnd.h"
#include "tex_gen/bitmap_float.h"

using namespace NLMISC;

// ***************************************************************************
// CDibWnd
// ***************************************************************************

CDibWnd::CDibWnd()
{
	_Invalidated = true;
	_Width = 256;
	_Height = 256;
	_DibBits = NULL;
	_Dib = NULL;
	_Ddb = NULL;
	_DibBitmapInfo.bmiHeader.biWidth = 0;
	_DibBitmapInfo.bmiHeader.biHeight = 0;
}

// ***************************************************************************

CDibWnd::~CDibWnd()
{
	if (_Dib)
		nlverify(DeleteObject (_Dib));
	if (_Ddb)
		nlverify(DeleteObject (_Ddb));
}

// ***************************************************************************
// CDibWnd message handlers
// ***************************************************************************

void CDibWnd::updateDIB ()
{
	// No dib or wrong size ?
	if ((_DibBits == NULL) || ((uint)_DibBitmapInfo.bmiHeader.biWidth	!= _Width) || ((uint)_DibBitmapInfo.bmiHeader.biHeight != _Height))
	{
		// Create the dib
		_DibBitmapInfo.bmiHeader.biSize	= sizeof(BITMAPINFO);
		_DibBitmapInfo.bmiHeader.biWidth	= _Width;
		_DibBitmapInfo.bmiHeader.biHeight	= _Height;
		_DibBitmapInfo.bmiHeader.biPlanes	= 1;
		_DibBitmapInfo.bmiHeader.biBitCount = 32;
		_DibBitmapInfo.bmiHeader.biCompression  = BI_RGB;
		_DibBitmapInfo.bmiHeader.biSizeImage	= 0;
		_DibBitmapInfo.bmiHeader.biXPelsPerMeter	= 0;
		_DibBitmapInfo.bmiHeader.biYPelsPerMeter	= 0;
		_DibBitmapInfo.bmiHeader.biClrUsed	= 0;
		_DibBitmapInfo.bmiHeader.biClrImportant	= 0;
		HWND desktop = ::GetDesktopWindow();
		HDC dc = ::GetDC(desktop);
		nlverify(_Dib = CreateDIBSection(dc, &_DibBitmapInfo, DIB_RGB_COLORS, (void**)&_DibBits, NULL, 0));
		::ReleaseDC(desktop, dc);
		_Invalidated = true;
	}
}

// ***************************************************************************

uint8	*CDibWnd::getPixels()
{
	updateDIB ();
	_Invalidated = true;
	return _DibBits;
}

// ***************************************************************************

const uint8	*CDibWnd::getConstPixels()
{
	updateDIB ();
	return _DibBits;
}

// ***************************************************************************

void CDibWnd::draw (CDC *pDC, sint x, sint y, sint zoomFactor)
{
	updateDIB ();

	// Build the bitmap ?
	if (_Invalidated)
	{
		if (_Ddb)
		{
			nlverify(DeleteObject (_Ddb));
			_Ddb = NULL;
		}

		nlverify (_Ddb = CreateDIBitmap(*pDC, &(_DibBitmapInfo.bmiHeader), CBM_INIT, 
			_DibBits, &_DibBitmapInfo, DIB_RGB_COLORS));
		_Invalidated = false;
	}

	// Draw the bitmap
	::CBitmap *temp = ::CBitmap::FromHandle(_Ddb);
	CDC dc2;
	dc2.CreateCompatibleDC(pDC);
	dc2.SelectObject (temp);

	if (zoomFactor == 0)
	{
		nlverify (pDC->BitBlt(x, y, _Width, _Height, &dc2, 0, 0, SRCCOPY));
	}
	else
	{
		int bmWidth = (zoomFactor>0)?_Width<<zoomFactor:_Width>>-zoomFactor;
		int bmHeight = (zoomFactor>0)?_Height<<zoomFactor:_Height>>-zoomFactor;
		nlverify (pDC->StretchBlt(x, y, bmWidth, bmHeight, &dc2, 0, 0, _Width, _Height, SRCCOPY));
	}
}

// ***************************************************************************

void CDibWnd::setSize (uint width, uint height)
{
	_Width = width;
	_Height = height;
	_Invalidated = true;
}

// ***************************************************************************

void CDibWnd::setPixels (const NLTEXGEN::CFloatBitmap &bitmap, TDisplayMode mode)
{
	const float *pixels = bitmap.getPixels();
	if (pixels)
	{
		setSize (bitmap.getWidth(), bitmap.getHeight());
		uint8 *dst = getPixels();
		const uint height = _Height;
		uint y;
		for (y=0; y<height; y++)
		{
			const uint width = _Width*4 + y*_Width*4;
			uint i = y*_Width*4;
			uint j = (height-y-1)*_Width*4;
			for (;i<width; i+=4, j+=4)
			{
				if (mode == Alpha)
				{
					int r = (int)(pixels[i+0] * 255.f);
					clamp (r, 0, 255);
					dst[j+0] = r;
					dst[j+1] = r;
					dst[j+2] = r;
				}
				else
				{
					int r;
					int g;
					int b;
					if (mode == AlphaPremultiplied)
					{
						const float alpha = pixels[i+3];
						const float alpha_minus = 1.f - alpha;
						const float background = ((((i>>2)^y)&8)!=0)?alpha_minus*0.75f:alpha_minus*0.25f;
						r = (int)((pixels[i+0]*alpha + background)*255.f);
						g = (int)((pixels[i+1]*alpha + background)*255.f);
						b = (int)((pixels[i+2]*alpha + background)*255.f);
					}
					else
					{
						r = (int)(pixels[i+0] * 255.f);
						g = (int)(pixels[i+1] * 255.f);
						b = (int)(pixels[i+2] * 255.f);
					}
					clamp (r, 0, 255);
					clamp (g, 0, 255);
					clamp (b, 0, 255);
					dst[j+0] = b;
					dst[j+1] = g;
					dst[j+2] = r;
				}
			}
		}
		_Invalidated = true;
	}
}

// ***************************************************************************
