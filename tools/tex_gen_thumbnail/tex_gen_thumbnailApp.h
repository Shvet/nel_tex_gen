// tex_gen_thumbnail.h : main header file for the TEX_GEN_THUMBNAIL DLL
//

#if !defined(AFX_THUMBSCB_H__4CCAB995_C650_42B1_ABBC_F85BBFC0D265__INCLUDED_)
#define AFX_THUMBSCB_H__4CCAB995_C650_42B1_ABBC_F85BBFC0D265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ThumbTexGenThumbnail_i.h"

/////////////////////////////////////////////////////////////////////////////
// CTex_gen_thumbnailApp
// See tex_gen_thumbnail.cpp for the implementation of this class
//
#include "ExtractImageApp.h"

class CTex_gen_thumbnailApp : public CExtractImageApp
{
public:
	CTex_gen_thumbnailApp();
	virtual void OnDraw(CDC *pDC);
	virtual CSize GetDocSize();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTex_gen_thumbnailApp)
	public:
	virtual BOOL InitInstance();
		virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTex_gen_thumbnailApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL InitATL();
};

extern CTex_gen_thumbnailApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THUMBSCB_H__4CCAB995_C650_42B1_ABBC_F85BBFC0D265__INCLUDED_)
