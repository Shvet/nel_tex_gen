// op_property_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "action.h"
#include "nel_tex_gen.h"
#include "nel_tex_gen_view.h"
#include "nel_tex_gen_doc.h"
#include "operator_win.h"
#include "op_property_dlg.h"
#include "my_edit.h"
#include "color_box.h"
#include "gradient_rect_wnd.h"
#include "rot_button.h"
#include "tex_gen/tex_gen_op.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

#define DIALOG_CLIENT_WIDTH 150
#define DIALOG_MARGIN 4
#define WIDGET_LEFT (DIALOG_MARGIN+GROUPBOX_WIDTH)
#define WIDGET_RIGHT (DIALOG_CLIENT_WIDTH-(DIALOG_MARGIN+GROUPBOX_WIDTH))
#define WIDGET_SPACE 5

#define GROUPBOX_WIDTH 8
#define GROUPBOX_HEIGHT 16
#define GROUPBOX_HEIGHT_END 8

#define COMBO_HEIGHT 24
#define COMBO_WINDOW_HEIGHT 200

#define LABEL_WIDTH 100
#define LABEL_HEIGHT 16

#define COLOR_HEIGHT 16

#define SLIDER_HEIGHT 32

#define EDIT_WIDTH 50
#define EDIT_HEIGHT 20
#define EDIT_MULTI_LINE_HEIGHT 48

#define SPIN_WIDTH 16

#define CHECKBOX_HEIGHT 16

#define SPINNER_WIDTH 16

#define SPINNER_MIN 0
#define SPINNER_MAX 1000

#define GRADIENT_HEIGHT 16

#define ROT_BUTTON_WIDTH 32
#define ROT_BUTTON_HEIGHT 32
#define ROT_BUTTON_LEFT_MARGIN 10
#define ROT_BUTTON_LABEL_MARGIN 5

// ***************************************************************************
// COpPropertyDlg dialog
// ***************************************************************************

COpPropertyDlg::COpPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpPropertyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpPropertyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	_Initialized = false;
}

// ***************************************************************************

COpPropertyDlg::~COpPropertyDlg()
{
	uint j;
	for (j=0; j<Widgets.size(); j++)
		delete Widgets[j];
	for (j=0; j<Others.size(); j++)
		delete Others[j];
	Widgets.clear();
	Others.clear();
}

// ***************************************************************************

void COpPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpPropertyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

// ***************************************************************************

BEGIN_MESSAGE_MAP(COpPropertyDlg, CDialog)
	//{{AFX_MSG_MAP(COpPropertyDlg)
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
// COpPropertyDlg message handlers
// ***************************************************************************

void COpPropertyDlg::addLabel(RECT &client, const std::string &str)
{
	CStatic *label = new CStatic;
	string tmp = str;
	if (!tmp.empty())
		tmp[0] = toupper (tmp[0]);
	label->Create (tmp.c_str(), WS_CHILD|SS_LEFT|SS_CENTERIMAGE|WS_VISIBLE, client, this);
	label->SetFont (GetFont());
	Others.push_back(label);
}

// ***************************************************************************

void COpPropertyDlg::beginCategory(RECT &client)
{
	_CategoryTop = client.top;
	client.top += GROUPBOX_HEIGHT;
	client.left = WIDGET_LEFT;
}

// ***************************************************************************

void COpPropertyDlg::endCategory(RECT &client, const char *name)
{
	RECT clientBox;
	clientBox.bottom = client.top+GROUPBOX_HEIGHT_END;
	clientBox.top = _CategoryTop;
	clientBox.left = DIALOG_MARGIN;
	clientBox.right = DIALOG_CLIENT_WIDTH - DIALOG_MARGIN;
	CExtBtnOnFlat *button = new CExtBtnOnFlat ();
	string tmp = name;
	tmp[0] = toupper (tmp[0]);
	button->Create (tmp.c_str(), BS_GROUPBOX|WS_CHILD|WS_VISIBLE, clientBox, this, 0xffff);
	button->SetFont (GetFont());
	Others.push_back(button);
	client.top = clientBox.bottom;
}

