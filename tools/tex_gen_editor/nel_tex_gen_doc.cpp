// nel_tex_gen_doc.cpp : implementation of the CNelTexGenDoc class
//

#include "stdafx.h"
#include "nel_tex_gen.h"
#include "action.h"

#include "nel_tex_gen_doc.h"
#include "nel_tex_gen_view.h"

#include "tex_gen/op_layer.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

#define MARGIN_OPERATOR 10
#define SPACE_BETWEEN_OPERATORS 20

/////////////////////////////////////////////////////////////////////////////
// CNelTexGenDoc

IMPLEMENT_DYNCREATE(CNelTexGenDoc, CDocument)

BEGIN_MESSAGE_MAP(CNelTexGenDoc, CDocument)
	//{{AFX_MSG_MAP(CNelTexGenDoc)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_VIEW_ZOOM_IN, OnViewZoomIn)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_IN, OnUpdateViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, OnViewZoomOut)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_OUT, OnUpdateViewZoomOut)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_VIEW_AUTO_ARRANGE, OnViewAutoArrange)
	ON_COMMAND(ID_VIEW_TILE, OnViewTile)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TILE, OnUpdateViewTile)
	ON_COMMAND(ID_VIEW_OPERATOR, OnViewOperator)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OPERATOR, OnUpdateViewOperator)
	ON_COMMAND(ID_VIEW_ALPHA, OnViewAlpha)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ALPHA, OnUpdateViewAlpha)
	ON_COMMAND(ID_VIEW_RGB, OnViewRgb)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RGB, OnUpdateViewRgb)
	ON_COMMAND(ID_VIEW_LAYER_RESULT, OnViewLayerResult)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LAYER_RESULT, OnUpdateViewLayerResult)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_EDIT_ADD_OPERATOR, (ID_EDIT_ADD_OPERATOR+100), OnAddOperator)
END_MESSAGE_MAP()

// ***************************************************************************
// CNelTexGenDoc construction/destruction
// ***************************************************************************

CNelTexGenDoc::CNelTexGenDoc()
{
	// TODO: add one-time construction code here
	_OperatorIndex = 0;
	_BitmapModified = true;
	_GraphModified = true;
	_Tile = false;
	_LayerResult = true;
	_ViewMode = ViewModeOperator;
}

// ***************************************************************************

uint CNelTexGenDoc::addInternal (NLTEXGEN::ITexGenOperator *op, int index)
{
	if (index == -1)
		index = _OperatorIndex++;
	_OperatorMap.insert (TOpMap::value_type(op, index));
	_OperatorMapInv.insert (TOpMapInv::value_type(index, op));
	op->Selected = false;

	modifyOperator (op, true);
	return index;
}

// ***************************************************************************

uint CNelTexGenDoc::add (NLTEXGEN::ITexGenOperator *op, int index)
{
	index = addInternal (op, index);
	NeLDoc.Operators.push_back (op);
	return index;
}

// ***************************************************************************

void CNelTexGenDoc::remove (NLTEXGEN::ITexGenOperator *op)
{	
	// Unselect
	setSelection(op, false);

	// Remove from view
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CNelTexGenView *pView = (CNelTexGenView*)GetNextView(pos);
		pView->removeOperator(op);
	}   

	TOpMap::iterator ite = _OperatorMap.find (op);
	nlassert (ite != _OperatorMap.end());
	_OperatorMapInv.erase (ite->second);
	_OperatorMap.erase (ite);
	_ModifiedOperators.erase (op);

	// Remove from nel array
	uint i;
	for (i=0; i<NeLDoc.Operators.size(); i++)
	{
		if (NeLDoc.Operators[i] == op)
			NeLDoc.Operators.erase (NeLDoc.Operators.begin()+i);
	}

	// Delete the operator
	delete (op);
}

// ***************************************************************************

void CNelTexGenDoc::erase (TModifications &modif)
{
	uint i;
	for (i=0; i<modif.size(); i++)
		delete modif[i];
	modif.clear ();
}

// ***************************************************************************

