#if !defined(AFX_DLG_SEL_TABLE_H__CD85A764_44B8_464E_92A8_D2153C1C073F__INCLUDED_)
#define AFX_DLG_SEL_TABLE_H__CD85A764_44B8_464E_92A8_D2153C1C073F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_sel_table.h : header file
//

#include "..\Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlg_Sel_Table dialog

class CDlg_Sel_Table : public CDialog
{
// Construction
public:
	CDlg_Sel_Table(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_Sel_Table)
	enum { IDD = IDD_DIALOG_SELECT_TABLE };
	//}}AFX_DATA

public:
	CString		GetSql() const;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:


	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()


protected:
	CString		m_strSql ; 
	CComboBox	m_combobox_sel_table;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_SEL_TABLE_H__CD85A764_44B8_464E_92A8_D2153C1C073F__INCLUDED_)
