// TexGenThumbnailExtractor.cpp : Implementation of CTexGenThumbnailExtractor
#include "stdafx.h"
#include "tex_gen_thumbnailApp.h"
#include "ThumbTexGenThumbnail_i.h"
#include "TexGenThumbnailExtractor.h"
#include <shlobj.h>

extern CTex_gen_thumbnailApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CTexGenThumbnailExtractor

HRESULT CTexGenThumbnailExtractor::GetLocation(LPWSTR pszPathBuffer,
		DWORD cchMax, DWORD *pdwPriority,
		const SIZE *prgSize, DWORD dwRecClrDepth,
		DWORD *pdwFlags)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//*pdwFlags &= ~IEIFLAG_ASYNC;
	// if (*pdwFlags & IEIFLAG_ASYNC)  MessageBox(0,"async",0,0);
	// if (*pdwFlags & IEIFLAG_ASPECT)  MessageBox(0,"aspect",0,0);
	//if (*pdwFlags & IEIFLAG_SCREEN)  MessageBox(0,"screen",0,0);

	//*pdwFlags &= IEIFLAG_REFRESH;
	m_bmSize = *prgSize;
	if (*pdwFlags & IEIFLAG_ASYNC)	return E_PENDING; 
	return NOERROR;
}

HRESULT CTexGenThumbnailExtractor::Load(LPCOLESTR wszFile, DWORD dwMode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;
	_tcscpy(m_szFile, OLE2T((WCHAR*)wszFile)); 
	//MessageBox(0,m_szFile,0,0);
	return S_OK;	
};

	// IExtractImage::Extract
HRESULT CTexGenThumbnailExtractor::Extract(HBITMAP* phBmpThumbnail)
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	theApp.LoadDoc(m_szFile);
	m_hPreview = theApp.CreateThumbnail(m_bmSize);
	//::LoadBitmap(_Module.GetModuleInstance(),MAKEINTRESOURCE(IDB_BITMAP2)); 
	*phBmpThumbnail = m_hPreview;
	return NOERROR; 
}

HRESULT CTexGenThumbnailExtractor::GetDateStamp(FILETIME *pDateStamp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	FILETIME ftCreationTime,ftLastAccessTime,ftLastWriteTime;
	// open the file and get last write time
	HANDLE hFile = CreateFile(m_szFile,GENERIC_READ,FILE_SHARE_READ,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(!hFile) return E_FAIL;
	GetFileTime(hFile,&ftCreationTime,&ftLastAccessTime,&ftLastWriteTime);
	CloseHandle(hFile);
	*pDateStamp = ftLastWriteTime;
	return NOERROR; 
}