void CNelTexGenDoc::erase (TUndoList &modif)
{
	TUndoList::iterator ite = modif.begin();
	while (ite != modif.end())
	{
		erase (*ite);
		ite++;
	}
	modif.clear();
}

// ***************************************************************************

CNelTexGenDoc::~CNelTexGenDoc()
{
	erase(_Modifications);
	erase(_Undo);
	erase(_Redo);
}

// ***************************************************************************

BOOL CNelTexGenDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	COpRoot *root = new COpRoot;
	add (root);
	placeOperators ();

	return TRUE;
}

// ***************************************************************************
// CNelTexGenDoc diagnostics
// ***************************************************************************

#ifdef _DEBUG
void CNelTexGenDoc::AssertValid() const
{
	CDocument::AssertValid();
}

// ***************************************************************************

void CNelTexGenDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// ***************************************************************************
// CNelTexGenDoc commands
// ***************************************************************************

void CNelTexGenDoc::addModification(class IAction *action)
{
	// Add to modification list ?
	if (action->redo(this))
	{
		// Action added
		bool added = false;

		// Action is not accumulative ?
		if (!_Modifications.empty())
		{
			// The same type ?
			class IAction *back = _Modifications.back();
			if (typeid(*back) == typeid(*action))
			{
				if (action->accum(*back))
				{
					// Replace the last one
					delete (back);
					_Modifications.back() = action;
					added = true;
				}
			}
		}

		if (!added)
		{
			// Add a new modification
			_Modifications.push_back(action);
		}
	}
	else
	{
		delete action;
	}
}

// ***************************************************************************

void CNelTexGenDoc::endModification()
{
	// If modification array not empty
	if (!_Modifications.empty())
	{
		// Add to undo list
		_Undo.push_back (_Modifications);
		
		// End of modifications
		_Modifications.clear ();

		// No more redo
		erase(_Redo);

		// Modify the document
		SetModifiedFlag ();
	}
}

// ***************************************************************************

void CNelTexGenDoc::undo()
{
	// Something to undo ?
	if (!_Undo.empty())
	{
		TModifications &toUndo = _Undo.back();
		TModifications::reverse_iterator ite = toUndo.rbegin();
		while (ite != toUndo.rend())
		{
			(*ite)->undo(this);

			ite++;
		}
		
		// Add to redo
		_Redo.push_back(toUndo);
		_Undo.pop_back();

		// Modify the document
		SetModifiedFlag ();
	}
}

// ***************************************************************************

void CNelTexGenDoc::redo()
{
	// Something to redo ?
	if (!_Redo.empty())
	{
		TModifications &toRedo = _Redo.back();
		TModifications::iterator ite = toRedo.begin();
		while (ite != toRedo.end())
		{
			(*ite)->redo(this);

			ite++;
		}
		
		// Add to redo
		_Undo.push_back(toRedo);
		_Redo.pop_back();

		// Modify the document
		SetModifiedFlag ();
	}
}

// ***************************************************************************

void CNelTexGenDoc::OnEditRedo() 
{
	redo();
	updateData();
}

// ***************************************************************************

void CNelTexGenDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_Redo.empty()?FALSE:TRUE);
}

// ***************************************************************************

void CNelTexGenDoc::OnEditUndo() 
{
	undo();	
	updateData();
}

// ***************************************************************************

void CNelTexGenDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(_Undo.empty()?FALSE:TRUE);
}

// ***************************************************************************

void CNelTexGenDoc::updateData ()
{
	TOpSet::iterator ite = _ModifiedOperators.begin();
	while (ite != _ModifiedOperators.end())
	{
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CNelTexGenView *pView = (CNelTexGenView*)GetNextView(pos);
			pView->updateOperator(*ite);
		}   
		ite++;
	}

	// Clear the modified list
	_ModifiedOperators.clear();

	// Bitmap modified ?
	if (_BitmapModified)
	{
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CNelTexGenView *pView = (CNelTexGenView*)GetNextView(pos);
			pView->updateBitmap();
		}   
		
		_BitmapModified = false;
	}

	// Graph modified ?
	if (_GraphModified)
	{
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CNelTexGenView *pView = (CNelTexGenView*)GetNextView(pos);
			pView->updateGraph();
		}
		_GraphModified = true;
	}
}

