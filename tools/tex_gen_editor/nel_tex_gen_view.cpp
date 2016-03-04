// nel_tex_gen_view.cpp : implementation of the CNelTexGenView class
//

#include "stdafx.h"
#include "nel_tex_gen.h"

#include "nel_tex_gen_doc.h"
#include "nel_tex_gen_view.h"
#include "operator_win.h"
#include "tex_gen/tex_gen_op.h"
#include "tex_gen/op_layer.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

#define TANG_Y_MIN 50
#define LINK_COLOR_COUNT 20

COLORREF LinkColors[LINK_COLOR_COUNT]=
{
	RGB (128, 0, 0),
	RGB (128, 128, 0),
	RGB (0, 128, 0),
	RGB (0, 128, 128),
	RGB (0, 0, 128),
	RGB (128, 0, 128),
	RGB (64, 0, 0),
	RGB (64, 64, 0),
	RGB (0, 64, 0),
	RGB (0, 64, 64),
	RGB (0, 0, 64),
	RGB (64, 0, 64),
	RGB (128, 64, 0),
	RGB (0, 128, 64),
	RGB (64, 0, 128),
	RGB (64, 128, 0),
	RGB (0, 64, 128),
	RGB (128, 0, 64),
};

// ***************************************************************************
// CNelTexGenView
// ***************************************************************************

IMPLEMENT_DYNCREATE(CNelTexGenView, CScrollView)

// ***************************************************************************

BEGIN_MESSAGE_MAP(CNelTexGenView, CScrollView)
	//{{AFX_MSG_MAP(CNelTexGenView)
	ON_WM_RBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CNelTexGenView construction/destruction
// ***************************************************************************

CNelTexGenView::CNelTexGenView()
{
	Vue = VueGraph;
	nlverify(OperatorFont.CreateFont(12, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, NULL));
	_ZoomFactor = 0;
}

// ***************************************************************************

CNelTexGenView::~CNelTexGenView()
{
	uninit();
}

// ***************************************************************************

BOOL CNelTexGenView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	CMyCreateContext *pContext=(CMyCreateContext*)cs.lpCreateParams;
	Vue = pContext->Vue;
	if (Vue == VueGraph)
		cs.style |= WS_CLIPCHILDREN;

	return CScrollView::PreCreateWindow(cs);
}

// ***************************************************************************
// CNelTexGenView drawing
// ***************************************************************************

void CNelTexGenView::updateOperator(NLTEXGEN::ITexGenOperator *op)
{
	if (Vue == VueGraph)
	{
		// Window exist ?
		std::map<NLTEXGEN::ITexGenOperator*, COperator>::iterator iteMap = _OpWinMap.find(op);
		if (iteMap == _OpWinMap.end())
		{
			// Create a window
			COperator opView;
			opView.Win = new COperatorWin;
			nlverify (opView.Win->Create (this, op) != FALSE);
			opView.Win->SetFont(&OperatorFont);

			// Add one entry
			iteMap = _OpWinMap.insert (std::map<NLTEXGEN::ITexGenOperator*, COperator>::value_type (op, opView)).first;
		}

		iteMap->second.Win->updateData(op);
	}
}

// ***************************************************************************

void CNelTexGenView::removeOperator(NLTEXGEN::ITexGenOperator *op)
{
	std::map<NLTEXGEN::ITexGenOperator*, COperator>::iterator ite = _OpWinMap.find (op);
	if (ite != _OpWinMap.end())
	{
		// Clear the window
		ite->second.erase();

		// Remove the iterator from the map
		_OpWinMap.erase(ite);
	}
}

// ***************************************************************************

void toVector (CVector &vect, const CPoint &point)
{
	vect.x = (float)point.x;
	vect.y = (float)point.y;
}

// ***************************************************************************

