#if !defined(AFX_COLOR_PICKER_RGB_PANEL_H__0BCF92D0_B0C1_4E87_B2A4_39EB81851E27__INCLUDED_)
#define AFX_COLOR_PICKER_RGB_PANEL_H__0BCF92D0_B0C1_4E87_B2A4_39EB81851E27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// color_picker_rgb_panel.h : header file
//

#include "dib_wnd.h"

#define PANEL_WIDTH 258
#define PANEL_HEIGHT 258

#define RGB_CHANGED 0x1118

// ***************************************************************************
// CColorPickerRGBPanel window
// ***************************************************************************

class CColorPickerRGBPanel : public CWnd
{
// Construction
public:
	CColorPickerRGBPanel();

// Attributes
public:

	// Set the hue
	void setHue (float hue);
	void setSat (float sat);
	void setValue (float val);

	// Get the Sat and Value
	float getSat () const
	{
		return _Sat;
	}
	float getValue () const
	{
		return _Val;
	}

	// Create
	BOOL Create (DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);

private:

	// Color panel
	CDibWnd		_Panel;

	// Sat and value 
	float	_Sat;
	float	_Val;

	void invalidateCursor ();
	void getCursorPos (sint &x, sint &y);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorPickerRGBPanel)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorPickerRGBPanel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorPickerRGBPanel)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ***************************************************************************

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOR_PICKER_RGB_PANEL_H__0BCF92D0_B0C1_4E87_B2A4_39EB81851E27__INCLUDED_)
