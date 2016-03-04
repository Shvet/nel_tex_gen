// gradient_cursor_wnd.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "gradient_cursor_wnd.h"
#include "gradient_wnd.h"
#include "tex_gen/transform_float.h"

#define CURSOR_TANGENT_SIZE 18

// ***************************************************************************
// CGradientCursorWnd
// ***************************************************************************

CGradientCursorWnd::CGradientCursorWnd()
{
	NotifyMove = false;
}

// ***************************************************************************

CGradientCursorWnd::~CGradientCursorWnd()
{
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CGradientCursorWnd, CWnd)
	//{{AFX_MSG_MAP(CGradientCursorWnd)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_MOVING()
	ON_WM_MOVE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCRBUTTONDOWN()
	ON_COMMAND(ID_GRADIENT_CURSOR_DELETE, OnGradientCursorDelete)
	ON_COMMAND(ID_GRADIENT_CURSOR_DUPLICATE, OnGradientCursorDuplicate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CGradientCursorWnd message handlers
// ***************************************************************************

void CGradientCursorWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	RECT client;
	GetClientRect (&client);
	dc.DrawEdge( &client, EDGE_RAISED, BF_RECT);
	client.left += 2;
	client.right -= 2;
	client.top += 2;
	client.bottom -= 2;

	if (_IsColor)
	{
		CBrush color(getColorRef(_Color));
		dc.FillRect(&client, &color);
	}
	else
	{
		CBrush color;
		color.CreateSysColorBrush (COLOR_3DFACE);
		dc.FillRect(&client, &color);
		CDC dc2;
		dc2.CreateCompatibleDC(&dc);
		dc2.SelectObject (_Quadratic?&Quadratic:&Linear);
		dc.MaskBlt (client.left, client.top, client.right-client.left, client.bottom-client.top, &dc2, 0, 0, _Quadratic?Quadratic:Linear, 0, 0, MAKEROP4 (SRCAND, BLACKNESS));
	}
}

// ***************************************************************************

BOOL CGradientCursorWnd::Create (DWORD dwStyle, uint x, uint y, class CWnd *parent, UINT nID, const float *color)
{
	LPCTSTR className = AfxRegisterWndClass (CS_DBLCLKS, 0, 0, 0);
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + CURSOR_WIDTH;
	rect.bottom = y + CURSOR_HEIGHT;
	NLTEXGEN::copy (_Color, color);
	_IsColor = true;
	if (CWnd::CreateEx (0, className, "", dwStyle, rect, parent, nID))
	{
		NotifyMove = true;
		return TRUE;
	}
	return FALSE;
}

// ***************************************************************************

BOOL CGradientCursorWnd::Create (DWORD dwStyle, uint x, uint y, class CWnd *parent, UINT nID, bool quadratic)
{
	LPCTSTR className = AfxRegisterWndClass (CS_DBLCLKS, 0, 0, 0);
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + CURSOR_TANGENT_SIZE;
	rect.bottom = y + CURSOR_TANGENT_SIZE;
	_Quadratic = quadratic;
	_IsColor = false;
	if (CWnd::CreateEx (0, className, "", dwStyle, rect, parent, nID))
	{
		NotifyMove = true;
		return TRUE;
	}
	return FALSE;
}

// ***************************************************************************

void CGradientCursorWnd::setColor(const float *color)
{
	NLTEXGEN::copy (_Color, color);
	Invalidate();
}

// ***************************************************************************

void CGradientCursorWnd::setQuadratic(bool quadratic)
{
	_Quadratic = quadratic;
	Invalidate();
}

// ***************************************************************************

UINT CGradientCursorWnd::OnNcHitTest(CPoint point) 
{
	CRect r;
	GetClientRect(&r);
	ClientToScreen(&r);

	if(r.PtInRect(point))
		return HTCAPTION  ;
	
	return CWnd::OnNcHitTest(point);
}

// ***************************************************************************

void CGradientCursorWnd::OnMoving(UINT fwSide, LPRECT pRect) 
{
	if (NotifyMove)
	{
		RECT minMax;
		CGradientWnd *parent = (CGradientWnd*)GetParent();
		parent->getCursorMinMax(minMax);
		RECT window;
		GetWindowRect (&window);
		parent->ScreenToClient(&window);
		parent->ScreenToClient(pRect);
		pRect->top = window.top;
		pRect->bottom = window.bottom;
		if (pRect->left < minMax.left)
		{
			pRect->right += minMax.left - pRect->left;
			pRect->left = minMax.left;
		}
		if (pRect->left > minMax.right)
		{
			pRect->right -= pRect->left - minMax.right;
			pRect->left = minMax.right;
		}
		parent->ClientToScreen(pRect);
	}
	CWnd::OnMoving(fwSide, pRect);
}

// ***************************************************************************

void CGradientCursorWnd::OnMove(int x, int y) 
{
	CWnd::OnMove(x, y);
	if (NotifyMove)
	{
		CGradientWnd *parent = (CGradientWnd*)GetParent();
		if (_IsColor)
			parent->cursorChanged();
		else
			parent->tangentChanged();
	}
}

// ***************************************************************************

void CGradientCursorWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	if (_IsColor)
	{
		theApp.ColorPicker.setInitialColor (_Color);
		theApp.ColorPicker.ShowWindow (SW_SHOW);
		theApp.ColorPicker.WndToInform = this->m_hWnd;
	}
	else
	{
		_Quadratic ^= true;
		CGradientWnd *parent = (CGradientWnd*)GetParent();
		parent->tangentChanged();
	}
	
	CWnd::OnNcLButtonDblClk(nHitTest, point);
}

// ***************************************************************************

void CGradientCursorWnd::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	CWnd::OnNcLButtonDown(nHitTest, point);
}

// ***************************************************************************

void CGradientCursorWnd::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	CWnd::OnNcMouseMove(nHitTest, point);
}

// ***************************************************************************

void CGradientCursorWnd::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	if (_IsColor)
	{
		// Check the sub menu
		CExtPopupMenuWnd * pPopupWnd = new CExtPopupMenuWnd;
		VERIFY(pPopupWnd->LoadMenu(*this, IDR_GRADIENT_CURSOR));			
		VERIFY(pPopupWnd->TrackPopupMenu(0, point.x, point.y));
	}
	
	CWnd::OnNcRButtonDown(nHitTest, point);
}

// ***************************************************************************

void CGradientCursorWnd::OnGradientCursorDelete() 
{
	CGradientWnd *parent = (CGradientWnd*)GetParent();
	parent->cursorDeleted(GetDlgCtrlID());	
}

// ***************************************************************************

void CGradientCursorWnd::OnGradientCursorDuplicate() 
{
	CGradientWnd *parent = (CGradientWnd*)GetParent();
	parent->cursorDuplicate(GetDlgCtrlID());	
}

// ***************************************************************************

BOOL CGradientCursorWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// Get parameters
	uint idWidget = (int) LOWORD(wParam);  // identifier of combo box 
	HWND hwndWidget = (HWND) lParam;       // handle to combo box 
	WORD command = HIWORD(wParam);
	
	if (command == CP_COLOR_CHANGE)
	{
		float color[4];
		theApp.ColorPicker.getColor (color);
		setColor (color);

		Invalidate();

		// Notify parent
		CGradientWnd *parent = (CGradientWnd*)GetParent();
		parent->cursorChanged();
	}
	
	return CWnd::OnCommand(wParam, lParam);
}
