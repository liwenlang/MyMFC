 // ComDB.cpp: implementation of the CComDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "CComDB.h"
#include <sys/timeb.h>

#define CPUB_BUFLEN			256

/*#include "..\..\actx\ACTX\COMDATA\comdata.h"*/

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//CComDB *CComDB::m_pComDB = NULL;

CComDB::CComDB()
{
    m_pIDBMS = NULL;
	m_connecting = FALSE;
	m_bOpening = FALSE;
	m_TrackSQLErr = FALSE;
	
}

CComDB::~CComDB()
{


	if (m_bOpening) {
		m_cFile.Close();
	}
}


BOOL CComDB::Init()
{
 //HRESULT hr = CoCreateInstance(CLSID_DBMS,NULL,CLSCTX_INPROC_SERVER,IID_IDBMS,reinterpret_cast<void**>(&m_pIDBMS));
 HRESULT hr=m_pIDBMS.CreateInstance(__uuidof(COMDATALib::DBMS2));

	//HRESULT hr=m_pIDBMS
   if (!FAILED(hr)) 
   {
	      SetDBErrLogFile("ngdblog.log");
	   return TRUE;
   }
   CString text;
   text.Format(_T("错误号:%d"),hr);
   //m_pIDBMS->SQLLog("ngdblog.log");

   MessageBox(NULL,_T("初始化数据库连接组件失败,请确认是否成功安装了数据库连接组件！")+text,_T("错误"),MB_OK);
   return FALSE;
}

BOOL CComDB::Connect(CString connectstr, CString userID, CString PassWord)
{
	if (m_pIDBMS == NULL) 
	{
		return FALSE;
	}
    BSTR btrconnect = connectstr.AllocSysString();
	BSTR btrID = userID.AllocSysString();
	BSTR btrPassWord = PassWord.AllocSysString();

	HRESULT hr = m_pIDBMS->Connect(btrconnect,btrID,btrPassWord);
	SysFreeString(btrconnect);
	SysFreeString(btrID);
	SysFreeString(btrPassWord);

	VARIANT_BOOL boolr = m_pIDBMS->IsInTrans();
	if (FAILED(hr)||hr  == 0)
	{

		return FALSE;
	}
	m_connecting = TRUE;
    return TRUE;
}

BOOL CComDB::Retrieve(CString &sbuff, CString &sSql)
{
	if (!m_connecting) {
		return FALSE;
	}
	  VARIANT_BOOL bresult; 
	   BSTR  ptr = NULL;
	   BSTR  bssql = sSql.AllocSysString();
       bresult = m_pIDBMS->GetRecordset(bssql,&ptr);

	   SysFreeString(bssql);

	   sbuff = ptr;
	   //修改unicode时，屏蔽
// 	   char *aa1 = NULL;
// 
// 	   /*USES_CONVERSION;
// 	   aa1 = OLE2T(ptr);//W2A(ptr);*/
// 
//        DWORD size =  WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)ptr, -1,
//                      aa1, 0, NULL, NULL );
//        //aa1 = new char[size];
//        
// 	   aa1 = sbuff.GetBufferSetLength(size);
// 
//        WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)ptr, -1,
//                      aa1, size, NULL, NULL );
// 
// 
// 	   //sbuff = aa1;
//        //delete []aa1;
 	   if (ptr != NULL) {
 		   SysFreeString(ptr);
 	   }
	if (m_TrackSQLErr && (!bresult)) 
	{
		TrackDBError();
	}
	 return bresult;
}

BOOL CComDB::BeginTrans()
{
	if (!m_connecting) {
		return FALSE;
	}
	return m_pIDBMS->BeginTrans();
}

BOOL CComDB::Commit()
{
	if (!m_connecting) {
		return FALSE;
	}
	
	return m_pIDBMS->Commit();
}

BOOL CComDB::Rollback()
{
	if (!m_connecting) {
		return FALSE;
	}
	return m_pIDBMS->Rollback();
}

BOOL CComDB::ExecuteNonQuery(CString sSQL)
{
	if (!m_connecting) 
	{
		return FALSE;
	}
	VARIANT_BOOL bresult;
	long RecordsAffect;
	BSTR btrSQL = sSQL.AllocSysString();
	bresult = m_pIDBMS->ExecuteNonQuery(btrSQL,&RecordsAffect);
	SysFreeString(btrSQL);
	if (m_TrackSQLErr && (!bresult)) 
	{
		TrackDBError();
	}
	return bresult;
}

