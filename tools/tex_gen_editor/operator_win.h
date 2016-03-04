#if !defined(AFX_OPERATOR_WIN_H__D220DCE5_8589_43FA_8F3D_AACD6C5ED5DB__INCLUDED_)
#define AFX_OPERATOR_WIN_H__D220DCE5_8589_43FA_8F3D_AACD6C5ED5DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// operator_win.h : header file
//

#include "my_edit.h"
#include "port.h"
#include "bitmap_wnd.h"

namespace NLTEXGEN
{
class ITexGenOperator;
}

#define MAX_INPUT 4

/////////////////////////////////////////////////////////////////////////////
// COperatorWin window

class COperatorWin : public CWnd
{
	friend class CPortWnd;
// Construction
public:
	COperatorWin();

// Attributes
public:

// Operations
public:

	enum
	{
		NameID = 0,
		EnableID,
		OutputID,
		InputID,
		InputName = InputID + 4,
		PreviewID = InputName + 4,
	};

	// Create an operator window
	BOOL Create (CWnd *parent, class NLTEXGEN::ITexGenOperator *op);

	// Update an operator window
	void updateData (NLTEXGEN::ITexGenOperator *op);

	// Link this input with output
	void link (uint input, COperatorWin *output);

	// Link this input with output
	void unlink (uint input);

	// Hit test with ports
	CPortWnd *getPort (const POINT &point);

	// Get the output port
	const CPortWnd *getOutputPort () const { return &_Output; }

	// Get an intput  port
	const CPortWnd *getInputPort (uint port) const { return _Inputs+port; }

private:

	// The related dialog
	class COpPropertyDlg	*_Dialog;

	// The operator
	const NLTEXGEN::ITexGenOperator *_Op;

	// Handle move
	bool	_Moving;
	CPoint	_MoveOrigine;
	CPoint	_WindowPos;
	
	// Enable button
	CExtBtnOnFlat _Enable;

	// Output port
	CPortWnd	_Output;
	CPortWnd	_Inputs[MAX_INPUT];
	CStatic	_InputsName[MAX_INPUT];
	uint		_NumInputs;

	// The preview window
	CBitmapWnd	_Preview;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COperatorWin)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COperatorWin();

	// Generated message map functions
protected:
	//{{AFX_MSG(COperatorWin)
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnNcLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPERATOR_WIN_H__D220DCE5_8589_43FA_8F3D_AACD6C5ED5DB__INCLUDED_)
