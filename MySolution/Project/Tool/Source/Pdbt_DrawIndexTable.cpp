#include "stdafx.h"
#include "Pdbt_DrawIndexTable.h"
#include "..\Pdbt_Common.h"
#include "Pdbt_Db_Connect.h"
#include "Pdbt_ExcelOpr.h"

int CPdbt_DrawIndexTable::CConstData::Id_Index  ;
int CPdbt_DrawIndexTable::CConstData::FloorId_Index ; 
int CPdbt_DrawIndexTable::CConstData::ComType_Index ; 
int CPdbt_DrawIndexTable::CConstData::ComName_Index ; 
int CPdbt_DrawIndexTable::CConstData::ShowIndex_Index ; 

CPdbt_DrawIndexTable::CPdbt_DrawIndexTable(void)
{
}

CPdbt_DrawIndexTable::~CPdbt_DrawIndexTable(void)
{
}

BOOL CPdbt_DrawIndexTable::Export2Db( const CString & strExcelPath )
{
	CConnect_sqlite* pDbBaseCtrl = CConnectManager::Instance()->GetConnectCtrl((CPmDataBase::DataBaseName)CDbConnect::eMyDb);
	if (pDbBaseCtrl == NULL)
	{ 
		return FALSE;
	}

	/// 初始化列
	InitData() ; 

	/// 清除老数据
	{
		CDrawIndexTable* pDataSet = PMNew(CDrawIndexTable);
		pDataSet->InitFieldStr();


		CString strTableName = pDataSet->GetTableName();

		CString strSelectTableName;
		strSelectTableName.Format(_T("select * from sqlite_master  where type = 'table' and tbl_name = '%s'"), strTableName);

		CPMSQLTableCtrl* pTableCtrl = NULL;

		pDbBaseCtrl->GetSQLTableCtrl(strSelectTableName, pTableCtrl);

		BOOL bHasTable = pTableCtrl->TableNextRow();
		PMDelete(pTableCtrl);
		if (bHasTable)
		{
			CString strSql;
			strSql += pmdataservice::g_strDelete.c_str();
			strSql += pmdataservice::g_strFrom.c_str();
			strSql += CDrawIndexTable::CFieldInfo::table_name;

			Db_Opr.ExecuteSQL(strSql);
		}
		else
		{
			pDbBaseCtrl->DoCreateSQLTable(pDataSet);
		}

		PMDelete(pDataSet);
	}

	// 	CAccessSysData_FloorBGModeTable table ; 
	// 	NDataModelDataSet * pDataSet =  table.GetDataSet() ; 
	CDrawIndexTable * pDataSet = PMNew( CDrawIndexTable ) ; 
	pDataSet->InitFieldStr() ; 

	Db_Opr.BindRegDbCtrl( pDataSet ) ;

	if ( NULL == pDataSet )
	{
		return FALSE; 
	}


	/// 
	CExcelOpr excelOpr ; 

	excelOpr.StartExcel() ; 

	excelOpr.OpenBook((LPCTSTR) strExcelPath ) ;

	excelOpr.LoadSheet( pmdbtool::eDrawIndex  ) ; 

	int iRowCount = excelOpr.GetRowCount() ; 
	int iColCount = excelOpr.GetColCount() ; 

	int iRow = CComParamSetBase::CComParamsSetData::m_iFieldRow + pmdataservice::eNum_1 ;  


	for ( ; iRow <= iRowCount ; ++iRow )
	{
		CString strId = excelOpr.GetCell( CConstData::Id_Index , iRow ) ;

		CString strFloorId = excelOpr.GetCell( CConstData::FloorId_Index , iRow ) ; 

		CString strComType = excelOpr.GetCell( CConstData::ComType_Index , iRow ) ; 

		CString strComName = excelOpr.GetCell( CConstData::ComName_Index , iRow ) ; 

		CString strBgMode = excelOpr.GetCell( CConstData::ShowIndex_Index , iRow  ) ; 


		int iDbRow = pDataSet->InsertRow(0) ; 

		pDataSet->SetAsString( CDrawIndexTable::CFieldInfo::id , iDbRow , strId ) ;

		pDataSet->SetAsString( CDrawIndexTable::CFieldInfo::floorid , iDbRow , strFloorId ) ; 

		pDataSet->SetAsString( CDrawIndexTable::CFieldInfo::comtype , iDbRow , strComType ) ;

		pDataSet->SetAsString( CDrawIndexTable::CFieldInfo::name , iDbRow , strComName ) ;

		pDataSet->SetAsString( CDrawIndexTable::CFieldInfo::showindex , iDbRow , strBgMode ) ;

	}

	((CSQLDataSet *)pDataSet)->DoSqlUpdate() ;

	PMDelete( pDataSet ) ;

	/// 
	excelOpr.CloseBook() ;

	/// 
	excelOpr.ReleaseExcel() ;

	/// 
	excelOpr.CloseExcel() ; 

	return TRUE;
}

void CPdbt_DrawIndexTable::InitData()
{
	CComParamSetBase::CComParamsSetData::m_iFieldCol = pmdataservice::eNum_2 ;		/// 列名开始列
	CComParamSetBase::CComParamsSetData::m_iFieldRow = pmdataservice::eNum_2;		/// 列名所在的行

	CConstData::Id_Index			= pmdataservice::eNum_2 ;
	CConstData::FloorId_Index			= pmdataservice::eNum_3 ;
	CConstData::ComType_Index			= pmdataservice::eNum_4 ;
	CConstData::ComName_Index	= pmdataservice::eNum_5 ;
	CConstData::ShowIndex_Index = pmdataservice::eNum_6 ;
}