void CNelTexGenView::OnDraw(CDC* pDC)
{
	CNelTexGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPoint scrollPos = GetDeviceScrollPosition();

	if (Vue == VueGraph)
	{
		// Need to delete a drag ?
		if (DragValid)
		{
			CPen pen(PS_SOLID, 1, GetSysColor(COLOR_WINDOW));
			pDC->SelectObject(&pen);
			drawBezier (*pDC, DragBezier[0], DragBezier[1], DragBezier[2], DragBezier[3], 5);
			DragValid = false;
		}

		// Need a bezier for drag ?
		if (Drag[0] || Drag[1])
		{
			POINT p0, p1, p2, p3;
			
			if (Drag[0])
				Drag[0]->getBezierPos(p0);
			else
			{
				p0 = DragPos;
			}
			if (Drag[1])
				Drag[1]->getBezierPos(p3);
			else
			{
				p3 = DragPos;
			}
			getBezierTang (p0, p1, p2, p3);

			ScreenToClient(&p0);
			ScreenToClient(&p1);
			ScreenToClient(&p2);
			ScreenToClient(&p3);
			p0.x += scrollPos.x;
			p0.y += scrollPos.y;
			p1.x += scrollPos.x;
			p1.y += scrollPos.y;
			p2.x += scrollPos.x;
			p2.y += scrollPos.y;
			p3.x += scrollPos.x;
			p3.y += scrollPos.y;
			toVector (DragBezier[0], p0);
			toVector (DragBezier[1], p1);
			toVector (DragBezier[2], p2);
			toVector (DragBezier[3], p3);
			
			CPen pen(PS_SOLID, 1, RGB(0,0,0));
			pDC->SelectObject(&pen);
			drawBezier (*pDC, DragBezier[0], DragBezier[1], DragBezier[2], DragBezier[3], 5);
			DragValid = true;
		}

		// Draw links
		sint linkColor = 0;
		std::map<NLTEXGEN::ITexGenOperator*, COperator>::const_iterator ite = _OpWinMap.begin();
		while (ite != _OpWinMap.end())
		{
			const ITexGenOperator *op = ite->first;
			uint i;
			for (i=0; i<op->getNumInputBitmap(); i++)
			{
				const CPortWnd *input = ite->second.Win->getInputPort (i);
				if (input)
				{
					const ITexGenOperator *outputOp = op->getBindedOp(i);
					if (outputOp)
					{
						std::map<NLTEXGEN::ITexGenOperator*, COperator>::const_iterator iteOutput = _OpWinMap.find((NLTEXGEN::ITexGenOperator*)outputOp);
						nlassert (iteOutput != _OpWinMap.end());
						const CPortWnd *output = iteOutput->second.Win->getOutputPort ();
						if (output)
						{
							POINT p0, p1, p2, p3;
							CVector v[4];
							input->getBezierPos(p0);
							output->getBezierPos(p3);
							getBezierTang (p0, p1, p2, p3);

							ScreenToClient(&p0);
							ScreenToClient(&p1);
							ScreenToClient(&p2);
							ScreenToClient(&p3);
							p0.x += scrollPos.x;
							p0.y += scrollPos.y;
							p1.x += scrollPos.x;
							p1.y += scrollPos.y;
							p2.x += scrollPos.x;
							p2.y += scrollPos.y;
							p3.x += scrollPos.x;
							p3.y += scrollPos.y;
							toVector (v[0], p0);
							toVector (v[1], p1);
							toVector (v[2], p2);
							toVector (v[3], p3);

							CPen pen(PS_SOLID, 2, LinkColors[(linkColor++)%LINK_COLOR_COUNT]);
							pDC->SelectObject(&pen);
							drawBezier (*pDC, v[0], v[1], v[2], v[3], 5);
						}
					}
				}
			}
			ite++;
		}		
	}
	else
	{
		RECT client;
		GetClientRect (&client);
		int width = client.right-client.left;
		int height = client.bottom-client.top;

		int bmWidth = (_ZoomFactor>0)?Texture.getWidth()<<_ZoomFactor:Texture.getWidth()>>-_ZoomFactor;
		int bmHeight = (_ZoomFactor>0)?Texture.getHeight()<<_ZoomFactor:Texture.getHeight()>>-_ZoomFactor;

		int x = (width-(int)bmWidth)/2;
		int y = (height-(int)bmHeight)/2;
		if (width < (int)bmWidth)
			x = 0;
		if (height < (int)bmHeight)
			y = 0;

		if (pDoc->getTile())
		{
			sint yStart = (sint)y-(((y-1)/bmHeight)+1)*bmHeight;
			sint xStart = (sint)x-(((x-1)/bmWidth)+1)*bmWidth;
			while (yStart < client.bottom)
			{
				sint x2 = xStart;
				while (x2 < client.right)
				{
					Texture.draw (pDC, x2, yStart, _ZoomFactor);
					x2 += bmWidth;
				}
				yStart += bmHeight;
			}
		}
		else
		{
			// Draw the bitmap
			Texture.draw (pDC, x, y, _ZoomFactor);
		}
	}
}

