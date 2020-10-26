#pragma once

#include "../Source/ConnectManager.h"

#include "../Source/DataService_Common.h "

#include "../Source/DataBaseAndTableName.h"

/**
* @brief 数据库访问 基类  
*		用于封装数据访问
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [16/1/2014]
*/
class DataServiceAPI CPMSLAccessTable
{
public:
	CPMSLAccessTable(void);
	virtual ~CPMSLAccessTable(void);

	/**
	* @brief   尝试 升级表结构
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [29/6/2012]
	*/
	virtual BOOL TryUpdateTableField() = 0;

	/// 
	/// 获取表对应的Index 
	/// 返回的是枚举变量 : CPmTable::TableName
	/// 
	virtual	int	 GetTableIndex()  
	{
		return eTableIndex_Init ;
	}

protected:

	CConnect_sqlite * m_pDbBaseCtrl;//是读写数据库的控制 指针
};
