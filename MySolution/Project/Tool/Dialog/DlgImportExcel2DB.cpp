// dlg_export_excel_to_db.cpp : implementation file
//

#include "stdafx.h"
#include "DlgImportExcel2DB.h"
#include "..\Pdbt_Common.h"
#include "..\Source\Pdbt_Db_Connect.h"
#include "..\Source\Pdbt_DrawIndexTable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImportExcel2DB dialog


CDlgImportExcel2DB::CDlgImportExcel2DB(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImportExcel2DB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImportExcel2DB)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgImportExcel2DB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImportExcel2DB)
	DDX_Control(pDX, IDC_STATIC_DB_PATH, m_edit_db_path);
	DDX_Control(pDX, IDC_EDIT_EXCEL_PATH, m_edit_excel_path);
	DDX_Control(pDX, IDC_COMBO_SEL_TABLE, m_combo_sel_table);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgImportExcel2DB, CDialog)
	//{{AFX_MSG_MAP(CDlgImportExcel2DB)
	ON_CBN_SELCHANGE(IDC_COMBO_SEL_TABLE, OnSelchangeComboSelTable)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgImportExcel2DB::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SELECT, &CDlgImportExcel2DB::OnBnClickedBtnSelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImportExcel2DB message handlers

void CDlgImportExcel2DB::OnOK() 
{
	// TODO: Add extra validation here
	
	
	if (m_strExcelPath.IsEmpty())
	{
		MessageBox(_T("未选择excel路径！"));
		return;
	}
	
	int iIndex = m_combo_sel_table.GetCurSel() ; 

	CString strItemText ; 
	m_combo_sel_table.GetLBText( iIndex , strItemText ) ; 

	CComParamSetBase * pComParmSetOpr = NULL ;

	if(strItemText.Compare(pmdbtool::g_strComParamSet_Global) == 0)
	{
		pComParmSetOpr = PMNew(CPdbt_DrawIndexTable);
	}
	
	BOOL bSuccess = TRUE;
	if ( NULL != pComParmSetOpr )
	{
		bSuccess = pComParmSetOpr->Export2Db(m_strExcelPath) ;

		//if ( pmdbtool::eCombo_Check == m_check_build_const_file.GetCheck() )
		//{
		//	pComParmSetOpr->Export2ConstFile( (LPCTSTR)strExcelFile , (LPCTSTR)strConstFile ) ;
		//}
	}

	PMDelete( pComParmSetOpr ) ; 

	if (bSuccess)
	{
		AfxMessageBox(_T("处理成功!"));
	}
	else
	{
		AfxMessageBox(_T("导入失败!"));
	}

	//CDialog::OnOK();
}

BOOL CDlgImportExcel2DB::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitFace() ; 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgImportExcel2DB::OnSelchangeComboSelTable() 
{
	// TODO: Add your control notification handler code here
	int iIndex = m_combo_sel_table.GetCurSel() ; 

	CString strItemText ; 
	m_combo_sel_table.GetLBText( iIndex , strItemText ) ; 

}


void	CDlgImportExcel2DB::InitFace()
{

	m_edit_excel_path.SetWindowText(m_strExcelPath) ;

	/// db path 
	CString strDbPath = CConnectManager::Instance()->GetFilePath( (CPmDataBase::DataBaseName)CDbConnect::eMyDb ) ; 
	m_edit_db_path.SetWindowText( strDbPath ) ; 

	/// sel table list 
	m_combo_sel_table.AddString( pmdbtool::g_strComParamSet_Total) ;
	m_combo_sel_table.AddString( pmdbtool::g_strComParamSet_Global);

	m_combo_sel_table.SetCurSel(1) ; 

}

void CDlgImportExcel2DB::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void CDlgImportExcel2DB::OnBnClickedBtnSelect()
{
	CFileDialog dlg(TRUE);
	if (IDOK == dlg.DoModal())
	{
		m_strExcelPath = dlg.GetPathName();
	}
	m_edit_excel_path.SetWindowText(m_strExcelPath);

}