// ***************************************************************************
// CNelTexGenView diagnostics
// ***************************************************************************

#ifdef _DEBUG
void CNelTexGenView::AssertValid() const
{
	CScrollView::AssertValid();
}

// ***************************************************************************

void CNelTexGenView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

// ***************************************************************************

CNelTexGenDoc* CNelTexGenView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNelTexGenDoc)));
	return (CNelTexGenDoc*)m_pDocument;
}
#endif //_DEBUG

// ***************************************************************************
// CNelTexGenView message handlers
// ***************************************************************************

void CNelTexGenView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (Vue == VueGraph)
	{
		CNelTexGenDoc* pDoc = GetDocument();
		if ((nFlags & MK_CONTROL) == 0)
			pDoc->unSelectAll();

		MenuPos = point;
		ClientToScreen (&point);

		// Check the sub menu
		CExtPopupMenuWnd * pPopupWnd = new CExtPopupMenuWnd;
		VERIFY(pPopupWnd->LoadMenu(*theApp.m_pMainWnd, IDR_VIEW_MENU));
		theApp.addOperatorsCreatorToMenu (*pPopupWnd);
		VERIFY(pPopupWnd->TrackPopupMenu(0, point.x, point.y));
	}
	
	CScrollView::OnRButtonDown(nFlags, point);
}

// ***************************************************************************

void CNelTexGenView::OnVueGraph() 
{
	Vue = VueGraph;
}

// ***************************************************************************

void CNelTexGenView::OnVueTexture() 
{
	Vue = VueTexture;
}

// ***************************************************************************

void toPoint (CPoint &point, const CVector &vect)
{
	point.x = (int)floor(vect.x+0.5f);
	point.y = (int)floor(vect.y+0.5f);
}

// ***************************************************************************

void drawBezier(CDC &dc, NLMISC::CVector &p0, NLMISC::CVector &p1, NLMISC::CVector &p2, NLMISC::CVector &p3, int steps)
{
	if (steps == 0)
	{
		CPoint p;
		toPoint (p, p0);
		dc.MoveTo(p);
		toPoint (p, p1);
		dc.LineTo(p);
		toPoint (p, p2);
		dc.LineTo(p);
		toPoint (p, p3);
		dc.LineTo(p);
	}
	else
	{
		CVector p01 = (p0+p1)/2;
		CVector p12 = (p1+p2)/2;
		CVector p23 = (p2+p3)/2;
		CVector p0112 = (p01+p12)/2;
		CVector p1223 = (p12+p23)/2;
		CVector p01121223 = (p0112+p1223)/2;
		drawBezier(dc, p0, p01, p0112, p01121223, steps-1);
		drawBezier(dc, p01121223, p1223, p23, p3, steps-1);
	}
}

// ***************************************************************************

void getBezierTang (const POINT &p0, POINT &p1, POINT &p2, const POINT &p3)
{
	p1.x = p0.x;
	p1.y = (p3.y - p0.y) / 3;
	if (p1.y < TANG_Y_MIN)
		p1.y = TANG_Y_MIN;
	p1.y += p0.y;
	p2.x = p3.x;
	p2.y = (p0.y - p3.y) / 3;
	if (p2.y > -TANG_Y_MIN)
		p2.y = -TANG_Y_MIN;
	p2.y += p3.y;
}

