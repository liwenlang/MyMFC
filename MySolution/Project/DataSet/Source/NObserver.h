// NObserver.h: interface for the NObserver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOBSERVER_H__0EAC617B_F75F_45ED_ACD7_728EDA6C59C0__INCLUDED_)
#define AFX_NOBSERVER_H__0EAC617B_F75F_45ED_ACD7_728EDA6C59C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//观察者基类，可与窗口类进行组合继承后作为视图与数据分开使用的类。
class NObserver  
{
public:
	virtual BOOL UpdateObserver( CObject * pModel,LPARAM lHint, CObject*pHint) = 0;
	NObserver();
	virtual ~NObserver();

};

#endif // !defined(AFX_NOBSERVER_H__0EAC617B_F75F_45ED_ACD7_728EDA6C59C0__INCLUDED_)