BOOL CComDB::RetrieveXml(CString &sBuff, CString &sSQL)
{

	VARIANT_BOOL bresult; 
	if (!m_connecting) {
		return FALSE;
	}

	   BSTR  ptr = NULL;
	   BSTR  bssql = sSQL.AllocSysString();
	   int rowcount;
       bresult = m_pIDBMS->GetRecordsetXMLBuf(bssql,&ptr,&rowcount,0,-1);

	   SysFreeString(bssql);

	   sBuff = ptr;
// 	   char *aa1 = NULL;
// 
// 	   /*USES_CONVERSION;
// 	   aa1 = OLE2T(ptr);//W2A(ptr);*/
// 
//        DWORD size =  WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)ptr, -1,
//                      aa1, 0, NULL, NULL );
//        //aa1 = new char[size];
//        
// 	   aa1 = sBuff.GetBufferSetLength(size);
// 
//        WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)ptr, -1,
//                      aa1, size, NULL, NULL );
// 
// 
// 	   //sbuff = aa1;
//        //delete []aa1;
	   if (ptr != NULL) {
		   SysFreeString(ptr);
	   }
	if (m_TrackSQLErr && (!bresult)) 
	{
		TrackDBError();
	}
	 return bresult;

}

BOOL CComDB::GetConnecting()
{
   return m_connecting;
}

CString CComDB::GetLastError()
{
		CString serr;
		if(m_pIDBMS != NULL)
		{
			serr = (LPSTR)m_pIDBMS->GetLastError();
		}
		
		return serr;
}

BOOL CComDB::OpenLogFile()
{
	///////////////////////////////////
	///王建业 
	///为测试08运行 将LogFile关闭
	///[27/2/2009]
	//////////////////////////////////
	return TRUE;

	////if(!IsEnabled()) return FALSE;
	//if(m_bOpening) return TRUE;

	////创建文件
	//if(!FileExists(m_LogFileName)) {
	//	if(!Create1File(m_LogFileName)) {
	//		m_LogFileName.Empty();
	//		return FALSE;
	//	}
	//}

	////尝试打开4秒
	//CFileException ex;
	//BOOL b=FALSE;
	//for(int i=0; i<10; i++) {
	//	try {
	//		b=m_cFile.Open(m_LogFileName, CFile::modeWrite|CFile::shareExclusive, &ex);
	//	}
	//	catch(CFileException *exp) {
	//	}
	//	catch(...) {
	//	}
	//	if(b)
	//		break;
	//	if(i>8) {
	//		m_LogFileName.Empty();
	//		return FALSE;
	//	}
	//	Sleep(500);
	//}

	////Unicode头
	////if(m_cFile.GetLength()==0) CPub::WriteUnicodeFileFlag(m_cFile);
	//m_cFile.SeekToEnd();
	//m_bOpening=TRUE;
	//return TRUE;
}

//文件是否存在(用CFileFind方式)
BOOL  CComDB::FileExists(LPCTSTR filename)
{
	CFileStatus cStatus;
	if(CFile::GetStatus(filename, cStatus))
		if(!(cStatus.m_attribute & CFile::directory)) return TRUE;
	return FALSE;
}

//创建一个文件
BOOL  CComDB::Create1File(LPCTSTR filename)
{
	CFile cFile;
	CFileException ex;

	//除去只读标志
	if(FileExists(filename)) {
		if(IsFileReadOnly(filename))
			if(!SetFileReadOnly(filename, FALSE)) return FALSE;
	}

	//生成
	try {
		if(!cFile.Open(filename,CFile::modeCreate | CFile::shareExclusive,&ex)) {
			TCHAR szError[CPUB_BUFLEN];
			ex.GetErrorMessage(szError, CPUB_BUFLEN);
			Error(szError);
			return FALSE;
		}
		cFile.Close();
	}
	catch(CFileException *exp) {
		TCHAR szError[CPUB_BUFLEN];
		exp->GetErrorMessage(szError, CPUB_BUFLEN);
		Error(szError);
		return FALSE;
	}
	catch(...) {
		CString s;
		s.Format(_T("File %s create error!	"), filename);
		Error(s);
		return FALSE;
	}
	return TRUE;
}

