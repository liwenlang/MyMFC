// PMSQLBaseCtrl.cpp: implementation of the CPMSQLBaseCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PMSQLBaseCtrl.h"
#include "NDataModelDataSet.h"
#include "CPub.h"

#include "../SQLiteCtrl/sqlite3.h"
typedef std::set<int>  CPmIntSet ;
typedef CPmIntSet::iterator CPmIntSetIter ; 


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CPMSQLTableCtrl

CPMSQLTableCtrl::CPMSQLTableCtrl(CPMSQLBaseCtrl * pSqlBaseCtrl, sqlite3_stmt * pStmt)
{
	m_pSqlBaseCtrl = pSqlBaseCtrl;
	m_pStmt = pStmt;
	m_ParmID = 0;
}

CPMSQLTableCtrl::~CPMSQLTableCtrl()
{
	if(NULL != m_pStmt)
	{
		sqlite3_finalize(m_pStmt);
	}
}

BOOL CPMSQLTableCtrl::InitFieldNameMap()
{
	if(0 != m_MapNameField.GetSize())
		return TRUE;
	if(NULL == m_pStmt)
		return FALSE;

	CString strTemFieldName;
	int iCount = sqlite3_column_count(m_pStmt);
	for (int i = 0;i < iCount;i++)
	{
		strTemFieldName = sqlite3_column_name(m_pStmt,i);
		m_MapNameField[strTemFieldName] = (void*)i;
	}
	return TRUE;
}

BOOL CPMSQLTableCtrl::CheckTableCtrl_ErrorCode(int iErrorCode)
{
	if(SQLITE_ROW == iErrorCode || SQLITE_DONE == iErrorCode)
		return TRUE;

	static BOOL bHasShowInfo = FALSE;
	if(FALSE == bHasShowInfo)
	{
		CString strError ;
		strError.Format(_T("数据库发现异常，请进行修复。(原因可能是主键重复：ParmID=%d )"),m_ParmID);
		AfxMessageBox(strError);
		bHasShowInfo = TRUE;
	}

	LPCSTR pASqlError = sqlite3_sql(m_pStmt);
	LPTSTR pTStrSQL = NULL;
	if(pASqlError)
		pTStrSQL = CPub::PM_A2T(pASqlError);
	else
		return FALSE;

	CString strError = m_pSqlBaseCtrl->GetLastError();

	CString strTemErrInfo;
	strTemErrInfo.Format(_T("SQL语句执行：%s  出错：%s"),pTStrSQL,strError);
	delete []pTStrSQL;

	CDataSetErrorInfo::Instance()->PrintInfo(strTemErrInfo);

	return FALSE;
}

BOOL CPMSQLTableCtrl::TableNextRow()
{
//#ifdef _TESTTIME
//	CTiming timing(_T("CPMSQLTableCtrl::TableNextRow"),TRUE,_T("CPMSQLTableCtrl"));
//#endif
	if(NULL == m_pStmt)
		return FALSE;

	int iRunMode = sqlite3_step(m_pStmt);

	CheckTableCtrl_ErrorCode(iRunMode);

	if(SQLITE_ROW == iRunMode 
		/*|| SQLITE_DONE == iRunMode */)
		return TRUE;
	return FALSE;
}

BOOL CPMSQLTableCtrl::GetColumnCount(int & iCount)
{
	if(NULL == m_pStmt)
		return FALSE;

	iCount = sqlite3_column_count(m_pStmt);
	return TRUE;
}

BOOL CPMSQLTableCtrl::GetFieldName(int iCol,CString & strFieldName)
{
	if(NULL == m_pStmt)
		return FALSE;
//	strFieldName = sqlite3_column_name(m_pStmt,iCol);

#ifdef UNICODE 
	const void * pData = (void *)sqlite3_column_name16(m_pStmt,iCol);
	strFieldName =  (LPCWSTR) pData ;
#else
	/// 获取的数据是unicode 编码的
	const void * pData = (void *)sqlite3_column_name16(m_pStmt,iCol);
	if(NULL != pData)
	{//有出现空指针的可能性，所以要防御下
		LPSTR pAStr = CPub::PM_W2A( (LPCWSTR) pData ) ;
		strFieldName = pAStr ;
		delete [](LPSTR)pAStr ;
	}
#endif
	return TRUE;
}

