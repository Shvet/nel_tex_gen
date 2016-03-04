// gradient_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "gradient_dlg.h"

// ***************************************************************************
// CGradientDlg dialog
// ***************************************************************************

CGradientDlg::CGradientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGradientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGradientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	_CanHide = true;
}

// ***************************************************************************

void CGradientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGradientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CGradientDlg, CDialog)
	//{{AFX_MSG_MAP(CGradientDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CGradientDlg message handlers
// ***************************************************************************

BOOL CGradientDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	RECT rect;
	GetClientRect (&rect);

	nlverify (Gradient.Create (WS_CHILD|WS_VISIBLE, rect, this, 0));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ***************************************************************************

BOOL CGradientDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// Get parameters
	uint idWidget = (int) LOWORD(wParam);  // identifier of combo box 
	HWND hwndWidget = (HWND) lParam;       // handle to combo box 
	
	if (HIWORD(wParam) == CGW_GRADIENT_CHANGED)
	{
		// Notify parent
		if (WndToInform)
			::PostMessage(WndToInform, WM_COMMAND, (GDD_GRADIENT_CHANGED<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	}
	
	return CDialog::OnCommand(wParam, lParam);
}

BOOL CGradientDlg::PreTranslateMessage(MSG* pMsg) 
{
	theApp.m_pMainWnd->PreTranslateMessage(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}
