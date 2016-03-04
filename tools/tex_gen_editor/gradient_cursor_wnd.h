#if !defined(AFX_GRADIENT_CURSOR_WND_H__DC354872_BFAB_4ABF_A496_20FC2868986F__INCLUDED_)
#define AFX_GRADIENT_CURSOR_WND_H__DC354872_BFAB_4ABF_A496_20FC2868986F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// gradient_cursor_wnd.h : header file
//

// ***************************************************************************
// CGradientCursorWnd window
// ***************************************************************************

class CGradientCursorWnd : public CWnd
{
// Construction
public:
	CGradientCursorWnd();

// Attributes
public:

	bool		NotifyMove;

private:

	float		_Color[4];
	bool		_Quadratic;
	bool		_IsColor;

// Operations
public:

	BOOL Create (DWORD dwStyle, uint x, uint y, class CWnd *parent, UINT nID, const float *color);
	BOOL Create (DWORD dwStyle, uint x, uint y, class CWnd *parent, UINT nID, bool quadratic);
	void setColor(const float *);
	const float *getColor() const
	{
		return _Color;
	}
	void setQuadratic(bool quadratic);
	bool getQuadratic() const
	{
		return _Quadratic;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradientCursorWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGradientCursorWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGradientCursorWnd)
	afx_msg void OnPaint();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnGradientCursorDelete();
	afx_msg void OnGradientCursorDuplicate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADIENT_CURSOR_WND_H__DC354872_BFAB_4ABF_A496_20FC2868986F__INCLUDED_)
