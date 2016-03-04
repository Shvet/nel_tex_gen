// port.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "nel_tex_gen_doc.h"
#include "operator_win.h"
#include "tex_gen/tex_gen_op.h"
#include "nel_tex_gen_view.h"
#include "port.h"

using namespace NLTEXGEN;

// ***************************************************************************
// CPortWnd
// ***************************************************************************

CPortWnd::CPortWnd()
{
	_Drag = false;
}

// ***************************************************************************

CPortWnd::~CPortWnd()
{
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CPortWnd, CWnd)
	//{{AFX_MSG_MAP(CPortWnd)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// CPortWnd message handlers
// ***************************************************************************

BOOL CPortWnd::Create(const char *name, DWORD style, RECT &rect, CWnd *parent, UINT nID, bool output, uint input)
{
	_Output = output;
	_Input = input;
	LPCTSTR className = AfxRegisterWndClass (0, 0, (HBRUSH)COLOR_WINDOW, 0);
	if (CWnd::Create (className, name, style, rect, parent, nID))
	{
		rect.bottom = rect.bottom-rect.top;
		rect.right = rect.right-rect.left;
		rect.top = 0;
		rect.left = 0;
		Label.Create ("", WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_CENTERIMAGE, rect, this, 0);
		Label.SetBitmap(output?ArrowBitmapOutput:ArrowBitmapInput);
		return TRUE;
	}
	return FALSE;
}

// ***************************************************************************

void CPortWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// Drop..
	if (_Drag)
	{
		// Get the drop port
		CNelTexGenView *view = (CNelTexGenView*)(GetParent()->GetParent());
		ClientToScreen (&point);
		CPortWnd *dropPort = view->getPort (point);
		if (dropPort)
		{
			Drag[(int)dropPort->_Output] = dropPort;
		
			// Link two operators
			if ((Drag[0] && Drag[1]) && (Drag[0] != Drag[1]))
			{
				COperatorWin *input = (COperatorWin*)(Drag[0]->GetParent());
				COperatorWin *output = (COperatorWin*)(Drag[1]->GetParent());
				input->link(Drag[0]->_Input, output);
			}
		}

		CNelTexGenDoc *doc = view->GetDocument();
		doc->endModification ();
		doc->updateData();

		view->Invalidate(FALSE);
		// view->validateChildren();
		ReleaseCapture();
		Drag[0] = NULL;
		Drag[1] = NULL;
		_Drag = false;
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

// ***************************************************************************

void CPortWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Drag..
	SetCapture();
	_Drag = true;
	Drag[(int)_Output] = this;
	Drag[(int)(!_Output)] = NULL;
	ClientToScreen(&point);
	DragPos = point;

	if (!_Output)
	{
		COperatorWin *wnd = (COperatorWin*)(GetParent());
		const ITexGenOperator *target = wnd->_Op->getBindedOp(_Input);
		if (target)
		{
			COperatorWin *opTarget = ((CNelTexGenView*)(GetParent()->GetParent()))->getOperatorWin (target);
			Drag[1] = (CPortWnd*)(opTarget->getOutputPort());
			Drag[0] = NULL;
			wnd->unlink(_Input);
		}
	}


	CWnd::OnLButtonDown(nFlags, point);
}

// ***************************************************************************

void CPortWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (_Drag)
	{
		ClientToScreen(&point);
		DragPos = point;
		GetParent()->GetParent()->Invalidate(FALSE);
		((CNelTexGenView*)(GetParent()->GetParent()))->validateChildren();
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

// ***************************************************************************

void CPortWnd::getBezierPos (POINT &pt) const
{
	RECT rect;
	if (_Output)
	{
		GetWindowRect (&rect);
		pt.x = (rect.right + rect.left)/2;
		pt.y = rect.top+1;
	}
	else
	{
		GetWindowRect (&rect);
		pt.x = (rect.right + rect.left)/2;
		pt.y = rect.bottom-1;
	}
}

// ***************************************************************************

