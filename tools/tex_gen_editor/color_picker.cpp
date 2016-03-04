// color_picker.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "color_picker.h"
#include "tex_gen/transform_float.h"

using namespace NLTEXGEN;

#define RGB_PANEL_ID 0
#define HUE_PANEL_ID 1
#define R_ID 2
#define G_ID 3
#define B_ID 4
#define H_ID 5
#define V_ID 6
#define S_ID 7
#define MEMORY_ID 8

// ***************************************************************************
// CColorPicker dialog
// ***************************************************************************

CColorPicker::CColorPicker(CWnd* pParent /*=NULL*/)
	: CDialog(CColorPicker::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorPicker)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	WndToInform = NULL;
}

// ***************************************************************************

void CColorPicker::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorPicker)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CColorPicker, CDialog)
	//{{AFX_MSG_MAP(CColorPicker)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CColorPicker message handlers
// ***************************************************************************

#define MARGE 5
#define PANEL_X MARGE
#define PANEL_Y MARGE
#define HUE_PANEL_X (PANEL_X+MARGE+PANEL_WIDTH)
#define COLOR_PANEL_X (HUE_PANEL_X+MARGE+HUE_PANEL_WIDTH)
#define COLOR_WIDTH 40
#define COLOR_HEIGHT 34
#define MEMORY_W 16
#define MEMORY_H 16

CFloatValueSetup CenteredMT256_256Setup (CFloatValueSetup::Linear, "", 0, 256, 0.001f, true, true, NULL, 0);

void createButton (CRect &rect, CFloatValueSetup &setup, UINT id, const char *label,
				   CRotButton &rotButton, CExtEdit &edit, CStatic &labelStatic, CWnd *parent, float value)
{
	CRect rect2 = rect;
	rect2.right = rect2.left + 32;
	rotButton.Create (WS_CHILD|WS_TABSTOP|WS_VISIBLE, rect2, parent, id);
	rotButton.init (setup);

	// Label
	rect2.top --;
	rect2.left = rect2.right + MARGE;
	rect2.right = rect2.left + 2*COLOR_WIDTH - 1 - 32 - MARGE;
	rect2.bottom = rect2.top + 16;
	labelStatic.Create (label, WS_CHILD|SS_LEFT|SS_CENTERIMAGE|WS_VISIBLE, rect2, parent);
	labelStatic.SetFont (parent->GetFont());

	// Edit
	rect2.top = rect2.bottom;
	rect2.bottom = rect2.top + 20;
	edit.Create (WS_TABSTOP|ES_LEFT|WS_VISIBLE|WS_CHILD, rect2, parent, id);
	edit.SetFont (parent->GetFont());
	rotButton.setEditBox (&edit);
	rotButton.setValue (value);
}

