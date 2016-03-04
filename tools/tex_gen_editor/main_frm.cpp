// main_frm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "nel_tex_gen.h"

#include "main_frm.h"

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_FULL_SCREEN, OnViewFullScreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULL_SCREEN, OnUpdateViewFullScreen)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bFullScreen = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/*if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}*/

CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	ASSERT( pApp->m_pszProfileName != NULL );
	g_CmdManager->ProfileSetup(
		pApp->m_pszProfileName,
		GetSafeHwnd()
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_MAINFRAME
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_TEX_GETYPE
			)
		);

	m_wndMenuBar.SetMdiWindowPopupName( _T("Window") );
	if( !m_wndMenuBar.Create(
			NULL, // _T("Menu Bar"),
			this,
			ID_VIEW_MENUBAR
			)
		)
    {
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }

	if( !m_wndToolBar.Create(
			_T("ToolBar"),
			this,
			AFX_IDW_TOOLBAR
			)
		||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if( !m_wndToolBar2.Create(
			_T("ToolBar 2"),
			this,
			IDR_FULLSCREEN
			)
		||
		!m_wndToolBar2.LoadToolBar(IDR_FULLSCREEN)
		)
	{
		TRACE0("Failed to create toolbar3\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar2.EnableDocking(CBRS_ALIGN_ANY);

	if( !CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
	
	/*if(	!CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP
			)
		)
	{*/
		DockControlBar(&m_wndMenuBar);
		DockControlBar(&m_wndToolBar);
		RecalcLayout();
		CRect wrAlredyDockedBar;
		m_wndToolBar.GetWindowRect( &wrAlredyDockedBar );
		wrAlredyDockedBar.OffsetRect( 1, 0);
		DockControlBar(&m_wndToolBar2, (UINT)0, &wrAlredyDockedBar);
		RecalcLayout();
	//}

	g_CmdManager->SerializeState(
		pApp->m_pszProfileName,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		false
		);

	/*if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);*/

	// Register operators

	UINT id = ID_EDIT_ADD_OPERATOR;
	theApp.AddFolder (id++, "Inputs");
		theApp.AddOperator (id++, "Bg", "Background");
	theApp.AddFolder (id++, "Mixing");
		theApp.AddOperator (id++, "Mi", "Merge");
	theApp.AddFolder (id++, "Shapes");
		theApp.AddOperator (id++, "Ft", "Text");
		theApp.AddOperator (id++, "Qd", "Rectangle");
	theApp.AddFolder (id++, "Color fillers");
		theApp.AddOperator (id++, "FC", "Solid color");
		theApp.AddSeparator (id++);
		theApp.AddOperator (id++, "LG", "Linear gradient");
		theApp.AddOperator (id++, "RG", "Radial gradient");
		theApp.AddOperator (id++, "AG", "Angular gradient");
		theApp.AddOperator (id++, "Ba", "Bars gradient");
	theApp.AddFolder (id++, "Image modification");
		theApp.AddOperator (id++, "CL", "Contrast / luminosity");
		theApp.AddOperator (id++, "HS", "Hue / saturation");
		theApp.AddOperator (id++, "Bl", "Blur");
		theApp.AddOperator (id++, "Gr", "Gradient image");
	theApp.AddFolder (id++, "Image deformation");
		theApp.AddOperator (id++, "Tr", "Transform");
		theApp.AddOperator (id++, "Si", "Sinus");
	theApp.AddFolder (id++, "Noise fillers");
		theApp.AddOperator (id++, "Ns", "Noise");
		theApp.AddOperator (id++, "Cl", "Clouds");
		theApp.AddOperator (id++, "Cr", "Cellular");
	theApp.AddFolder (id++, "Lighting");
		theApp.AddOperator (id++, "No", "Convert to normal");
		theApp.AddOperator (id++, "Li", "Directionnal light");

	VERIFY( doBarStateSerialization(false,false,true) );

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.x = 0;
	cs.y = 0;
	cs.cx = 1024;
	cs.cy = 768;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnViewFullScreen() 
{
	doViewFullScreen( true );
}

void CMainFrame::OnUpdateViewFullScreen(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_bFullScreen);
}

void CMainFrame::doViewFullScreen( bool bSavePrevLayout )
{
	if( m_bFullScreen )
	{
		m_bFullScreen = false;
		if( bSavePrevLayout )
		{
			VERIFY( doBarStateSerialization(true,true) );
		}
		SetWindowPlacement( &m_wpSavedBeforeFullScreen );
		VERIFY( doBarStateSerialization(false,false) );
	} // if( m_bFullScreen )
	else
	{
		GetWindowPlacement( &m_wpSavedBeforeFullScreen );
		CRect rcDesktop;
		::GetWindowRect( ::GetDesktopWindow(), &rcDesktop );
		CRect rcMdiClient;
		GetClientRect( &rcMdiClient );
		ClientToScreen( &rcMdiClient );
		CRect rcMdiFrame;
		GetWindowRect( &rcMdiFrame );
		rcMdiFrame.InflateRect(
			rcMdiClient.left-rcDesktop.left,
			rcMdiClient.top-rcDesktop.top,
			rcDesktop.right-rcMdiClient.right,
			rcDesktop.bottom-rcMdiClient.bottom
			);
		WINDOWPLACEMENT wpFullScreen;
		::memcpy( &wpFullScreen, &m_wpSavedBeforeFullScreen, sizeof(WINDOWPLACEMENT) );
		wpFullScreen.showCmd = SW_SHOWNORMAL;
		wpFullScreen.rcNormalPosition = rcMdiFrame;
		wpFullScreen.ptMaxPosition = rcMdiFrame.TopLeft();
		m_sizeFullScreenDesktopDifference =
			rcMdiFrame.Size() - rcDesktop.Size();
		m_bFullScreen = true;
		if( bSavePrevLayout )
		{
			VERIFY( doBarStateSerialization(false,true) );
		}
		SetWindowPlacement( &wpFullScreen );
		VERIFY( doBarStateSerialization(true,false) );
	} // else from if( m_bFullScreen )
}

bool CMainFrame::doBarStateSerialization(
	bool bFullScreenState,
	bool bSaveState,
	bool bInitialLoad // = false
	)
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );
CString sSpan = pApp->m_pszProfileName;
	sSpan += _T("\\");
	sSpan +=
		bFullScreenState
		? _T("BarPos-FullScreen")
		: _T("BarPos-Normal")
		;
	RecalcLayout();
bool bRetVal = false;
	if( bSaveState )
		bRetVal =
			CExtControlBar::ProfileBarStateSave(
				this,
				pApp->m_pszRegistryKey,
				pApp->m_pszProfileName,
				sSpan
				);
	else
		bRetVal =
			CExtControlBar::ProfileBarStateLoad(
				this,
				pApp->m_pszRegistryKey,
				pApp->m_pszProfileName,
				sSpan,
				&m_wpInitial
				);
	if( !bSaveState )
	{
		if( !bRetVal )
		{
			// doInitialDock( bFullScreenState );
			bRetVal = true;
		}
		else if( (!bInitialLoad) && (!m_bFullScreen) )
		{
			ActivateFrame( m_wpInitial.showCmd );
			RedrawWindow( NULL, NULL,
				RDW_INVALIDATE|RDW_UPDATENOW
					|RDW_ERASE|RDW_ERASENOW
					|RDW_FRAME|RDW_ALLCHILDREN
				);
		}
		m_wpInitial.showCmd = SW_HIDE;
	}
	return bRetVal;
}

void CMainFrame::OnClose() 
{
	VERIFY( doBarStateSerialization(m_bFullScreen!=0,true) );
	
	CMDIFrameWnd::OnClose();
}