BOOL CPMSQLTableCtrl::GetColumnDataType(int iCol,TableDataType & tableDataType)
{
	if(NULL == m_pStmt)
		return FALSE;
	int iColType = sqlite3_column_type(m_pStmt, iCol);
	if(SQLITE_INTEGER == iColType)
	{
		tableDataType = CPMSQLTableCtrl::TableCol_Int;
	}
	else if(SQLITE_FLOAT == iColType)
	{
		tableDataType = CPMSQLTableCtrl::TableCol_Float;
	}
	else if(SQLITE_TEXT == iColType)
	{
		tableDataType = CPMSQLTableCtrl::TableCol_String;
	}
	else if(SQLITE_BLOB == iColType)
	{
		tableDataType = CPMSQLTableCtrl::TableCol_Blob;
	}
	else
		return FALSE;
	return TRUE;
}

BOOL CPMSQLTableCtrl::GetAsInterger(const CString & strFieldName,int & iValue)
{
	if(!InitFieldNameMap())
		return FALSE;
	void* pFieldIndex = NULL;
	if(!m_MapNameField.Lookup(strFieldName,pFieldIndex))
		return FALSE; 

	return GetAsInterger((int)pFieldIndex,iValue);
}

BOOL CPMSQLTableCtrl::GetAsDouble(const CString & strFieldName,double & dValue)
{
	if(!InitFieldNameMap())
		return FALSE;
	void* pFieldIndex = NULL;
	if(!m_MapNameField.Lookup(strFieldName,pFieldIndex))
		return FALSE;

	return GetAsDouble((int)pFieldIndex,dValue);
}

BOOL CPMSQLTableCtrl::GetAsString(const CString & strFieldName,CString & strValue)
{
	if(!InitFieldNameMap())
		return FALSE;
	void* pFieldIndex = NULL;
	if(!m_MapNameField.Lookup(strFieldName,pFieldIndex))
		return FALSE;

	return GetAsString((int)pFieldIndex,strValue);
}

BOOL CPMSQLTableCtrl::GetAsBlob(const CString & strFieldName,const void *& pBuffer,long & lBufferSize)
{
	if(!InitFieldNameMap())
		return FALSE;
	void* pFieldIndex = NULL;
	if(!m_MapNameField.Lookup(strFieldName,pFieldIndex))
		return FALSE;

	return GetAsBlob((int)pFieldIndex,pBuffer,lBufferSize);
}

BOOL CPMSQLTableCtrl::GetBlob(const CString & strFieldName,void *& pBuffer,long & lBufferSize)
{
	if(!InitFieldNameMap())
		return FALSE;
	void* pFieldIndex = NULL;
	if(!m_MapNameField.Lookup(strFieldName,pFieldIndex))
		return FALSE;

	return GetBlob((int)pFieldIndex,pBuffer,lBufferSize);
}

BOOL CPMSQLTableCtrl::GetAsInterger(int iCol,int & iValue)
{
	if(NULL == m_pStmt)
		return FALSE;
	iValue = sqlite3_column_int(m_pStmt,iCol);
	return TRUE;
}

BOOL CPMSQLTableCtrl::GetAsDouble(int iCol,double & dValue)
{
	if(NULL == m_pStmt)
		return FALSE;
	dValue = sqlite3_column_double(m_pStmt,iCol);
	return TRUE;
}

BOOL	CPMSQLTableCtrl::GetColAsString( const int iCol,CString & strValue , sqlite3_stmt * pStmt  ) 
{
	if ( NULL == pStmt)
	{
		pStmt = m_pStmt ; 
	}

	if(NULL == pStmt )
		return FALSE;

#ifdef UNICODE 
	const void * pData = (void *)sqlite3_column_text16(pStmt,iCol);
	strValue =  (LPCWSTR) pData ;
#else
	/// 获取的数据是unicode 编码的
	const void * pData = (void *)sqlite3_column_text16(pStmt,iCol);
	if(NULL != pData)
	{//有出现空指针的可能性，所以要防御下
		LPSTR pAStr = CPub::PM_W2A( (LPCWSTR) pData ) ;
		strValue = pAStr ;
		delete [](LPSTR)pAStr ;
	}
#endif
	return TRUE;
}

BOOL CPMSQLTableCtrl::GetAsString(int iCol,CString & strValue)
{
	return GetColAsString( iCol , strValue ) ;
}
	