// ***************************************************************************

BOOL COpPropertyDlg::Create (COperatorWin *opWindow, const NLTEXGEN::ITexGenOperator *op)
{
	// Save operator parameters
	_OpWindow = opWindow;
	_Op = op;

	// Create the dialog
	if (CDialog::Create (IDD_OP_PROPERTY, theApp.m_pMainWnd))
	{
		// The client RECT
		RECT client;
		GetClientRect (&client);
		client.top = DIALOG_MARGIN;
		client.left = DIALOG_MARGIN;

		// Create each widget
		uint i;
		std::string currentCategory;
		for (i=0; i<_Op->getNumParameter(); i++)
		{
			// The parameter
			const CParameter &param = _Op->getParameter(i);

			// New category ?
			if ((i == 0) || (currentCategory != param.Category))
			{
				if (i != 0)
					endCategory (client, currentCategory.c_str ());
				currentCategory = param.Category;
				beginCategory (client);
			}
			else
			{
				client.top += WIDGET_SPACE;
			}
			

			// Type ?
			switch (param.Type)
			{
			case CParameter::TypeEnum:
				{
					if (param.EnumString)
					{
						// Add label
						client.bottom = client.top + LABEL_HEIGHT;
						client.right = client.left + LABEL_WIDTH;
						addLabel(client, (param.Name+string(":")).c_str());

						client.right = WIDGET_RIGHT;
						client.top = client.bottom;
						client.bottom = client.top + COMBO_WINDOW_HEIGHT;
						CExtComboBox *comboBox = new CExtComboBox;
						comboBox->Create (WS_TABSTOP|CBS_DROPDOWNLIST|WS_VISIBLE|WS_CHILD|WS_VSCROLL|CBS_DISABLENOSCROLL, client, this, i+FirstID);
						client.bottom = client.top + COMBO_HEIGHT;
						comboBox->SetFont (GetFont());
						Widgets.push_back(comboBox);
						int j;
						for (j=0; j<param.IntegerMax; j++)
							comboBox->AddString(param.EnumString[j]);
					}
					else
						Widgets.push_back (NULL);
				}
				break;
			case CParameter::TypeColor:
				{
					// Add label
					client.bottom = client.top + LABEL_HEIGHT;
					client.right = client.left + LABEL_WIDTH;
					addLabel(client, (param.Name+string(":")).c_str());

					client.top = client.bottom;
					client.bottom += COLOR_HEIGHT;
					client.right = WIDGET_RIGHT;
					CColorBox *colorBox = new CColorBox;
					colorBox->Create (WS_TABSTOP|WS_VISIBLE|WS_CHILD, client, this, i+FirstID);
					colorBox->SetFont (GetFont());
					Widgets.push_back(colorBox);
				}
				break;
			case CParameter::TypeGradient:
				{
					// Add label
					client.bottom = client.top + LABEL_HEIGHT;
					client.right = client.left + LABEL_WIDTH;
					addLabel(client, (param.Name+string(":")).c_str());

					client.right = WIDGET_RIGHT;
					client.top = client.bottom;
					client.bottom = client.top+GRADIENT_HEIGHT;
					CGradientRectWnd *gradient = new CGradientRectWnd (true);
					gradient->Create (WS_TABSTOP|WS_VISIBLE|WS_CHILD|/*WS_BORDER*/0, client, this, i+FirstID);
					Widgets.push_back(gradient);
				}
				break;
			/*case CParameter::TypeFloat:
				{
					// Add label
					client.bottom = client.top + LABEL_HEIGHT;
					client.right = client.left + LABEL_WIDTH;
					addLabel(client, (param.Name+string(":")).c_str());

					client.right = WIDGET_RIGHT;
					client.top = client.bottom;
					client.bottom = client.top+SLIDER_HEIGHT;
					CSliderCtrl *slider = new CSliderCtrl;
					slider->Create (WS_TABSTOP|TBS_HORZ|WS_VISIBLE|WS_CHILD, client, this, i+FirstID);
					slider->SetRange(SPINNER_MIN, SPINNER_MAX);
					slider->SetFont (GetFont());
					Widgets.push_back(slider);
				}
				break;*/
			case CParameter::TypeFloat:
				{
					client.right = client.left + ROT_BUTTON_WIDTH;
					client.bottom = client.top+ROT_BUTTON_HEIGHT;
					CRotButton *rotButton = new CRotButton;
					rotButton->Create (WS_TABSTOP|WS_VISIBLE|WS_CHILD, client, this, i+FirstID);
					rotButton->init (param.Setup);
					Widgets.push_back(rotButton);

					// Add label
					RECT clientBox = client;
					clientBox.top --;
					clientBox.left = clientBox.right + ROT_BUTTON_LEFT_MARGIN;
					clientBox.bottom = clientBox.top + LABEL_HEIGHT;
					clientBox.right = clientBox.left + LABEL_WIDTH;
					addLabel(clientBox, (param.Name+string(":")).c_str());

					clientBox = client;
					clientBox.left = clientBox.right + ROT_BUTTON_LEFT_MARGIN;
					clientBox.right = clientBox.left + EDIT_WIDTH;
					clientBox.top = clientBox.bottom - EDIT_HEIGHT;
					CMyEdit *edit = new CMyEdit;
					edit->Create (/*WS_BORDER*/0|WS_TABSTOP|ES_LEFT|WS_VISIBLE|WS_CHILD, clientBox, this, i+FirstID);
					edit->SetFont (GetFont());
					Others.push_back(edit);
					rotButton->setEditBox (edit);

					clientBox.left = clientBox.right + ROT_BUTTON_LABEL_MARGIN;
					clientBox.right = clientBox.left + LABEL_WIDTH;
					clientBox.top = clientBox.bottom - EDIT_HEIGHT;
					addLabel(clientBox, param.Setup.Unit);
				}
				break;
			case CParameter::TypeInt:
				{
					// Add label
					client.bottom = client.top + LABEL_HEIGHT;
					client.right = client.left + LABEL_WIDTH;
					addLabel(client, (param.Name+string(":")).c_str());

					client.top = client.bottom;
					client.bottom += EDIT_HEIGHT;
					client.right = client.left+EDIT_WIDTH;
					CMyEdit *edit = new CMyEdit;
					edit->Create (/*WS_BORDER*/0|WS_TABSTOP|ES_LEFT|ES_NUMBER|WS_VISIBLE|WS_CHILD, client, this, i+FirstID);
					edit->SetFont (GetFont());
					Widgets.push_back(edit);

					RECT clientBox = client;
					clientBox.left = clientBox.right;
					clientBox.right = clientBox.left+SPIN_WIDTH;
					CSpinButtonCtrl *spin = new CSpinButtonCtrl;
					spin->Create (/*WS_BORDER*/0|WS_TABSTOP|UDS_WRAP|UDS_ARROWKEYS|WS_VISIBLE|WS_CHILD, clientBox, this, i+FirstID);
					spin->SetRange32( param.IntegerMin, param.IntegerMax);
					Others.push_back(spin);
				}
				break;
			case CParameter::TypeString:
				{
					// Add label
					client.bottom = client.top + LABEL_HEIGHT;
					client.right = client.left + LABEL_WIDTH;
					addLabel(client, (param.Name+string(":")).c_str());

					client.top = client.bottom;
					client.bottom += param.Integer?EDIT_MULTI_LINE_HEIGHT:EDIT_HEIGHT;
					client.right = WIDGET_RIGHT;
					CExtEdit *edit;
					if (param.Integer)
					{
						edit = new CExtEdit;
						edit->Create (WS_VSCROLL|ES_OEMCONVERT|ES_MULTILINE|ES_WANTRETURN|WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL|ES_AUTOVSCROLL, client, this, i+FirstID);
					}
					else
					{
						edit = new CMyEdit;
						edit->Create (/*WS_BORDER*/0|WS_TABSTOP|ES_LEFT|WS_VISIBLE|WS_CHILD, client, this, i+FirstID);
					}
					edit->SetFont (GetFont());
					Widgets.push_back(edit);
				}
				break;
			case CParameter::TypeBool:
				{
					client.right = WIDGET_RIGHT;
					client.bottom = client.top+CHECKBOX_HEIGHT;
					CExtBtnOnFlat *edit = new CExtBtnOnFlat;
					string tmp = param.Name;
					tmp[0] = toupper (tmp[0]);
					edit->Create (tmp.c_str(), WS_TABSTOP|BS_AUTOCHECKBOX|WS_VISIBLE|WS_CHILD, client, this, i+FirstID);
					//edit->SetFlat (FALSE);
					edit->SetFont (GetFont());
					Widgets.push_back(edit);
				}
				break;
			default:
				Widgets.push_back (NULL);
			}
		
			client.top = client.bottom;
		}

		if (_Op->getNumParameter())
			endCategory (client, currentCategory.c_str ());
		client.bottom = client.top + DIALOG_MARGIN;
		client.top = 0;
		client.left = 0;
		client.right = DIALOG_CLIENT_WIDTH;

		// Resize the client window
		RECT window;
		RECT realClient;
		GetWindowRect (&window);
		GetClientRect (&realClient);
		SetWindowPos (NULL, 0, 0, 
			(window.right-window.left)+(client.right-client.left)-(realClient.right-realClient.left),
			(window.bottom-window.top)+(client.bottom-client.top)-(realClient.bottom-realClient.top),
			SWP_NOMOVE|SWP_NOZORDER);

		_Initialized = true;

		// Set the name
		updateData();

		return TRUE;
	}
	return FALSE;
}

