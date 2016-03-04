// bitmap_wnd.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "nel_tex_gen_view.h"
#include "nel_tex_gen_doc.h"
#include "bitmap_wnd.h"
#include "tex_gen/tex_gen_op.h"
#include "tex_gen/bitmap_float.h"
#include "tex_gen/op_layer.h"

using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************
// CBitmapWnd
// ***************************************************************************

CBitmapWnd::CBitmapWnd()
{
	_Dib = NULL;
	_Ddb = NULL;
	_ZoomFactor = 1;
	_Op = NULL;
	_Validated = false;
}

// ***************************************************************************

CBitmapWnd::~CBitmapWnd()
{
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CBitmapWnd, CWnd)
	//{{AFX_MSG_MAP(CBitmapWnd)
	ON_WM_PAINT()
	ON_WM_MOVE()
	ON_WM_FONTCHANGE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CBitmapWnd message handlers

void CBitmapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Invalidated ?
	updateBitmap();

	// Dib here ?
	if (_Ddb)
	{
		::CBitmap *temp = ::CBitmap::FromHandle(_Ddb);
		CDC dc2;
		dc2.CreateCompatibleDC(&dc);
		dc2.SelectObject (temp);
		nlverify (dc.BitBlt(0, 0, _DibWidth, _DibHeight, &dc2, 0, 0, SRCCOPY));
	}
}

// ***************************************************************************

BOOL CBitmapWnd::Create (DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, 
						 NLTEXGEN::ITexGenOperator *op, bool fixedSize, CCreateContext *pContext)
{
	_Op = op;
	_FixedSize = fixedSize;
	if (_FixedSize)
	{
		_DibWidth = rect.right - rect.left;
		_DibHeight = rect.bottom - rect.top;
	}
	LPCTSTR className = AfxRegisterWndClass (0, 0, (HBRUSH)COLOR_WINDOW, 0);
	return CWnd::Create(className, "", dwStyle, rect, pParentWnd, nID, pContext);
}

// ***************************************************************************

void CBitmapWnd::OnMove(int x, int y) 
{
	CWnd::OnMove(x, y);
}

// ***************************************************************************

void CBitmapWnd::setZoomFactor (uint zoomFactor)
{
	_ZoomFactor = zoomFactor;
	SetWindowPos(NULL, 0, 0, _DibWidth*_ZoomFactor, _DibHeight*_ZoomFactor, SWP_NOZORDER|SWP_NOMOVE);
}

// ***************************************************************************

BOOL CBitmapWnd::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE; // CWnd::OnEraseBkgnd(pDC);
}

// ***************************************************************************

