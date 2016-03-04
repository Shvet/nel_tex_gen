#if !defined(AFX_GRADIENT_WND_H__29589CA6_057F_494E_8453_29BD5670C74E__INCLUDED_)
#define AFX_GRADIENT_WND_H__29589CA6_057F_494E_8453_29BD5670C74E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// gradient_wnd.h : header file
//

#include "gradient_rect_wnd.h"
#include "tex_gen\basics.h"

#define CURSOR_WIDTH 9
#define CURSOR_HEIGHT 18

#define CGW_GRADIENT_CHANGED 0x1113

// ***************************************************************************
// CGradientWnd window
// ***************************************************************************

class CGradientWnd : public CWnd
{
	friend class CGradientRectWnd;
	friend class CGradientCursorWnd;
// Construction
public:
	CGradientWnd();

// Attributes
public:

	void setGradient(const std::vector<NLTEXGEN::CGradientColor> &gradient);
	const std::vector<NLTEXGEN::CGradientColor>	&getGradient() const
	{
		return _GradientWnd.getGradient();
	}
	BOOL Create (DWORD dwStyle, RECT &rect, CWnd *parent, UINT nID);
	void getCursorMinMax(RECT &rect) const;

	float getPos(int clientX) const;
	int getClientX(float pos) const;

private:

	void resizeChildren();
	void cursorChanged();
	void tangentChanged();
	void gradientChanged(bool notifyParent);
	void cursorDeleted(uint child);
	void cursorDuplicate(uint child);

	enum
	{
		Gradient,
		FirstCursor
	};

	// The windows
	CGradientRectWnd					_GradientWnd;
	std::vector<class CGradientCursorWnd*>	_CursorsWnd;
	std::vector<class CGradientCursorWnd*>	_CursorsTangentWnd;
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradientWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGradientWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGradientWnd)
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ***************************************************************************

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADIENT_WND_H__29589CA6_057F_494E_8453_29BD5670C74E__INCLUDED_)
