#pragma once
#include <afxwin.h>


class MyApp : public CWinApp
{
public:
	virtual BOOL InitInstance()						//②程序入点   
	{
		CFrameWnd* Frame = new CFrameWnd();			//构造框架   
		m_pMainWnd = Frame;							//将m_pMainWnd设定为Frame;   
		Frame->Create(NULL, _T("最简单的窗口"));	//建立框架   
		Frame->ShowWindow(SW_SHOW);					//显示框架   
		return true;								//返回   
	}
};
MyApp theApp;										//①建立应用程序。  

//CRuntimeClass

//RTTI
//DECLARE_DYNAMIC
//IMPLEMENT_DYNAMIC   IMPLEMENT_RUNTIMECLASS

//RUNTIME_CLASS

//动态创建
//DECLARE_DYNCREATE
//IMPLEMENT_DYNCREATE

//串化
//DECLARE_SERIAL 
// IMPLEMENT_SERIAL

//消息映射
//DECLARE_MESSAGE_MAP()
//BEGIN_MESSAGE_MAP(CDlgTestScore, CDialog)
	//ON_BN_CLICKED(IDC_BTN_CMP, &CDlgTestScore::OnBnClickedBtnCmp)
//END_MESSAGE_MAP()

//ON_COMMAND