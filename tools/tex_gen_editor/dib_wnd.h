#if !defined(AFX_DIB_WND_H__06433841_EFC9_4B62_8827_3AD7F4398426__INCLUDED_)
#define AFX_DIB_WND_H__06433841_EFC9_4B62_8827_3AD7F4398426__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dib_wnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDibWnd window

namespace NLTEXGEN
{
	class CFloatBitmap;
}

class CDibWnd
{
// Construction
public:
	CDibWnd();

	enum TDisplayMode
	{
		Normal,
		Alpha,
		AlphaPremultiplied,
	};

// Attributes
public:

// Operations
public:

	// Acces pixels
	const uint8	*getConstPixels();
	uint8	*getPixels();
	void	setPixels (const NLTEXGEN::CFloatBitmap &bitmap, TDisplayMode mode);

	// Set the size
	void setSize (uint width, uint height);

	// Get bitmap
	void draw (CDC *pDC, sint x, sint y, sint zoomFactor);

	// Get size
	uint	getWidth() const
	{
		return _Width;
	}
	uint	getHeight() const
	{
		return _Height;
	}

private:

	// Bitmap invalidated
	bool	_Invalidated;

	// Size
	uint			_Width;
	uint			_Height;

	// DIB info
	HBITMAP			_Dib;
	BITMAPINFO		_DibBitmapInfo;
	uint8			*_DibBits;

	// DDB info
	HBITMAP			_Ddb;

	// Internal methods
	void	updateDIB ();
	
// Overrides

// Implementation
public:
	virtual ~CDibWnd();

	// Generated message map functions
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIB_WND_H__06433841_EFC9_4B62_8827_3AD7F4398426__INCLUDED_)
