// DataServiceAbsolute.h: interface for the CDataServiceAbsolute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASERVICEABSOLUTE_H__3CB30CCC_D639_497F_B561_B776BD184C02__INCLUDED_)
#define AFX_DATASERVICEABSOLUTE_H__3CB30CCC_D639_497F_B561_B776BD184C02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "IdataFill.h"

/**
* @brief   读取数据的 抽象接口
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: 王建业
* @date  : [18/7/2013]
*/
class CDataServiceAbsolute  
{
public:
	virtual BOOL FillDataSet(IDataFill *pDataSet,CString selectSQL)
	{
		return FALSE;
	}
	virtual BOOL Retrieve(CString &sbuff,CString &sSql) = 0;
	virtual BOOL RetrieveXml(CString &sBuff,CString &sSQL) = 0;
	virtual BOOL ExecuteNonQuery(CString sSQL) = 0;
	virtual BOOL Rollback()
	{
		return TRUE;
	}
	virtual BOOL Commit()
	{
		return TRUE;
	}
	virtual BOOL BeginTrans()
	{
		return TRUE;
	}
	virtual BOOL Connect(CString connectstr,CString userID,CString PassWord)
	{
		return TRUE;
	}
	virtual BOOL Init()
	{
		return TRUE;
	}
	virtual BOOL GetConnecting()
	{
		return TRUE;
	}
	//大数据块的支持
	virtual BOOL SelectBlob(CString SQLStatement, LPSTR *Buf, int *size)
	{
		return TRUE;
	}
	virtual BOOL SelectBlobToFile(CString SQLStatement, CString Filename)
	{
		return TRUE;
	}
	virtual BOOL UpdateBlob(CString SQLStatement, LPSTR Buf, int size)
	{
		return TRUE;
	}
	virtual BOOL UpdateBlobFromFile(CString SQLStatement, CString Filename)
	{
		return TRUE;
	}
	virtual CString GetLastError()
	{
		return m_error;
	}
public:
	CDataServiceAbsolute();
	virtual ~CDataServiceAbsolute();
    CString m_error;
};

#endif // !defined(AFX_DATASERVICEABSOLUTE_H__3CB30CCC_D639_497F_B561_B776BD184C02__INCLUDED_)
