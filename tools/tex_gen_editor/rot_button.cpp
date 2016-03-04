// rot_button.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "rot_button.h"

#define LEFT_ANGLE (140.f*(float)NLMISC::Pi/180.f)
#define RIGHT_ANGLE (-140.f*(float)NLMISC::Pi/180.f)

#define RADIUS_CURSOR 8
#define SIZE_CURSOR 8

#define SNAP_ANGLE (10.f*(float)NLMISC::Pi/180.f)

#define WHEEL_ANGLE (10.f*(float)NLMISC::Pi/180.f)

using namespace NLMISC;

// ***************************************************************************
// CRotButton
// ***************************************************************************

CRotButton::CRotButton()
{
	_Edit = NULL;
}

// ***************************************************************************

CRotButton::~CRotButton()
{
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CRotButton, CWnd)
	//{{AFX_MSG_MAP(CRotButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CRotButton message handlers

void CRotButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Draw the bitmap
	RECT rect;
	GetClientRect (&rect);

	CDC dc2;
	dc2.CreateCompatibleDC(&dc);
	dc2.SelectObject (Button);
	nlverify (dc.MaskBlt(0, 0, rect.right-rect.left, rect.bottom-rect.top, &dc2, 0, 0, ButtonMask, 0, 0, MAKEROP4 (SRCCOPY, SRCPAINT)));

	// Draw the cursor
	sint x,y;
	getCursorPos (_Value, x, y);

	dc2.SelectObject (ButtonHole);
	nlverify (dc.MaskBlt(x, y, SIZE_CURSOR, SIZE_CURSOR, &dc2, 0, 0, ButtonHoleMask, 0, 0, MAKEROP4 (SRCCOPY, SRCPAINT)));

	// If selected, draw a selection square
	if (GetFocus() == this)
		dc.DrawFocusRect(&rect);
}

// ***************************************************************************

BOOL CRotButton::Create (DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	LPCTSTR className = AfxRegisterWndClass (CS_DBLCLKS, 0, (HBRUSH)COLOR_WINDOW, 0);
	return CWnd::Create (className, "", dwStyle, rect, pParentWnd, nID);
}

// ***************************************************************************

void CRotButton::init (const NLTEXGEN::CFloatValueSetup &setup)
{
	_Setup = setup;
	uint i;
	_AngleSnaped.resize (_Setup.SnapedValuesCount);
	for (i=0; i<_Setup.SnapedValuesCount; i++)
	{
		_AngleSnaped[i] = valueToAngle (_Setup.SnapedValues[i]);
	}
}

// ***************************************************************************

float CRotButton::valueToAngle (float value) const
{
	switch (_Setup.Formula)
	{
	case NLTEXGEN::CFloatValueSetup::TwoPower:
	case NLTEXGEN::CFloatValueSetup::Linear:
		{
			static const float ln10 = (float)log(2);
			if (_Setup.Formula == NLTEXGEN::CFloatValueSetup::TwoPower)
				value = (float)log(value)/ln10;

			float delta = _Setup.MultiTour?(float)-Pi*2.f:RIGHT_ANGLE-LEFT_ANGLE;
			if (_Setup.CenterOrigine)
				return (float)(delta * (value-_Setup.ValueOrigine)) / _Setup.ValueTour;
			else
				return LEFT_ANGLE + (float)((RIGHT_ANGLE-LEFT_ANGLE) * (value-_Setup.ValueOrigine)) / _Setup.ValueTour;
		}
	}
	return 0;
}

// ***************************************************************************

float CRotButton::angleToValue (float angle, bool doClamp) const
{
	switch (_Setup.Formula)
	{
	case NLTEXGEN::CFloatValueSetup::TwoPower:
	case NLTEXGEN::CFloatValueSetup::Linear:
		{
			float value;
			float delta = _Setup.MultiTour?(float)-Pi*2.f:RIGHT_ANGLE-LEFT_ANGLE;
			if (_Setup.CenterOrigine)
			{
				value = _Setup.ValueOrigine + _Setup.ValueTour * angle / delta;
			}
			else
			{
				value = _Setup.ValueOrigine + _Setup.ValueTour * (angle-LEFT_ANGLE) / (float)(RIGHT_ANGLE-LEFT_ANGLE);
			}

			value = (float)floor(0.5f + value / _Setup.Epsilon) * _Setup.Epsilon;
			if (doClamp)
				clamp (value, _Setup.ValueLeft, _Setup.ValueRight);
			if (_Setup.Formula == NLTEXGEN::CFloatValueSetup::TwoPower)
				value = (float)pow(2, value);
			return value;
		}
	}
	return 0;
}

// ***************************************************************************

float CRotButton::getNextAngle (float previousAngle, float newAngle) const
{
	while ((newAngle - previousAngle) > NLMISC::Pi)
		newAngle -= (float)NLMISC::Pi*2.f;
	while ((previousAngle - newAngle) > NLMISC::Pi)
		newAngle += (float)NLMISC::Pi*2.f;

	return valueToAngle (angleToValue (newAngle));
}