BOOL CPMSQLTableCtrl::GetAsBlob(int iCol,const void * & pBuffer,long & lBufferSize)
{
	if(NULL == m_pStmt)
		return FALSE;
	lBufferSize = sqlite3_column_bytes(m_pStmt, iCol);
	pBuffer = sqlite3_column_blob(m_pStmt, iCol);
	return TRUE;
}

BOOL CPMSQLTableCtrl::GetBlob(int iCol,void *& pBuffer,long & lBufferSize)
{
	if(NULL == m_pStmt)
		return FALSE;
	lBufferSize = sqlite3_column_bytes(m_pStmt, iCol);
	const void * pTemBuffer = sqlite3_column_blob(m_pStmt, iCol);

	if(lBufferSize <= 0)
	{//空的特殊情况
		pBuffer = NULL;
		lBufferSize = 0;
		return TRUE;
	}
	pBuffer = new BYTE[lBufferSize];
	memset(pBuffer,0,lBufferSize);
	memcpy(pBuffer,pTemBuffer,lBufferSize);
	return TRUE;
}

BOOL CPMSQLTableCtrl::BindAsInterger(int iCol,int iValue)
{
	if(NULL == m_pStmt || NULL == m_pSqlBaseCtrl)
		return FALSE;

	int iRunMode = sqlite3_bind_int(m_pStmt,iCol,iValue);
	if(SQLITE_OK != iRunMode)
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMSQLTableCtrl::SetAsInterger(int %d,int %d):%s"),\
			iCol,iValue,m_pSqlBaseCtrl->GetLastError());
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	return TRUE;
}

BOOL CPMSQLTableCtrl::BindAsDouble(int iCol,double dValue)
{
	if(NULL == m_pStmt || NULL == m_pSqlBaseCtrl)
		return FALSE;

	int iRunMode = sqlite3_bind_double(m_pStmt,iCol,dValue);
	if(SQLITE_OK != iRunMode)
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMSQLTableCtrl::SetAsDouble(int %d,double %f):%s"),\
			iCol,dValue,m_pSqlBaseCtrl->GetLastError());
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	return TRUE;
}


BOOL CPMSQLTableCtrl::BindDataSet( NDataModelDataSet * pDataSet , const CStringArray & aryNotBindCol ) 
{
	CUIntArray aryNotBindCol_Int ; 

	int i = 0 ; 
	for ( ; i < aryNotBindCol.GetSize() ; ++i )
	{
		int iColIndex = pDataSet->GetFieldIndex( aryNotBindCol.GetAt(i) )  ;
		aryNotBindCol_Int.Add( iColIndex ) ; 
	}

	return BindDataSet( pDataSet , aryNotBindCol_Int ) ; 
}

BOOL CPMSQLTableCtrl::BindDataSet( NDataModelDataSet * pDataSet , const CUIntArray & aryNotBindCol ) 
{
	CPmIntSet setNotBindCol ; 

	{
		int i = 0 ; 
		for ( ; i < aryNotBindCol.GetSize() ; ++i )
		{
			setNotBindCol.insert( aryNotBindCol.GetAt(i) ) ; 
		}
	}

	///  添加事务
	CPMSQLBaseCtrl::CSQLTransaction doSqlTransaction( m_pSqlBaseCtrl );
	
	const int iCount = pDataSet->GetRowCount() ; 

	const int iFieldCount = pDataSet->GetFieldCount() ; 

	int i = 1 ; 
	for ( ; i<= iCount ; ++i )
	{
		int j = 0 ; 
		for ( ; j < iFieldCount ; ++j ) 
		{
			if ( setNotBindCol.end() != setNotBindCol.find( j ) )
			{
				continue ; 
			}

			NDataField * pTemField = pDataSet->GetField(j);
			if(NULL == pTemField)
				continue;

			FieldType temFieldType = pTemField->GetDataType();

			if(ftInteger == temFieldType)
			{
				int iTemValue = pDataSet->GetAsInteger( j,i );
				BindAsInterger( j + 1,iTemValue);
			}
			else if(ftFloat == temFieldType)
			{
				double dTemValue = pDataSet->GetAsFloat( j,i );
				BindAsDouble( j + 1,dTemValue);
			}
			else if(ftBlob == temFieldType && pTemField->IsUpdateAsBlobFiled() )
			{
				LPVOID pbuffer = NULL;
				long lbufferSize = pDataSet->GetBlob( j,i ,pbuffer);
				BindAsBlob( j + 1,pbuffer,lbufferSize);
			}
			else
			{
				CString strTemValue = pDataSet->GetAsString( j ,i );
				BindAsString( j + 1,strTemValue);
			}
		}

		Update() ; 

		if ( i != iCount)
		{
			ResetSql() ; 
		}
	}

	return TRUE ;
}

