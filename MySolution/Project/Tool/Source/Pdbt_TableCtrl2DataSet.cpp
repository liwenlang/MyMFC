
#include "StdAfx.h"

#include "Pdbt_TableCtrl2DataSet.h"


CTableCtrl2DataSet::CTableCtrl2DataSet() 
{

}

CTableCtrl2DataSet::~CTableCtrl2DataSet() 
{

}

NDataModelDataSet *	CTableCtrl2DataSet::GetDataSet(  CPMSQLTableCtrl * pTableCtrl)
{
	NDataModelDataSet * pDataSet = BuildDataSet( pTableCtrl ) ;

	FillDataSet( pTableCtrl , pDataSet ) ; 

	return pDataSet ; 
}

/// 根据记录集创建DataSet 
NDataModelDataSet * CTableCtrl2DataSet::BuildDataSet(   CPMSQLTableCtrl * pTableCtrl ) 
{
	NDataModelDataSet * pDataSet = PMNew( NDataModelDataSet ) ;

	int iFieldCount =0 ;
	pTableCtrl->GetColumnCount( iFieldCount ) ; 

	int i = 0 ; 
	for ( ; i < iFieldCount ; ++i )
	{
		CString strFieldName ;
		
		pTableCtrl->GetFieldName( i , strFieldName ) ;

		pDataSet->AddField( strFieldName , ftBlob , 0 ) ; 
	}

	return pDataSet ; 
}

/// 根据记录集填充DataSet
void CTableCtrl2DataSet::FillDataSet(  CPMSQLTableCtrl * pTableCtrl , NDataModelDataSet * pDataSet ) 
{
 	int iFieldCount =0 ;
 	pTableCtrl->GetColumnCount( iFieldCount ) ; 
 
 	while ( pTableCtrl->TableNextRow() )
 	{
 		int iRow = pDataSet->InsertRow(0) ; 
 
 		int i = 0 ; 
 		for ( ; i < iFieldCount ; ++i )
 		{
 			CString strValue ; 
 			pTableCtrl->GetAsString( i , strValue ) ;
 
 			pDataSet->SetAsString( i , iRow , strValue ) ; 
 		}
 	}
	//pTableCtrl->GetDataSet( (CSQLDataSet *)pDataSet ) ; 
}
