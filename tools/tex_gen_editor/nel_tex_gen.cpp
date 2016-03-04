// nel_tex_gen.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "nel_tex_gen.h"

#include "main_frm.h"
#include "child_frm.h"
#include "nel_tex_gen_doc.h"
#include "nel_tex_gen_view.h"

#include "tex_gen/basics.h"
#include "tex_gen/transform_float.h"

// Operators
#include "tex_gen/op_fill_color.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

::CBitmap	Button;
::CBitmap	ButtonMask;
::CBitmap	ButtonHole;
::CBitmap	ButtonHoleMask;
::CBitmap	Linear;
::CBitmap	Quadratic;
HACCEL		DialogShortcut;
// ***************************************************************************
// CNelTexGenApp
// ***************************************************************************

BEGIN_MESSAGE_MAP(CNelTexGenApp, CWinApp)
	//{{AFX_MSG_MAP(CNelTexGenApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_DIGITAL_MURDER_WEB, OnHelpDigitalMurderWeb)
	ON_COMMAND(ID_HELP_HISTORY, OnHelpHistory)
	ON_COMMAND(ID_HELP_EMAIL, OnHelpEmail)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

// ***************************************************************************
// CNelTexGenApp construction
// ***************************************************************************

CNelTexGenApp::CNelTexGenApp()
{
}

// ***************************************************************************
// The one and only CNelTexGenApp object
// ***************************************************************************

CNelTexGenApp theApp;

HBITMAP ArrowBitmapInput;
HBITMAP ArrowBitmapOutput;

CPortWnd	*Drag[2] = { NULL, NULL };
POINT		DragPos;
POINT		MenuPos;
CVector		DragBezier[4];
bool		DragValid = false;
bool		Draft = false;

void SetNull(CRect &rect)
{
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;
}

void ExpandRect(CRect &rect, POINT &pt)
{
	if (rect.IsRectNull())
	{
		rect.left = pt.x;
		rect.right = pt.x;
		rect.top = pt.y;
		rect.bottom = pt.y;
	}
	else
	{
		rect.left = std::min(pt.x, rect.left);
		rect.right = std::max(pt.x, rect.right);
		rect.top = std::min(pt.y, rect.top);
		rect.bottom = std::max(pt.y, rect.bottom);
	}
}

// ***************************************************************************
// CNelTexGenApp initialization
// ***************************************************************************