BOOL CColorPicker::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Create the panel
	CRect rect (PANEL_X, PANEL_Y, PANEL_X+PANEL_WIDTH, PANEL_Y+PANEL_HEIGHT);
	_PickerRGBPanel.Create (WS_CHILD|WS_VISIBLE, rect, this, RGB_PANEL_ID);
	
	CRect rect2 (HUE_PANEL_X, PANEL_Y, HUE_PANEL_X+HUE_PANEL_WIDTH, PANEL_Y+PANEL_HEIGHT);
	_PickerHuePanel.Create (WS_CHILD|WS_VISIBLE, rect2, this, HUE_PANEL_ID);

	CRect rect3 (COLOR_PANEL_X, PANEL_Y, COLOR_PANEL_X+COLOR_WIDTH, PANEL_Y+COLOR_HEIGHT);
	_NewColor.Create (WS_CHILD|WS_VISIBLE, rect3, this, HUE_PANEL_ID);
	rect3.left += COLOR_WIDTH-1;
	rect3.right += COLOR_WIDTH-1;
	_OldColor.Create (WS_CHILD|WS_VISIBLE, rect3, this, HUE_PANEL_ID);

	hvsChanged ();

	rect3.left = COLOR_PANEL_X;
	rect3.top = rect3.bottom + MARGE;
	rect3.bottom = rect3.top + 32;
	createButton (rect3, CenteredMT256_256Setup, R_ID, "Red", _R, _REdit, _RLabel, this, 0);

	rect3.top += MARGE + 32;
	rect3.bottom += MARGE + 32;
	createButton (rect3, CenteredMT256_256Setup, G_ID, "Green", _G, _GEdit, _GLabel, this, 0);

	rect3.top += MARGE + 32;
	rect3.bottom += MARGE + 32;
	createButton (rect3, CenteredMT256_256Setup, B_ID, "Blue", _B, _BEdit, _BLabel, this, 0);

	rect3.top += MARGE + 32;
	rect3.bottom += MARGE + 32;
	createButton (rect3, AngleSetup, H_ID, "Hue", _H, _HEdit, _HLabel, this, 0);

	rect3.top += MARGE + 32;
	rect3.bottom += MARGE + 32;
	createButton (rect3, CenteredMT1_1Setup, S_ID, "Sat", _S, _SEdit, _SLabel, this, 0);

	rect3.top += MARGE + 32;
	rect3.bottom += MARGE + 32;
	createButton (rect3, CenteredMT1_1Setup, V_ID, "Value", _V, _VEdit, _VLabel, this, 0);

	int left = rect3.right + MARGE;

	uint x, y;
	for (y=0; y<MEMORY_COUNT_H; y++)
	for (x=0; x<MEMORY_COUNT_W; x++)
	{
		CColorPickerColorWnd &memory = _Memory[y*MEMORY_COUNT_W+x];
		CRect rectMemory (left + x*MEMORY_W, MARGE + y*MEMORY_H, left + (x+1)*MEMORY_W, MARGE + (y+1)*MEMORY_H);
		memory.Create (WS_CHILD|WS_VISIBLE, rectMemory, this, MEMORY_ID+y*MEMORY_COUNT_W+x);
	}

	// Set default colors
	uint r, g, b;
	uint i=0;
	for (r=0; r<4; r++)
	for (g=0; g<4; g++)
	for (b=0; b<4; i++, b++)
	{
		_Memory[i].setColor (RGB(r*255/3, g*255/3, b*255/3));
	}
	for (; i<96; i++)
	{
		int value = ((i-64)<<3) + ((i-64)>>2);
		_Memory[i].setColor (RGB(value, value, value));
	}
	for (; i<128; i++)
	{
		_Memory[i].setColor (RGB(255, 255, 255));
	}


	RECT window;
	GetWindowRect (&window);
	RECT client;
	GetClientRect (&client);

	int width = MARGE + 256 + MARGE + HUE_PANEL_WIDTH + MARGE + COLOR_WIDTH * 2 + MARGE + MEMORY_W*MEMORY_COUNT_W + MARGE;
	int height = MARGE + 256 + MARGE;
	SetWindowPos (NULL, 0, 0, window.right - window.left - client.right + client.left + width, 
		window.bottom - window.top - client.bottom + client.top + height, SWP_NOMOVE|SWP_NOZORDER); 


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ***************************************************************************

BOOL CColorPicker::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// Get parameters
	uint idWidget = (int) LOWORD(wParam);  // identifier of combo box 
	HWND hwndWidget = (HWND) lParam;       // handle to combo box 
	
	if ((HIWORD(wParam) == EN_RETURN) || (HIWORD(wParam) == EN_KILLFOCUS))
	{
		// Get current colors
		char number[100];
		::GetWindowText (hwndWidget, number, 100);

		float newValue = (float)atof(number);
		switch (idWidget)
		{
		case R_ID:
			_R.setValue (newValue);
			rgbChanged ();
			break;
		case G_ID:
			_G.setValue (newValue);
			rgbChanged ();
			break;
		case B_ID:
			_B.setValue (newValue);
			rgbChanged ();
			break;
		case H_ID:
			_H.setValue (newValue);
			hvsChanged ();
			break;
		case V_ID:
			_V.setValue (newValue);
			hvsChanged ();
			break;
		case S_ID:
			_S.setValue (newValue);
			hvsChanged ();
			break;
		}
	}
	else if ((HIWORD(wParam) ==  CW_LBUTTON) && (idWidget >= MEMORY_ID) && (idWidget < MEMORY_ID+128))
	{
		COLORREF ref = _Memory[idWidget-MEMORY_ID].getColor ();
		_R.setValue (GetRValue(ref));
		_G.setValue (GetGValue(ref));
		_B.setValue (GetBValue(ref));
		rgbChanged ();
	}
	else if ((HIWORD(wParam) ==  CW_RBUTTON) && (idWidget >= MEMORY_ID) && (idWidget < MEMORY_ID+128))
	{
		_Memory[idWidget-MEMORY_ID].setColor (RGB((int)_R.getValue(), (int)_G.getValue(), (int)_B.getValue()));
	}
	switch (idWidget)
	{
	case HUE_PANEL_ID:
		_H.setValue (_PickerHuePanel.getHue());
		hvsChanged ();
		break;
	case RGB_PANEL_ID:
		_S.setValue (_PickerRGBPanel.getSat());
		_V.setValue (_PickerRGBPanel.getValue());
		hvsChanged ();
		break;
	case H_ID:
	case S_ID:
	case V_ID:
		if (HIWORD(wParam) == ROB_VALUE_CHANGED)
			hvsChanged ();
		break;
	case R_ID:
	case G_ID:
	case B_ID:
		if (HIWORD(wParam) == ROB_VALUE_CHANGED)
			rgbChanged ();
		break;
	}

	return CDialog::OnCommand(wParam, lParam);
}

