#if !defined(AFX_OP_PROPERTY_DLG_H__FE0FB30B_7E46_4EA4_8C14_4C12675BD77C__INCLUDED_)
#define AFX_OP_PROPERTY_DLG_H__FE0FB30B_7E46_4EA4_8C14_4C12675BD77C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// op_property_dlg.h : header file
//

#include "exdlgbase.h"

namespace NLTEXGEN
{
	class ITexGenOperator;
}

/////////////////////////////////////////////////////////////////////////////
// COpPropertyDlg dialog

class COpPropertyDlg : public CDialog
{
// Construction
public:
	COpPropertyDlg(CWnd* pParent = NULL);   // standard constructor
	~COpPropertyDlg();						// standard destructor

	enum
	{
		FirstID = 3
	};

// Dialog Data
	//{{AFX_DATA(COpPropertyDlg)
	enum { IDD = IDD_OP_PROPERTY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// Create the dialog
	BOOL	Create (class COperatorWin *opWindow, const NLTEXGEN::ITexGenOperator *op);

	// Add a label window
	void	addLabel(RECT &client, const std::string &str);

	// Category helpers
	void	beginCategory(RECT &client);
	void	endCategory(RECT &client, const char *name);

	// Update data of the dialog
	void	updateData ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpPropertyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	COperatorWin						*_OpWindow;
	const NLTEXGEN::ITexGenOperator		*_Op;
	bool								_Initialized;
	
	// Top of the last category
	int	_CategoryTop;

	std::vector<CWnd*>	Widgets;
	std::vector<CWnd*>	Others;

	// Generated message map functions
	//{{AFX_MSG(COpPropertyDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OP_PROPERTY_DLG_H__FE0FB30B_7E46_4EA4_8C14_4C12675BD77C__INCLUDED_)
