#pragma once
//#include "..\accesstable.h"

class DataServiceAPI CDrawIndexTable : public CSQLDataSet
{
public:
	DECLARE_DYNCREATE(CDrawIndexTable)

	CDrawIndexTable();
	virtual ~CDrawIndexTable();

	struct DataServiceAPI CFieldInfo
	{
		static const CString id;							//序号
		static const CString floorid;						//楼层ID
		static const CString comtype;						//构件类型
		static const CString name;						//显示名称
		static const CString showindex;					//显示顺序

		static const CString table_name;					//表名
	};

	virtual int InitFieldStr();
};


class DataServiceAPI CAccessDrawIndexTable :
	public CPMSLAccessTable
{
public:
	CAccessDrawIndexTable(void);
public:
	virtual ~CAccessDrawIndexTable(void);

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
	virtual BOOL TryUpdateTableField();

	/**
	* @brief  获取指定行 表内容 ，需要删除 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [11/7/2012]
	*/
	CSQLDataSet * GetFloorID_DataSet(int iFloorID);

	/**
	* @brief   
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [31/5/2013]
	*/
	BOOL SaveDataSet(CSQLDataSet * pDrawIndexDataSet);

	/**
	* @brief 获取 最大的 ID值  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [11/7/2012]
	*/
	BOOL GetTable_MaxID(int & iMaxID);


	/******************************************************************** 
	功能描述：获取指定层 构件类型的绘制顺序
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[10/10/2009]
	*********************************************************************/ 
	BOOL GetDrawIndex_ComTypeArr( const int iFloorID,CUIntArray & iComTypeArr);

	/**
	* @brief   获取整表的数据，不推荐使用，
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [2012-7-12]
	*/
	NDataModelDataSet * GetDataSet() ; 

	///	
public:
	static PMTSTRING	GetSql( const int iType ) ;


public:
	//固定的SQL 集合
	struct SQLInfo
	{
		/// 
		static const int m_iGetDataSet ; 
		static PMTSTRING m_strGetDataSet ; 

		/// 
		static const int m_iGetDrawIndex_ComTypeArr ;
		static PMTSTRING m_strGetDrawIndex_ComTypeArr ; 
	} ;
};