// ***************************************************************************

void CRotButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture ();	
	SetFocus ();

	// Notify parent
	CWnd *parent = GetParent();
	if (parent)
		parent->PostMessage(WM_COMMAND, (ROB_CAPTURED<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	
	CWnd::OnLButtonDown(nFlags, point);
}

// ***************************************************************************

void CRotButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (GetCapture () == this)
	{
		RECT rect;

		// Previous angle
		float previousAngle = valueToAngle (_Value);

		invalidateCursor ();

		// Compute angle
		GetClientRect (&rect);
		float dx = (float)(point.x - (rect.right + rect.left) / 2);
		float dy = -(float)(point.y - (rect.bottom + rect.top) / 2);

		// Snaped angle ?
		float angle = getNextAngle (previousAngle, -(float)atan2 (dx, dy));

		// Snaped ?
		float value;
		if ((nFlags&(MK_CONTROL|MK_SHIFT))==0)
		{
			uint i;
			for (i=0; i<_Setup.SnapedValuesCount; i++)
			{
				// Good ?
				if (fabs(angle-_AngleSnaped[i])<=SNAP_ANGLE)
				{
					value = _Setup.SnapedValues[i];
					break;
				}
			}
			if (i==_Setup.SnapedValuesCount)
				value = angleToValue (angle);
		}
		else
			value = angleToValue (angle);

		_Value = value;
		valueAsBeenChanged (true);
	}

	CWnd::OnMouseMove(nFlags, point);
}

// ***************************************************************************

void CRotButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture ();	

	// Notify parent
	CWnd *parent = GetParent();
	if (parent)
		parent->PostMessage(WM_COMMAND, (ROB_RELEASED<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	
	CWnd::OnLButtonUp(nFlags, point);
}

// ***************************************************************************

void CRotButton::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	Invalidate ();
}

// ***************************************************************************

void CRotButton::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	Invalidate ();
}

// ***************************************************************************

void CRotButton::getCursorPos (float value, sint &x, sint &y) const
{
	RECT rect;
	GetClientRect (&rect);
	sint width = rect.right-rect.left;
	sint height = rect.bottom-rect.top;

	const float angle = valueToAngle (value);
	const float sa = (float)sin(angle);
	const float ca = (float)cos(angle);

	x = (sint)floor(0.5 - sa*(RADIUS_CURSOR)) + RADIUS_CURSOR - SIZE_CURSOR/2 + (width/2-RADIUS_CURSOR);
	y = (-(sint)floor(0.5 + ca*(RADIUS_CURSOR)) + RADIUS_CURSOR) - SIZE_CURSOR/2 + (height/2-RADIUS_CURSOR);
}

// ***************************************************************************

void CRotButton::updateEditBox ()
{
	if (_Edit)
	{
		_Edit->SetWindowText(toString(_Value).c_str());
		CDC *dc = _Edit->GetDC();
		_Edit->SendMessage (WM_PAINT, (WPARAM)(HDC)*dc, 0);
		_Edit->ReleaseDC (dc);
	}
}

// ***************************************************************************

void CRotButton::setEditBox (CExtEdit *edit)
{
	_Edit = edit;
	updateEditBox ();
}

// ***************************************************************************

void CRotButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	invalidateCursor ();
	
	switch (_Setup.Formula)
	{
	case NLTEXGEN::CFloatValueSetup::TwoPower:
		_Value = (float)pow(2, _Setup.ValueOrigine);
		break;
	case NLTEXGEN::CFloatValueSetup::Linear:
		_Value = _Setup.ValueOrigine;
		break;
	}

	valueAsBeenChanged (true);

	CWnd::OnLButtonDblClk(nFlags, point);
}

// ***************************************************************************

void CRotButton::valueAsBeenChanged(bool notify)
{
	invalidateCursor();

	CDC *dc = GetDC();
	SendMessage (WM_PAINT, (WPARAM)(HDC)*dc, 0);
	ReleaseDC (dc);

	// Change the edit box
	updateEditBox ();

	// Notify parent
	if (notify)
	{
		CWnd *parent = GetParent();
		if (parent)
			parent->PostMessage(WM_COMMAND, (ROB_VALUE_CHANGED<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
	}
}

// ***************************************************************************

void CRotButton::invalidateCursor()
{
	sint x,y;
	getCursorPos (_Value, x, y);
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = rect.left+SIZE_CURSOR;
	rect.bottom = rect.top+SIZE_CURSOR;
	InvalidateRect (&rect, FALSE);
}

// ***************************************************************************

void CRotButton::setValue (float value)
{
	invalidateCursor ();

	_Value = value;
	
	valueAsBeenChanged (false);
}

// ***************************************************************************

BOOL CRotButton::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	invalidateCursor ();

	float angle = valueToAngle (_Value);
	angle = (float)floor(0.5f + angle / WHEEL_ANGLE) * WHEEL_ANGLE;
	angle += (float)zDelta*WHEEL_ANGLE/(float)WHEEL_DELTA;
	_Value = angleToValue (angle);

	valueAsBeenChanged (true);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
