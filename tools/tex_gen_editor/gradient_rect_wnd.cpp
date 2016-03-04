// gradient_rect_wnd.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "gradient_rect_wnd.h"
#include "gradient_dlg.h"
#include "tex_gen\transform_float.h"

using namespace NLTEXGEN;


// ***************************************************************************
// CGradientRectWnd
// ***************************************************************************

CGradientRectWnd::CGradientRectWnd(bool editable)
{
	_GradientDlg = NULL;
	_Editable = editable;
}

// ***************************************************************************

CGradientRectWnd::~CGradientRectWnd()
{
	if (_GradientDlg)
		delete _GradientDlg;
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CGradientRectWnd, CWnd)
	//{{AFX_MSG_MAP(CGradientRectWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CGradientRectWnd message handlers
// ***************************************************************************

void CGradientRectWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// The client rect
	RECT client;
	GetClientRect (&client);
	
	// Draw the gradient
	uint x;
	const uint limitX = client.right;
	const uint limitY = client.bottom;
	const float delta = 1.f / (float)(limitX-1);
	for (x=0; x<limitX; x++)
	{
		// Get the color
		float color[4];
		gradient (color, _Gradient, (float)x*delta);
		CPen pen(PS_SOLID, 1, getColorRef(color));
		dc.SelectObject(&pen);

		// Draw the line
		dc.MoveTo(x, 0);
		dc.LineTo(x, limitY);
	}
	
	// Do not call CWnd::OnPaint() for painting messages
}

// ***************************************************************************

BOOL CGradientRectWnd::Create (DWORD dwStyle, RECT &rect, CWnd *parent, UINT nID)
{
	LPCTSTR className = AfxRegisterWndClass (CS_DBLCLKS, 0, 0, 0);
	return CWnd::Create (className, "", dwStyle, rect, parent, nID);
}

// ***************************************************************************

void CGradientRectWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (_Editable)
	{
		if (!_GradientDlg)
		{
			_GradientDlg = new CGradientDlg;
			_GradientDlg->Create (IDD_GRADIENT, theApp.m_pMainWnd);
			_GradientDlg->WndToInform = *this;
			_GradientDlg->Gradient.setGradient (getGradient());
		}
		_GradientDlg->ShowWindow (SW_SHOW);
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

// ***************************************************************************

void CGradientRectWnd::setGradient (const std::vector<NLTEXGEN::CGradientColor> &gradient)
{
	_Gradient = gradient;
	Invalidate ();
	if (_GradientDlg)
	{
		_GradientDlg->Gradient.setGradient (gradient);
	}
}

// ***************************************************************************

BOOL CGradientRectWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// Get parameters
	uint idWidget = (int) LOWORD(wParam);  // identifier of combo box 
	HWND hwndWidget = (HWND) lParam;       // handle to combo box 
	
	if (HIWORD(wParam) == GDD_GRADIENT_CHANGED)
	{
		setGradient (_GradientDlg->Gradient.getGradient());

		// Notify parent
		CWnd *parent = GetParent();
		if (parent)
			parent->PostMessage(WM_COMMAND, (GDR_GRADIENT_CHANGED<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	}

	return CWnd::OnCommand(wParam, lParam);
}
