#if !defined(AFX_ROT_BUTTON_H__2D00274D_BADA_47F5_A9DF_3DD276B96651__INCLUDED_)
#define AFX_ROT_BUTTON_H__2D00274D_BADA_47F5_A9DF_3DD276B96651__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// rot_button.h : header file
//

#include "tex_gen/basics.h"

#define ROB_VALUE_CHANGED 0x1114
#define ROB_RELEASED 0x1115
#define ROB_CAPTURED 0x1116

// ***************************************************************************
// CRotButton window
// ***************************************************************************

class CRotButton : public CWnd
{
// Construction
public:
	CRotButton();

// Attributes
public:

// Operations
public:

	BOOL Create (DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);

	void init (const NLTEXGEN::CFloatValueSetup &setup);
	void getCursorPos (float value, sint &x, sint &y) const;
	void setEditBox (CExtEdit *edit);
	void setValue (float value);
	float getValue () const
	{
		return _Value;
	}

	// Conversion
	float	valueToAngle (float value) const;
	float	angleToValue (float angle, bool doClamp = true) const;

private:

	// The current value
	float _Value;
	NLTEXGEN::CFloatValueSetup _Setup;
	std::vector<float>			_AngleSnaped;
	// Edit box
	CExtEdit	*_Edit;

	// Delta
	float CRotButton::getNextAngle (float previousAngle, float newAngle) const;

	// Update
	void updateEditBox ();
	void valueAsBeenChanged (bool notify);
	void invalidateCursor ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRotButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRotButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRotButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ***************************************************************************

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROT_BUTTON_H__2D00274D_BADA_47F5_A9DF_3DD276B96651__INCLUDED_)