// ***************************************************************************

void COpPropertyDlg::updateData ()
{
	// Operator name
	SetWindowText (theApp.getOperatorName (const_cast<ITexGenOperator*>(_Op)->getClassName().c_str()));

	// Update each widget
	// Create each widget
	uint i;
	for (i=0; i<_Op->getNumParameter(); i++)
	{
		// The parameter
		const CParameter &param = _Op->getParameter(i);

		// Type ?
		if (Widgets[i])
		{
			switch (param.Type)
			{
			case CParameter::TypeEnum:
				{
					if (param.EnumString)
					{
						CExtComboBox *comboBox = (CExtComboBox*)Widgets[i];
						comboBox->SetCurSel(param.Integer);
					}
				}
				break;
			case CParameter::TypeColor:
				{
					CColorBox *colorBox = (CColorBox*)Widgets[i];
					colorBox->setColor(getColorRef(param.Color));
				}
				break;
			case CParameter::TypeGradient:
				{
					CGradientRectWnd *gradientWnd = (CGradientRectWnd*)Widgets[i];
					gradientWnd->setGradient(param.Gradient);
				}
				break;
			case CParameter::TypeFloat:
				{
					/* CSliderCtrl *spinner = (CSliderCtrl*)Widgets[i];
					spinner->SetPos((int)(param.Color[0]*(float)SPINNER_MAX));					*/

					CRotButton *rotButton = (CRotButton*)Widgets[i];
					rotButton->setValue (param.Value);
				}
				break;
			case CParameter::TypeInt:
				{
					char number[100];
					Widgets[i]->GetWindowText (number, 100);
					int oldValue = atoi(number);
					if ((oldValue != param.Integer) || (number[0] == 0))
					{
						itoa(param.Integer, number, 10);
						Widgets[i]->SendMessage(WM_SETTEXT, 0, (LPARAM)number);
					}

					uint j;
					for (j=0; j<Others.size(); j++)
					{
						if (Others[j]->GetDlgCtrlID() == (sint)(i + FirstID))
						{
							CSpinButtonCtrl *spin = (CSpinButtonCtrl*)Others[j];
							spin->SetPos (param.Integer);
						}
					}
				}
				break;
			case CParameter::TypeBool:
				{
					((CExtBtnOnFlat*)(Widgets[i]))->SetCheck(param.Integer);
				}
				break;
			case CParameter::TypeString:
				{
					CString text;
					Widgets[i]->GetWindowText (text);
					if ((const char*)text != param.StringValue)
					{
						Widgets[i]->SendMessage(WM_SETTEXT, 0, (LPARAM)param.StringValue.c_str());
					}
				}
				break;
			}
		}
	}

	// Update
	UpdateData (FALSE);
}

