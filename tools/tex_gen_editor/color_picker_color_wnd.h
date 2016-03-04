#if !defined(AFX_COLOR_PICKER_COLOR_WND_H__92ED06F3_C70E_4D78_9AC5_1E25B4ECF3E9__INCLUDED_)
#define AFX_COLOR_PICKER_COLOR_WND_H__92ED06F3_C70E_4D78_9AC5_1E25B4ECF3E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// color_picker_color_wnd.h : header file
//

#define CW_LBUTTON 0x1119
#define CW_RBUTTON 0x1120

/////////////////////////////////////////////////////////////////////////////
// CColorPickerColorWnd window

class CColorPickerColorWnd : public CWnd
{
// Construction
public:
	CColorPickerColorWnd();

// Attributes
public:

	BOOL Create (DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);
	void setColor (COLORREF color);
	COLORREF getColor () const
	{
		return _Color;
	}

// Operations
public:

	COLORREF _Color;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorPickerColorWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorPickerColorWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorPickerColorWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOR_PICKER_COLOR_WND_H__92ED06F3_C70E_4D78_9AC5_1E25B4ECF3E9__INCLUDED_)
