// TexGenThumbnailDoc.cpp : implementation of the CTexGenThumbnailDoc class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
//#include "TexGenThumbnail.h"

#include "TexGenThumbnailDoc.h"

#include "tex_gen/basics.h"
#include "tex_gen/bitmap_float.h"
#include "tex_gen/tex_gen_op.h"
#include "tex_gen_thumbnailapp.h"

#include "tex_gen/tex_gen_document.h"

using namespace NLMISC;
using namespace NLTEXGEN;


/////////////////////////////////////////////////////////////////////////////
// CTexGenThumbnailDoc

IMPLEMENT_DYNCREATE(CTexGenThumbnailDoc, CDocument)

BEGIN_MESSAGE_MAP(CTexGenThumbnailDoc, CDocument)
	//{{AFX_MSG_MAP(CTexGenThumbnailDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTexGenThumbnailDoc construction/destruction

CTexGenThumbnailDoc::CTexGenThumbnailDoc()
{
	_Dib = NULL;
	_DibBits = NULL;
	// _Ddb = NULL;
}

CTexGenThumbnailDoc::~CTexGenThumbnailDoc()
{
	if (_Dib)
		nlverify(DeleteObject (_Dib));
	/* if (_Ddb)
		nlverify(DeleteObject (_Ddb));*/
}

BOOL CTexGenThumbnailDoc::OnNewDocument()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTexGenThumbnailDoc serialization

void CTexGenThumbnailDoc::Serialize(CArchive& ar)
{
	// TODO: Serialize your document here
	if (ar.IsStoring())
	{
	//	ar << m_sizeDoc;
	}
	else
	{
	//	ar >> m_sizeDoc;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTexGenThumbnailDoc diagnostics

#ifdef _DEBUG
void CTexGenThumbnailDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTexGenThumbnailDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTexGenThumbnailDoc commands

BOOL CTexGenThumbnailDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CIFile file;
	if (file.open (lpszPathName))
	{
		try
		{
			NLTEXGEN::CTexGenDocument	doc;
			file.serial(doc);

			CFloatBitmap bitmap;
			CRenderParameter renderParameters (false, false, false);
			doc.Operators[0]->eval (bitmap, renderParameters);
			if ((bitmap.getWidth() != 0) && (bitmap.getHeight() != 0))
			{
				bitmap.resample (m_sizeDoc.cx, m_sizeDoc.cy);

				_DibBitmapInfo.bmiHeader.biSize	= sizeof(BITMAPINFO);
				_DibBitmapInfo.bmiHeader.biWidth	= m_sizeDoc.cx;
				_DibBitmapInfo.bmiHeader.biHeight	= m_sizeDoc.cy;
				_DibBitmapInfo.bmiHeader.biPlanes	= 1;
				_DibBitmapInfo.bmiHeader.biBitCount = 32;
				_DibBitmapInfo.bmiHeader.biCompression  = BI_RGB;
				_DibBitmapInfo.bmiHeader.biSizeImage	= 0;
				_DibBitmapInfo.bmiHeader.biXPelsPerMeter	= 0;
				_DibBitmapInfo.bmiHeader.biYPelsPerMeter	= 0;
				_DibBitmapInfo.bmiHeader.biClrUsed	= 0;
				_DibBitmapInfo.bmiHeader.biClrImportant	= 0;
				HWND desktop = ::GetDesktopWindow();
				HDC dc = ::GetDC (desktop);
				nlverify(_Dib = CreateDIBSection(dc, &_DibBitmapInfo, DIB_RGB_COLORS, (void**)&_DibBits, NULL, 0));

				const float *pixels = bitmap.getPixels();
				if (pixels)
				{
					uint8 *dst = _DibBits;
					const uint height = m_sizeDoc.cy;
					uint y;
					for (y=0; y<height; y++)
					{
						const uint width = m_sizeDoc.cx*4 + y*m_sizeDoc.cx*4;
						uint i = y*m_sizeDoc.cx*4;
						uint j = (height-y-1)*m_sizeDoc.cx*4;
						for (;i<width; i+=4, j+=4)
						{
							/*if (alpha)
							{
								int r = (int)(pixels[i+0] * 255.f);
								clamp (r, 0, 255);
								dst[j+0] = r;
								dst[j+1] = r;
								dst[j+2] = r;
							}
							else*/
							{
								int r = (int)(pixels[i+0] * 255.f);
								int g = (int)(pixels[i+1] * 255.f);
								int b = (int)(pixels[i+2] * 255.f);
								clamp (r, 0, 255);
								clamp (g, 0, 255);
								clamp (b, 0, 255);
								dst[j+0] = b;
								dst[j+1] = g;
								dst[j+2] = r;
							}
						}
					}
				}

				::ReleaseDC (desktop, dc);
				
				return TRUE;
			}
		}
		catch (Exception &)
		{
		}
	}
	
	return FALSE;
}

void CTexGenThumbnailDoc::DeleteContents()
{
	// TODO: Delete the contents of your documents here
	m_sizeDoc = CSize(100,100);
	CDocument::DeleteContents();
}

// copied from CTexGenThumbnailView, replace GetDocument() with this
/*void CTexGenThumbnailDoc::OnDraw(CDC* pDC)
{
	CTexGenThumbnailDoc* pDoc = this; //GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: Draw your document here
	pDC->Ellipse(0,0,100,-100);

	// Draw the bitmap
	if (_Ddb)
	{
		::CBitmap *temp = ::CBitmap::FromHandle(_Ddb);
		CDC dc2;
		dc2.CreateCompatibleDC(pDC);
		dc2.SelectObject (temp);
		nlverify (pDC->BitBlt(0, 0, m_sizeDoc.cx, m_sizeDoc.cy, &dc2, 0, 0, SRCCOPY));
	}
}*/

HBITMAP CTexGenThumbnailDoc::CreateBitmap ()
{
	// Create the bitmap
	HWND desktop = ::GetDesktopWindow();
	HDC dc = ::GetDC (desktop);
	HBITMAP hbmp;
	nlverify (hbmp = CreateDIBitmap(dc, &(_DibBitmapInfo.bmiHeader), CBM_INIT, 
		_DibBits, &_DibBitmapInfo, DIB_RGB_COLORS));
	::ReleaseDC (desktop, dc);
	return hbmp;
}