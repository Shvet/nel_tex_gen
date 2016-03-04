#if !defined(AFX_MY_EDIT_H__AC41D702_EE92_41EB_B2AF_40DA6DEB1E99__INCLUDED_)
#define AFX_MY_EDIT_H__AC41D702_EE92_41EB_B2AF_40DA6DEB1E99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// my_edit.h : header file
//

#define EN_RETURN 0x1111

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window

class CMyEdit : public CExtEdit
{
// Construction
public:
	CMyEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MY_EDIT_H__AC41D702_EE92_41EB_B2AF_40DA6DEB1E99__INCLUDED_)
