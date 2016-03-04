#if !defined(AFX_GRADIENT_RECT_WND_H__FE011E92_24E8_4CA8_997C_5663B2F9AB9B__INCLUDED_)
#define AFX_GRADIENT_RECT_WND_H__FE011E92_24E8_4CA8_997C_5663B2F9AB9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// gradient_rect_wnd.h : header file
//

#define GDR_GRADIENT_CHANGED 0x1123

// ***************************************************************************
// CGradientRectWnd window
// ***************************************************************************

class CGradientRectWnd : public CWnd
{
// Construction
public:
	CGradientRectWnd(bool editable);

// Attributes
public:

	BOOL Create (DWORD dwStyle, RECT &rect, class CWnd *parent, UINT nID);
	void setGradient (const std::vector<NLTEXGEN::CGradientColor> &gradient);
	const std::vector<NLTEXGEN::CGradientColor>	&getGradient () const
	{
		return _Gradient;
	}
	std::vector<NLTEXGEN::CGradientColor>	&getGradient ()
	{
		Invalidate ();
		return _Gradient;
	}

	//	The gradient dialog
	class CGradientDlg	*_GradientDlg;

private:

	bool _Editable;
	std::vector<NLTEXGEN::CGradientColor>	_Gradient;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradientRectWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGradientRectWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGradientRectWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADIENT_RECT_WND_H__FE011E92_24E8_4CA8_997C_5663B2F9AB9B__INCLUDED_)