//文件是否只读
BOOL   CComDB::IsFileReadOnly(LPCTSTR filename)
{
	CFileStatus cStatus;
	if(CFile::GetStatus(filename, cStatus)==FALSE) return FALSE;
	if(cStatus.m_attribute & CFile::readOnly) return TRUE;
	return FALSE;
}
//更改文件的readOnly/normal属性
BOOL  CComDB::SetFileReadOnly(LPCTSTR filename, BOOL isReadonly)
{
	CFileStatus cStatus;
	if(CFile::GetStatus(filename, cStatus)==FALSE) return FALSE;

	if(isReadonly)
		cStatus.m_attribute |=CFile::readOnly;
	else
		cStatus.m_attribute &=~(CFile::readOnly);
	try {
		CFile::SetStatus(filename, cStatus);
	}
#if _MSC_VER < 1900	/// VS2015编译通不过
	catch(CFileException ex) {
		TCHAR szError[CPUB_BUFLEN];
		ex.GetErrorMessage(szError, CPUB_BUFLEN);
		Error(szError);
		return FALSE;
	}
#endif
	catch(...) {
		CString s;
		s.Format(_T("File %s attributes set error!	"), filename);
		Error(s);
		return FALSE;
	}
	return TRUE;
}

void CComDB::Error(CString error)
{

}

BOOL CComDB::TrackDBError()
{
	CString sErr;
	if (m_bOpening&&m_TrackSQLErr) 
	{
		//CString sTime = GetCurFullTime();
		COleDateTime  cdt;
		cdt = COleDateTime::GetCurrentTime();
		sErr = cdt.Format() + "  "+GetLastError();
		WriteLine(m_cFile, sErr.GetBuffer(0));
	}
   return TRUE;
}

//写一行
void CComDB::WriteLine(CFile& cFile, LPCTSTR buf)
{
	cFile.Write(buf, CString(buf).GetLength());
	cFile.Write(_T("\r\n"), 2);
}

BOOL CComDB::SetDBErrLogFile(CString sFileName)
{
   if (sFileName == _T("")) 
   {
	   return FALSE;
   }
   m_LogFileName = sFileName;
   OpenLogFile();
   m_TrackSQLErr = TRUE;
   return TRUE;
}

//获得当前时间
CString CComDB::GetCurFullTime()
{
	TCHAR tmpbuf[16];
    struct _timeb tstruct;
    _tzset();
    _tstrtime( tmpbuf );
    _ftime( &tstruct );

	CString s;
	s.Format(_T("%s %d"),tmpbuf, tstruct.millitm);
	return s;
}

BOOL CComDB::SelectBlob(CString SQLStatement, LPSTR *Buf, int *size)
{
	if (!m_connecting) {
		return FALSE;
	}
    BOOL bresult;
	SAFEARRAY *psa;
    bresult = m_pIDBMS->SelectBlob(SQLStatement.GetBuffer(0),&psa);
	//开始存储二进制数据
	LPSTR ptr;

	SafeArrayAccessData(psa, (void HUGEP**)&ptr);
	//*size = SafeArrayGetElemsize(psa); 
	*size=psa->rgsabound[0].cElements;
	
	*Buf = new (char[(*size)+1]);
	memset(*Buf,0,(*size)+1);
	memcpy(*Buf,ptr,*size);
    
	SafeArrayUnaccessData(psa);
	SafeArrayDestroy(psa);
	if (m_TrackSQLErr && (!bresult)) 
	{
		TrackDBError();
	}
	
    return bresult;
}

BOOL CComDB::SelectBlobToFile(CString SQLStatement, CString Filename)
{
	if (!m_connecting) {
		return FALSE;
	}
	BOOL bresult;
	bresult = m_pIDBMS->SelectBlobToFile(SQLStatement.GetBuffer(0),Filename.GetBuffer(0));
	if (m_TrackSQLErr && (!bresult)) 
	{
		TrackDBError();
	}
	return bresult ;
}

