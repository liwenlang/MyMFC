// MyTreeCombo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyTreeComboApp:
// �йش����ʵ�֣������ MyTreeCombo.cpp
//

class CMyTreeComboApp : public CWinApp
{
public:
	CMyTreeComboApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyTreeComboApp theApp;