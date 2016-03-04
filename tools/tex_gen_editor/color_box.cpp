// color_box.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "color_box.h"

// ***************************************************************************
// CColorBox
// ***************************************************************************

CColorBox::CColorBox()
{
	_Color = RGB(0,0,0);
}

// ***************************************************************************

CColorBox::~CColorBox()
{
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CColorBox, CWnd)
	//{{AFX_MSG_MAP(CColorBox)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CColorBox message handlers
// ***************************************************************************

void CColorBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Create a brush
	CBrush brush(_Color);

	// Get the client rect
	RECT rect;
	GetClientRect(&rect);

	// Draw with a fill color
	dc.FillRect( &rect, &brush);
	
	// If selected, draw a selection square
	if (GetFocus() == this)
		dc.DrawFocusRect(&rect);
}

// ***************************************************************************

void CColorBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	selectColor();
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

// ***************************************************************************

void CColorBox::selectColor()
{
	float color[4];
	::setColor(color, _Color);
	theApp.ColorPicker.setInitialColor (color);
	theApp.ColorPicker.ShowWindow (SW_SHOW);
	theApp.ColorPicker.WndToInform = this->m_hWnd;
}

// ***************************************************************************

BOOL CColorBox::Create (DWORD dwStyle, RECT &rect, CWnd *parent, UINT nID)
{
	LPCTSTR className = AfxRegisterWndClass (CS_DBLCLKS, 0, 0, 0);
	return CWnd::Create (className, "", dwStyle, rect, parent, nID);
}

// ***************************************************************************

void CColorBox::setColor(COLORREF color)
{
	_Color = color;
	Invalidate();
}

// ***************************************************************************

void CColorBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Get focus
	SetFocus();
	selectColor();
		
	CWnd::OnLButtonDown(nFlags, point);
}

// ***************************************************************************

void CColorBox::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	Invalidate();
}

// ***************************************************************************

void CColorBox::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	Invalidate();
}

// ***************************************************************************

BOOL CColorBox::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_KEYDOWN) && (int) pMsg->wParam == VK_RETURN)
	{
		selectColor();
		return TRUE;
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

// ***************************************************************************

BOOL CColorBox::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// Get parameters
	uint idWidget = (int) LOWORD(wParam);  // identifier of combo box 
	HWND hwndWidget = (HWND) lParam;       // handle to combo box 
	WORD command = HIWORD(wParam);
	
	if (command == CP_COLOR_CHANGE)
	{
		float color[4];
		theApp.ColorPicker.getColor (color);
		setColor (::getColorRef(color));

		Invalidate();

		// Notify parent
		CWnd *parent = GetParent();
		if (parent)
			parent->PostMessage(WM_COMMAND, (COB_COLOR_CHANGED<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	}
	else if (command == CP_COLOR_END)
	{
		// Notify parent
		CWnd *parent = GetParent();
		if (parent)
			parent->PostMessage(WM_COMMAND, (COB_COLOR_END<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	}
	
	return CWnd::OnCommand(wParam, lParam);
}

// ***************************************************************************
