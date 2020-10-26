
#pragma once 

/**
 * @brief �Ӽ�¼��ת��ΪDataSet �� ��DataSet���ݼ�¼������
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
	/// ���ݼ�¼������DataSet 
	static NDataModelDataSet * BuildDataSet(   CPMSQLTableCtrl * pTableCtrl ) ;

	/// ���ݼ�¼�����DataSet
	static void FillDataSet(  CPMSQLTableCtrl * pTableCtrl , NDataModelDataSet * pDataSet ) ;

};