// ***************************************************************************

void CColorPicker::hvsChanged ()
{
	_PickerRGBPanel.setHue (_H.getValue());
	_PickerRGBPanel.setSat (_S.getValue());
	_PickerRGBPanel.setValue (_V.getValue());
	CDC *dc = _PickerRGBPanel.GetDC();
	_PickerRGBPanel.SendMessage (WM_PAINT, (WPARAM)(HDC)*dc, 0);
	_PickerRGBPanel.ReleaseDC (dc);
	_PickerHuePanel.setHue (_H.getValue());
	dc = _PickerHuePanel.GetDC();
	_PickerHuePanel.SendMessage (WM_PAINT, (WPARAM)(HDC)*dc, 0);
	_PickerHuePanel.ReleaseDC (dc);
	float rgb[3];
	HVS2RGB (_H.getValue()*6.f/360.f, _V.getValue(), _S.getValue(), rgb);
	_NewColor.setColor (getColorRef(rgb));
	dc = _NewColor.GetDC();
	_NewColor.SendMessage (WM_PAINT, (WPARAM)(HDC)*dc, 0);
	_NewColor.ReleaseDC (dc);
	if (IsWindow (_R))
	{
		_R.setValue (rgb[0]*255.f);
		_G.setValue (rgb[1]*255.f);
		_B.setValue (rgb[2]*255.f);
	}

	// Notify WndToInform
	if (WndToInform)
		::PostMessage(WndToInform, WM_COMMAND, (CP_COLOR_CHANGE<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
}

// ***************************************************************************

void CColorPicker::rgbChanged ()
{
	float rgb[3] = {_R.getValue()/255.f, _G.getValue()/255.f, _B.getValue()/255.f};
	float h, v, s;
	RGB2HVS (rgb, h, v, s);
	h *= 360.f / 6.f;
	if (IsWindow (_H))
	{
		_H.setValue (h);
		_V.setValue (v);
		_S.setValue (s);
	}
	_PickerRGBPanel.setHue (h);
	_PickerRGBPanel.setSat (s);
	_PickerRGBPanel.setValue (v);
	_PickerHuePanel.setHue (h);
	_NewColor.setColor (getColorRef(rgb));

	// Notify WndToInform
	if (WndToInform)
		::PostMessage(WndToInform, WM_COMMAND, (CP_COLOR_CHANGE<<16)|GetDlgCtrlID(), (LONG)m_hWnd);
}

// ***************************************************************************

LRESULT CColorPicker::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if ((message == WM_ACTIVATE) && (LOWORD(wParam) == WA_INACTIVE))
	{
		ShowWindow (SW_HIDE);

		// Notify WndToInform
		if (WndToInform)
			::PostMessage(WndToInform, WM_COMMAND, (CP_COLOR_END<<16)|GetDlgCtrlID(), (LONG)m_hWnd);

		WndToInform = NULL;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

// ***************************************************************************

void CColorPicker::setInitialColor (float *color)
{
	_R.setValue (color[0] * 255.f);
	_G.setValue (color[1] * 255.f);
	_B.setValue (color[2] * 255.f);
	rgbChanged ();
	_OldColor.setColor (getColorRef(color));
}

// ***************************************************************************

void CColorPicker::getColor (float *color) const
{
	color[0] = _R.getValue() / 255.f;
	color[1] = _G.getValue() / 255.f;
	color[2] = _B.getValue() / 255.f;
}

// ***************************************************************************
