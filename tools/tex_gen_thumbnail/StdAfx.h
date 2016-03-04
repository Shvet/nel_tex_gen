// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__BA56C57E_75FC_4E34_A374_AEA259EB50FA__INCLUDED_)
#define AFX_STDAFX_H__BA56C57E_75FC_4E34_A374_AEA259EB50FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxtempl.h>       // MFC templates

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


	#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>


#undef min
#undef max

#include "nel/misc/types_nl.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <exception>
#include <utility>
#include <deque>

#include "nel/misc/common.h"
#include "nel/misc/debug.h"

#include "nel/misc/stream.h"
#include "nel/misc/time_nl.h"
#include "nel/misc/vector.h"
#include "nel/misc/matrix.h"
#include "nel/misc/bitmap.h"
#include "nel/misc/rgba.h"
#include "nel/misc/file.h"
#include "nel/misc/bitmap.h"
#include "nel/misc/path.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__BA56C57E_75FC_4E34_A374_AEA259EB50FA__INCLUDED_)
