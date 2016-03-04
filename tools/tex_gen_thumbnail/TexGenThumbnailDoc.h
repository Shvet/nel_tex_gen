// TexGenThumbnailDoc.h : interface of the CTexGenThumbnailDoc class
//

class CTexGenThumbnailDoc : public CDocument
{
protected: // create from serialization only
	CTexGenThumbnailDoc();
	DECLARE_DYNCREATE(CTexGenThumbnailDoc)

protected:
	CSize           m_sizeDoc;
public:
	virtual CSize GetDocSize() { return m_sizeDoc; }
	// virtual void OnDraw(CDC* pDC);
	HBITMAP CreateBitmap ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTexGenThumbnailDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// DIB info
	HBITMAP			_Dib;
	BITMAPINFO		_DibBitmapInfo;
	uint8			*_DibBits;

	// DDB info
	// HBITMAP			_Ddb;

public:
	virtual ~CTexGenThumbnailDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
	//{{AFX_MSG(CTexGenThumbnailDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
