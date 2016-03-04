// my_edit.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "my_edit.h"

// ***************************************************************************
// CMyEdit
// ***************************************************************************

CMyEdit::CMyEdit()
{
}

// ***************************************************************************

CMyEdit::~CMyEdit()
{
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CMyEdit, CExtEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CMyEdit message handlers
// ***************************************************************************

void CMyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Return ?
	if (nChar == VK_RETURN)
	{
		// Notify parent
		CWnd *parent = GetParent();
		if (parent)
			parent->PostMessage(WM_COMMAND, (EN_RETURN<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	}
	
	CExtEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

// ***************************************************************************

BOOL CMyEdit::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_KEYDOWN) && (int) pMsg->wParam == VK_RETURN)
	{
		// Notify parent
		CWnd *parent = GetParent();
		if (parent)
			parent->PostMessage(WM_COMMAND, (EN_RETURN<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
		return TRUE;
	}
	
	return CExtEdit::PreTranslateMessage(pMsg);
}

// ***************************************************************************

