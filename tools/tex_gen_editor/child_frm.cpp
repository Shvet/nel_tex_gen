// child_frm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "nel_tex_gen_view.h"

#include "child_frm.h"

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::OnCreateClient( LPCREATESTRUCT lpcs,
	CCreateContext* pContext)
{
	if (m_wndSplitter.CreateStatic(this, 1, 2))
	{
		CMyCreateContext myContext;
		myContext.m_pCurrentDoc = pContext->m_pCurrentDoc;
		myContext.m_pCurrentFrame = pContext->m_pCurrentFrame;
		myContext.m_pLastView = pContext->m_pLastView;
		myContext.m_pNewDocTemplate = pContext->m_pNewDocTemplate;
		myContext.m_pNewViewClass = pContext->m_pNewViewClass;
		myContext.Vue = CNelTexGenView::VueTexture;
		if (m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CNelTexGenView), CSize( 550, 550 ), &myContext))
		{
			myContext.Vue = CNelTexGenView::VueGraph;
			if (m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CNelTexGenView), CSize( 800, 550 ), &myContext))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.cx = 900;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
