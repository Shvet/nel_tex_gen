// operator_win.cpp : implementation file
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "action.h"
#include "operator_win.h"
#include "nel_tex_gen_view.h"
#include "nel_tex_gen_doc.h"
#include "tex_gen/tex_gen_op.h"
#include "tex_gen/op_root.h"
#include "op_property_dlg.h"

#define OPERATOR_ENABLE_X 0
#define OPERATOR_ENABLE_WIDTH 15
#define OPERATOR_ENABLE_HEIGHT 17
#define OPERATOR_ENABLE_Y (-1)

#define OPERATOR_EDIT_WIDTH 83
#define OPERATOR_EDIT_HEIGHT 17
#define OPERATOR_EDIT_X 15
#define OPERATOR_EDIT_Y (-1)

#define OPERATOR_OUTPUT_WIDTH 17
#define OPERATOR_OUTPUT_HEIGHT 17
#define OPERATOR_OUTPUT_X 105
#define OPERATOR_OUTPUT_Y (-1)

#define OPERATOR_INPUT_WIDTH 17
#define OPERATOR_INPUT_HEIGHT 17
#define OPERATOR_INPUT_X 8
#define OPERATOR_INPUT_Y 33
#define OPERATOR_INPUT_SPACE 15

#define OPERATOR_INPUT_NAME_WIDTH 32
#define OPERATOR_INPUT_NAME_HEIGHT 10
#define OPERATOR_INPUT_NAME_X 0
#define OPERATOR_INPUT_NAME_Y 23

#define OPERATOR_PREVIEW_WIDTH 48
#define OPERATOR_PREVIEW_HEIGHT 48
#define OPERATOR_PREVIEW_X 129
#define OPERATOR_PREVIEW_Y 0

#define OPERATOR_TITLE_LEFT_MARGIN 4

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************
// COperatorWin
// ***************************************************************************

COperatorWin::COperatorWin()
{
	_Moving = false;
}

// ***************************************************************************

COperatorWin::~COperatorWin()
{
}

// ***************************************************************************

