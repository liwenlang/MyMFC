// ComDB.h: interface for the CComDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMDB_H__E95F22B9_881A_44F5_A1EB_3EFEBBAA6475__INCLUDED_)
#define AFX_COMDB_H__E95F22B9_881A_44F5_A1EB_3EFEBBAA6475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#import <COMDATA.dll>
#import "COMDATA.dll"
//#include "..\actx\act2.3.0.0\ACTX\COleDb\COleDb.h"

using namespace COMDATALib;

#include "DataServiceAbsolute.h"

//Access读取数据的 具体实现
class DATASETDLLEXPORT CComDB :public CDataServiceAbsolute
{
public:
	virtual BOOL FillDataSet(IDataFill *pDataSet,CString selectSQL);
	BOOL DisConnect();
	//单一对象支持
	//static void Release();
	//static CComDB* GetSingle();
	//大数据存取函数
	virtual BOOL SelectBlob(CString SQLStatement, LPSTR *Buf, int *size);
	virtual BOOL SelectBlobToFile(CString SQLStatement, CString Filename);
	virtual BOOL UpdateBlob(CString SQLStatement, LPSTR Buf, int size);
	virtual BOOL UpdateBlobFromFile(CString SQLStatement, CString Filename);
	
    //数据检索等数据操作支持
	virtual BOOL GetConnecting();
	virtual BOOL RetrieveXml(CString &sBuff,CString &sSQL);
	virtual BOOL ExecuteNonQuery(CString sSQL);
	virtual BOOL Rollback();
	virtual BOOL Commit();
	virtual BOOL BeginTrans();
	virtual BOOL Retrieve(CString &sbuff,CString &sSql);
	virtual BOOL Connect(CString connectstr,CString userID,CString PassWord);
	virtual BOOL Init();
//log文件支持
	CString GetCurFullTime();
	BOOL SetDBErrLogFile(CString sFileName);
	void WriteLine(CFile& cFile, LPCTSTR buf);
	BOOL TrackDBError();
	void Error(CString error);
	BOOL OpenLogFile();
	BOOL FileExists(LPCTSTR filename);
	BOOL Create1File(LPCTSTR filename);
	BOOL IsFileReadOnly(LPCTSTR filename);
	BOOL SetFileReadOnly(LPCTSTR filename, BOOL isReadonly);
	virtual CString GetLastError();

	CComDB();
	virtual ~CComDB();

protected:
	//IDBMS *m_pIDBMS;
	COMDATALib::IDBMS2Ptr m_pIDBMS;
	BOOL m_connecting;
	//log
	CString m_LogFileName;
	BOOL    m_bOpening;
	CFile	m_cFile;
	BOOL    m_TrackSQLErr;
	//static CComDB * m_pComDB;
};

#endif // !defined(AFX_COMDB_H__E95F22B9_881A_44F5_A1EB_3EFEBBAA6475__INCLUDED_)
