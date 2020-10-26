#include "stdafx.h"
#include "AccessDrawIndexTable.h"

const CString CDrawIndexTable::CFieldInfo::id = _T("id");
const CString CDrawIndexTable::CFieldInfo::floorid = _T("floorid");
const CString CDrawIndexTable::CFieldInfo::comtype = _T("comtype");
const CString CDrawIndexTable::CFieldInfo::name = _T("name");
const CString CDrawIndexTable::CFieldInfo::showindex = _T("showindex");

const CString CDrawIndexTable::CFieldInfo::table_name = _T("drawindex");


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// select * from drawindex 
const int CAccessDrawIndexTable::SQLInfo::m_iGetDataSet = eSql_Idx_0; 
PMTSTRING CAccessDrawIndexTable::SQLInfo::m_strGetDataSet = CAccessDrawIndexTable::GetSql( CAccessDrawIndexTable::SQLInfo::m_iGetDataSet ) ; 

/// select comtype from drawindex where floorid = ?
const int CAccessDrawIndexTable::SQLInfo::m_iGetDrawIndex_ComTypeArr = eSql_Idx_1 ;
PMTSTRING CAccessDrawIndexTable::SQLInfo::m_strGetDrawIndex_ComTypeArr = CAccessDrawIndexTable::GetSql( CAccessDrawIndexTable::SQLInfo::m_iGetDrawIndex_ComTypeArr ) ; 

IMPLEMENT_DYNCREATE( CDrawIndexTable,CSQLDataSet )

CDrawIndexTable::CDrawIndexTable()
{

}

CDrawIndexTable::~CDrawIndexTable()
{

}

int CDrawIndexTable::InitFieldStr()
{
	AddField(CFieldInfo::id,ftInteger,0);
	AddField(CFieldInfo::floorid,ftInteger,0);
	AddField(CFieldInfo::comtype,ftInteger,0);
	AddField(CFieldInfo::name,ftBlob,0);
	AddField(CFieldInfo::showindex,ftInteger,0);


	SetFieldPK(GetColNum(CFieldInfo::id));
	SetTableName(CFieldInfo::table_name);
	return GetFieldCount();	
}


CAccessDrawIndexTable::CAccessDrawIndexTable(void)
{
	m_pDbBaseCtrl = CConnectManager::Instance()->GetConnectCtrl(CPmDataBase::Project);
	TryUpdateTableField();
}

CAccessDrawIndexTable::~CAccessDrawIndexTable(void)
{
}


BOOL CAccessDrawIndexTable::TryUpdateTableField()
{
	return TRUE;
}

BOOL CAccessDrawIndexTable::SaveDataSet(CSQLDataSet * pDrawIndexDataSet)
{
	if(NULL == m_pDbBaseCtrl)
		return FALSE;
	return pDrawIndexDataSet->DoSqlUpdate();
}

CSQLDataSet * CAccessDrawIndexTable::GetFloorID_DataSet(int iFloorID)
{
	if(NULL == m_pDbBaseCtrl)
		return NULL;

	CDrawIndexTable * pDrawIndexDataSet = PMNew(CDrawIndexTable);
	pDrawIndexDataSet->InitFieldStr();
	CString strTemWhere;
	strTemWhere.Format(_T("%s=%d"),CDrawIndexTable::CFieldInfo::floorid,iFloorID);
	pDrawIndexDataSet->SetWhere(strTemWhere);

	if(!m_pDbBaseCtrl->GetDataSet(pDrawIndexDataSet))
	{
		PMDelete(pDrawIndexDataSet);
		return NULL;
	}
	return pDrawIndexDataSet;
}

BOOL CAccessDrawIndexTable::GetTable_MaxID(int & iMaxID)
{
	if(NULL == m_pDbBaseCtrl)
		return FALSE;
	m_pDbBaseCtrl->GetTable_ColMaxValue(CDrawIndexTable::CFieldInfo::id,CDrawIndexTable::CFieldInfo::table_name,iMaxID);
	return TRUE;
}

BOOL CAccessDrawIndexTable::GetDrawIndex_ComTypeArr( const int iFloorID,CUIntArray & iComTypeArr)
{
	if ( NULL == m_pDbBaseCtrl )
	{
		return FALSE ; 
	}

	CPMSQLTableCtrl * pTableCtrl = NULL;

	m_pDbBaseCtrl->GetSQLTableCtrl( SQLInfo::m_strGetDrawIndex_ComTypeArr.c_str() , pTableCtrl);

	if ( NULL == pTableCtrl )
	{
		return NULL ;
	}

	/// 注意下标从1开始
	pTableCtrl->BindAsInterger( eCol_1 , iFloorID ) ;  
	iComTypeArr.Add(5100);//临时设入值以后取到就可删掉
	while ( pTableCtrl->TableNextRow() )
	{
		/// 下标从0开始
		int iComType = eComType_Err ;
		pTableCtrl->GetAsInterger( eCol_0 , iComType ) ; 

		/// 
		iComTypeArr.Add( iComType ) ;
	}

	PMDelete( pTableCtrl ) ;
	pTableCtrl = NULL ; 

	
	return TRUE  ; 
}

PMTSTRING	CAccessDrawIndexTable::GetSql( const int iType )
{
	PMTSTRING strSql ; 

	switch( iType )
	{
		/// select * from drawindex
	case SQLInfo::m_iGetDataSet:
		{
			CDrawIndexTable table ; 
			table.InitFieldStr() ; 
			CConnect_sqlite::BuildSQL(CPMSQLBaseCtrl::eSelectSQL , &table ,strSql );
		}
		break ; 

		/// select comtype from drawindex where floorid = ?
	case SQLInfo::m_iGetDrawIndex_ComTypeArr:
		{
			strSql.append( pmdataservice::g_strSelect ) ;
			strSql.append( CDrawIndexTable::CFieldInfo::comtype ) ;
			strSql.append( pmdataservice::g_strFrom ) ;
			strSql.append( CDrawIndexTable::CFieldInfo::table_name ) ; 
			strSql.append( pmdataservice::g_strWhere ) ;
			strSql.append( CDrawIndexTable::CFieldInfo::floorid ) ; 
			strSql.append( pmdataservice::g_strEqu ) ;
			strSql.append( pmdataservice::g_strQue ) ;

			strSql.append( pmdataservice::g_strOrderBy ) ; 
			strSql.append( CDrawIndexTable::CFieldInfo::showindex ) ; 
			strSql.append( pmdataservice::g_strSem ) ;
		}
		break ; 
	}
	return strSql ; 
}

NDataModelDataSet * CAccessDrawIndexTable::GetDataSet()
{
	if ( NULL == m_pDbBaseCtrl )
	{
		return NULL ; 
	}

	CPMSQLTableCtrl * pTableCtrl = NULL;

	m_pDbBaseCtrl->GetSQLTableCtrl( SQLInfo::m_strGetDataSet.c_str() , pTableCtrl);

	if ( NULL == pTableCtrl )
	{
		return NULL ;
	}

	CSQLDataSet * pDataSet = PMNew( CDrawIndexTable ) ;
	pDataSet->InitFieldStr() ;

	pTableCtrl->GetDataSet( pDataSet ) ;

	PMDelete( pTableCtrl ) ;
	pTableCtrl = NULL ; 

	return pDataSet ; 
}