BOOL CNelTexGenApp::InitInstance()
{
	 VERIFY(
        g_PaintManager.InstallPaintManager(
            // new CExtPaintManagerOffice2003
            new CExtPaintManagerXP
            )
        );
	 CExtPopupMenuWnd::g_bMenuExpanding = false;
	
	ExePath = GetCommandLine ();
	if (ExePath.size()>0)
	{
		if (ExePath[0] == '\"')
		{
			uint end=ExePath.find ('\"', 1);
			if (end != string::npos)
			{
				ExePath = ExePath.substr (1, end-1);
			}
			else
			{
				nlassert (0);	// no!
			}
		}
		else
		{
			uint end=ExePath.find (' ', 1);
			ExePath = ExePath.substr (0, end);
		}
	}
	ExePath = NLMISC::CFile::getPath (ExePath);

	// Register classes
	registerTypes();

	// Create bitmaps
	nlverify (Button.LoadBitmap(IDB_BUTTON));
	nlverify (ButtonMask.LoadBitmap(IDB_BUTTON_MASK));
	nlverify (ButtonHole.LoadBitmap(IDB_BUTTON_HOLE));
	nlverify (ButtonHoleMask.LoadBitmap(IDB_BUTTON_HOLE_MASK));
	nlverify (Linear.LoadBitmap(IDB_LINEAR));
	nlverify (Quadratic.LoadBitmap(IDB_QUADRATIC));
	nlverify (DialogShortcut = LoadAccelerators (m_hInstance, MAKEINTRESOURCE(IDR_DIALOG)));

	// Init the default bitmap
	ITexGenOperator::DefaultBitmap.resize (256, 256);

	unsigned char bitmaps[8*8]={
		0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
		0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,
	};
	float *pixels = ITexGenOperator::DefaultBitmap.getPixels();
	for (uint y=0; y<8; y++)
	{
		for (uint x=0; x<7; x++)
		{
			for (uint sy=0; sy<32; sy++)
			{
				for (uint sx=0; sx<32; sx++)
				{
					replaceConstRGBA (pixels+(16+x*32+y*256*32+sx+sy*256)*4, bitmaps[x+y*8]?ColorWhite:ColorBlack, 1);
				}
			}
		}
	}

	for (y=0; y<256; y++)
	{
		for (uint x=0; x<256; x++)
		{
			replaceBlendConstRGBA (pixels+(x+y*256)*4, (((x+y)&7) == 0)?ColorWhite:ColorBlack, 1, 0.75);
		}
	}
	
	// Load a bitmap
	nlverify (ArrowBitmapInput = LoadBitmap (m_hInstance, MAKEINTRESOURCE(IDB_ARROW_INPUT)));
	nlverify (ArrowBitmapOutput = LoadBitmap (m_hInstance, MAKEINTRESOURCE(IDB_ARROW_OUTPUT)));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

/*#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif*/

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Nel TexGen"));

	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_TEX_GETYPE,
		RUNTIME_CLASS(CNelTexGenDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CNelTexGenView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	// RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// Create the color picker
	nlverify (ColorPicker.CDialog::Create (IDD_COLOR_PICKER, m_pMainWnd));

	return TRUE;
}

// ***************************************************************************
// CAboutDlg dialog used for App About
// ***************************************************************************

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ***************************************************************************

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

// ***************************************************************************

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************

// App command to run the dialog
void CNelTexGenApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// ***************************************************************************
// CNelTexGenApp message handlers
// ***************************************************************************

COLORREF getColorRef(const float *floatColor)
{
	float R = floatColor[0]*255.f;
	float G = floatColor[1]*255.f;
	float B = floatColor[2]*255.f;
	float A = floatColor[3]*255.f;
	clamp(R, 0.f, 255.f);
	clamp(G, 0.f, 255.f);
	clamp(B, 0.f, 255.f);
	clamp(A, 0.f, 255.f);
	return RGB((int)R, (int)G, (int)B);
}

// ***************************************************************************

NLMISC::CRGBAF getCRGBAF(COLORREF color)
{
	return CRGBAF(CRGBA (GetRValue(color), GetGValue(color), GetBValue(color)));
}

// ***************************************************************************

void setColor(float *dst, COLORREF color)
{
	dst[0] = (float)GetRValue(color) / 255.f;
	dst[1] = (float)GetGValue(color) / 255.f;
	dst[2] = (float)GetBValue(color) / 255.f;
}

// ***************************************************************************

const char *CNelTexGenApp::getOperatorName (const char *className) const
{
	uint i;
	if (strcmp (className, "Rt") == 0)
		return "Output";
	for (i=0; i<_Operators.size(); i++)
	{
		if (_Operators[i].ClassName == className)
			return _Operators[i].OperatorName.c_str ();
	}
	return "Unknwon";
}

// ***************************************************************************

bool CNelTexGenApp::yesNoMessage (const char *format, ... )
{
	// Make a buffer string
	va_list args;
	va_start( args, format );
	char buffer[1024];
	sint ret = vsnprintf( buffer, 1024, format, args );
	va_end( args );

	return MessageBox (m_pMainWnd?m_pMainWnd->m_hWnd:NULL, buffer, "NeL TexGen", MB_YESNO|MB_ICONQUESTION) == IDYES;
}

// ***************************************************************************

void CNelTexGenApp::errorMessage (const char *format, ... )
{
	// Make a buffer string
	va_list args;
	va_start( args, format );
	char buffer[1024];
	sint ret = vsnprintf( buffer, 1024, format, args );
	va_end( args );

	MessageBox (m_pMainWnd?m_pMainWnd->m_hWnd:NULL, buffer, "NeL TexGen", MB_OK|MB_ICONEXCLAMATION);
}

// ***************************************************************************

void CNelTexGenApp::addOperatorsCreatorToMenu (CExtPopupMenuWnd &menu)
{
	const std::vector<COperatorDesc> &operators = getOperators();
	uint i;
	CExtPopupMenuWnd *subMenu = &menu;
	for (i=0; i<operators.size (); i++)
	{
		if (operators[i].Separator)
		{
			nlverify(subMenu->ItemInsert( (UINT)CExtPopupMenuWnd::TYPE_SEPARATOR));
		}
		else if (operators[i].Folder)
		{
			menu.ItemInsert((UINT)CExtPopupMenuWnd::TYPE_POPUP, -1, operators[i].OperatorName.c_str());
			subMenu = menu.ItemGetPopup(menu.ItemGetCount()-1);
		}
		else
		{
			nlverify(subMenu->ItemInsert(ID_EDIT_ADD_OPERATOR+i));
		}
	}
}

// ***************************************************************************

LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) 
	{
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata,data);
        RegCloseKey(hkey);
    }

    return retval;
}

