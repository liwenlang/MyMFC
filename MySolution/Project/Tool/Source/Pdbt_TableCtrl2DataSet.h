
#pragma once 

/**
 * @brief 从记录集转化为DataSet ， 且DataSet根据记录集生成
 *
 *        
 * @author liwenlang
 * @version v1.0
 * @date  [2012-7-21]
 * @note  
 */


class CTableCtrl2DataSet
{
public:
	CTableCtrl2DataSet() ; 

	~CTableCtrl2DataSet() ; 

public:

	static NDataModelDataSet *	GetDataSet(  CPMSQLTableCtrl * pTableCtrl) ;

private:
	/// 根据记录集创建DataSet 
	static NDataModelDataSet * BuildDataSet(   CPMSQLTableCtrl * pTableCtrl ) ;

	/// 根据记录集填充DataSet
	static void FillDataSet(  CPMSQLTableCtrl * pTableCtrl , NDataModelDataSet * pDataSet ) ;

};