BOOL COperatorWin::Create (CWnd *parent, NLTEXGEN::ITexGenOperator *op)
{
	COpRoot *rootOp = dynamic_cast<COpRoot*>(op);
	_Op = op;
	LPCTSTR className = AfxRegisterWndClass (CS_DBLCLKS, 0, (HBRUSH)COLOR_WINDOW, 0);
	RECT rect;
	rect.top = op->Y;
	rect.left = op->X;
	rect.right = rect.left + OPERATOR_WIN_WIDTH;
	rect.bottom = rect.top + OPERATOR_WIN_HEIGHT;
	if (CWnd::Create(className, "", WS_BORDER|WS_OVERLAPPED|WS_VISIBLE, rect, parent, 0, NULL))
	{
		SetFont(&(((CNelTexGenView*)GetParent())->OperatorFont));

		// Create a check box
		rect.left = OPERATOR_ENABLE_X;
		rect.top = OPERATOR_ENABLE_Y;
		rect.right = OPERATOR_ENABLE_X+OPERATOR_ENABLE_WIDTH;
		rect.bottom = OPERATOR_ENABLE_Y+OPERATOR_ENABLE_HEIGHT;
		
		if (_Enable.Create("", BS_AUTOCHECKBOX|WS_CHILD|(rootOp?0:WS_VISIBLE), rect, this, EnableID))
		{
			// Support output ?
			if (typeid(*op) != typeid(COpRoot))
			{
				rect.left = OPERATOR_OUTPUT_X;
				rect.top = OPERATOR_OUTPUT_Y;
				rect.right = OPERATOR_OUTPUT_X+OPERATOR_OUTPUT_WIDTH;
				rect.bottom = OPERATOR_OUTPUT_Y+OPERATOR_OUTPUT_HEIGHT;
				_Output.Create("", WS_VISIBLE|WS_CHILD, rect, this, OutputID, true, 0);
			}

			uint i;
			_NumInputs = std::min((uint)MAX_INPUT, op->getNumInputBitmap ());
			for (i=0; i<_NumInputs; i++)
			{
				rect.left = OPERATOR_INPUT_X + (OPERATOR_INPUT_WIDTH+OPERATOR_INPUT_SPACE) * i;
				rect.top = OPERATOR_INPUT_Y;
				rect.right = OPERATOR_INPUT_X+OPERATOR_INPUT_WIDTH + (OPERATOR_INPUT_WIDTH+OPERATOR_INPUT_SPACE) * i;
				rect.bottom = OPERATOR_INPUT_Y+OPERATOR_INPUT_HEIGHT;
				_Inputs[i].Create("", WS_VISIBLE|WS_CHILD, rect, this, InputID+i, false, i);

				rect.left = OPERATOR_INPUT_NAME_X + OPERATOR_INPUT_NAME_WIDTH * i;
				rect.top = OPERATOR_INPUT_NAME_Y;
				rect.right = rect.left + OPERATOR_INPUT_NAME_WIDTH;
				rect.bottom = rect.top + OPERATOR_INPUT_NAME_HEIGHT;
				string name = op->getInputBitmapDescriptor (i).Name;
				name[0] = toupper (name[0]);
				_InputsName[i].Create(name.c_str(), WS_VISIBLE|WS_CHILD|SS_CENTER, 
					rect, this, InputName+i);
				_InputsName[i].SetFont(&(((CNelTexGenView*)GetParent())->OperatorFont));
			}

			// Create the dialog
			_Dialog = new COpPropertyDlg;
			if (_Dialog->Create (this, _Op))
			{
				// Create the preview window
				rect.left = OPERATOR_PREVIEW_X;
				rect.top = OPERATOR_PREVIEW_Y;
				rect.right = OPERATOR_PREVIEW_X + OPERATOR_PREVIEW_WIDTH;
				rect.bottom = OPERATOR_PREVIEW_Y + OPERATOR_PREVIEW_HEIGHT;
				_Preview.Create (WS_CHILD|WS_VISIBLE, rect, this, PreviewID, 
					const_cast<NLTEXGEN::ITexGenOperator*>(_Op), true);
				return TRUE;
			}
		}
	}

	return FALSE;
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(COperatorWin, CWnd)
	//{{AFX_MSG_MAP(COperatorWin)
	ON_WM_NCHITTEST()
	ON_WM_MOVE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_MOVING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// COperatorWin message handlers
// ***************************************************************************

UINT COperatorWin::OnNcHitTest(CPoint point) 
{
	CRect r;
	GetClientRect(&r);
	ClientToScreen(&r);

	if(r.PtInRect(point))
		return HTCAPTION  ;
	
	return CWnd::OnNcHitTest(point);
}

// ***************************************************************************

void COperatorWin::OnMove(int x, int y) 
{
	CWnd::OnMove(x, y);
	
	// Update parent size
	// ((CNelTexGenView*)GetParent())->updateSize();

	GetParent()->Invalidate(TRUE);

	// Force position
	RECT rect;
	GetWindowRect (&rect);
	GetParent()->ScreenToClient (&rect);

	((NLTEXGEN::ITexGenOperator*)_Op)->X = rect.left;
	((NLTEXGEN::ITexGenOperator*)_Op)->Y = rect.top;
	// ((CNelTexGenView*)(GetParent()))->validateChildren();
}

// ***************************************************************************

BOOL COperatorWin::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CNelTexGenDoc *doc = ((CNelTexGenView*)GetParent())->GetDocument();
	// Change the name ?
	switch (HIWORD(wParam))
	{
	case BN_CLICKED:
		// Enable ?
		if (LOWORD(wParam) == EnableID)
		{
			doc->addModification(new CActionEnable(doc->getOperatorId(_Op), _Enable.GetCheck()!=0));
			doc->endModification();
			doc->updateData ();
		}
		break;
	case EN_KILLFOCUS:
	case EN_RETURN:
		// Name ?
		break;
	}
	
	return CWnd::OnCommand(wParam, lParam);
}

// ***************************************************************************

void COperatorWin::updateData (NLTEXGEN::ITexGenOperator *op)
{	
	_Enable.SetCheck(op->isEnable()?1:0);
	SetWindowPos (NULL, op->X, op->Y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	_Dialog->updateData ();
	_Preview.invalidate ();
}

// ***************************************************************************

void COperatorWin::link (uint input, COperatorWin *output)
{
	// Is linked ?
	if(_Op->getBindedOp(input) != NULL)
		unlink (input);

	// Is linked ?
/*	const NLTEXGEN::ITexGenOperator *outputOp = output->_Op;
	const vector<NLTEXGEN::ITexGenOperator *> outputParentOp = outputOp->getParents();
	if (outputParentOp.size())
	{
		uint j;
		uint i;
		for (j=0; j<outputParentOp.size(); j++)
		{
			// Search output in parent
			for (i=0; i<outputParentOp[j]->getNumInputBitmap(); i++)
			{
				if (((const NLTEXGEN::ITexGenOperator *)(outputParentOp[j]))->getBindedOp(i) == outputOp)
					break;
			}
			if (i<outputParentOp[j]->getNumInputBitmap())
				break;
		}

		nlassert (j<outputParentOp.size());
		nlassert (i<outputParentOp[j]->getNumInputBitmap());

		class COperatorWin *parentOp = ((CNelTexGenView*)GetParent())->getOperatorWin (outputParentOp[j]);
		parentOp->unlink (i);
	}*/

	CNelTexGenDoc *doc = ((CNelTexGenView*)GetParent())->GetDocument();
	doc->addModification (new CActionLink (doc->getOperatorId(_Op), input, doc->getOperatorId(output->_Op)));
}

// ***************************************************************************

void COperatorWin::unlink (uint input)
{
	const ITexGenOperator *target = ((const ITexGenOperator*)_Op)->getBindedOp(input);
	if (target)
	{
		CNelTexGenDoc *doc = ((CNelTexGenView*)GetParent())->GetDocument();
		doc->addModification (new CActionUnlink (doc->getOperatorId(_Op), input, doc->getOperatorId(target)));
	}
}

// ***************************************************************************

CPortWnd *COperatorWin::getPort (const POINT &point)
{
	if (IsWindow(_Output))
	{
		CRect rect;
		_Output.GetWindowRect(rect);
		if (rect.PtInRect (point) != FALSE)
			return &_Output;
	}
	uint i;
	for (i=0; i<_NumInputs; i++)
	{
		CRect rect;
		_Inputs[i].GetWindowRect(rect);
		if (rect.PtInRect (point) != FALSE)
			return _Inputs+i;
	}
	return NULL;
}

// ***************************************************************************

void COperatorWin::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// Show the property dialog
	if (_Dialog->IsWindowVisible ())
	{
		_Dialog->FlashWindow(TRUE);
		_Dialog->SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE);
	}
	else
		_Dialog->SetWindowPos (&wndTop, point.x, point.y, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE);
	
	CWnd::OnNcLButtonDblClk(nHitTest, point);
}

