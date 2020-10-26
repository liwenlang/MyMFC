// dlg_sel_table.cpp : implementation file
//
#include "stdafx.h"
#include "DlgSelTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const CString SEL_ALL_TABLE = _T("select * from sqlite_master  where type = 'table' order by name;") ;
const CString SEL_DATA_FROM_TABLE = _T("select * from %s") ; 
/////////////////////////////////////////////////////////////////////////////
// CDlg_Sel_Table dialog


CDlg_Sel_Table::CDlg_Sel_Table(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Sel_Table::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Sel_Table)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_Sel_Table::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Sel_Table)
	DDX_Control(pDX, IDC_COMBO_TABLE_LIST, m_combobox_sel_table);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_Sel_Table, CDialog)
	//{{AFX_MSG_MAP(CDlg_Sel_Table)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_Sel_Table message handlers

void CDlg_Sel_Table::OnOK() 
{
	// TODO: Add extra validation here
	
	int iIndex = m_combobox_sel_table.GetCurSel() ; 

	CString strItemText ; 
	m_combobox_sel_table.GetLBText( iIndex , strItemText ) ; 

	m_strSql.Format( SEL_DATA_FROM_TABLE , strItemText ) ;

	CDialog::OnOK();
}

CString		CDlg_Sel_Table::GetSql() const 
{
	return m_strSql ;
}


BOOL CDlg_Sel_Table::OnInitDialog()
{
	CDialog::OnInitDialog() ;

	NDataModelDataSet * pDataSet = NULL ; 
	
	Db_Opr.ExecuteSQL( SEL_ALL_TABLE ,pDataSet ) ;

	if ( NULL == pDataSet )
	{
		return FALSE ; 
	}

	int iCount = pDataSet->GetRowCount() ; 

	int i = 1 ; 
	for ( ; i <= iCount ; ++i )
	{
		CString strTableName = pDataSet->GetAsString( _T("name") , i ) ;

		m_combobox_sel_table.AddString( strTableName ) ;
	}

	PMDelete( pDataSet ) ; 
	return TRUE ; 
}