BOOL CComDB::UpdateBlob(CString SQLStatement, LPSTR Buf, int size)
{
	if (!m_connecting) {
		return FALSE;
	}
	BOOL bResult;
	SAFEARRAY * psa;
	SAFEARRAYBOUND rgsabound;
	LPSTR ptr;
	rgsabound.lLbound = 0;
	rgsabound.cElements = size;
	psa = SafeArrayCreate(VT_UI1, 1, &rgsabound);
	SafeArrayAccessData(psa, (void HUGEP**)&ptr);
	memcpy(ptr, Buf, size);
	SafeArrayUnaccessData(psa);
	bResult = m_pIDBMS->UpdateBlob(SQLStatement.GetBuffer(0),psa);
	SafeArrayDestroy(psa);
	if (m_TrackSQLErr && (!bResult)) 
	{
		TrackDBError();
	}
	return bResult;
}

BOOL CComDB::UpdateBlobFromFile(CString SQLStatement, CString Filename)
{
	if (!m_connecting) {
		return FALSE;
	}
	BOOL bresult;
	bresult = m_pIDBMS->UpdateBlobFromFile(SQLStatement.GetBuffer(0),Filename.GetBuffer(0));
	if (m_TrackSQLErr && (!bresult)) 
	{
		TrackDBError();
	}
	return bresult;
}
   /*
    VARIANT_BOOL SelectBlob (
           _bstr_t SQLStatement,
           SAFEARRAY * * ppsa );
       VARIANT_BOOL SelectBlobToFile (
           _bstr_t SQLStatement,
           _bstr_t Filename );
       VARIANT_BOOL UpdateBlob (
           _bstr_t SQLStatement,
           SAFEARRAY * psa );
       VARIANT_BOOL UpdateBlobFromFile (
           _bstr_t SQLStatement,
           _bstr_t Filename );*/
   
/*
CComDB* CComDB::GetSingle()
{
   if (m_pComDB == NULL) 
   {
	   m_pComDB = new CComDB;
   }
   return m_pComDB;
}

void CComDB::Release()
{
  if (m_pComDB != NULL) 
  {
	  delete m_pComDB;
	  m_pComDB = NULL;
  }
  
}*/

BOOL CComDB::DisConnect()
{
	if (m_pIDBMS != NULL) {
		m_pIDBMS->Disconnect();
		//m_pIDBMS->();
		m_pIDBMS = NULL;
	}
	return TRUE;
}

BOOL CComDB::FillDataSet(IDataFill *pDataSet, CString selectSQL)
{
	if (!m_connecting) {
		return FALSE;
	}
	//先取得数据对象
	//按照对象的内容进行填充
	//IUnknownPtr pIUnknown = NULL;
	COMDATALib::IDataSetPtr pIDataset = NULL;
	HRESULT hr = m_pIDBMS->GetDataSet(selectSQL.GetBuffer(0),&pIDataset);
	//COMDATALib::IIDataSetPtr pIDataset = NULL;
	//HRESULT hr = pIUnknown->QueryInterface(__uuidof(COMDATALib::IIDataSet),(void **)&pIDataset);
	 if (SUCCEEDED(hr))
	{
		 //pDataSetFill->FillItem()
         //bresult = m_pIDBMS->FillDataSet(&pIUnknown,bssql);
		 int cols = pIDataset->GetColCount();
		 //int rowsCount = pIDataset->GetRowCount();
		 BOOL datasetReady = pIDataset->IsRecordSetReady();
		 //_variant_t value;
		 BSTR  colName;
		 CString scolName;
		 for (int i = 0 ; datasetReady == TRUE ; i ++)
		 {
			 pDataSet->AddRow();
			 for (int j = 0 ; j < cols ; j++)
			 {
				 variant_t temvalue = pIDataset->GetValue(j,&colName);
				 _variant_t value(temvalue,false);
				 scolName = colName;
				 ::SysFreeString(colName);
				 pDataSet->SetValue(j,i,scolName,value);
			 }
			 datasetReady = (pIDataset->MoveNext() != 0);
		 }
		 //pIUnknown->Release();
	}
// 	  VARIANT_BOOL bresult; 
// 	   //BSTR  ptr = NULL;
// 	   BSTR  bssql = selectSQL.AllocSysString();
// 	   IUnknown *pIUnknown;
// 	  HRESULT hr = pDataSet->QueryInterface(__uuidof( IUnknown),(void**)&pIUnknown);
// 	  if (SUCCEEDED(hr))
// 	{
// 		//pDataSetFill->FillItem()
//          bresult = m_pIDBMS->FillDataSet(&pIUnknown,bssql);
// 		 pIUnknown->Release();
// 	}
// 	SysFreeString(bssql);
   return TRUE;
}