// ***************************************************************************

void COperatorWin::OnDestroy() 
{
	CWnd::OnDestroy();
	_Dialog->PostMessage(WM_DESTROY, 0, 0);
	delete this;
}

// ***************************************************************************

void COperatorWin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (_Op)
	{
		const COpRoot *rootOp = dynamic_cast<const COpRoot*>(_Op);

		// Draw the label box
		RECT rect;
		rect.left = rootOp?-1:OPERATOR_EDIT_X;
		rect.top = OPERATOR_EDIT_Y;
		rect.right = rect.left+OPERATOR_EDIT_WIDTH;
		rect.bottom = OPERATOR_EDIT_Y+OPERATOR_EDIT_HEIGHT;

		// Select the font
		dc.SelectObject (&(((CNelTexGenView*)GetParent())->OperatorFont));

		// Draw the box
		CBrush brush (GetSysColor(_Op->Selected?COLOR_ACTIVECAPTION:COLOR_INACTIVECAPTION));
		dc.FillRect (&rect, &brush);

		// Draw the title
		const char *name = theApp.getOperatorName (const_cast<ITexGenOperator*>(_Op)->getClassName().c_str());
		dc.SetTextColor (GetSysColor(_Op->Selected?COLOR_CAPTIONTEXT:COLOR_INACTIVECAPTIONTEXT));
		dc.SetBkMode(TRANSPARENT);
		rect.left += OPERATOR_TITLE_LEFT_MARGIN;
		dc.DrawText( name, strlen(name), &rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	}
}

// ***************************************************************************

void COperatorWin::OnNcLButtonDown(UINT nFlags, CPoint point) 
{
	CNelTexGenDoc *doc = ((CNelTexGenView*)GetParent())->GetDocument();

	// Unselect ?
	bool ctrl = (GetAsyncKeyState (VK_CONTROL)&0x8000)!=0;
	if (ctrl)
	{
		doc->setSelection(const_cast<NLTEXGEN::ITexGenOperator *>(_Op), !_Op->Selected);
	}
	else
	{
		doc->unSelectAll();
		doc->setSelection(const_cast<NLTEXGEN::ITexGenOperator *>(_Op), true);
	}

	// Set the window topmost
	// ((CNelTexGenView*)GetParent ())->setTopMost (this);
	SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
//	SetWindowPos (&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	Invalidate ();

	doc->updateData();

	CWnd::OnLButtonDown(nFlags, point);
}

// ***************************************************************************

void COperatorWin::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	CNelTexGenDoc *doc = ((CNelTexGenView*)GetParent())->GetDocument();

	// Unselect ?
	bool ctrl = (GetAsyncKeyState (VK_CONTROL)&0x8000)!=0;
	if (!ctrl && !_Op->Selected)
		doc->unSelectAll();

	// Force selection of this one
	doc->setSelection(const_cast<NLTEXGEN::ITexGenOperator *>(_Op), true);

	MenuPos = point;
	// ClientToScreen (&point);

	// Check the sub menu
	CExtPopupMenuWnd * pPopupWnd = new CExtPopupMenuWnd;
	VERIFY(pPopupWnd->LoadMenu(*theApp.m_pMainWnd, IDR_OPERATOR_MENU));
	theApp.addOperatorsCreatorToMenu (*pPopupWnd);
	VERIFY(pPopupWnd->TrackPopupMenu(0, point.x, point.y));

	doc->updateData();

	CWnd::OnNcRButtonDown(nHitTest, point);
}

// ***************************************************************************

void COperatorWin::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnNcLButtonUp(nHitTest, point);
}

// ***************************************************************************

void COperatorWin::OnMoving(UINT fwSide, LPRECT pRect) 
{
	CWnd::OnMoving(fwSide, pRect);
	
	((CNelTexGenView*)GetParent())->updateSize();
}

// ***************************************************************************