// ***************************************************************************

bool openDocument (LPCTSTR filename, LPCTSTR ext)
{
    TCHAR key[MAX_PATH + MAX_PATH];

    // First try ShellExecute()
    // HINSTANCE result = ShellExecute(NULL, "open", filename, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    // if ((UINT)result <= HINSTANCE_ERROR) 
	{
        if (GetRegKey(HKEY_CLASSES_ROOT, ext, key) == ERROR_SUCCESS) 
		{
            lstrcat(key, "\\shell\\open\\command");

            if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) 
			{
                TCHAR *pos;
                pos = strstr(key, "\"%1\"");
                if (pos == NULL) {                     // No quotes found
                    pos = strstr(key, "%1");       // Check for %1, without quotes 
                    if (pos == NULL)                   // No parameter at all...
                        pos = key+lstrlen(key)-1;
                    else
                        *pos = '\0';                   // Remove the parameter
                }
                else
                    *pos = '\0';                       // Remove the parameter

                lstrcat(pos, " ");
                lstrcat(pos, filename);
                int res = WinExec(key, SW_SHOWDEFAULT);
				return (res>31);
            }
        }
    }

	return false;
}

// ***************************************************************************

bool openFile (LPCTSTR filename)
{
	string ext = "." + NLMISC::CFile::getExtension (filename);
	return openDocument (filename, ext.c_str());
}

// ***************************************************************************

bool openURL (LPCTSTR filename)
{
	return openDocument (filename, ".html");
}

// ***************************************************************************

void CNelTexGenApp::OnHelpDigitalMurderWeb() 
{
	openURL ("http://www.digital-murder.org");
}

// ***************************************************************************

void CNelTexGenApp::OnHelpHistory() 
{
	openFile ((ExePath+"history.txt").c_str());
}

// ***************************************************************************

void CNelTexGenApp::OnHelpEmail() 
{
	openURL ("mailto:hulud@digital-murder.org");
}

// ***************************************************************************

void CNelTexGenApp::AddSeparator (UINT id)
{
	COperatorDesc desc;
	desc.Folder = false;
	desc.Separator = true;
	_Operators.push_back (desc);

	CWinApp * pApp = ::AfxGetApp();
	CExtCmdItem item (id);
	item.m_sMenuText = "";
	g_CmdManager->CmdSetup (pApp->m_pszProfileName, item);
}

// ***************************************************************************

void CNelTexGenApp::AddFolder (UINT id, const char *folderName)
{
	COperatorDesc desc;
	desc.OperatorName = folderName;
	desc.Folder = true;
	desc.Separator = false;
	_Operators.push_back (desc);

	CWinApp * pApp = ::AfxGetApp();
	CExtCmdItem item (id);
	item.m_sMenuText = folderName;
	g_CmdManager->CmdSetup (pApp->m_pszProfileName, item);
}

// ***************************************************************************

void CNelTexGenApp::AddOperator (UINT id, const char *className, const char *operatorName)
{
	COperatorDesc desc;
	desc.ClassName = className;
	desc.OperatorName = operatorName;
	desc.Folder = false;
	desc.Separator = false;
	_Operators.push_back (desc);

	CWinApp * pApp = ::AfxGetApp();
	CExtCmdItem item (id);
	item.m_sMenuText = operatorName;
	g_CmdManager->CmdSetup (pApp->m_pszProfileName, item);
}

// ***************************************************************************