void FreeLPWSTR(void * pBuffer)
{
	delete [](LPWSTR )pBuffer;
	//PMUnregMemory(pBuffer);
}

BOOL CPMSQLTableCtrl::BindAsString(int iCol,const CString & strValue)
{
	if(NULL == m_pStmt || NULL == m_pSqlBaseCtrl)
		return FALSE;

	/// 
	/// 保存的的时候用Unicode ，双字节存储
	/// 
	LPWSTR pwchValue  =CPub::PM_T2W( (LPCTSTR)strValue ) ; 

	/// 
	/// 第3个参数为字节数，不是字符数
	/// 不需要把最后一个字符保存进去
	/// 
	int iRunMode = sqlite3_bind_text16(m_pStmt,iCol, pwchValue ,int(wcslen( pwchValue ) * sizeof( WCHAR )) , FreeLPWSTR);

	

	if(SQLITE_OK != iRunMode)
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMSQLTableCtrl::SetAsString(int %d,CString %s):%s"),\
			iCol,strValue,m_pSqlBaseCtrl->GetLastError());
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);

		return FALSE;
	}

	return TRUE;
}

BOOL CPMSQLTableCtrl::BindAsBlob( const int iCol,const LPVOID & pBuffer,const long lBufferSize)
{
	if(NULL == m_pStmt || NULL == m_pSqlBaseCtrl)
		return FALSE;

	int iRunMode = SQLITE_OK;
	if(0 == lBufferSize)
		iRunMode = sqlite3_bind_zeroblob(m_pStmt,iCol,0);
	else
		iRunMode = sqlite3_bind_blob(m_pStmt,iCol,pBuffer,lBufferSize,NULL);
	if(SQLITE_OK != iRunMode)
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMSQLTableCtrl::SetAsBlob(int %d,long %d):%s"),\
			iCol,lBufferSize,m_pSqlBaseCtrl->GetLastError());
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	return TRUE;
}

BOOL CPMSQLTableCtrl::Update()
{
	if(NULL == m_pStmt)
		return FALSE;
	int iRunMode = sqlite3_step(m_pStmt);

	CheckTableCtrl_ErrorCode(iRunMode);

	if(SQLITE_DONE == iRunMode
		|| SQLITE_ROW == iRunMode )
		return TRUE;
	return FALSE;
}

BOOL CPMSQLTableCtrl::ResetSql()
{
	if(NULL == m_pStmt)
		return FALSE;
	int iRunMode = sqlite3_reset(m_pStmt);
	return TRUE;
}

