// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

using namespace std;	//“ifstream”: 未声明的标识符

//包含一些必要的头文件
#include <fstream>
#include <strstream>
#include <set>
#include <atlbase.h>
#include <tlhelp32.h>
#include <afxtempl.h>

#include <RpcDce.h>  // LNK2019: 无法解析的外部符号 __imp_RpcStringFreeW
#pragma comment(lib, "Rpcrt4.lib")

//模板类 头文件
#include <afxtempl.h>	//tempname
#include <vector>		//
#include <map>
#include <comdef.h>  //“_bstr_t”: 未声明的标识符

#define MaxFieldBufferSize				100000000  //认为是buffer申请的极大值

#ifndef DataSet_Moudle
#define DataSet_Moudle
#endif

//包含自身
#include "DataSetH.h"