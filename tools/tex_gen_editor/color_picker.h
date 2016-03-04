#if !defined(AFX_COLOR_PICKER_H__043936BD_51C7_4117_8850_63D9523EC0AF__INCLUDED_)
#define AFX_COLOR_PICKER_H__043936BD_51C7_4117_8850_63D9523EC0AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// color_picker.h : header file
//

#include "color_picker_rgb_panel.h"
#include "color_picker_hue_panel.h"
#include "color_picker_color_wnd.h"
#include "rot_button.h"
#include "my_edit.h"

#define MEMORY_COUNT_W 8
#define MEMORY_COUNT_H 16

#define CP_COLOR_CHANGE 0x1121
#define CP_COLOR_END 0x1125

/////////////////////////////////////////////////////////////////////////////
// CColorPicker dialog

class CColorPicker : public CDialog
{
// Construction
public:
	CColorPicker(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColorPicker)
	enum { IDD = IDD_COLOR_PICKER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	void setInitialColor (float *color);
	void getColor (float *color) const;

	// Window to inform
	HWND	WndToInform;

private:

	CColorPickerRGBPanel _PickerRGBPanel;
	CColorPickerHUEPanel _PickerHuePanel;
	CColorPickerColorWnd _OldColor;
	CColorPickerColorWnd _NewColor;
	CColorPickerColorWnd _Memory[MEMORY_COUNT_W*MEMORY_COUNT_H];

	CRotButton	_R, _G, _B, _H, _V, _S;
	CMyEdit		_REdit, _GEdit, _BEdit, _HEdit, _VEdit, _SEdit;
	CStatic	_RLabel, _GLabel, _BLabel, _HLabel, _VLabel, _SLabel;

	void hvsChanged ();
	void rgbChanged ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorPicker)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorPicker)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOR_PICKER_H__043936BD_51C7_4117_8850_63D9523EC0AF__INCLUDED_)