void CBitmapWnd::updateBitmap()
{
	if (!_Validated)
	{
		CDC *dc = GetDesktopWindow()->GetDC();

		// Eval the bitmap with cache
		CRenderParameter renderParameters (true, Draft, false);
		_Op->eval (_Bitmap, renderParameters);
		CNelTexGenDoc* pDoc = ((CNelTexGenView*)GetParent ()->GetParent ())->GetDocument ();
		IOpLayer *layer = dynamic_cast<IOpLayer*>(_Op);
		bool showAlphaLayer = false;
		if (!pDoc->getLayerResult() && layer)
		{
			_Bitmap.ghostCopy (&(layer->getCachedLayer()));
			showAlphaLayer = layer->outputMask ();
		}

		// Dib section exist ?
		if (_Dib)
		{
			// Not the good size ?
			if (!_FixedSize && ((_DibWidth != _Bitmap.getWidth()) || (_DibHeight != _Bitmap.getHeight())))
			{
				// Erase the dib
				nlverify(DeleteObject(_Dib));

				// todo bitmap cache nlverify(DeleteObject(Bitmap));
				_Dib = NULL;

				// Resize the window
				SetWindowPos(NULL, 0, 0, _Bitmap.getWidth()*_ZoomFactor, _Bitmap.getHeight()*_ZoomFactor, SWP_NOZORDER|SWP_NOMOVE);
			}
		}

		if (!_FixedSize)
		{
			// Get the bitmap size
			_DibWidth = _Bitmap.getWidth();
			_DibHeight = _Bitmap.getHeight();
		}

		// Bitmap NULL
		if ((_DibWidth==0) || (_DibHeight==0))
		{
			if (_Dib)
			{
				nlverify(DeleteObject(_Dib));
				_Dib = NULL;
			}
		}
		else
		{
			// _Dib section doesn't exist ?
			if (!_Dib)
			{
				_DibBitmapInfo.bmiHeader.biSize	= sizeof(BITMAPINFO);
				_DibBitmapInfo.bmiHeader.biWidth	= _DibWidth;
				_DibBitmapInfo.bmiHeader.biHeight	= _DibHeight;
				_DibBitmapInfo.bmiHeader.biPlanes	= 1;
				_DibBitmapInfo.bmiHeader.biBitCount = 32;
				_DibBitmapInfo.bmiHeader.biCompression  = BI_RGB;
				_DibBitmapInfo.bmiHeader.biSizeImage	= 0;
				_DibBitmapInfo.bmiHeader.biXPelsPerMeter	= 0;
				_DibBitmapInfo.bmiHeader.biYPelsPerMeter	= 0;
				_DibBitmapInfo.bmiHeader.biClrUsed	= 0;
				_DibBitmapInfo.bmiHeader.biClrImportant	= 0;
				nlverify(_Dib = CreateDIBSection(*dc, &_DibBitmapInfo, DIB_RGB_COLORS, (void**)&_DibBits, NULL, 0));
			}
		}

		// Fill the dib
		if (!_FixedSize)
		{
			nlassert ((_Bitmap.getWidth() == _DibWidth) && (_Bitmap.getHeight() == _DibHeight));
			const float *pixels = _Bitmap.getPixels();
			if (pixels)
			{
				const uint height = _DibHeight;
				uint y;
				for (y=0; y<height; y++)
				{
					const uint width = _DibWidth*4 + y*_DibWidth*4;
					uint i = y*_DibWidth*4;
					uint j = (height-y-1)*_DibWidth*4;
					for (;i<width; i+=4, j+=4)
					{
						int r;
						int g;
						int b;
						if (showAlphaLayer)
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
						_DibBits[j+0] = b;
						_DibBits[j+1] = g;
						_DibBits[j+2] = r;
					}
				}
			}
		}
		else
		{
			const float *pixels = _Bitmap.getPixels();
			if (pixels)
			{
				const bitmapWidth = _Bitmap.getWidth();
				const bitmapHeight = _Bitmap.getHeight();
				const uint heightDest = _DibHeight;
				const uint deltaYSrc = (bitmapHeight<<8)/_DibHeight;
				uint yDest;
				uint ySrc = deltaYSrc>>1;
				for (yDest=0; yDest<heightDest; ySrc+=deltaYSrc, yDest++)
				{
					const uint yFinal = ySrc>>8;
					const uint widthDest = _DibWidth*4 + (heightDest-yDest-1)*_DibWidth*4;
					const uint deltaXSrc = (bitmapWidth<<8)/_DibWidth;
					uint xSrc = ((yFinal*bitmapWidth)<<8) + (deltaXSrc>>1);
					uint xDest = (heightDest-yDest-1)*_DibWidth*4;
					for (;xDest<widthDest; xSrc+=deltaXSrc, xDest+=4)
					{
						uint xFinal = (xSrc>>8)*4;
						int r;
						int g;
						int b;
						if (showAlphaLayer)
						{
							const float alpha = pixels[xFinal+3];
							const float alpha_minus = 1.f - alpha;
							const float background = (((yDest^(xDest>>2))&8)!=0)?alpha_minus*0.75f:alpha_minus*0.25f;
							r = (int)((pixels[xFinal+0]*alpha + background)*255.f);
							g = (int)((pixels[xFinal+1]*alpha + background)*255.f);
							b = (int)((pixels[xFinal+2]*alpha + background)*255.f);
						}
						else
						{
							r = (int)(pixels[xFinal+0] * 255.f);
							g = (int)(pixels[xFinal+1] * 255.f);
							b = (int)(pixels[xFinal+2] * 255.f);
						}
						clamp (r, 0, 255);
						clamp (g, 0, 255);
						clamp (b, 0, 255);
						_DibBits[xDest+0] = b;
						_DibBits[xDest+1] = g;
						_DibBits[xDest+2] = r;
					}
				}
			}
		}

		// Convert in DDB
		if (_Ddb)
		{
			nlverify(DeleteObject (_Ddb));
			_Ddb = NULL;
		}
		nlverify (_Ddb = CreateDIBitmap(*dc, &(_DibBitmapInfo.bmiHeader), CBM_INIT, 
			_DibBits, &_DibBitmapInfo, DIB_RGB_COLORS));

		GetDesktopWindow()->ReleaseDC(dc);
		_Validated = true;
	}
}

// ***************************************************************************