// ***************************************************************************

void CNelTexGenView::init()
{
	if (Vue == VueGraph)
	{
	}
	else if (Vue == VueTexture)
	{
	}
}

// ***************************************************************************

void CNelTexGenView::uninit()
{
}

// ***************************************************************************

void CNelTexGenView::COperator::erase()
{
	Win->DestroyWindow();
}

// ***************************************************************************

void CNelTexGenView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CScrollView::OnShowWindow(bShow, nStatus);
	
}

// ***************************************************************************

void CNelTexGenView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	uninit();
	init();

	// Update from document
	GetDocument()->updateView(this);
}

// ***************************************************************************

void CNelTexGenView::updateSize()
{
	if (Vue == VueTexture)
	{
		SIZE size;

		size.cx = (_ZoomFactor>0)?Texture.getWidth()<<_ZoomFactor:Texture.getWidth()>>-_ZoomFactor;
		size.cy = (_ZoomFactor>0)?Texture.getHeight()<<_ZoomFactor:Texture.getHeight()>>-_ZoomFactor;

		/* RECT rect;
		Texture.GetWindowRect(&rect);
		ScreenToClient(&rect);

		POINT pt;
		pt.x = -rect.left;
		pt.y = -rect.top; */

		// Update view size
		SetScrollSizes (MM_TEXT, size);
		// ScrollToPosition (pt);
	}
	if (Vue == VueGraph)
	{
		SIZE size;
		size.cx = 0;
		size.cy = 0;
		std::map<NLTEXGEN::ITexGenOperator*, COperator>::iterator ite = _OpWinMap.begin();
		while (ite != _OpWinMap.end())
		{
			// 
			RECT rect;
			ite->second.Win->GetWindowRect (&rect);
			ScreenToClient (&rect);
			rect.right += 10;
			rect.bottom += 10;
			size.cx = std::max(rect.right, size.cx);
			size.cy = std::max(rect.bottom, size.cy);

			ite++;
		}

		// Update view size
		SetScrollSizes (MM_TEXT, size);
	}
}

// ***************************************************************************

void CNelTexGenView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	updateSize();
}

// ***************************************************************************

void CNelTexGenView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	// centerTexture();
}

// ***************************************************************************
/*
void CNelTexGenView::centerTexture() 
{
	RECT client;
	GetClientRect(&client);

	// Texture ?
	if (Vue == VueTexture)
	{
	}
}*/

// ***************************************************************************

BOOL CNelTexGenView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//if (Vue == VueGraph)
		return CScrollView::OnEraseBkgnd(pDC);
	/*else
		return FALSE;*/
}

// ***************************************************************************

void CNelTexGenView::invalidateBitmap()
{
	if (Vue == VueTexture)
	{
		Invalidate ();
	}
}

// ***************************************************************************

