// nel_tex_gen_doc.h : interface of the CNelTexGenDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEL_TEX_GEN_DOC_H__929F0A1B_F137_4189_A73B_70FFBEEC3C0C__INCLUDED_)
#define AFX_NEL_TEX_GEN_DOC_H__929F0A1B_F137_4189_A73B_70FFBEEC3C0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tex_gen/op_root.h"
#include "tex_gen/tex_gen_document.h"
#include "action.h"

class CNelTexGenDoc : public CDocument
{
	friend CActionEnable;
	friend CActionAddOperator;
	friend CActionDeleteOperator;
	friend CActionLink;
	friend CActionUnlink;
	friend CActionSetParameterInt;
	friend CActionSetParameterString;
	friend CActionSetParameterFloat;
	friend CActionSetParameterCRGBAF;
	friend CActionSetParameterGradient;
public:
	typedef std::map<NLTEXGEN::ITexGenOperator*, uint32> TOpMap;

	enum TViewMode
	{
		ViewModeOperator,
		ViewModeRGB,
		ViewModeAlpha,
	};

	// The nel tex gen document
	NLTEXGEN::CTexGenDocument			NeLDoc;

	// Tile output ?
	bool getTile() const
	{
		return _Tile;
	}
	bool getLayerResult() const
	{
		return _LayerResult;
	}
	TViewMode getViewMode() const
	{
		return _ViewMode;
	}

protected: // create from serialization only
	CNelTexGenDoc();
	DECLARE_DYNCREATE(CNelTexGenDoc)

	// Set of the operator
	typedef std::map<uint32, NLTEXGEN::ITexGenOperator*> TOpMapInv;
	uint32	_OperatorIndex;
	TOpMap	_OperatorMap;
	TOpMapInv	_OperatorMapInv;
	uint add (NLTEXGEN::ITexGenOperator*, int index = -1);
	void remove (NLTEXGEN::ITexGenOperator*);

private:
	uint addInternal (NLTEXGEN::ITexGenOperator *op, int index = -1);
	void placeOperators ();
	void placeOperatorY (NLTEXGEN::ITexGenOperator *op, sint &y);
	void placeOperatorX (NLTEXGEN::ITexGenOperator *op, uint8 *firstX, uint xParent);
protected:
	NLTEXGEN::ITexGenOperator *getOperator(uint opId)
	{
		TOpMapInv::iterator ite = _OperatorMapInv.find (opId);
		nlassert (ite != _OperatorMapInv.end());
		return ite->second;
	}


	// \name Modification
	typedef std::set<NLTEXGEN::ITexGenOperator*> TOpSet;
	typedef std::vector<class IAction*> TModifications;
	typedef std::list<std::vector<IAction*> > TUndoList;
	TModifications	_Modifications;
	TUndoList	_Undo;
	TUndoList	_Redo;
	TOpSet	_ModifiedOperators;
	void erase (TModifications &modif);
	void erase (TUndoList &modif);
	void modifyOperator (NLTEXGEN::ITexGenOperator *op, bool parent);
	void modifyBitmap () { _BitmapModified = true; }
	void modifyGraph () { _GraphModified = true; modifyBitmap (); }
	bool _BitmapModified;
	bool _GraphModified;
	bool	_Tile;
	bool	_LayerResult;
	TViewMode	_ViewMode;
// Attributes
public:

// Operations
public:

	// \name Modification
	void addModification(class IAction *action);
	void endModification();
	void undo();
	void redo();
	void updateData ();
	void updateOperatorsView ();

	// \name Selection
	void unSelectAll();
	void setSelection(NLTEXGEN::ITexGenOperator *op, bool select);
	bool isSomethingDeletableSelected() const;

	// Get the operator set
	/* const TOpMap &getOperatorMap() const
	{
		return _OperatorMap;
	}*/

	// Get the operator id
	uint getOperatorId(const NLTEXGEN::ITexGenOperator *op)
	{
		TOpMap::iterator ite = _OperatorMap.find ((NLTEXGEN::ITexGenOperator*)op);
		nlassert (ite != _OperatorMap.end());
		return ite->second;
	}

	// Get the operator with the id
	const NLTEXGEN::ITexGenOperator *getOperator(uint opId) const
	{
		TOpMapInv::const_iterator ite = _OperatorMapInv.find (opId);
		nlassert (ite != _OperatorMapInv.end());
		return ite->second;
	}

	// Update the view
	void updateView(class CNelTexGenView *view);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNelTexGenDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNelTexGenDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
public:
	//{{AFX_MSG(CNelTexGenDoc)
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomIn();
	afx_msg void OnUpdateViewZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomOut();
	afx_msg void OnUpdateViewZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnFileExport();
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnViewAutoArrange();
	afx_msg void OnViewTile();
	afx_msg void OnUpdateViewTile(CCmdUI* pCmdUI);
	afx_msg void OnViewOperator();
	afx_msg void OnUpdateViewOperator(CCmdUI* pCmdUI);
	afx_msg void OnViewAlpha();
	afx_msg void OnUpdateViewAlpha(CCmdUI* pCmdUI);
	afx_msg void OnViewRgb();
	afx_msg void OnUpdateViewRgb(CCmdUI* pCmdUI);
	afx_msg void OnViewLayerResult();
	afx_msg void OnUpdateViewLayerResult(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnAddOperator(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEL_TEX_GEN_DOC_H__929F0A1B_F137_4189_A73B_70FFBEEC3C0C__INCLUDED_)