// ***************************************************************************

void CNelTexGenDoc::modifyOperator (NLTEXGEN::ITexGenOperator *op, bool parent)
{
	_ModifiedOperators.insert(op);
	if (parent)
	{
		const vector<NLTEXGEN::ITexGenOperator *> parentOp = op->getParents();
		uint i;
		for (i=0; i<parentOp.size(); i++)
		{
			modifyOperator (parentOp[i], true);
		}
	}
	modifyBitmap ();
}

// ***************************************************************************

void CNelTexGenDoc::updateView(CNelTexGenView *view) 
{
	updateData ();

	// Modify operators
	modifyBitmap ();
	TOpMap::iterator ite = _OperatorMap.begin();
	while (ite != _OperatorMap.end())
	{
		view->updateOperator(ite->first);
		ite++;
	}

	// Update the bitmap
	view->updateBitmap();
}

// ***************************************************************************

void CNelTexGenDoc::OnViewZoomIn() 
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CNelTexGenView *pView = (CNelTexGenView*)GetNextView(pos);
		pView->zoomIn(true);
	}   
}

// ***************************************************************************

void CNelTexGenDoc::OnUpdateViewZoomIn(CCmdUI* pCmdUI) 
{
}

// ***************************************************************************

void CNelTexGenDoc::OnViewZoomOut() 
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CNelTexGenView *pView = (CNelTexGenView*)GetNextView(pos);
		pView->zoomOut(true);
	}   
}

// ***************************************************************************

void CNelTexGenDoc::OnUpdateViewZoomOut(CCmdUI* pCmdUI) 
{
}

// ***************************************************************************

afx_msg void CNelTexGenDoc::OnAddOperator(UINT nID)
{
	// Add an operator
	const COperatorDesc &desc = theApp.getOperators()[nID-ID_EDIT_ADD_OPERATOR];
	addModification(new CActionAddOperator(&desc, MenuPos.x, MenuPos.y));
	endModification();
	updateData();
}

// ***************************************************************************

BOOL CNelTexGenDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	// File object
	CIFile file;
	if (file.open (lpszPathName))
	{
		try
		{
			// Read the document
			NeLDoc.serial (file);

			// Add the operators
			uint i;
			for (i=0; i<NeLDoc.Operators.size(); i++)
			{
				NeLDoc.Operators[i]->X = 10;
				NeLDoc.Operators[i]->Y = 10;
				addInternal (NeLDoc.Operators[i]);
			}

			// Place operators
			placeOperators ();

			return TRUE;
		}
		catch (Exception &e)
		{
			theApp.errorMessage (e.what());
		}
	}
	else
	{
		theApp.errorMessage ("Can't open the file %s for reading", lpszPathName);
	}
	
	return FALSE;
}

// ***************************************************************************

BOOL CNelTexGenDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// File object
	COFile file;
	if (file.open (lpszPathName))
	{
		try
		{
			// Read the document
			NeLDoc.serial (file);
			
			// Doc no more modified
			SetModifiedFlag (FALSE);

			return TRUE;
		}
		catch (Exception &e)
		{
			theApp.errorMessage (e.what());
		}
	}
	else
	{
		theApp.errorMessage ("Can't open the file %s for writing", lpszPathName);
	}
	
	return FALSE;
}

// ***************************************************************************

void CNelTexGenDoc::placeOperatorY (ITexGenOperator *op, sint &y)
{
	op->Y = max (y, (sint)op->Y);
	y = op->Y;
	
	// For each children
	const std::vector<ITexGenOperator*>	&binded = op->getBindedOp();
	uint i;
	for (i=0; i<binded.size(); i++)
	{
		if (binded[i])
		{
			sint yChild = y+1;
			placeOperatorY (binded[i], yChild);
			//y = std::max (y, yChild);
		}
	}
	y++;
}

// ***************************************************************************

