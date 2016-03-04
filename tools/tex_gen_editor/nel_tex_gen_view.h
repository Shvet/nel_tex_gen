// nel_tex_gen_view.h : interface of the CNelTexGenView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEL_TEX_GEN_VIEW_H__DF56FC40_A0C1_4FE9_904C_583545DCF21C__INCLUDED_)
#define AFX_NEL_TEX_GEN_VIEW_H__DF56FC40_A0C1_4FE9_904C_583545DCF21C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bitmap_wnd.h"

namespace NLTEXGEN
{
	class ITexGenOperator;
};

class CNelTexGenView : public CScrollView
{
	friend class CChildFrame;
protected: // create from serialization only
	CNelTexGenView();
	DECLARE_DYNCREATE(CNelTexGenView)

// Attributes
public:
	class CNelTexGenDoc* GetDocument();

	// Set operator window top
	void setTopMost (CWnd *wnd);

	// Remove an operator
	void removeOperator(NLTEXGEN::ITexGenOperator *op);

	// An operator as been modified
	void updateOperator(NLTEXGEN::ITexGenOperator *op);

	// Update view size
	void updateSize();

	// Update the bitmap
	void updateBitmap();

	// Invalidate the bitmap view (do not recompute the bitmap)
	void invalidateBitmap();

	// Update the graph
	void updateGraph();

	// The selection state of an operator has changed
	void selectionChange(const NLTEXGEN::ITexGenOperator *op);

	// Font used by the opertors
	CFont	OperatorFont;

	// Zoom
	void zoomIn (bool resizeLayout);
	void zoomOut (bool resizeLayout);

	enum TVue
	{
		VueTexture,
		VueGraph,
	};

	// Validate all operator windows
	void validateChildren();

	// Hit test with ports
	CPortWnd *getPort (const POINT &point);

	// Get the operator wnd
	class COperatorWin *getOperatorWin (const NLTEXGEN::ITexGenOperator *op);

// ***************************************************************************


private:

	TVue	Vue;

	// Init / uninit the view
	void	init();
	void	uninit();

	// \name Graph part
	class COperator
	{
	public:
		class COperatorWin	*Win;

		void erase();
	};

	std::map<NLTEXGEN::ITexGenOperator*, COperator>		_OpWinMap;

	// \name Texture part

	CDibWnd	Texture;

	// void centerTexture ();

	// Zoom factor
	sint	_ZoomFactor;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNelTexGenView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNelTexGenView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNelTexGenView)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnVueGraph();
	afx_msg void OnVueTexture();
	DECLARE_MESSAGE_MAP()
};

class CMyCreateContext : public CCreateContext
{
public:
	CNelTexGenView::TVue	Vue;
};

#ifndef _DEBUG  // debug version in nel_tex_gen_view.cpp
inline CNelTexGenDoc* CNelTexGenView::GetDocument()
   { return (CNelTexGenDoc*)m_pDocument; }
#endif

void drawBezier(CDC &dc, NLMISC::CVector &p0, NLMISC::CVector &p1, NLMISC::CVector &p2, NLMISC::CVector &p3, int steps);
void getBezierTang (const POINT &p0, POINT &p1, POINT &p2, const POINT &p3);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEL_TEX_GEN_VIEW_H__DF56FC40_A0C1_4FE9_904C_583545DCF21C__INCLUDED_)
