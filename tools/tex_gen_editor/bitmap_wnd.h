#if !defined(AFX_BITMAP_WND_H__8A358DA0_EAE4_44A7_8B32_6FE4824D4297__INCLUDED_)
#define AFX_BITMAP_WND_H__8A358DA0_EAE4_44A7_8B32_6FE4824D4297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// bitmap_wnd.h : header file
//

#include "tex_gen/bitmap_float.h"

namespace NLTEXGEN
{
	class CFloatBitmap;
	class ITexGenOperator;
}

// ***************************************************************************
// CBitmapWnd window
// ***************************************************************************

class CBitmapWnd : public CWnd
{
// Construction
public:
	CBitmapWnd();

// Attributes
public:

private:

	NLTEXGEN::CFloatBitmap _Bitmap;
	HBITMAP		_Dib;
	HBITMAP		_Ddb;
	// todo bitmap cache  CBitmap		Bitmap;
	uint		_DibWidth;
	uint		_DibHeight;
	BITMAPINFO	_DibBitmapInfo;
	uint8		*_DibBits;

	// Zoom factor
	sint		_ZoomFactor;

	// The operator
	NLTEXGEN::ITexGenOperator *_Op;

	// Fixed size
	bool		_FixedSize;

	// Bitmap valid
	bool		_Validated;

// Operations
public:

	// Create
	BOOL Create (DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, NLTEXGEN::ITexGenOperator *op, bool fixedSize, CCreateContext *pContext = NULL);

	// Invalidate bitmap
	void invalidate()
	{
		_Validated = false;
		Invalidate();
	}

	// Update the bitmap part now
	void updateBitmap();

	// Set the operator
	void setOperator (NLTEXGEN::ITexGenOperator *op)
	{
		_Op = op;
	}

	// Get the size
	uint getWidth() const { return _Dib?_DibWidth*_ZoomFactor:0; }
	uint getHeight() const { return _Dib?_DibHeight*_ZoomFactor:0; }

	// Zoom factor
	void setZoomFactor (uint zoomFactor);
	uint getZoomFactor () const
	{
		return _ZoomFactor;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBitmapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBitmapWnd)
	afx_msg void OnPaint();
	afx_msg void OnMove(int x, int y);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAP_WND_H__8A358DA0_EAE4_44A7_8B32_6FE4824D4297__INCLUDED_)
