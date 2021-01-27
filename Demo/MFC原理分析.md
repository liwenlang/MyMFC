# MFC原理分析

 首先，列出要讲的MFC六大核心机制：

​    1、MFC程序的初始化。
​    2、运行时类型识别（RTTI）。
​    3、动态创建。
​    4、永久保存。
​    5、消息映射。
​    6、消息传递。



## 一.MFC程序的初始化

#### **简单的MFC窗口程序**

建立一个MFC窗口很容易，只用两步：一是从CWinApp派生一个应用程序类（这里是MyApp）

二是建立应用程序对象（theApp），就可以产生一个自己需要的窗口

**步骤：**

1.VS创建创建空项目

2.选择共享DLL中使用MFC, 代码生成运行库选择多线程调试 DLL (/MDd)

3.配置window平台

​	1）菜单中选择 Project->Properties, 弹出Property Pages窗口

​	2）在左边栏中依次选择：Configuration Properties->C/C++->Preprocessor,然后在右边栏的Preprocessor Definitions对应的项中删除_CONSOLE, 添加_WINDOWS.

​	3）在左边栏中依次选择：Configuration Properties->Linker->System,然后在右边栏的SubSystem对应的项改为Windows(/SUBSYSTEM:WINDOWS)

```c++
#pragma once
#include <afxwin.h>


class MyApp : public CWinApp
{
public:
	BOOL InitInstance()						 //②程序入点   
	{
		CFrameWnd* Frame = new CFrameWnd();		//构造框架   
		m_pMainWnd = Frame;						//将m_pMainWnd设定为Frame;   
		Frame->Create(NULL, _T("最简单的窗口"));//建立框架   
		Frame->ShowWindow(SW_SHOW);				//显示框架   
		return true;						 //返回   
	}
};
MyApp theApp;								//①建立应用程序。  

```