BOOL CPMSQLTableCtrl::GetDataSet( CSQLDataSet * pDataSet )
{
	if ( NULL == m_pStmt )
	{
		return FALSE ;
	}


	m_pSqlBaseCtrl->GetDataSet( m_pStmt , pDataSet ) ;

	return TRUE ; 
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// 

CPMSQLBaseCtrl::CSQLTransaction::CSQLTransaction(CPMSQLBaseCtrl * pSQLBaseCtrl)
{
	m_pSqlBaseCtrl = NULL ; 

	if(NULL != pSQLBaseCtrl && pSQLBaseCtrl->AddSQLTransationCount())
	{
		m_pSqlBaseCtrl = pSQLBaseCtrl;
		pSQLBaseCtrl->ExecuteSQL(_T("begin transaction;"));
	}
	
}

CPMSQLBaseCtrl::CSQLTransaction::~CSQLTransaction()
{
	if(NULL != m_pSqlBaseCtrl)
	{
		m_pSqlBaseCtrl->ExecuteSQL(_T("commit transaction;"));
		m_pSqlBaseCtrl->SubSQLTransationCount();
	}
}

bool CPMSQLBaseCtrl::CSQLTransaction::RollBack()
{
	if(NULL == m_pSqlBaseCtrl)
		return false;

	m_pSqlBaseCtrl->ExecuteSQL(_T("rollback transaction;"));
	m_pSqlBaseCtrl->SubSQLTransationCount();
	m_pSqlBaseCtrl = NULL;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//CPMSQLBaseCtrl
CPMSQLBaseCtrl::CPMSQLBaseCtrl()
{
	m_pSQlite3 = NULL;//SQL的控制指针

	m_strDBFilePath.Empty();//连接的 SQL文件位置
	m_iSQLTransationCount = 0;
}

CPMSQLBaseCtrl::~CPMSQLBaseCtrl()
{
	if(NULL != m_pSQlite3)
	{
		CDataSetErrorInfo::Instance()->PrintInfo(_T("CPMSQLBaseCtrl::~CPMSQLBaseCtrl::连接没有释放!"));
	}
	m_iSQLTransationCount = 0;
}

BOOL CPMSQLBaseCtrl::AddSQLTransationCount()
{
	/// 只有一个有效
	if(m_iSQLTransationCount == 0)
	{
		m_iSQLTransationCount++;
		return TRUE ;
	}
	return FALSE ;
	
}

void CPMSQLBaseCtrl::SubSQLTransationCount()
{
	if(m_iSQLTransationCount > 0)
		m_iSQLTransationCount--;
}

bool CPMSQLBaseCtrl::ConnectSQLDataBase(const CString & strFilePath)
{
	if(NULL != m_pSQlite3)
		return false;
	
	/// 
	/// sqlite 对路径 只识别UTF-8 / UTF-16 , 
	/// 这里我们使用UTF16的编码，Unicode 双字节 
	/// 
	LPWSTR pwchFillPath = CPub::PM_T2W( (LPCTSTR)strFilePath ) ; 
	
	{//修改 默认 路径支持大小
		sqlite3_vfs * pDefault = sqlite3_vfs_find(NULL);
		if(pDefault)
		{
			pDefault->mxPathname = 1000;
			sqlite3_vfs_register(pDefault,0);
		}
	}
	sqlite3 * pSQlite3 = NULL;
	int iResult = sqlite3_open16( pwchFillPath  ,&pSQlite3);
	delete []pwchFillPath;
	//PMUnregMemory(pwchFillPath);

	if(SQLITE_OK != iResult)
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMSQLBaseCtrl::ConnectSQLDataBase(CString %s):%s"),\
			strFilePath,GetLastError());
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return false;
	}
	m_pSQlite3 = pSQlite3;
	return true;
}

bool CPMSQLBaseCtrl::DisConnectSQLDataBase()
{
	if(NULL == m_pSQlite3)
		return false;
	int iResult = sqlite3_close((sqlite3 *)m_pSQlite3);
	if(SQLITE_OK != iResult)
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMSQLBaseCtrl::DisConnectSQLDataBase():%s"),GetLastError());
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return false;
	}
	m_pSQlite3 = NULL;
	return true;
}

bool	CPMSQLBaseCtrl::BackUpDataBase( const CString & strToFilePath ) 
{
	/// 
	if ( NULL == m_pSQlite3 )
	{
		return false ; 
	}

	/// 
	sqlite3 * pToFile = NULL ;

	/// 
	{
		/// 
		LPWSTR pwchFillPath = CPub::PM_T2W( (LPCTSTR)strToFilePath ) ; 

		{//修改 默认 路径支持大小
			sqlite3_vfs * pDefault = sqlite3_vfs_find(NULL);
			if(pDefault)
			{
				pDefault->mxPathname = 1000;
				sqlite3_vfs_register(pDefault,0);
			}
		}

		/// 
		int iResult = sqlite3_open16( pwchFillPath  ,&pToFile);
		delete []pwchFillPath;
		//PMUnregMemory(pwchFillPath);

		/// 
		if(SQLITE_OK != iResult)
		{
			CString strTemMsg;
			strTemMsg.Format(_T("CPMSQLBaseCtrl::ConnectSQLDataBase(CString %s):%s"),\
				strToFilePath,GetLastError());
			CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
			return false;
		}
	}

	/// 
	{
		/// 
		sqlite3_backup *pBackup ;  

		/// 
		pBackup = sqlite3_backup_init( pToFile , "main", (sqlite3 *)m_pSQlite3, "main" );

		/// 
		if( pBackup ){
			(void)sqlite3_backup_step(pBackup, -1);
			(void)sqlite3_backup_finish(pBackup);
		}

		/// 
		sqlite3_errcode( pToFile ) ;

		int iResult = sqlite3_close((sqlite3 *)pToFile);
		if(SQLITE_OK != iResult)
		{
			CString strTemMsg;
			strTemMsg.Format(_T("CPMSQLBaseCtrl::DisConnectSQLDataBase():%s"),GetLastError());
			CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
			return false;
		}
	}

	/// 
	return true ;
}

