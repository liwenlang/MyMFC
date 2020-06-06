#if !defined(AFX_AUTOSIZEDLG_H__96D7D2D5_F2E3_4C6F_97FF_D3A6D54EA2BD__INCLUDED_)
#define AFX_AUTOSIZEDLG_H__96D7D2D5_F2E3_4C6F_97FF_D3A6D54EA2BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoSizeDlg.h : header file
//

#include "lxDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoSizeDlg dialog

class CAutoSizeDlg : public ClxDialog
{
// Construction
public:
	CAutoSizeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoSizeDlg)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoSizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoSizeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOSIZEDLG_H__96D7D2D5_F2E3_4C6F_97FF_D3A6D54EA2BD__INCLUDED_)
