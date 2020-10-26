#if !defined(AFX_DLG_EXPORT_EXCEL_TO_DB_H__A7C3BF76_033E_49EE_A023_83112E57E96B__INCLUDED_)
#define AFX_DLG_EXPORT_EXCEL_TO_DB_H__A7C3BF76_033E_49EE_A023_83112E57E96B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_export_excel_to_db.h : header file
//

#include "../Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgImportExcel2DB dialog

class CDlgImportExcel2DB : public CDialog
{
// Construction
public:
	CDlgImportExcel2DB(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgImportExcel2DB)
	enum { IDD = IDD_DIALOG_EXCEL2DB };
	CStatic	m_edit_db_path;
	CEdit	m_edit_excel_path;
	CComboBox	m_combo_sel_table;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImportExcel2DB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImportExcel2DB)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboSelTable();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	/// 初始化界面
	void	InitFace() ; 

private:
	CString	m_strExcelPath;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_EXPORT_EXCEL_TO_DB_H__A7C3BF76_033E_49EE_A023_83112E57E96B__INCLUDED_)
#pragma once