CString CPMSQLBaseCtrl::GetLastError()
{
	if(NULL == m_pSQlite3)
		return _T("没有数据连接");

	LPTSTR pTStr = CPub::PM_A2T(sqlite3_errmsg((sqlite3*)m_pSQlite3));
	CString strTemErrorInfo;
	strTemErrorInfo = pTStr;
	delete []pTStr;
	return strTemErrorInfo;
}

bool CPMSQLBaseCtrl::GetSQLTableCtrl(LPCTSTR sSQL,CPMSQLTableCtrl *& pTableCtrl)
{
	if(NULL == m_pSQlite3)
		return false;

	//{
	//	CPMMemoryManager::Instance()->AppendSqlLog2File( sSQL ) ; 
	//}

	sqlite3_stmt * pStmt = NULL;
	int iResult = SQLITE_OK;
#ifdef UNICODE
	iResult = sqlite3_prepare16_v2((sqlite3*)m_pSQlite3,sSQL,-1,&pStmt,NULL);
#else
	LPWSTR pWSQL = CPub::PM_T2W(sSQL);
	iResult = sqlite3_prepare16_v2((sqlite3*)m_pSQlite3,pWSQL,-1,&pStmt,NULL);
	delete []pWSQL;
	//PMUnregMemory(pWSQL);
#endif

	if(SQLITE_OK != iResult)
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMSQLBaseCtrl::GetSQLTableCtrl(%s):%s , return id : %d "),sSQL,GetLastError() , iResult ) ;
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return false;
	}

	pTableCtrl = new CPMSQLTableCtrl(this,pStmt);
	return true;
}


bool CPMSQLBaseCtrl::BuildSQL(CPMSQLBaseCtrl::CSQLType sqlType,CSQLDataSet * pDataSet,PmTSQLString & sNewSQL)
{
	if(NULL == pDataSet)
		return false;
	int iFieldCount = pDataSet->GetFieldCount();
	if(CPMSQLBaseCtrl::eInsertSQL == sqlType)
	{
		sNewSQL = _T("INSERT INTO \"") + pDataSet->GetTableName() + _T("\" (");
		PmTSQLString temValue;
		for (int i = 0;i < iFieldCount;i++)
		{
			if(i > 0)
			{
				sNewSQL += _T(",");
				temValue += _T(",");
			}
			sNewSQL += pDataSet->GetFieldName(i);
			temValue += _T("?");
		}
		sNewSQL += _T(") VALUES(");
		sNewSQL +=temValue.c_str();
		sNewSQL += _T(")");
	}
	else if(CPMSQLBaseCtrl::eSelectSQL == sqlType)
	{
		sNewSQL.append(_T("select "));
		for (int i = 0;i < iFieldCount;i++)
		{
			if(i > 0)
			{
				sNewSQL.append(_T(","));
			}
			sNewSQL.append(pDataSet->GetFieldName(i));
		}
		sNewSQL.append(_T(" from "));
		sNewSQL.append(pDataSet->GetTableName());

		//连接where
		CString strTemWhere = pDataSet->GetWhere();
		if(strTemWhere.GetLength() > 0)
		{
			sNewSQL.append(_T(" where "));
			sNewSQL.append(strTemWhere.GetBuffer(0));
		}

	}
	else if(CPMSQLBaseCtrl::eUpdateSQL == sqlType)
	{//update %s set %s where %s
		sNewSQL = _T("update ") + pDataSet->GetTableName() + _T(" set ");

		for (int i = 0;i < iFieldCount;i++)
		{
			if(i > 0)
			{
				sNewSQL += _T(",");
			}
			sNewSQL += pDataSet->GetFieldName(i);
			sNewSQL += _T("=?");
		}
		sNewSQL += _T(" where ") + pDataSet->GetWhere();
	}
	return true;
}