// ***************************************************************************

BOOL COpPropertyDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	bool done = false;

	// Window initialized ?
	if (_Initialized && IsWindow(_OpWindow->m_hWnd))
	{
		// Get parameters
		uint idWidget = (int) LOWORD(wParam)-FirstID;  // identifier of combo box 
		HWND hwndWidget = (HWND) lParam;       // handle to combo box 

		// Change the document
		CNelTexGenView *view = (CNelTexGenView*)(_OpWindow->GetParent());
		CNelTexGenDoc *doc = view->GetDocument();

		switch (HIWORD(wParam))
		{
		case CBN_SELCHANGE:
			{
				// Get current selection
				int newSelection = ::SendMessage (hwndWidget, CB_GETCURSEL, 0, 0);

				// Add an action
				int oldSelection = _Op->getParameter (idWidget).Integer;

				doc->addModification (new CActionSetParameterInt (doc->getOperatorId(_Op), idWidget, 
					oldSelection, newSelection));
				doc->endModification();
				doc->updateData();
				done = true;
			}
			break;
		case COB_COLOR_CHANGED:
			{
				// Get current colors
				CRGBAF newColor = getCRGBAF(((CColorBox*)(Widgets[idWidget]))->getColor());

				// Add an action
				const CParameter &param = _Op->getParameter (idWidget);
				CRGBAF oldColor (param.Color[0], param.Color[1], param.Color[2], param.Color[3]);

				doc->addModification (new CActionSetParameterCRGBAF (doc->getOperatorId(_Op), idWidget,
					oldColor, newColor));
				doc->updateData();
				done = true;
			}
			break;
		case COB_COLOR_END:
			{
				doc->endModification();
				done = true;
			}
			break;
		case GDR_GRADIENT_CHANGED:
			{
				// Get current colors
				const std::vector<NLTEXGEN::CGradientColor>	&newGradient = 
					((CGradientRectWnd*)(Widgets[idWidget]))->getGradient();

				// Add an action
				const CParameter &param = _Op->getParameter (idWidget);
				const std::vector<NLTEXGEN::CGradientColor>	&oldGradient = param.Gradient;

				doc->addModification (new CActionSetParameterGradient (doc->getOperatorId(_Op), idWidget,
					oldGradient, newGradient));
				doc->endModification();
				doc->updateData();
				done = true;
			}
			break;
		case ROB_VALUE_CHANGED:
			{
				const CParameter &param = _Op->getParameter (idWidget);
				float oldValue = param.Value;
				float newValue = ((CRotButton*)(Widgets[idWidget]))->getValue();

				doc->addModification (new CActionSetParameterFloat (doc->getOperatorId(_Op), Widgets[idWidget]->GetDlgCtrlID()-FirstID,
					oldValue, newValue));
				doc->updateData();
			}
			break;
		case ROB_CAPTURED:
			((CFrameWnd*)(theApp.m_pMainWnd))->SetMessageText ("Hold Ctrl or Shift to disable the snap");
			break;
		case ROB_RELEASED:
			((CFrameWnd*)(theApp.m_pMainWnd))->SetMessageText ("");
			doc->endModification();
			break;
		case EN_RETURN:
		case EN_KILLFOCUS:
			{
				// Add an action
				const CParameter &param = _Op->getParameter (idWidget);
				if (param.Type == CParameter::TypeFloat)
				{
					// Get current colors
					char number[100];
					::GetWindowText (hwndWidget, number, 100);

					float newValue = (float)atof(number);
					CRotButton *rot = (CRotButton*)(Widgets[idWidget]);
					newValue = rot->angleToValue (rot->valueToAngle (newValue));

					float oldValue = param.Value;

					doc->addModification (new CActionSetParameterFloat (doc->getOperatorId(_Op), idWidget,
						oldValue, newValue));
					done = true;
				}
				else if (param.Type == CParameter::TypeInt)
				{
					// Get current colors
					char number[100];
					::GetWindowText (hwndWidget, number, 100);
					int newValue = atoi(number);

					int oldValue = param.Integer;

					doc->addModification (new CActionSetParameterInt (doc->getOperatorId(_Op), idWidget,
						oldValue, newValue));
					done = true;
				}
				else // if (param.Type == CParameter::TypeString)
				{
					if ((HIWORD(wParam) != EN_RETURN) || !param.Integer)
					{
						// Get current colors
						CString newValue;
						CExtEdit *edit = (CExtEdit*)(Widgets[idWidget]);
						edit->GetWindowText (newValue);
						string oldValue = param.StringValue;

						doc->addModification (new CActionSetParameterString (doc->getOperatorId(_Op), idWidget,
							oldValue.c_str(), (const char*)newValue));
						done = true;
					}
				}
				if (done)
				{
					doc->endModification();
					doc->updateData();
				}
			}
			break;
		case BN_CLICKED:
			{
				if (idWidget < Widgets.size())
				{
					const CParameter &param = _Op->getParameter (idWidget);
					int oldValue = param.Integer;
					int newValue = ((CExtBtnOnFlat*)(Widgets[idWidget]))->GetCheck();

					doc->addModification (new CActionSetParameterInt (doc->getOperatorId(_Op), idWidget,
						oldValue, newValue));
					doc->endModification();
					doc->updateData();
					done = true;
				}
			}
			break;
		}
	}	
	
	return done ? TRUE : CDialog::OnCommand(wParam, lParam);
}

