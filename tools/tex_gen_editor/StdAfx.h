// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2C2EAC09_CFE7_464F_9DD2_8BA8BA172A0C__INCLUDED_)
#define AFX_STDAFX_H__2C2EAC09_CFE7_464F_9DD2_8BA8BA172A0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Pro UI
#include "prof-uis.h"

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

#include "3d/transform.h"
#include "3d/mot.h"
#include "3d/quad_grid.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2C2EAC09_CFE7_464F_9DD2_8BA8BA172A0C__INCLUDED_)
