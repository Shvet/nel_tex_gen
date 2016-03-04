#if !defined(AFX_COLOR_BOX_H__F0E9B8EB_4371_46C8_8140_32436256E3C8__INCLUDED_)
#define AFX_COLOR_BOX_H__F0E9B8EB_4371_46C8_8140_32436256E3C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// color_box.h : header file
//

#define COB_COLOR_CHANGED 0x1112
#define COB_COLOR_END 0x1122

// ***************************************************************************
// CColorBox window
// ***************************************************************************

class CColorBox : public CWnd
{
// Construction
public:
	CColorBox();

// Attributes
public:

// Operations
public:

	BOOL Create (DWORD dwStyle, RECT &rect, CWnd *parent, UINT nID);
	void selectColor();
	COLORREF getColor() const
	{
		return _Color;
	}
	void setColor(COLORREF color);

protected:
	COLORREF	_Color;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorBox)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ***************************************************************************

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOR_BOX_H__F0E9B8EB_4371_46C8_8140_32436256E3C8__INCLUDED_)
