#pragma once

#include "../Source/ConnectManager.h"

#include "../Source/DataService_Common.h "

#include "../Source/DataBaseAndTableName.h"

/**
* @brief ���ݿ���� ����  
*		���ڷ�װ���ݷ���
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
	* @brief   ���� ������ṹ
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
	/// ��ȡ���Ӧ��Index 
	/// ���ص���ö�ٱ��� : CPmTable::TableName
	/// 
	virtual	int	 GetTableIndex()  
	{
		return eTableIndex_Init ;
	}

protected:

	CConnect_sqlite * m_pDbBaseCtrl;//�Ƕ�д���ݿ�Ŀ��� ָ��
};
