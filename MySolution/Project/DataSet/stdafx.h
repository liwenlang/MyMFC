// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

using namespace std;	//��ifstream��: δ�����ı�ʶ��

//����һЩ��Ҫ��ͷ�ļ�
#include <fstream>
#include <strstream>
#include <set>
#include <atlbase.h>
#include <tlhelp32.h>
#include <afxtempl.h>

#include <RpcDce.h>  // LNK2019: �޷��������ⲿ���� __imp_RpcStringFreeW
#pragma comment(lib, "Rpcrt4.lib")

//ģ���� ͷ�ļ�
#include <afxtempl.h>	//tempname
#include <vector>		//
#include <map>
#include <comdef.h>  //��_bstr_t��: δ�����ı�ʶ��

#define MaxFieldBufferSize				100000000  //��Ϊ��buffer����ļ���ֵ

#ifndef DataSet_Moudle
#define DataSet_Moudle
#endif

//��������
#include "DataSetH.h"