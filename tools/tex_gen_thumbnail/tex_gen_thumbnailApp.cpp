// tex_gen_thumbnail.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "tex_gen_thumbnailApp.h"
#include <initguid.h>
#include "ThumbTexGenThumbnail_i.c"
#include "TexGenThumbnailExtractor.h"

#include "TexGenThumbnailDoc.h"

#include "tex_gen/basics.h"

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CTex_gen_thumbnailApp

BEGIN_MESSAGE_MAP(CTex_gen_thumbnailApp, CWinApp)
	//{{AFX_MSG_MAP(CTex_gen_thumbnailApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTex_gen_thumbnailApp construction

CTex_gen_thumbnailApp::CTex_gen_thumbnailApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTex_gen_thumbnailApp object

CTex_gen_thumbnailApp theApp;

BOOL CTex_gen_thumbnailApp::InitInstance() 
{
	if (!InitATL())
		return FALSE;
	
	// Register texgen
	NLTEXGEN::registerTypes();

	// Create document template
	AddDocTemplate(new CMultiDocTemplate(	IDR_TEXGEN_TYPE,
		RUNTIME_CLASS(CTexGenThumbnailDoc),RUNTIME_CLASS(CMDIChildWnd), RUNTIME_CLASS(CView)));

	return CWinApp::InitInstance();
}

void CTex_gen_thumbnailApp::OnDraw(CDC *pDC)
{
	/*CTexGenThumbnailDoc *mydoc = (CTexGenThumbnailDoc *)m_pDoc;
	mydoc->OnDraw(pDC);*/
}

CSize CTex_gen_thumbnailApp::GetDocSize()	 
{
	CTexGenThumbnailDoc *mydoc = (CTexGenThumbnailDoc *)m_pDoc;
	return mydoc->GetDocSize();
}

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
 OBJECT_ENTRY(CLSID_TexGenThumbnailExtractor, CTexGenThumbnailExtractor)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}
/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);
}
/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer(TRUE); //TRUE indicates that typelib is unreg'd
	return S_OK;
}

int CTex_gen_thumbnailApp::ExitInstance()
{
	_Module.Term();
	return CExtractImageApp::ExitInstance();
}

BOOL CTex_gen_thumbnailApp::InitATL()
{
	_Module.Init(ObjectMap, AfxGetInstanceHandle(),&LIBID_THUMBTEXGENTHUMBNAILLib);
	return TRUE;
}
