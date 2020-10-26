#include "stdafx.h"
#include "ConnectManager.h"
#include "DataService_Common.h"

////////////////////////////////////////////////////////////////////
//CConnectBase
IMPLEMENT_DYNCREATE( CConnectBase, CObject )
CConnectBase::CConnectBase()
{
	m_bHasBak = FALSE;
	m_iConnectUseCount = 0;
}

CConnectBase::~CConnectBase()
{
	if(0 != m_iConnectUseCount)
	{
		CString strMsg;
		strMsg.Format(_T("CConnectBase连接没有释放:%s"),m_strFilePath);

	}
}

CString CConnectBase::GetFilePath() const
{
	return m_strFilePath;
}

CString CConnectBase::GetFileDir() const
{
	CString strFileDir = m_strFilePath;
	int iLast = strFileDir.ReverseFind('\\');
	if(iLast > 0)
		strFileDir.Delete(iLast,strFileDir.GetLength()-iLast);
	return strFileDir;
}

void	CConnectBase::SetIniPath( const CString & strIniPath )
{
	m_strIniPath = strIniPath ; 
}

CString	CConnectBase::GetIniPath( ) const 
{
	return m_strIniPath ; 
}

bool CConnectBase::AddUseCount()
{
	m_iConnectUseCount++;
	return true;
}

