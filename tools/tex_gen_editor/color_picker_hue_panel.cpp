// color_picker_rgb_panel.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "color_picker_hue_panel.h"
#include "tex_gen/transform_float.h"

using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************
// CColorPickerHUEPanel
// ***************************************************************************

CColorPickerHUEPanel::CColorPickerHUEPanel()
{
}

// ***************************************************************************

CColorPickerHUEPanel::~CColorPickerHUEPanel()
{
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CColorPickerHUEPanel, CWnd)
	//{{AFX_MSG_MAP(CColorPickerHUEPanel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CColorPickerHUEPanel message handlers
// ***************************************************************************

#define HUE_WIDTH 25
#define HUE_X ((HUE_PANEL_WIDTH - HUE_WIDTH)/2)

BOOL CColorPickerHUEPanel::Create (DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	LPCTSTR className = AfxRegisterWndClass (0, 0, 0, 0);
	if (CWnd::Create(className, "", dwStyle, rect, pParentWnd, nID))
	{
		// Create the panel
		_Panel.setSize (HUE_WIDTH-2, HUE_PANEL_HEIGHT-2);
		
		uint8 *pixels = _Panel.getPixels ();

		uint y;
		for (y=0; y<256; y++)
		{
			float hue = (float)y * 6.f / 256.f;
			float color[3];
			HVS2RGB (hue, 1, 1, color);

			sint r = (sint)(color[0]*255);
			sint g = (sint)(color[1]*255);
			sint b = (sint)(color[2]*255);
			uint x;
			for (x=0; x<HUE_WIDTH-2; x++, pixels += 4)
			{
				pixels[0] = b;
				pixels[1] = g;
				pixels[2] = r;
			}
		}
		return TRUE;
	}
	return FALSE;
}

// ***************************************************************************

void drawTrigangle (CDC &dc, float hue)
{
	int y = 255 - (int)(256.f*(hue/360.f)) + 1;
	clamp (y, 1, 256);
	dc.MoveTo(0, y-HUE_X+1);
	dc.LineTo(HUE_X-1, y);
	dc.LineTo(0, y+HUE_X-1);
	dc.LineTo(0, y-HUE_X+1);
}

// ***************************************************************************

void CColorPickerHUEPanel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	_Panel.draw (&dc, HUE_X+1, 1, 0);

	// Paint over the old one
	CRect rect2 (0, 0, HUE_X, HUE_PANEL_HEIGHT);
	dc.FillSolidRect( rect2, GetSysColor(COLOR_3DFACE));

	CRect rect (HUE_X, 0, HUE_X+HUE_WIDTH, HUE_PANEL_HEIGHT);
	CBrush brush;
	brush.CreateSolidBrush (RGB(0,0,0));
	dc.FrameRect (&rect, &brush);

	// Draw triangle

	CPen pen;
	pen.CreatePen (PS_SOLID, 1, RGB(0,0,0));
	dc.SelectObject(pen);
	drawTrigangle (dc, _Hue);
}

// ***************************************************************************

BOOL CColorPickerHUEPanel::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_MOUSEMOVE) || (pMsg->message == WM_LBUTTONDOWN))
	{
		if (pMsg->wParam & MK_LBUTTON)
		{
			_Hue = 360.f * (256 - (sint16)HIWORD(pMsg->lParam)) / 256.f;
			clamp (_Hue, 0.f, 359.f);
			CRect rect2 (0, 0, HUE_X, HUE_PANEL_HEIGHT);
			InvalidateRect (&rect2);

			CWnd *parent = GetParent();
			if (parent)
				parent->PostMessage(WM_COMMAND, (HUE_CHANGED<<16)|GetDlgCtrlID(), (LONG)m_hWnd);

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

void CColorPickerHUEPanel::setHue (float hue)
{
	_Hue = hue;
	CRect rect2 (0, 0, HUE_X, HUE_PANEL_HEIGHT);
	InvalidateRect (&rect2);
}

// ***************************************************************************
