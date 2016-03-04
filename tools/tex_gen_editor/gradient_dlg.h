#if !defined(AFX_GRADIENT_DLG_H__C386D4C1_EC3B_4DCC_9127_E8D9B9D1857A__INCLUDED_)
#define AFX_GRADIENT_DLG_H__C386D4C1_EC3B_4DCC_9127_E8D9B9D1857A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// gradient_dlg.h : header file
//

#include "gradient_wnd.h"

#define GDD_GRADIENT_CHANGED 0x1124

/////////////////////////////////////////////////////////////////////////////
// CGradientDlg dialog

class CGradientDlg : public CDialog
{
// Construction
public:
	CGradientDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGradientDlg)
	enum { IDD = IDD_GRADIENT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CGradientWnd	Gradient;

	// Window to inform
	HWND	WndToInform;

private:
	bool	_CanHide;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradientDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGradientDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADIENT_DLG_H__C386D4C1_EC3B_4DCC_9127_E8D9B9D1857A__INCLUDED_)