bool CConnectBase::SubUseCount()
{
	m_iConnectUseCount--;
	if(m_iConnectUseCount < 0)
	{
		CString strMsg = _T("CConnectBase::SubUseCount:连接释放异常");

		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////
///CConnect_sqlite
IMPLEMENT_DYNCREATE( CConnect_sqlite,CConnectBase )
CConnect_sqlite::CConnect_sqlite()
{
	m_pSQLBaseCtrl = NULL ;
}

CConnect_sqlite::~CConnect_sqlite()
{
	if(NULL != m_pSQLBaseCtrl)
	{
		PMDelete(m_pSQLBaseCtrl);
	}
}

bool CConnect_sqlite::ConnectBase(const CString & strFilePath)
{
	m_pSQLBaseCtrl = PMNew(CPMSQLBaseCtrl);
	if(!m_pSQLBaseCtrl->ConnectSQLDataBase(strFilePath))
	{
		PMDelete(m_pSQLBaseCtrl);
		return false;
	}

	m_strFilePath = strFilePath;
	m_iConnectUseCount = 0;//连接计数为0
	return true;
}

bool CConnect_sqlite::DisconnectBase()
{
	if(0 != m_iConnectUseCount)
	{
		CString strMsg = _T("CConnectBase::DisconnectBase:没有完全释放连接") + m_strFilePath;

		return false;
	}
	if(NULL != m_pSQLBaseCtrl)
	{
		DisconnectExternBase();
		m_pSQLBaseCtrl->DisConnectSQLDataBase();
		return true;
	}
	return false;
}

bool CConnect_sqlite::BackUpDataBase( const CString & strToFileName )
{
	if ( NULL != m_pSQLBaseCtrl )
	{
		return m_pSQLBaseCtrl->BackUpDataBase( strToFileName ) ;
	}
	return false ;
}

BOOL CConnect_sqlite::RegSQLCtrl(CSQLDataSet * pTemDataSet)
{
	if(NULL == pTemDataSet)
		return FALSE;
	pTemDataSet->SetSqlBaseCtrl(m_pSQLBaseCtrl);
	return TRUE;
}

bool CConnect_sqlite::BuildSQL(CPMSQLBaseCtrl::CSQLType sqlType,CSQLDataSet * pDataSet,PmTString & sNewSQL)
{
	return CPMSQLBaseCtrl::BuildSQL(sqlType,pDataSet,sNewSQL);
}

bool CConnect_sqlite::GetDataSet(CSQLDataSet * pDataSet)
{
	if(NULL == pDataSet)
		return false;
//	PmTSQLString temSQLString = _T("select * from ");
	PmTSQLString temSQLString = pmdataservice::g_strSelect;
	int iFieldCount = pDataSet->GetFieldCount();
	for (int i = 0;i < iFieldCount;i++)
	{
		if(i > 0)
			temSQLString += _T(",");
		temSQLString += pDataSet->GetFieldName(i);
	}
	temSQLString += pmdataservice::g_strFrom;
	temSQLString += pDataSet->GetTableName();

	//添加 where 部分
	CString strWhere = pDataSet->GetWhere();
	if(strWhere.GetLength() > 0)
	{
		temSQLString += pmdataservice::g_strWhere.c_str() + strWhere;
	}
	return GetDataSet(temSQLString.c_str(),pDataSet);
}

bool CConnect_sqlite::GetDataSet(LPCTSTR strSQL,CSQLDataSet * pDataSet)
{
#ifdef _TESTTIME
	CTiming timing(_T("CConnect_sqlite::GetDataSet"),TRUE,_T("CConnect_sqlite"));
#endif
	if(NULL == m_pSQLBaseCtrl)
		return false;
	pDataSet->SetSqlBaseCtrl(m_pSQLBaseCtrl);
	return m_pSQLBaseCtrl->GetDataSet(strSQL,pDataSet);
}

bool CConnect_sqlite::GetSQLTableCtrl(LPCTSTR sSQL,CPMSQLTableCtrl *& pTableCtrl)
{
#ifdef _TESTTIME
	CTiming timing(_T("CConnect_sqlite::GetSQLTableCtrl"),TRUE,_T("CConnect_sqlite"));
#endif
	if(NULL == m_pSQLBaseCtrl)
		return false;
	if(m_pSQLBaseCtrl->GetSQLTableCtrl(sSQL,pTableCtrl))
	{
		PMRegMemory(pTableCtrl);
		return true;
	}
	return false;
}

bool CConnect_sqlite::GetSQLTableCtrl(CStringArray & strSelectArr,CStringArray & strWhereKeyArr,\
									  CStringArray & strWhereValueArr,const CString & strTableName,CPMSQLTableCtrl *& pTableCtrl)
{
	if(NULL == m_pSQLBaseCtrl || strWhereKeyArr.GetSize() != strWhereValueArr.GetSize())
		return false;

	PmTSQLString temSQLString = _T("select ");
	int i = 0;
	//生成select
	for (i = 0;i < strSelectArr.GetSize();i++)
	{
		if(i > 0)
			temSQLString += _T(",");
		temSQLString += strSelectArr.GetAt(i);
	}

	//生成 from
	temSQLString += _T(" from ") + strTableName + _T(" where ");

	//生成where
	for (i = 0;i < strWhereKeyArr.GetSize();i++)
	{
		if(i > 0)
			temSQLString += _T(" and ");
		temSQLString += strWhereKeyArr.GetAt(i);
		temSQLString += _T("=");
		temSQLString += strWhereValueArr.GetAt(i);
	}

	if(m_pSQLBaseCtrl->GetSQLTableCtrl(temSQLString.c_str(),pTableCtrl))
	{
		PMRegMemory(pTableCtrl);
		return true;
	}
	return false;
}


bool CConnect_sqlite::GetTable_ColMaxValue(const CString & strColName,const CString & strTableName, CString & strMaxID)
{
	//select max(_id) AS maxId from tableName 
	if(NULL == m_pSQLBaseCtrl)
		return false;

	PmTString sTemSQL = pmdataservice::g_strSelect.c_str();
	sTemSQL += _T("max(");
	sTemSQL += strColName;
	sTemSQL += _T(") AS maxid");
	sTemSQL += pmdataservice::g_strFrom.c_str() + strTableName;

	CPMSQLTableCtrl * pSqlTableCtrl = NULL;
	if(!m_pSQLBaseCtrl->GetSQLTableCtrl(sTemSQL.c_str(),pSqlTableCtrl))
		return false;
	PMRegMemory(pSqlTableCtrl);
	if(!pSqlTableCtrl->TableNextRow())
	{
		PMDelete(pSqlTableCtrl);
		return false;
	}
	
	pSqlTableCtrl->GetAsString( 0,strMaxID );
	PMDelete(pSqlTableCtrl);
	return true;
}

bool CConnect_sqlite::GetTable_ColMaxValue(const CString & strColName,const CString & strTableName,int & iMaxID)
{//select max(_id) AS maxId from tableName  
	if(NULL == m_pSQLBaseCtrl)
		return false;

	PmTString sTemSQL = pmdataservice::g_strSelect.c_str();
	sTemSQL += _T("max(");
	sTemSQL += strColName;
	sTemSQL += _T(") AS maxid");
	sTemSQL += pmdataservice::g_strFrom.c_str() + strTableName;

	CPMSQLTableCtrl * pSqlTableCtrl = NULL;
	if(!m_pSQLBaseCtrl->GetSQLTableCtrl(sTemSQL.c_str(),pSqlTableCtrl))
		return false;
	PMRegMemory(pSqlTableCtrl);
	if(!pSqlTableCtrl->TableNextRow())
	{
		PMDelete(pSqlTableCtrl);
		return false;
	}
	iMaxID = 0;
	pSqlTableCtrl->GetAsInterger(0,iMaxID);
	PMDelete(pSqlTableCtrl);
	return true;
}

/// select count(rowid) as rowcount from table ;
bool CConnect_sqlite::GetTable_RowCount( const CString & strTableName,int & iRowCount )
{
	if(NULL == m_pSQLBaseCtrl)
		return false;

	PmTString sTemSQL = pmdataservice::g_strSelect.c_str();
	sTemSQL += _T("count(rowid) as rowcount ");
	sTemSQL += pmdataservice::g_strFrom.c_str() + strTableName;

	CPMSQLTableCtrl * pSqlTableCtrl = NULL;
	bool bResult = m_pSQLBaseCtrl->GetSQLTableCtrl(sTemSQL.c_str(),pSqlTableCtrl);
	if(false == bResult)
		return false;
	PMRegMemory(pSqlTableCtrl);
	if(!pSqlTableCtrl->TableNextRow())
	{
		PMDelete(pSqlTableCtrl);
		return false;
	}
	iRowCount = 0;
	pSqlTableCtrl->GetAsInterger( eCol_0 ,iRowCount);
	PMDelete(pSqlTableCtrl);
	return true;
}

bool CConnect_sqlite::DoCreateSQLTable(CSQLDataSet * pTableDataSet)
{
	if(NULL == m_pSQLBaseCtrl || NULL == pTableDataSet)
		return false;
	PmTString temSql = _T("CREATE TABLE ") + pTableDataSet->GetTableName() + _T("(");
	PmTString temPriKey = _T(", primary key (");
	int iFieldCount = pTableDataSet->GetFieldCount();
	BOOL bHasKey = FALSE;//是否已经出现 主键
	int i = 0;
	for (i = 0;i < iFieldCount;i++)
	{
		NDataField * pTemNataField = pTableDataSet->GetField(i);
		if(i > 0)
		{
			temSql += _T(",");
		}
		CString strTemFieldName = pTemNataField->GetFieldName();
		temSql += strTemFieldName + _T(" ");
		FieldType temFieldType = pTemNataField->GetDataType();
		if(ftInteger == temFieldType)
			temSql += _T("INTEGER");
		else if(ftFloat == temFieldType)
			temSql += _T("FLOAT");
		else if(ftString == temFieldType || (ftBlob == temFieldType && !pTemNataField->IsUpdateAsBlobFiled()))
		{
//			CString strTemValue;
//			strTemValue.Format(_T("VARCHAR(%d)"),pTemNataField->GetSize());
			temSql += _T("TEXT");
		}
		else
			temSql += _T("Blob");

		if(pTemNataField->IsPK())
		{
			if(bHasKey)
				temPriKey += _T(",");
			temPriKey += strTemFieldName;
			bHasKey = TRUE;
		}

	}
	
	if(bHasKey)
	{//添加 主键 结束符号
		temSql += temPriKey;
		temSql += _T(")");
	}
	temSql += _T(")");

	if(!m_pSQLBaseCtrl->ExecuteSQL(temSql.c_str()))
		return FALSE;

	CStringArray strFieldNameArr;
	CString strIndexName;
	if(!pTableDataSet->GetCreateIndexSQL(strFieldNameArr,strIndexName))
		return FALSE;

	//CREATE INDEX testtable_idx2 ON testtable(first_col ASC,second_col DESC)
	PmTString temIndexSql = _T("CREATE Index ");
	temIndexSql += strIndexName;
	temIndexSql += _T(" ON ");
	temIndexSql += pTableDataSet->GetTableName();
	temIndexSql += _T("(");
	for (i = 0;i < strFieldNameArr.GetSize();i++)
	{
		if(i > 0)
			temIndexSql += _T(",");
		temIndexSql += strFieldNameArr.GetAt(i);
		temIndexSql += _T(" ASC");
	}
	temIndexSql += _T(")");

	return m_pSQLBaseCtrl->ExecuteSQL(temIndexSql.c_str());
}

bool CConnect_sqlite::ExecuteSQL(LPCTSTR sSQL)
{
	if(NULL == m_pSQLBaseCtrl)
		return false;
	return m_pSQLBaseCtrl->ExecuteSQL(sSQL);
}


BOOL  CConnect_sqlite::IsTableExsit( const CString & strTableName ) 
{
	CString strSelectTableName ;
	strSelectTableName.Format( _T("select * from sqlite_master  where type = 'table' and tbl_name like '%s'") , strTableName ) ; 

	CPMSQLTableCtrl * pTableCtrl = NULL ;  

	GetSQLTableCtrl( strSelectTableName , pTableCtrl ) ;
	if (!pTableCtrl)
	{
		return FALSE;
	}
 
	BOOL bHasExsit = pTableCtrl->TableNextRow();
	PMDelete( pTableCtrl ) ; 

	return bHasExsit ; 
}

int CConnect_sqlite::Attach(const CString& strtDBPath,const CString& strDBName)
{
	// 附加数据库到当前数据库
	CString strSQL = _T("ATTACH ");
	strSQL += _T("'");
	strSQL += strtDBPath;
	strSQL += _T("'");
	strSQL += _T(" AS ");
	strSQL += strDBName;
	return ExecuteSQL(strSQL);
}

int CConnect_sqlite::Detach(const CString& strDBName)
{
	// 附加数据库到当前数据库
	CString strSQL = _T("DETACH DATABASE");
	strSQL += _T("'");
	strSQL += strDBName;
	strSQL += _T("'");
	return ExecuteSQL(strSQL);
}

///////////////////////////////////////////////////////////////
//CConnectManager
CConnectManager * CConnectManager::m_pInstance = NULL;
CConnectManager::CConnectManager(void)
{

}

CConnectManager::~CConnectManager(void)
{
	for (CConnectBaseCtrlMap_T::iterator iter = m_ConnenctBaseMap.begin(); iter != m_ConnenctBaseMap.end(); ++iter)
	{
		CConnectBase * pTemConnect = iter->second;
		pTemConnect->DisconnectBase();
		PMDelete(pTemConnect);
	}
	m_ConnenctBaseMap.clear();
}

bool CConnectManager::ConnectDataBase( CPmDataBase::DataBaseName dataBaseName , CConnectBase * pNewConnect ,const CString & strFilePath)
{
	int iDataBaseName = dataBaseName;
	CConnectBaseCtrlMap_T::const_iterator temIter= m_ConnenctBaseMap.find(iDataBaseName);
	if(temIter == m_ConnenctBaseMap.end() && pNewConnect != NULL )
	{
// 		CConnect_sqlite * pNewConnect = NULL;
// 		switch (dataBaseName)
// 		{
// 		case CPmDataBase::Project:
// 			pNewConnect = PMNew(CConnect_Project);
// 			break;
// 		default:
// 			pNewConnect = PMNew(CConnect_sqlite);
// 		}
//		pNewConnect = PMNew(CConnect_sqlite);
//		
// 		if(!pNewConnect->ConnectBase(strFilePath))
// 		{
// 			PMDelete(pNewConnect);
// 			return false;
// 		}
		pNewConnect->CheckTableIsLive();

		m_ConnenctBaseMap[iDataBaseName] = pNewConnect;

		return true;
	}
	return false;//已经存在
}

bool CConnectManager::ConnectDataBase(CPmDataBase::DataBaseName dataBaseName,const CString & strFilePath)
{
	int iDataBaseName = dataBaseName;
	CConnectBaseCtrlMap_T::const_iterator temIter= m_ConnenctBaseMap.find(iDataBaseName);
	if(temIter == m_ConnenctBaseMap.end())
	{
 		CConnect_sqlite * pNewConnect = NULL;

		CConnectBaseFactoryMap_T::const_iterator temFactoryIter = m_ConnenctBaseFactoryMap.find(iDataBaseName);
		if(temFactoryIter == m_ConnenctBaseFactoryMap.end())
			pNewConnect = PMNew(CConnect_sqlite);
		else
		{
			CObject* pTemObject = temFactoryIter->second->CreateObject();
			PMRegMemory(pTemObject);

			pNewConnect = static_cast<CConnect_sqlite*>(pTemObject);
		}
		
		if(!pNewConnect->ConnectBase(strFilePath))
		{
			PMDelete(pNewConnect);
			return false;
		}
		pNewConnect->CheckTableIsLive();

		m_ConnenctBaseMap[iDataBaseName] = pNewConnect;

		pNewConnect->ConnectExternBase();
		return true;
	}
	return false;//已经存在
}

bool CConnectManager::DisConnectDataBase(CPmDataBase::DataBaseName dataBaseName)
{
	
	/// 
	/// 
	/// cad 空闲时处理二维和构件删除的 联动
	/// 
	/// 
// 	if ( dataBaseName == CPmDataBase::Project ) {
// 		PmDbBatchOpr.Update() ; 
// 	}

	/// 
	int iDataBaseName = dataBaseName;
	CConnectBaseCtrlMap_T::iterator temIter= m_ConnenctBaseMap.find(iDataBaseName);
	if(temIter != m_ConnenctBaseMap.end())
	{
		CConnectBase * pTemConnect = temIter->second;
		if(!pTemConnect->DisconnectBase())
			return false;
		PMDelete(pTemConnect);
		m_ConnenctBaseMap.erase(temIter);
		return true;
	}
	return false;//之前没有连接
}

void CConnectManager::RegisterConnectBase(CPmDataBase::DataBaseName dataBaseName,CRuntimeClass *pRuntimeClass)
{
	m_ConnenctBaseFactoryMap[dataBaseName]=pRuntimeClass;
}

bool	CConnectManager::BackUpDataBase( CPmDataBase::DataBaseName dataBaseName,const CString & strToFilePath )
{
	bool bRet = false ; 
	CConnect_sqlite * pSqlite = GetConnectCtrl( dataBaseName ) ; 

	if ( NULL != pSqlite )
	{
		bRet = pSqlite->BackUpDataBase( strToFilePath ) ; 
	}

	return bRet ;
}

/// 
bool	CConnectManager::CkeckDataBaseIntegrity( CPmDataBase::DataBaseName dataBaseName , const CString & strFilePath )
{
	bool bRet = false ; 
	CConnect_sqlite * pSqlite = GetConnectCtrl( dataBaseName ) ; 

	BOOL bConnect = NULL == pSqlite ? FALSE : TRUE ;

	if ( !bConnect)
	{
		/// 
		CConnectManager::Instance()->ConnectDataBase( dataBaseName , strFilePath ) ;

		/// 
		pSqlite = GetConnectCtrl( dataBaseName ) ;
	}

	/// 
	/// 
	CPMSQLTableCtrl * pTableCtrl = NULL ; 

	/// 检测数据库的完整性
	CString strSql = _T("PRAGMA integrity_check  ") ;

	if ( NULL == pSqlite )
		goto funExit ;


	/// 
	pSqlite->GetSQLTableCtrl( strSql , pTableCtrl ) ;

	/// 
	if ( NULL == pTableCtrl )
	{
		goto funExit ;
	}

	/// 
	if ( pTableCtrl->TableNextRow() )
	{
		CString strValue ;
		pTableCtrl->GetAsString( 0 , strValue ) ;

		if ( strValue.Compare( _T("ok")) == 0 )
		{
			bRet = true ;
		}
	}

	PMDelete( pTableCtrl ) ; 

funExit:
	if ( !bConnect )
	{
		CConnectManager::Instance()->DisConnectDataBase( dataBaseName ) ;
	}

	return bRet ;
}

/// 检测数据库的完整性
bool	CConnectManager::CkeckProjectIntegrity ( )
{
	/// 
	bool bRet = false ; 

	CConnect_sqlite * pSqlite = GetConnectCtrl( CPmDataBase::Project ) ; 

	CString strFilePath = pSqlite->GetFilePath() ; 
	
	/// 
	return CkeckDataBaseIntegrity( CPmDataBase::Project  ,  strFilePath ) ;

}

bool	CConnectManager::ConnectSysDataDataBase()
{
	CConnect_sqlite * pSysDataConnect = CConnectManager::Instance()->GetConnectCtrl(CPmDataBase::SysData);

	if (NULL == pSysDataConnect)
	{
		//CString strSysData = PmMfc::Fun::GetAppPath_v2() + _T("\\sysdata\\sysdata.pmdb");
		CString strSysData;
		return CConnectManager::Instance()->ConnectDataBase(CPmDataBase::SysData, strSysData);
	}
	
	return true;
}


CConnectBase * CConnectManager::GetConnectCtrl_Base(CPmDataBase::DataBaseName dataBaseName)
{
	int iDataBaseName = dataBaseName;
	CConnectBaseCtrlMap_T::iterator temIter= m_ConnenctBaseMap.find(iDataBaseName);
	if(temIter != m_ConnenctBaseMap.end())
	{
		CConnectBase * pTemConnect = temIter->second;

		return pTemConnect;
	}
	return NULL;//之前没有连接
}

CConnect_sqlite * CConnectManager::GetConnectCtrl(CPmDataBase::DataBaseName dataBaseName)
{
	int iDataBaseName = dataBaseName;
	CConnectBaseCtrlMap_T::iterator temIter= m_ConnenctBaseMap.find(iDataBaseName);
	if(temIter != m_ConnenctBaseMap.end())
	{
		CConnectBase * pTemConnect = temIter->second;

		return (CConnect_sqlite*)pTemConnect;
	}
	return NULL;//之前没有连接
}

CString CConnectManager::GetFilePath( const CPmDataBase::DataBaseName dataBaseName ) 
{
	CConnectBaseCtrlMap_T::iterator temIter= m_ConnenctBaseMap.find((int)dataBaseName);
	if(temIter != m_ConnenctBaseMap.end())
	{
		CConnectBase * pTemConnect = temIter->second;

		return ((CConnect_sqlite*)pTemConnect)->GetFilePath();
	}

	return pmdataservice::g_strEmpty ; 
}

CString CConnectManager::GetFileDir( const CPmDataBase::DataBaseName dataBaseName )  
{
	CConnectBaseCtrlMap_T::iterator temIter= m_ConnenctBaseMap.find((int)dataBaseName);
	if(temIter != m_ConnenctBaseMap.end())
	{
		CConnectBase * pTemConnect = temIter->second;

		return ((CConnect_sqlite*)pTemConnect)->GetFileDir();
	}

	return pmdataservice::g_strEmpty ; 
}


CString	CConnectManager::GetIniPath( const CPmDataBase::DataBaseName dataBaseName ) 
{
	CConnectBaseCtrlMap_T::iterator temIter= m_ConnenctBaseMap.find((int)dataBaseName);
	if(temIter != m_ConnenctBaseMap.end())
	{
		CConnectBase * pTemConnect = temIter->second;

		return ((CConnect_sqlite*)pTemConnect)->GetIniPath();
	}

	return pmdataservice::g_strEmpty ; 
}

void	CConnectManager::SetIniPath( const CPmDataBase::DataBaseName dataBaseName , const CString & strIniPath ) 
{
	CConnectBaseCtrlMap_T::iterator temIter= m_ConnenctBaseMap.find((int)dataBaseName);
	if(temIter != m_ConnenctBaseMap.end())
	{
		CConnectBase * pTemConnect = temIter->second;

		((CConnect_sqlite*)pTemConnect)->SetIniPath( strIniPath );
	}
}

//////////////////////////////////////////////////////////////////////////
CTempDataBaseConnectOpr::CTempDataBaseConnectOpr()
{
	m_pConnectCtrl = NULL;
}
CTempDataBaseConnectOpr::~CTempDataBaseConnectOpr()
{
	if (m_pConnectCtrl)
	{
		m_pConnectCtrl->DisconnectBase();
		PMDelete(m_pConnectCtrl);
	}
}

BOOL CTempDataBaseConnectOpr::ConnectDataBase(const CString& strFilePath)
{
	return FALSE;
}

void CTempDataBaseConnectOpr::DisConnect()
{
	if (m_pConnectCtrl)
	{
		m_pConnectCtrl->DisconnectBase();
		PMDelete(m_pConnectCtrl);
	}
}

CString CTempDataBaseConnectOpr::GetDataBasePath()
{
	if (NULL == m_pConnectCtrl)
	{
		return _T("");
	}

	return m_pConnectCtrl->GetFilePath();
}

CConnectBase* CTempDataBaseConnectOpr::GetConnectCtrl()
{
	return m_pConnectCtrl;
}

//////////////////////////////////////////////////////////////////////////
CTempSqliteConnect::CTempSqliteConnect()
{

}
CTempSqliteConnect::~CTempSqliteConnect()
{

}

BOOL CTempSqliteConnect::ConnectDataBase(const CString& strFilePath)
{
	if (m_pConnectCtrl)
	{
		return FALSE;
	}

	CConnect_sqlite* sqlite = PMNew(CConnect_sqlite);
	if (!sqlite->ConnectBase(strFilePath))
	{
		PMDelete(sqlite);
		return FALSE;
	}

	m_pConnectCtrl = sqlite;
	return TRUE;
}

void CTempSqliteConnect::DisConnect()
{
	if (m_pConnectCtrl)
	{
		m_pConnectCtrl->DisconnectBase();
		PMDelete(m_pConnectCtrl);
	}
}