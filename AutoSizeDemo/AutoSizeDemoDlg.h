// AutoSizeDemoDlg.h : header file
//

#if !defined(AFX_AUTOSIZEDEMODLG_H__F42D7E29_6174_46BE_936B_04D3DC932762__INCLUDED_)
#define AFX_AUTOSIZEDEMODLG_H__F42D7E29_6174_46BE_936B_04D3DC932762__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAutoSizeDemoDlg dialog

class CAutoSizeDemoDlg : public CDialog
{
// Construction
public:
	CAutoSizeDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoSizeDemoDlg)
	enum { IDD = IDD_AUTOSIZEDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoSizeDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAutoSizeDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOSIZEDEMODLG_H__F42D7E29_6174_46BE_936B_04D3DC932762__INCLUDED_)