void	CPMSQLBaseCtrl::GetDataSet( sqlite3_stmt * pStmt , CSQLDataSet * pDataSet )
{
	int iColCount = sqlite3_column_count(pStmt);
	CUIntArray dataetFieldIDArr;
	for (int i = 0;i < iColCount;i++)
	{
		CString strFieldName(sqlite3_column_name(pStmt,i));
		dataetFieldIDArr.Add(pDataSet->GetFieldIndex(strFieldName));
	}

	pDataSet->SetSqlBaseCtrl(this);//注册控制指针

	int iStepMode = sqlite3_step(pStmt);
	while( SQLITE_ROW == iStepMode )
	{
		int iNewRow = pDataSet->InsertRow(0);

		for (int j = 0;j < dataetFieldIDArr.GetSize();j++)
		{
			int iTemDataSetFieldID = dataetFieldIDArr.GetAt(j);
			if(iTemDataSetFieldID < 0)
				continue;

			int iFieldType = sqlite3_column_type(pStmt,j);
			if(SQLITE_INTEGER == iFieldType)
			{
				int iTemValue = sqlite3_column_int(pStmt,j);
				pDataSet->SetAsInteger(iTemDataSetFieldID,iNewRow,iTemValue);
			}
			else if(SQLITE_FLOAT == iFieldType)
			{
				double dTemValue = sqlite3_column_double(pStmt,j);
				pDataSet->SetAsFloat(iTemDataSetFieldID,iNewRow,dTemValue);
			}
			else if(SQLITE_BLOB == iFieldType )
			{
				void * pBuffer = (void *)sqlite3_column_blob(pStmt,j);
				long lBufferSize = sqlite3_column_bytes(pStmt,j);
				pDataSet->SetBlob(iTemDataSetFieldID,iNewRow,pBuffer,lBufferSize);
			}
			else if(SQLITE_TEXT == iFieldType)
			{
				CString strTemValue ;
				{
#ifdef UNICODE 
					const void * pData = (void *)sqlite3_column_text16(pStmt, j );
					strTemValue =  (LPCWSTR) pData ;
#else
					/// 获取的数据是unicode 编码的
					const void * pData = (void *)sqlite3_column_text16(pStmt, j );
					LPSTR pAStr = CPub::PM_W2A( (LPCWSTR) pData ) ;
					strTemValue = pAStr ;
					delete [](LPSTR)pAStr ;
#endif
				}
				pDataSet->SetAsString(iTemDataSetFieldID,iNewRow,strTemValue);
			}
			
		}

		iStepMode = sqlite3_step(pStmt);
	}

	////////////////////////////////////////////
	/// liwenlang [2012:8:2   14:20]
	/// 说明:
	/// 重置所有新行标记
	for(int i = 1;i <= pDataSet->GetRowCount();i++)
	{
		NRecordID pTemRow = pDataSet->GetRecordID(i);
		pDataSet->SetRowNewInternal(pTemRow,FALSE);

		pDataSet->SetRowModifiedInternal(pTemRow,FALSE);
		for (int j = 0;j < dataetFieldIDArr.GetSize();j++)
		{
			int iTemDataSetFieldID = dataetFieldIDArr.GetAt(j);
			if(iTemDataSetFieldID < 0)
				continue;
			pDataSet->SetItemModifiedInternal(pTemRow,pDataSet->GetField(iTemDataSetFieldID),FALSE);
		}
	}
}

bool CPMSQLBaseCtrl::GetDataSet(LPCTSTR strSQL,CSQLDataSet * pDataSet)
{
	if(NULL == m_pSQlite3 || NULL == pDataSet)
		return false;

	//{
	//	CPMMemoryManager::Instance()->AppendSqlLog2File( strSQL ) ; 
	//}

	LPSTR pASql = CPub::PM_T2A(strSQL);
	sqlite3_stmt * pStmt = NULL;
	int iResult = sqlite3_prepare((sqlite3*)m_pSQlite3,pASql,-1,&pStmt,NULL);
	delete []pASql;

	if(SQLITE_OK != iResult)
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMSQLBaseCtrl::GetSQLTableCtrl(%s):%s"),strSQL,GetLastError());
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return false;
	}

	GetDataSet( pStmt , pDataSet ) ; 
	
	sqlite3_finalize(pStmt);
	
	pDataSet->ResetUpdate(FALSE);
	return true;
}

bool CPMSQLBaseCtrl::ExecuteSQL(LPCTSTR sSQL)
{
	if(NULL == m_pSQlite3)
		return false;

	//{
	//	CPMMemoryManager::Instance()->AppendSqlLog2File( sSQL ) ; 
	//}


	CPMSQLTableCtrl * pTemSQLTableCtrl = NULL;
	if(!GetSQLTableCtrl(sSQL,pTemSQLTableCtrl))
		return false;
	if(!pTemSQLTableCtrl->Update())
	{
		delete pTemSQLTableCtrl;
		return false;
	}
	delete pTemSQLTableCtrl;
	return true;
}
