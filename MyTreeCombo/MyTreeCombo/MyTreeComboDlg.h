// MyTreeComboDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "TreeComboBox.h"

// CMyTreeComboDlg �Ի���
class CMyTreeComboDlg : public CDialog
{
// ����
public:
	CMyTreeComboDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYTREECOMBO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CTreeComboBox m_combo;
};