void CNelTexGenView::updateBitmap()
{
	if (Vue == VueTexture)
	{
		NLTEXGEN::CFloatBitmap bitmap;
		CRenderParameter renderParameters (true, Draft, false);
		
		// Get the first selected operator
		CNelTexGenDoc* pDoc = GetDocument();

		bool alphaPremultiplied = false;

		if (pDoc->getViewMode() == CNelTexGenDoc::ViewModeOperator)
		{
			uint i;
			ITexGenOperator *op = NULL;
			for (i=0; i<pDoc->NeLDoc.Operators.size(); i++)
			{
				if (pDoc->NeLDoc.Operators[i]->Selected)
				{
					op = pDoc->NeLDoc.Operators[i];
					break;
				}
			}

			if (!op)
				op = pDoc->NeLDoc.Operators[0];

			// Update the texture

			// Eval the bitmap with cache
			op->eval (bitmap, renderParameters);

			// Layer ?
			IOpLayer *layer = dynamic_cast<IOpLayer*>(op);
			if (!pDoc->getLayerResult() && layer)
			{
				bitmap.ghostCopy (&(layer->getCachedLayer()));
				alphaPremultiplied = layer->outputMask();
			}
		}
		else
		{
			if (pDoc->getViewMode() == CNelTexGenDoc::ViewModeRGB)
				pDoc->NeLDoc.Operators[0]->eval (bitmap, renderParameters);
			else
				((COpRoot*)(pDoc->NeLDoc.Operators[0]))->evalAlpha (bitmap, renderParameters);
		}

		// Set the texture size
		Texture.setPixels (bitmap, (pDoc->getViewMode() == CNelTexGenDoc::ViewModeAlpha) ? CDibWnd::Alpha : alphaPremultiplied ? CDibWnd::AlphaPremultiplied : CDibWnd::Normal);
		Invalidate();
	
		// Update size
		updateSize();
		// centerTexture();
	}
}

// ***************************************************************************

void CNelTexGenView::updateGraph()
{
	if (Vue == VueGraph)
	{
		Invalidate();
	}
}

// ***************************************************************************

void CNelTexGenView::zoomIn(bool resizeLayout)
{
	if (Vue == VueTexture)
	{
		if (_ZoomFactor < 6)
		{
			_ZoomFactor++;
			Invalidate();
			updateSize();
		}
	}
}

// ***************************************************************************

void CNelTexGenView::zoomOut(bool resizeLayout)
{
	if (Vue == VueTexture)
	{
		_ZoomFactor--;
		Invalidate();
		updateSize();
	}
}

// ***************************************************************************

void CNelTexGenView::validateChildren()
{
	std::map<NLTEXGEN::ITexGenOperator*, COperator>::iterator ite = _OpWinMap.begin();
	while (ite != _OpWinMap.end())
	{
		RECT rect;
		ite->second.Win->GetWindowRect (&rect);
		ScreenToClient(&rect);
		ValidateRect(&rect);
		ite++;
	}
}

// ***************************************************************************

CPortWnd *CNelTexGenView::getPort (const POINT &point)
{
	std::map<NLTEXGEN::ITexGenOperator*, COperator>::const_iterator ite = _OpWinMap.begin();
	while (ite != _OpWinMap.end())
	{
		CPortWnd *port;
		if (port = ite->second.Win->getPort (point))
			return port;
		ite++;
	}
	return NULL;
}

// ***************************************************************************

COperatorWin *CNelTexGenView::getOperatorWin (const NLTEXGEN::ITexGenOperator *op)
{
	std::map<NLTEXGEN::ITexGenOperator*, COperator>::iterator ite = _OpWinMap.find((NLTEXGEN::ITexGenOperator*)op);
	nlassert(ite != _OpWinMap.end());
	return ite->second.Win;
}

// ***************************************************************************

void CNelTexGenView::selectionChange(const NLTEXGEN::ITexGenOperator *op)
{
	if (Vue == VueGraph)
	{
		class COperatorWin *opWin = getOperatorWin (op);
		opWin->Invalidate();
	}
}

// ***************************************************************************

void CNelTexGenView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (Vue == VueGraph)
	{
		CNelTexGenDoc* pDoc = GetDocument();
		if ((nFlags & MK_CONTROL) == 0)
			pDoc->unSelectAll();

		pDoc->updateData();
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

// ***************************************************************************

void CNelTexGenView::OnEditDelete() 
{
	CNelTexGenDoc* pDoc = GetDocument();
	pDoc->OnEditDelete();
}

// ***************************************************************************

void CNelTexGenView::OnUpdateEditDelete(CCmdUI* pCmdUI)
{
	CNelTexGenDoc* pDoc = GetDocument();	
	pDoc->OnUpdateEditDelete(pCmdUI);
}

// ***************************************************************************

void CNelTexGenView::setTopMost (CWnd *wnd)
{
	//GetTopWindow()
}

// ***************************************************************************
