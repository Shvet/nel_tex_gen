// main_frm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAIN_FRM_H__277CC08A_74CD_488F_80DA_9FF40550274F__INCLUDED_)
#define AFX_MAIN_FRM_H__277CC08A_74CD_488F_80DA_9FF40550274F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

	// window placement persistence
	WINDOWPLACEMENT m_wpInitial;
	// Fullscreen
	BOOL	m_bFullScreen;
	WINDOWPLACEMENT m_wpSavedBeforeFullScreen;

	// persistent size for OnGetMinMaxInfo() in full-screen mode
	CSize m_sizeFullScreenDesktopDifference;

	void doViewFullScreen( bool bSavePrevLayout );
	bool doBarStateSerialization( bool bFullScreenState, bool bSaveState, bool bInitialLoad = false);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public: // for child frame
	CExtMenuControlBar    m_wndMenuBar;
protected:  // control bar embedded members
	CExtStatusControlBar  m_wndStatusBar;
	CExtToolControlBar	m_wndToolBar;
	CExtToolControlBar	m_wndToolBar2;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewFullScreen();
	afx_msg void OnUpdateViewFullScreen(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAIN_FRM_H__277CC08A_74CD_488F_80DA_9FF40550274F__INCLUDED_)
