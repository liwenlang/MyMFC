// AutoSizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoSizeDemo.h"
#include "AutoSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoSizeDlg dialog


CAutoSizeDlg::CAutoSizeDlg(CWnd* pParent /*=NULL*/)
	: ClxDialog(CAutoSizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoSizeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAutoSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	ClxDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoSizeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoSizeDlg, ClxDialog)
	//{{AFX_MSG_MAP(CAutoSizeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoSizeDlg message handlers

BOOL CAutoSizeDlg::OnInitDialog() 
{
	ClxDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    GetDlgItem(IDC_EDIT1)->SetWindowText("Edit1");
	GetDlgItem(IDC_EDIT2)->SetWindowText("Edit2");
	GetDlgItem(IDC_EDIT3)->SetWindowText("Edit3");
	GetDlgItem(IDC_EDIT4)->SetWindowText("Edit4");
	GetDlgItem(IDC_EDIT5)->SetWindowText("Edit5");
	GetDlgItem(IDC_EDIT6)->SetWindowText("Edit6");

	static DLGCTLINFO  dcMenuGroup[] = {
		{IDOK, MOVEX, 100},
		{IDCANCEL, MOVEX, 100},
		{IDC_BUTTON1, MOVEX, 50},
		{IDC_BUTTON1, MOVEY, 100},
		{IDC_EDIT1, ELASTICX, 100},
		{IDC_EDIT2, ELASTICX, 50},
		{IDC_EDIT3, ELASTICX, 50},
		{IDC_EDIT3, MOVEX, 50},
		{IDC_EDIT4, ELASTICY, 100},
		{IDC_EDIT5, ELASTICX, 100},
		{IDC_EDIT5, ELASTICY, 50},
		{IDC_EDIT6, ELASTICX, 100},
		{IDC_EDIT6, ELASTICY, 50},
		{IDC_EDIT6, MOVEY, 50},
	};

	SetControlProperty(dcMenuGroup, sizeof(dcMenuGroup)/sizeof(DLGCTLINFO));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
