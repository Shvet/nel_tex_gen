// nel_tex_gen.h : main header file for the NEL_TEX_GEN application
//

#if !defined(AFX_NEL_TEX_GEN_H__5D279AC8_3B94_4F4C_8949_76FD94AE64A0__INCLUDED_)
#define AFX_NEL_TEX_GEN_H__5D279AC8_3B94_4F4C_8949_76FD94AE64A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "color_picker.h"

/////////////////////////////////////////////////////////////////////////////
// CNelTexGenApp:
// See nel_tex_gen.cpp for the implementation of this class
//

#define OPERATOR_WIN_WIDTH 179
#define OPERATOR_WIN_HEIGHT 50

class COperatorDesc
{
public:
	bool		Folder;
	bool		Separator;
	std::string	ClassName;
	std::string	OperatorName;
};

class CNelTexGenApp : public CWinApp
{
public:
	CNelTexGenApp();

private:
	std::vector<COperatorDesc>	_Operators;

public:

	// Helpers
	bool		yesNoMessage (const char *format, ... );
	void		errorMessage (const char *format, ... );

	// Exe path
	std::string	ExePath;

	// Register an operator
	void AddSeparator (UINT id);
	void AddFolder (UINT id, const char *folderName);
	void AddOperator (UINT id, const char *className, const char *operatorName);

	// Acces operators
	const std::vector<COperatorDesc>	&getOperators() const
	{
		return _Operators;
	}
	
	// Get the operator name
	const char *getOperatorName (const char *className) const;

	// Add submenu to create operators
	void addOperatorsCreatorToMenu (CExtPopupMenuWnd &menu);

	// Color picker
	CColorPicker	ColorPicker;

private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNelTexGenApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNelTexGenApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpDigitalMurderWeb();
	afx_msg void OnHelpHistory();
	afx_msg void OnHelpEmail();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CNelTexGenApp theApp;
extern HBITMAP ArrowBitmapInput;
extern HBITMAP ArrowBitmapOutput;
extern class CPortWnd *Drag[2];
extern POINT DragPos;
extern POINT MenuPos;
extern NLMISC::CVector DragBezier[4];
extern bool DragValid;
extern bool	Draft;
extern CBitmap	Button;
extern CBitmap	ButtonMask;
extern CBitmap	ButtonHole;
extern CBitmap	ButtonHoleMask;
extern CBitmap	Linear;
extern CBitmap	Quadratic;
extern HACCEL DialogShortcut;

void ExpandRect(CRect &rect, POINT &pt);
void SetNull(CRect &rect);
COLORREF getColorRef(const float *floatColor);
NLMISC::CRGBAF getCRGBAF(COLORREF color);
void setColor(float *dst, COLORREF color);
bool openFile (LPCTSTR filename);
bool openDocument (LPCTSTR filename, LPCTSTR ext);
bool openURL (LPCTSTR filename);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEL_TEX_GEN_H__5D279AC8_3B94_4F4C_8949_76FD94AE64A0__INCLUDED_)
