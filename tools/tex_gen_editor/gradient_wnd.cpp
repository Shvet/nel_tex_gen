// gradient_wnd.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "gradient_wnd.h"
#include "gradient_cursor_wnd.h"
#include "tex_gen\transform_float.h"

using namespace NLTEXGEN;

// ***************************************************************************
// CGradientWnd
// ***************************************************************************

CGradientWnd::CGradientWnd() : _GradientWnd (false)
{
}

// ***************************************************************************

CGradientWnd::~CGradientWnd()
{
	uint i;
	for (i=0; i<_CursorsWnd.size(); i++)
		delete _CursorsWnd[i];
	for (i=0; i<_CursorsTangentWnd.size(); i++)
		delete _CursorsTangentWnd[i];
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CGradientWnd, CWnd)
	//{{AFX_MSG_MAP(CGradientWnd)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CGradientWnd message handlers
// ***************************************************************************

#define BORDER_LEFT 5
#define BORDER_RIGHT 6
#define BORDER_TOP 2
#define CURSOR_TOP 18
#define CURSOR_TANG_TOP 18
#define BORDER_BOTTOM (1+CURSOR_TOP+CURSOR_TANG_TOP)
#define CURSOR_MIN_LEFT 2
#define CURSOR_MAX_RIGHT 10

void CGradientWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// The client rect
	RECT client;
	GetClientRect (&client);

	// Create a brush
	CBrush brush(RGB(0,0,0));
	dc.SelectObject(&brush);

	// Draw the border
	RECT border;
	border.left = BORDER_LEFT;
	border.right = client.right - client.left - BORDER_RIGHT;
	border.top= BORDER_TOP;
	border.bottom = client.bottom - client.top - BORDER_BOTTOM;
	dc.FrameRect(&border, &brush);

	// If selected, draw a selection square
	if (GetFocus() == this)
		dc.DrawFocusRect(&client);
}

// ***************************************************************************

BOOL CGradientWnd::Create (DWORD dwStyle, RECT &rect, CWnd *parent, UINT nID)
{
	LPCTSTR className = AfxRegisterWndClass (CS_DBLCLKS, 0, (HBRUSH)COLOR_WINDOW, 0);
	if (CWnd::Create (className, "", dwStyle|WS_CLIPCHILDREN, rect, parent, nID))
	{
		RECT rect;
		rect.top = 0;
		rect.bottom = 100;
		rect.left= 0;
		rect.right= 100;
		if (_GradientWnd.Create (WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, rect, this, Gradient))
		{
			resizeChildren();
			return TRUE;
		}
	}
	return FALSE;
}

// ***************************************************************************

void CGradientWnd::setGradient(const std::vector<NLTEXGEN::CGradientColor> &gradient)
{
	_GradientWnd.setGradient (gradient);
	gradientChanged(false);
	_GradientWnd.Invalidate();
}

// ***************************************************************************