// ***************************************************************************

BOOL COpPropertyDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (TranslateAccelerator(*(theApp.m_pMainWnd), DialogShortcut, pMsg))
		return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

// ***************************************************************************

void COpPropertyDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Window initialized ?
	if (_Initialized)
	{
		Draft = true;
		nPos = ((CSliderCtrl*)pScrollBar)->GetPos();

		// Change the document
		CNelTexGenView *view = (CNelTexGenView*)(_OpWindow->GetParent());
		CNelTexGenDoc *doc = view->GetDocument();

		// Get current colors
		float newValue = (float)nPos/(float)SPINNER_MAX;

		// Add an action
		const CParameter &param = _Op->getParameter (pScrollBar->GetDlgCtrlID()-FirstID);

		float oldValue = param.Value;

		((CFrameWnd*)(theApp.m_pMainWnd))->SetMessageText
			(((string)param.Name+" : "+toString(newValue)+(param.Setup.Unit?param.Setup.Unit:"")).c_str());

		doc->addModification (new CActionSetParameterFloat (doc->getOperatorId(_Op), pScrollBar->GetDlgCtrlID()-FirstID,
			oldValue, newValue));
		doc->updateData();
	}
	
	// CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// ***************************************************************************

BOOL COpPropertyDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR *pNMHDR =(NMHDR*)lParam;
	switch (pNMHDR->code)
	{
	case NM_RELEASEDCAPTURE:
		{
			if ((wParam<_Op->getNumParameter()) && (_Op->getParameter(wParam-FirstID).Type == CParameter::TypeFloat))
			{
				Draft = false;
				// Change the document
				CNelTexGenView *view = (CNelTexGenView*)(_OpWindow->GetParent());
				CNelTexGenDoc *doc = view->GetDocument();

				doc->endModification();
				doc->updateData();
			}
		}
		break;
	}
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

// ***************************************************************************

void COpPropertyDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	delete this;	
}

// ***************************************************************************

void COpPropertyDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (pScrollBar && (nSBCode==SB_THUMBPOSITION))
	{
		CNelTexGenView *view = (CNelTexGenView*)(_OpWindow->GetParent());
		CNelTexGenDoc *doc = view->GetDocument();
		uint idWidget = pScrollBar->GetDlgCtrlID()-FirstID;

		// Add an action
		const CParameter &param = _Op->getParameter (idWidget);
		doc->addModification (new CActionSetParameterInt (doc->getOperatorId(_Op), idWidget,
			param.Integer, nPos));

		doc->updateData ();
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

// ***************************************************************************
