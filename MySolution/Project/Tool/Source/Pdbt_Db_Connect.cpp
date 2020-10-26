

#include "StdAfx.h"

#include "Pdbt_Db_Connect.h "

#include "Pdbt_TableCtrl2DataSet.h"

#include "..\Pdbt_Common.h"


CDbConnect::CDbConnect() : m_bConnect(FALSE)
{

}


CDbConnect::~CDbConnect()
{
	DisConnect() ; 
}

CDbConnect & CDbConnect::Instance()
{
	static CDbConnect dbConnect ; 
	return dbConnect ; 
}

BOOL	CDbConnect::IsConnect() const 
{
	return m_bConnect ; 
}


BOOL	CDbConnect::Connect( const CString& strFilePath )
{
	if ( IsConnect() )
	{
		CString strCurDbPath = CConnectManager::Instance()->GetFilePath( ( CPmDataBase::DataBaseName)eMyDb ) ;
		if ( (LPCTSTR)strCurDbPath == strFilePath )
		{
			return TRUE ; 
		}

		DisConnect() ; 
	}

	m_bConnect = CConnectManager::Instance()->ConnectDataBase( ( CPmDataBase::DataBaseName)eMyDb , strFilePath) ? TRUE : FALSE ;

	return m_bConnect ;
}

BOOL	CDbConnect::DisConnect( ) 
{
	if ( !IsConnect() )
	{
		return TRUE ; 
	}

	BOOL bSucc = CConnectManager::Instance()->DisConnectDataBase( ( CPmDataBase::DataBaseName)eMyDb ) ? TRUE : FALSE ;

	/// 释放成功
	if ( bSucc )
	{
		m_bConnect = FALSE ; 
	}

	CConnectManager::Release() ; 

	return bSucc ;
}

BOOL	CDbConnect::ExecuteSQL( const CString & strSql )
{
	if ( !IsConnect() )
	{
		return FALSE ; 
	}

	CConnect_sqlite * pDbBaseCtrl = CConnectManager::Instance()->GetConnectCtrl( ( CPmDataBase::DataBaseName)eMyDb );

	return pDbBaseCtrl->ExecuteSQL( strSql) ; 
}

BOOL	CDbConnect::ExecuteSQL( const CString & strSql , CPMSQLTableCtrl *& pTableCtrl )
{
	if ( !IsConnect() )
	{
		return FALSE ;
	}

	CConnect_sqlite * pDbBaseCtrl = CConnectManager::Instance()->GetConnectCtrl( ( CPmDataBase::DataBaseName)eMyDb );

	return pDbBaseCtrl->GetSQLTableCtrl( strSql , pTableCtrl ) ? TRUE : FALSE ; 

}

/// 开启事务
void	CDbConnect::StartTransaction() 
{
	CConnect_sqlite * pDbBaseCtrl = CConnectManager::Instance()->GetConnectCtrl( ( CPmDataBase::DataBaseName)eMyDb );

	/// 开启事务
	if ( pDbBaseCtrl )
	{
		pDbBaseCtrl->ExecuteSQL(_T("begin transaction;"));
	}
}

/// 提交事务
void	CDbConnect::CommitTransaction() 
{
	CConnect_sqlite * pDbBaseCtrl = CConnectManager::Instance()->GetConnectCtrl( ( CPmDataBase::DataBaseName)eMyDb );

	/// 提交事务
	if ( pDbBaseCtrl )
	{
		pDbBaseCtrl->ExecuteSQL(_T("commit transaction;"));
	}
}


BOOL	CDbConnect::ExecuteSQL( const CString & strSql , NDataModelDataSet *& pDataSet ) 
{
	CPMSQLTableCtrl * pTableCtrl = NULL ; 

	CConnect_sqlite * pDbBaseCtrl = CConnectManager::Instance()->GetConnectCtrl( ( CPmDataBase::DataBaseName)eMyDb );

	if (NULL == pDbBaseCtrl)
	{
		return FALSE;
	}


	/// 开启事务
	if ( pDbBaseCtrl )
	{
		pDbBaseCtrl->ExecuteSQL(_T("begin transaction;"));
	}

	/// 记录时间
	pmdbtool::CTestClock testClock( _T("执行sql语句的时间:") )  ; 
	if ( !ExecuteSQL( strSql , pTableCtrl ) )
	{
		pDbBaseCtrl->ExecuteSQL(_T("commit transaction;"));
		return FALSE ;
	}

	pDataSet = CTableCtrl2DataSet::GetDataSet( pTableCtrl ) ;

	PMDelete( pTableCtrl ) ;

	if ( pDbBaseCtrl )
	{
		pDbBaseCtrl->ExecuteSQL(_T("commit transaction;"));
	}

	return TRUE ;  
}


BOOL	CDbConnect::ExecuteSQL_Test( const CString & strSql , NDataModelDataSet *& pDataSet ) 
{
	CConnect_sqlite * pDbBaseCtrl = CConnectManager::Instance()->GetConnectCtrl( ( CPmDataBase::DataBaseName)eMyDb );

	if ( NULL != pDbBaseCtrl )
	{
		pDbBaseCtrl->ExecuteSQL( _T("vacuum") ) ; 
	}

	return TRUE ;  
}

BOOL	CDbConnect::CompressDb()
{
	CConnect_sqlite * pDbBaseCtrl = CConnectManager::Instance()->GetConnectCtrl( ( CPmDataBase::DataBaseName)eMyDb );

	if ( NULL != pDbBaseCtrl )
	{
		/// 记录时间
		pmdbtool::CTestClock testClock( _T("压缩的时间:") )  ; 
		pDbBaseCtrl->ExecuteSQL( _T("vacuum") ) ; 
	}

	return TRUE ;
}


BOOL	CDbConnect::BindRegDbCtrl( CSQLDataSet * pDataSet )
{
	CConnectManager::Instance()->GetConnectCtrl( ( CPmDataBase::DataBaseName) CDbConnect::eMyDb )->RegSQLCtrl(pDataSet);

	return TRUE ; 
}

BOOL	CDbConnect::CreateTable( CSQLDataSet * pDataSet ) 
{
	/// 
	CConnect_sqlite * pDbBaseCtrl = CConnectManager::Instance()->GetConnectCtrl( ( CPmDataBase::DataBaseName)eMyDb );

	return pDbBaseCtrl->DoCreateSQLTable( pDataSet ) ;
}