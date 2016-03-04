// color_picker_color_wnd.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "color_picker_color_wnd.h"

// ***************************************************************************
// CColorPickerColorWnd
// ***************************************************************************

CColorPickerColorWnd::CColorPickerColorWnd()
{
}

// ***************************************************************************

CColorPickerColorWnd::~CColorPickerColorWnd()
{
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CColorPickerColorWnd, CWnd)
	//{{AFX_MSG_MAP(CColorPickerColorWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CColorPickerColorWnd message handlers
// ***************************************************************************

// ***************************************************************************

BOOL CColorPickerColorWnd::Create (DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	LPCTSTR className = AfxRegisterWndClass (0, 0, 0, 0);
	return CWnd::Create(className, "", dwStyle, rect, pParentWnd, nID);
}

// ***************************************************************************

void CColorPickerColorWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	RECT rect;
	GetClientRect (&rect);

	CBrush brush;
	brush.CreateSolidBrush (RGB(0,0,0));
	dc.FrameRect (&rect, &brush);
	rect.left++;
	rect.right--;
	rect.top++;
	rect.bottom--;
	dc.FillSolidRect(&rect, _Color);
}

// ***************************************************************************

void CColorPickerColorWnd::setColor (COLORREF color)
{
	_Color = color;
	Invalidate();
}

// ***************************************************************************

void CColorPickerColorWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd *parent = GetParent();
	if (parent)
		parent->PostMessage(WM_COMMAND, (CW_LBUTTON<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	
	CWnd::OnLButtonDown(nFlags, point);
}

// ***************************************************************************

void CColorPickerColorWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CWnd *parent = GetParent();
	if (parent)
		parent->PostMessage(WM_COMMAND, (CW_RBUTTON<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	
	CWnd::OnRButtonDown(nFlags, point);
}

// ***************************************************************************

void CColorPickerColorWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
}
