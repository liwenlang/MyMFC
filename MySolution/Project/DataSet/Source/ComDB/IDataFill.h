// IDataFill.h: interface for the IDataFill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDATAFILL_H__9F030E19_6A5D_4ABB_8759_2EB9EA852C99__INCLUDED_)
#define AFX_IDATAFILL_H__9F030E19_6A5D_4ABB_8759_2EB9EA852C99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IDataFill  
{
// public:
// 	IDataFill();
// 	virtual ~IDataFill();

public:
	virtual BOOL SetValue(int colIndex,int rowIndex,CString &columnName,VARIANT &value) = 0;
	virtual int  AddRow() = 0;
};

#endif // !defined(AFX_IDATAFILL_H__9F030E19_6A5D_4ABB_8759_2EB9EA852C99__INCLUDED_)
