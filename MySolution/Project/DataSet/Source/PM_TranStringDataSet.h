#pragma once

#include "PM_StorageDataSet.h"

/**
* @brief 用于合成字符串拆分的DataSet  
*
*          
* @note  : 这种DataSet 自能用于处理 简单数据，不能处理 DataSet嵌套，尽量不要保存浮点数
* @param : 合成规则
				列名1&&列名2&&列名3;;值1&&值2&&值3;;
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
	* @brief 读入字符串信息  
	*
	*          
	* @note  : 
	* @param : strInfo: 列名1&&列名2&&列名3;;值1&&值2&&值3;;
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [14/1/2013]
	*/
	BOOL LoadDataSetInfo(const CString & strInfo);

	/**
	* @brief  保存DataSet  字符串信息 
	*
	*          
	* @note  : strInfo: 列名1&&列名2&&列名3;;值1&&值2&&值3;;
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [14/1/2013]
	*/
	BOOL SaveDataSetInfo(CString & strInfo);

	/**
	* @brief   设置是否以二进制更新
	*
	*          
	* @note  : 这种DataSet 不能使用二进制更新，这个主要是用于判断是否有人 违例调用
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [14/1/2013]
	*/
	virtual void SetUpdateAsBlobFiled(int col,BOOL b);
	virtual void SetUpdateAsBlobFiled(CString colName,BOOL b);
};
