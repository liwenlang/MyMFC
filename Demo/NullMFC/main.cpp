#pragma once
#include <afxwin.h>


class MyApp : public CWinApp
{
public:
	virtual BOOL InitInstance()						//�ڳ������   
	{
		CFrameWnd* Frame = new CFrameWnd();			//������   
		m_pMainWnd = Frame;							//��m_pMainWnd�趨ΪFrame;   
		Frame->Create(NULL, _T("��򵥵Ĵ���"));	//�������   
		Frame->ShowWindow(SW_SHOW);					//��ʾ���   
		return true;								//����   
	}
};
MyApp theApp;										//�ٽ���Ӧ�ó���  

//CRuntimeClass

//RTTI
//DECLARE_DYNAMIC
//IMPLEMENT_DYNAMIC   IMPLEMENT_RUNTIMECLASS

//RUNTIME_CLASS

//��̬����
//DECLARE_DYNCREATE
//IMPLEMENT_DYNCREATE

//����
//DECLARE_SERIAL 
// IMPLEMENT_SERIAL

//��Ϣӳ��
//DECLARE_MESSAGE_MAP()
//BEGIN_MESSAGE_MAP(CDlgTestScore, CDialog)
	//ON_BN_CLICKED(IDC_BTN_CMP, &CDlgTestScore::OnBnClickedBtnCmp)
//END_MESSAGE_MAP()

//ON_COMMAND