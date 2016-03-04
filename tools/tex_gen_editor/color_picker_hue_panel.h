#if !defined(AFX_COLOR_PICKER_HUE_PANEL_H__0BCF92D0_B0C1_4E87_B2A4_39EB81851E27__INCLUDED_)
#define AFX_COLOR_PICKER_HUE_PANEL_H__0BCF92D0_B0C1_4E87_B2A4_39EB81851E27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// color_picker_rgb_panel.h : header file
//

#include "dib_wnd.h"

#define HUE_PANEL_WIDTH 40
#define HUE_PANEL_HEIGHT 258

#define HUE_CHANGED 0x1117

// ***************************************************************************
// CColorPickerHUEPanel window
// ***************************************************************************

class CColorPickerHUEPanel : public CWnd
{
// Construction
public:
	CColorPickerHUEPanel();

// Attributes
public:

	// Create
	BOOL Create (DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);

	void setHue (float hue);
	float getHue () const
	{
		return _Hue;
	}

private:

	// The hue value
	float _Hue;

	// Color panel
	CDibWnd		_Panel;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorPickerHUEPanel)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorPickerHUEPanel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorPickerHUEPanel)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ***************************************************************************

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOR_PICKER_HUE_PANEL_H__0BCF92D0_B0C1_4E87_B2A4_39EB81851E27__INCLUDED_)
