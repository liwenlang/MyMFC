#pragma once

#include "PM_StorageDataSet.h"

/**
* @brief ���ںϳ��ַ�����ֵ�DataSet  
*
*          
* @note  : ����DataSet �������ڴ��� �����ݣ����ܴ��� DataSetǶ�ף�������Ҫ���渡����
* @param : �ϳɹ���
				����1&&����2&&����3;;ֵ1&&ֵ2&&ֵ3;;
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [14/1/2013]
*/
class DataSetAPI CPM_TranStringDataSet : public CPM_StorageDataSet
{
	DECLARE_DYNCREATE(CPM_TranStringDataSet) 

public:
	CPM_TranStringDataSet(void);
	virtual ~CPM_TranStringDataSet(void);

	/**
	* @brief �����ַ�����Ϣ  
	*
	*          
	* @note  : 
	* @param : strInfo: ����1&&����2&&����3;;ֵ1&&ֵ2&&ֵ3;;
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [14/1/2013]
	*/
	BOOL LoadDataSetInfo(const CString & strInfo);

	/**
	* @brief  ����DataSet  �ַ�����Ϣ 
	*
	*          
	* @note  : strInfo: ����1&&����2&&����3;;ֵ1&&ֵ2&&ֵ3;;
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [14/1/2013]
	*/
	BOOL SaveDataSetInfo(CString & strInfo);

	/**
	* @brief   �����Ƿ��Զ����Ƹ���
	*
	*          
	* @note  : ����DataSet ����ʹ�ö����Ƹ��£������Ҫ�������ж��Ƿ����� Υ������
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [14/1/2013]
	*/
	virtual void SetUpdateAsBlobFiled(int col,BOOL b);
	virtual void SetUpdateAsBlobFiled(CString colName,BOOL b);
};