void CGradientWnd::gradientChanged(bool notifyParent)
{
	std::vector<NLTEXGEN::CGradientColor>	&gradient = _GradientWnd.getGradient();
	// Renormalize the grandient
	std::sort (gradient.begin(), gradient.end());

	// The client rect
	RECT client;
	GetClientRect (&client);

	// Create / delete children
	uint i=0;
	std::vector<NLTEXGEN::CGradientColor>::const_iterator ite = gradient.begin();
	while (ite != gradient.end())
	{
		// Create the window ?
		if (i>=_CursorsWnd.size())
		{
			_CursorsWnd.push_back (new CGradientCursorWnd);
			_CursorsWnd.back()->Create (WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, getClientX(ite->Pos), client.bottom-CURSOR_TOP-CURSOR_TANG_TOP, this, FirstCursor+i, ite->Color);
		}
		else
		{
			// Set the position
			_CursorsWnd[i]->NotifyMove = false;
			_CursorsWnd[i]->SetWindowPos(NULL, getClientX(ite->Pos), client.bottom-CURSOR_TOP-CURSOR_TANG_TOP, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
			_CursorsWnd[i]->NotifyMove = true;
			_CursorsWnd[i]->setColor (ite->Color);
		}

		if ((ite+1) != gradient.end())
		{
			// Create the window ?
			if (i>=_CursorsTangentWnd.size())
			{
				_CursorsTangentWnd.push_back (new CGradientCursorWnd);
				float pos = ite->Pos * (1.f - ite->RightPos) + (ite+1)->Pos * ite->RightPos;
				_CursorsTangentWnd.back()->Create (WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, getClientX(pos), client.bottom-CURSOR_TOP, this, FirstCursor+i, ite->RightQuadratic);
			}
			else
			{
				// Set the position
				_CursorsTangentWnd[i]->NotifyMove = false;
				float pos = ite->Pos * (1.f - ite->RightPos) + (ite+1)->Pos * ite->RightPos;
				_CursorsTangentWnd[i]->SetWindowPos(NULL, getClientX(pos), client.bottom-CURSOR_TOP, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
				_CursorsTangentWnd[i]->NotifyMove = true;
				_CursorsTangentWnd[i]->setQuadratic(ite->RightQuadratic);
			}
		}


		i++;
		ite++;
	}

	// Erase others..
	uint finalSize = i;
	for (;i<_CursorsWnd.size(); i++)
	{
		_CursorsWnd[i]->DestroyWindow();
		delete _CursorsWnd[i];
	}
	_CursorsWnd.resize (finalSize);

	// Erase others..
	i--;
	finalSize = i;
	for (;i<_CursorsTangentWnd.size(); i++)
	{
		_CursorsTangentWnd[i]->DestroyWindow();
		delete _CursorsTangentWnd[i];
	}
	_CursorsTangentWnd.resize (finalSize);

	if (notifyParent)
	{
		// Notify parent
		CWnd *parent = GetParent();
		if (parent)
			parent->PostMessage(WM_COMMAND, (CGW_GRADIENT_CHANGED<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	}
}

// ***************************************************************************

void CGradientWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	Invalidate();
}

// ***************************************************************************

void CGradientWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	Invalidate();
}

// ***************************************************************************

void CGradientWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	resizeChildren();
}

// ***************************************************************************

void CGradientWnd::resizeChildren()
{
	// The client rect
	RECT client;
	GetClientRect (&client);

	if (IsWindow(_GradientWnd))
	{

		_GradientWnd.SetWindowPos(&wndBottom, BORDER_LEFT+1, BORDER_TOP+1,
			client.right - client.left - BORDER_RIGHT - 1 - BORDER_LEFT - 1,
			client.bottom - client.top - BORDER_BOTTOM - 1 - BORDER_TOP - 1, SWP_SHOWWINDOW);
	}
}

// ***************************************************************************

void CGradientWnd::getCursorMinMax(RECT &rect) const
{
	// The client rect
	GetClientRect (&rect);
	rect.right -= CURSOR_MAX_RIGHT;
	rect.left += CURSOR_MIN_LEFT;
}

// ***************************************************************************

void CGradientWnd::cursorChanged()
{
	std::vector<NLTEXGEN::CGradientColor>	&gradient = _GradientWnd.getGradient();
	std::vector<NLTEXGEN::CGradientColor>::iterator ite = gradient.begin();
	uint i = 0;
	while (ite != gradient.end())
	{
		if (i<_CursorsWnd.size())
		{
			RECT child;
			_CursorsWnd[i]->GetWindowRect(&child);
			ScreenToClient (&child);
			ite->Pos = getPos(child.left);
			NLTEXGEN::copy (ite->Color, _CursorsWnd[i]->getColor());
		}

		ite++;
		i++;
	}

	uint j;
	for (i=0; i<gradient.size(); i++)
	for (j=0; j<i; j++)
	{
		if (gradient[i] < gradient[j])
		{
			std::swap (gradient[i], gradient[j]);
			if (i<_CursorsWnd.size())
				std::swap (_CursorsWnd[i], _CursorsWnd[j]);
			if ((i<_CursorsTangentWnd.size()) && (j<_CursorsTangentWnd.size()))
				std::swap (_CursorsTangentWnd[i], _CursorsTangentWnd[j]);
		}
	}

	// Resort windows
	for (i=0; i<_CursorsWnd.size(); i++)
		_CursorsWnd[i]->SetDlgCtrlID (i+FirstCursor);

	// Resort windows
	for (i=0; i<_CursorsTangentWnd.size(); i++)
		_CursorsTangentWnd[i]->SetDlgCtrlID (i+FirstCursor);

	// Notify parent
	CWnd *parent = GetParent();
	if (parent)
		parent->PostMessage(WM_COMMAND, (CGW_GRADIENT_CHANGED<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	_GradientWnd.Invalidate ();
}

// ***************************************************************************

void CGradientWnd::tangentChanged()
{
	std::vector<NLTEXGEN::CGradientColor>	&gradient = _GradientWnd.getGradient();
	std::vector<NLTEXGEN::CGradientColor>::iterator ite = gradient.begin();
	uint i = 0;
	while (ite != gradient.end())
	{
		if (i<_CursorsTangentWnd.size())
		{
			if ((ite+1) != gradient.end())
			{
				RECT child;
				_CursorsTangentWnd[i]->GetWindowRect(&child);
				ScreenToClient (&child);
				ite->RightPos = (getPos(child.left) - ite->Pos) / ((ite+1)->Pos - ite->Pos);
				ite->RightQuadratic = _CursorsTangentWnd[i]->getQuadratic();
			}
		}

		ite++;
		i++;
	}

	// Notify parent
	CWnd *parent = GetParent();
	if (parent)
		parent->PostMessage(WM_COMMAND, (CGW_GRADIENT_CHANGED<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	_GradientWnd.Invalidate ();
}

// ***************************************************************************

void CGradientWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// The client rect
	RECT client;
	GetClientRect (&client);

	if ( (point.x >= CURSOR_MIN_LEFT + CURSOR_WIDTH/2) &&
		(point.x <= client.right - CURSOR_MAX_RIGHT - CURSOR_WIDTH/2) &&
		(point.y >= client.bottom - BORDER_BOTTOM))
	{
		CGradientColor color;
		color.Pos = getPos(point.x - CURSOR_WIDTH/2);
		std::vector<NLTEXGEN::CGradientColor>	&_gradient = _GradientWnd.getGradient();
		gradient (color.Color, _gradient, color.Pos);
		_gradient.push_back (color);

		gradientChanged(true);
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

// ***************************************************************************

float CGradientWnd::getPos(int clientX) const
{
	RECT minMax;
	getCursorMinMax(minMax);

	return (float)(clientX-minMax.left)/(float)(minMax.right-minMax.left);
}

// ***************************************************************************

int CGradientWnd::getClientX(float pos) const
{
	RECT minMax;
	getCursorMinMax(minMax);

	return minMax.left + (int)floor(0.5f + pos * (float)(minMax.right - minMax.left));
}

// ***************************************************************************

void CGradientWnd::cursorDeleted(uint child)
{
	child -= FirstCursor;

	std::vector<NLTEXGEN::CGradientColor>	&gradient = _GradientWnd.getGradient();
	nlassert (child < gradient.size());

	gradient.erase(gradient.begin()+child);

	gradientChanged(true);
}

// ***************************************************************************

void CGradientWnd::cursorDuplicate(uint child)
{
	child -= FirstCursor;
	std::vector<NLTEXGEN::CGradientColor>	&gradient = _GradientWnd.getGradient();
	nlassert (child < gradient.size());

	gradient.insert(gradient.begin()+child, *(gradient.begin()+child));

	gradientChanged(true);
}

// ***************************************************************************
