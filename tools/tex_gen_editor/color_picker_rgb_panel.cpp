// color_picker_rgb_panel.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "color_picker_rgb_panel.h"
#include "tex_gen/transform_float.h"

using namespace NLTEXGEN;

// ***************************************************************************
// CColorPickerRGBPanel
// ***************************************************************************

CColorPickerRGBPanel::CColorPickerRGBPanel()
{
}

// ***************************************************************************

CColorPickerRGBPanel::~CColorPickerRGBPanel()
{
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CColorPickerRGBPanel, CWnd)
	//{{AFX_MSG_MAP(CColorPickerRGBPanel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CColorPickerRGBPanel message handlers
// ***************************************************************************

BOOL CColorPickerRGBPanel::Create (DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	LPCTSTR className = AfxRegisterWndClass (0, 0, 0, 0);
	if (CWnd::Create(className, "", dwStyle, rect, pParentWnd, nID))
	{
		// Create the panel
		_Panel.setSize (PANEL_WIDTH-2, PANEL_HEIGHT-2);
		return TRUE;
	}
	return FALSE;
}

// ***************************************************************************

#define CROSS_CENTER 5
#define CROSS_RECT_HEIGHT 1
#define CROSS_RECT_WIDTH 10

void CColorPickerRGBPanel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	_Panel.draw (&dc, 1, 1, 0);

	RECT rect;
	GetClientRect (&rect);
	CBrush brush;
	brush.CreateSolidBrush (RGB(0,0,0));
	dc.FrameRect (&rect, &brush);

	sint x,y;
	getCursorPos (x, y);

	dc.FillSolidRect (x-CROSS_CENTER-CROSS_RECT_WIDTH, y-CROSS_RECT_HEIGHT, CROSS_RECT_WIDTH, 3*CROSS_RECT_HEIGHT, RGB(255,255,255));
	dc.FillSolidRect (x+CROSS_CENTER+1, y-CROSS_RECT_HEIGHT, CROSS_RECT_WIDTH, 3*CROSS_RECT_HEIGHT, RGB(255,255,255));
	dc.FillSolidRect (x-CROSS_RECT_HEIGHT, y-CROSS_CENTER-CROSS_RECT_WIDTH, 3*CROSS_RECT_HEIGHT, CROSS_RECT_WIDTH, RGB(255,255,255));
	dc.FillSolidRect (x-CROSS_RECT_HEIGHT, y+CROSS_CENTER+1, 3*CROSS_RECT_HEIGHT, CROSS_RECT_WIDTH, RGB(255,255,255));
}

// ***************************************************************************

void CColorPickerRGBPanel::setHue (float hue)
{
	uint8 *pixels = _Panel.getPixels ();

	hue *= 6.f/360.f;
	float color[3];

	HVS2RGB (hue, 1, 1, color);
	
	sint r0 = 255<<8;
	sint g0 = 255<<8;
	sint b0 = 255<<8;
	sint r1 = (sint)(color[0]*255)<<8;
	sint g1 = (sint)(color[1]*255)<<8;
	sint b1 = (sint)(color[2]*255)<<8;
	uint y;
	for (y=0; y<256; y++)
	{
		sint r = (r0*y)>>8;
		sint g = (g0*y)>>8;
		sint b = (b0*y)>>8;
		sint dr = (((r1*y)>>8) - r)>>8;
		sint dg = (((g1*y)>>8) - g)>>8;
		sint db = (((b1*y)>>8) - b)>>8;
		uint x;
		for (x=0; x<256; x++, pixels += 4)
		{
			pixels[0] = b>>8;
			pixels[1] = g>>8;
			pixels[2] = r>>8;
			b += db;
			g += dg;
			r += dr;
		}
	}

	Invalidate();
}

// ***************************************************************************

void CColorPickerRGBPanel::setSat (float sat)
{
	_Sat = sat;
	invalidateCursor ();
}

// ***************************************************************************

void CColorPickerRGBPanel::setValue (float val)
{
	_Val = val;
	invalidateCursor ();
}

// ***************************************************************************

BOOL CColorPickerRGBPanel::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_MOUSEMOVE) || (pMsg->message == WM_LBUTTONDOWN))
	{
		if (pMsg->wParam & MK_LBUTTON)
		{
			invalidateCursor ();
			_Sat = (float)((sint16)LOWORD(pMsg->lParam) - 1) / (float)(PANEL_WIDTH-2);
			_Val = 1.f - (float)((sint16)HIWORD(pMsg->lParam) - 1) / (float)(PANEL_WIDTH-2);
			NLMISC::clamp (_Sat, 0.f, 1.f);
			NLMISC::clamp (_Val, 0.f, 1.f);
			invalidateCursor ();

			CWnd *parent = GetParent();
			if (parent)
				parent->PostMessage(WM_COMMAND, (RGB_CHANGED<<16)|GetDlgCtrlID(), (LONG)m_hWnd);

			if (pMsg->message == WM_LBUTTONDOWN)
			{
				SetCapture ();
			}
		}
		return TRUE;
	}
	if (pMsg->message == WM_LBUTTONUP)
	{
		ReleaseCapture ();
		return TRUE;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

// ***************************************************************************

void CColorPickerRGBPanel::invalidateCursor ()
{
	sint x,y;
	getCursorPos (x, y);
	CRect rect (x-CROSS_CENTER-CROSS_RECT_WIDTH, y-CROSS_CENTER-CROSS_RECT_WIDTH, 
		x+CROSS_CENTER+CROSS_RECT_WIDTH, y+CROSS_CENTER+CROSS_RECT_WIDTH);
	InvalidateRect (&rect);
}

// ***************************************************************************

void CColorPickerRGBPanel::getCursorPos (sint &x, sint &y)
{
	x = 1 + (sint)floor(0.5f+(_Sat * (float)(PANEL_WIDTH-2)));
	y = 1 + (sint)floor(0.5f+((1.f-_Val) * (float)(PANEL_HEIGHT-2)));
}

// ***************************************************************************