void CNelTexGenDoc::placeOperators ()
{
	// Clear Y
	uint i;
	for (i=0; i<NeLDoc.Operators.size(); i++)
	{
		NeLDoc.Operators[i]->X = 0;
		NeLDoc.Operators[i]->Y = 0;
	}

	sint y = 0;
	for (i=0; i<NeLDoc.Operators.size(); i++)
	{
		if (NeLDoc.Operators[i]->getParents().size() == 0)
			placeOperatorY (NeLDoc.Operators[i], y);
	}

	// Set X
	uint8 firstX[256];
	memset (firstX, 0, 256);
	set<ITexGenOperator*> placed;
	for (i=0; i<NeLDoc.Operators.size(); i++)
	{
		if (NeLDoc.Operators[i]->getParents().size() == 0)
			placeOperatorX (NeLDoc.Operators[i], firstX, 0);
	}

	// Set Y
	for (i=0; i<NeLDoc.Operators.size(); i++)
	{
		NeLDoc.Operators[i]->Y = (SPACE_BETWEEN_OPERATORS+OPERATOR_WIN_HEIGHT) * NeLDoc.Operators[i]->Y + MARGIN_OPERATOR;
	}
}

// ***************************************************************************

void CNelTexGenDoc::placeOperatorX (ITexGenOperator *op, uint8 *firstX, uint xParent)
{
	// Compute the x coordinate
	sint y = op->Y;
	sint x = std::max ((sint)firstX[y], (sint)xParent);
	firstX[y] = x+1;
	op->X = (SPACE_BETWEEN_OPERATORS+OPERATOR_WIN_WIDTH) * x + MARGIN_OPERATOR;
	
	// For each children
	const std::vector<ITexGenOperator*>	&binded = op->getBindedOp();
	uint i;
	for (i=0; i<binded.size(); i++)
	{
		if (binded[i] && (binded[i]->X == 0))
		{
			placeOperatorX (binded[i], firstX, xParent++);
		}
	}
	y++;
}

// ***************************************************************************

void CNelTexGenDoc::OnFileExport() 
{
	// Eval the document
	CFloatBitmap floatBitmap;
	CRenderParameter renderParameters (true, false, false);
	NeLDoc.Operators[0]->eval (floatBitmap, renderParameters);
	NLMISC::CBitmap bitmap;
	floatBitmap.export (bitmap);
	((COpRoot*)NeLDoc.Operators[0])->evalAlpha (floatBitmap, renderParameters);
	floatBitmap.exportAlpha (bitmap);

	// Save the document
	static char BASED_CODE szFilter[] = "Targa Files (*.tga)|*.tga|All Files (*.*)|*.*||";

	string docName = (const char*)GetPathName ();
	docName = NLMISC::CFile::getFilenameWithoutExtension(docName);
	
	CFileDialog outputFile ( FALSE, "tga", (docName+".tga").c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, theApp.m_pMainWnd);
	if (outputFile.DoModal() == IDOK)
	{
		// Save the file
		COFile file;
		if (file.open ((const char*)outputFile.GetPathName()))
		{
			try
			{
				// Write the bitmap
				bitmap.writeTGA (file, 32);
			}
			catch (Exception &e)
			{
				theApp.errorMessage (e.what());
			}
		}
		else
		{
			theApp.errorMessage ("Can't open the file %s for writing", (const char*)outputFile.GetPathName());
		}
	}
}

// ***************************************************************************

void CNelTexGenDoc::unSelectAll()
{
	uint i;
	for (i=0; i<NeLDoc.Operators.size(); i++)
	{
		setSelection(NeLDoc.Operators[i], false);
	}
}

// ***************************************************************************

void CNelTexGenDoc::setSelection(NLTEXGEN::ITexGenOperator *op, bool select)
{
	if (op->Selected != select)
	{
		op->Selected = select;

		// Notify view
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CNelTexGenView *pView = (CNelTexGenView*)GetNextView(pos);
			pView->selectionChange(op);
			modifyBitmap ();
		}
	}
}

// ***************************************************************************

