#if !defined(AFX_PORT_H__237E4B69_C55C_4B8E_AF6D_EEB1635F9513__INCLUDED_)
#define AFX_PORT_H__237E4B69_C55C_4B8E_AF6D_EEB1635F9513__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// port.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPortWnd window

class CPortWnd : public CWnd
{
// Construction
public:
	CPortWnd();

// Attributes
public:

	BOOL Create(const char *name, DWORD style, RECT &rect, CWnd *parent, UINT nID, bool output, uint input);

	bool isOutput() const { return _Output; }

	uint getInput() const { return _Input; }

	void getBezierPos (POINT &pt) const;

private:

	CStatic	Label;
	bool	_Output;
	bool	_Drag;
	uint	_Input;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPortWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPortWnd)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORT_H__237E4B69_C55C_4B8E_AF6D_EEB1635F9513__INCLUDED_)