void CNelTexGenDoc::OnEditDelete() 
{
	// Delete the operators
	uint i;
	for (i=1; i<NeLDoc.Operators.size(); i++)
	{
		if (NeLDoc.Operators[i]->Selected)
		{
			// Id 
			uint id = getOperatorId (NeLDoc.Operators[i]);

			// Unlink from parent
			std::vector<ITexGenOperator*> &parents = NeLDoc.Operators[i]->getParents ();
			while (parents.size())
			{
				addModification(new CActionUnlink (getOperatorId (parents[0]), parents[0]->whereIsBindOp(NeLDoc.Operators[i]), id));
			}

			// Unlink children
			uint j;
			const std::vector<ITexGenOperator*> &children = NeLDoc.Operators[i]->getBindedOp();
			for (j=0; j<children.size(); j++)
			{
				if (children[j])
					addModification(new CActionUnlink (id, NeLDoc.Operators[i]->whereIsBindOp(children[j]), getOperatorId (children[j])));
			}

			addModification(new CActionDeleteOperator (id, this));
		}
	}

	// One undo step
	endModification();
	
	// Update data
	updateData();
}

// ***************************************************************************

void CNelTexGenDoc::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isSomethingDeletableSelected());
}

// ***************************************************************************

bool CNelTexGenDoc::isSomethingDeletableSelected() const
{
	uint i;
	for (i=1; i<NeLDoc.Operators.size(); i++)
	{
		if (NeLDoc.Operators[i]->Selected)
			return true;
	}
	return false;
}

// ***************************************************************************

void CNelTexGenDoc::OnViewAutoArrange() 
{
	placeOperators ();
	
	// Update data
	updateOperatorsView ();
}

// ***************************************************************************

void CNelTexGenDoc::updateOperatorsView ()
{
	uint i;
	for (i=0; i<NeLDoc.Operators.size(); i++)
	{
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CNelTexGenView *pView = (CNelTexGenView*)GetNextView(pos);
			pView->updateOperator(NeLDoc.Operators[i]);
		}   
	}
}

// ***************************************************************************

void CNelTexGenDoc::OnViewTile() 
{
	_Tile ^= true;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CNelTexGenView *pView = (CNelTexGenView*)GetNextView(pos);
		pView->invalidateBitmap ();
	}   
}

// ***************************************************************************

void CNelTexGenDoc::OnUpdateViewTile(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (_Tile?1:0);
}

// ***************************************************************************

void CNelTexGenDoc::OnViewRgb() 
{
	_ViewMode = ViewModeRGB;
	modifyBitmap ();
	updateData ();
}

// ***************************************************************************

void CNelTexGenDoc::OnUpdateViewRgb(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio (_ViewMode == ViewModeRGB);
}

// ***************************************************************************

void CNelTexGenDoc::OnViewAlpha() 
{
	_ViewMode = ViewModeAlpha;
	modifyBitmap ();
	updateData ();
}

// ***************************************************************************

void CNelTexGenDoc::OnUpdateViewAlpha(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio (_ViewMode == ViewModeAlpha);
}

// ***************************************************************************

void CNelTexGenDoc::OnViewOperator() 
{
	_ViewMode = ViewModeOperator;
	modifyBitmap ();
	updateData ();
}

// ***************************************************************************

void CNelTexGenDoc::OnUpdateViewOperator(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio (_ViewMode == ViewModeOperator);
}

// ***************************************************************************

void CNelTexGenDoc::OnViewLayerResult() 
{
	_LayerResult ^= true;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CNelTexGenView *pView = (CNelTexGenView*)GetNextView(pos);
		pView->updateBitmap ();
	}   
	updateOperatorsView ();
}

// ***************************************************************************

void CNelTexGenDoc::OnUpdateViewLayerResult(CCmdUI* pCmdUI) 
{
	if (_ViewMode == ViewModeOperator)
	{
		ITexGenOperator *op = NULL;
		uint i;
		for (i=0; i<NeLDoc.Operators.size(); i++)
		{
			if (NeLDoc.Operators[i]->Selected)
			{
				op = NeLDoc.Operators[i];
				break;
			}
		}

		if (!op)
			op = NeLDoc.Operators[0];

		pCmdUI->Enable (dynamic_cast<IOpLayer*>(op) != NULL);
	}
	else
		pCmdUI->Enable (FALSE);
	pCmdUI->SetCheck (_LayerResult?1:0);
}

// ***************************************************************************
