// PMSQLBaseCtrl.h: interface for the CPMSQLBaseCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMSQLBASECTRL_H__5B8A1036_5F31_4D1A_B4DD_785B28F9D987__INCLUDED_)
#define AFX_PMSQLBASECTRL_H__5B8A1036_5F31_4D1A_B4DD_785B28F9D987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SQLDataSet.h"

/**
* @brief 作为 SQL数据库的 连接池  
*
*          
* @note  : 这个类，一次只能连 一个SQLite文件，内部可以对 一个文件的 多表进行查询
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [8/6/2012]
*/
struct sqlite3_stmt;

class CPMSQLTableCtrl;

class DataSetAPI CPMSQLBaseCtrl  
{
public:

	CPMSQLBaseCtrl();
	virtual ~CPMSQLBaseCtrl();

	enum CSQLType//SQL 语句的 类型
	{
		eInsertSQL,
		eUpdateSQL,
		eSelectSQL
	};

	class DataSetAPI CSQLTransaction
	{
	public:
		CSQLTransaction(CPMSQLBaseCtrl * pSQLBaseCtrl);

		virtual ~CSQLTransaction();

		// 事务回滚
		bool RollBack();

	protected:
		
		CPMSQLBaseCtrl * m_pSqlBaseCtrl;
	};

	/**
	* @brief  创建SQL语句 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [10/7/2012]
	*/
	static bool BuildSQL(CSQLType sqlType,CSQLDataSet * pDataSet,PmTSQLString & sNewSQL);

	/**
	* @brief  连接 SQL 数据库 
	*
	*          
	* @note  : 执行 这个函数 SQL的 数据表格没有 更新
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [8/6/2012]
	*/
	bool ConnectSQLDataBase(const CString & strFilePath);

	/**
	* @brief  断开SQL 数据库 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [8/6/2012]
	*/
	bool DisConnectSQLDataBase();

	bool	BackUpDataBase( const CString & strToFilePath ) ;
	
	/**
	* @brief 执行SQL语句 获取 DataSet 
	*
	*          
	* @note  : 如果需要保存，执行 DataSet.UpdateSQL()函数
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [8/6/2012]
	*/
	bool GetDataSet(LPCTSTR strSQL,CSQLDataSet * pDataSet);

	/**
	* @brief   添加的一个中间函数, 从记录集转到DataSet
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [2012-7-11]
	*/
	void	GetDataSet( sqlite3_stmt * pStmt , CSQLDataSet * pDataSet ) ;

	/**
	* @brief  获取 执行sql之后的 结果集 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [20/6/2012]
	*/
	bool GetSQLTableCtrl(LPCTSTR sSQL,CPMSQLTableCtrl *& pTableCtrl);

	/**
	* @brief 执行SQL 语句  
	*
	*          
	* @note  : 作为 DataSet 保存的唯一 执行接口
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [8/6/2012]
	*/
	bool ExecuteSQL(LPCTSTR sSQL);

	/**
	* @brief 获取 最后的一个错误；  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [20/6/2012]
	*/
	CString GetLastError();

	//增减 事务个数，计数器 使用
	BOOL AddSQLTransationCount();
	void SubSQLTransationCount();

protected:
	
	LPVOID m_pSQlite3;//SQL的控制指针

	CString m_strDBFilePath;//连接的 SQL文件位置

	int		m_iSQLTransationCount;//当前事务个数
//	CSQLTableInfoArr m_SQLTableInfoArr;//SQL 表信息 队列
};



class DataSetAPI CPMSQLTableCtrl
{
public:

	enum TableDataType
	{
		TableCol_Int = 1,\
		TableCol_Float = 2,\
		TableCol_String = 3,\
		TableCol_Blob = 4,
	};

	CPMSQLTableCtrl(CPMSQLBaseCtrl * pSqlBaseCtrl, sqlite3_stmt * pStmt);
	virtual ~CPMSQLTableCtrl();

	/**
	* @brief  移到表格下一行 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [20/6/2012]
	*/
	BOOL TableNextRow();

	BOOL GetColumnCount(int &iCount);
	BOOL GetColumnDataType(int iCol,TableDataType & tableDataType);

	/**
	* @brief 获取指定位置的 列名称  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [5/7/2012]
	*/
	BOOL GetFieldName(int iCol,CString & strFieldName);

	BOOL GetAsInterger(int iCol,int & iValue);
	BOOL GetAsDouble(int iCol,double & dValue);
	BOOL GetAsString(int iCol,CString & strValue);
	BOOL GetAsBlob(int iCol,const void *& pBuffer,long & lBufferSize);
	BOOL GetBlob(int iCol,void *& pBuffer,long & lBufferSize);

	BOOL GetAsInterger(const CString & strFieldName,int & iValue);
	BOOL GetAsDouble(const CString & strFieldName,double & dValue);
	BOOL GetAsString(const CString & strFieldName,CString & strValue);
	BOOL GetAsBlob(const CString & strFieldName,const void *& pBuffer,long & lBufferSize);
	BOOL GetBlob(const CString & strFieldName,void *& pBuffer,long & lBufferSize);

	//////////////////////////////////////////////////////////////////////////
	//更新 操作开始
	//只有在 执行 Insert Update语句时 才有效，对Sql语句中的 问号，部分进行数据绑定
	//主要用于更新 Blob数据
	BOOL BindAsInterger(int iCol,int iValue);
	BOOL BindAsDouble(int iCol,double dValue);
	BOOL BindAsString(int iCol,const CString & strValue);
	BOOL BindAsBlob( const int iCol,const LPVOID & pBuffer,const long lBufferSize);

	

	/**
	* @brief   封装对整个DataSet的绑定。 具体实现的时候还是一个字段一个字段去处理。
	*
	*          
	* @note  : 注意 aryNotBindCol 是记录集中的下标
	* @param : aryNotBindCol 不需要绑定的字段
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [2012-7-19]
	*/
	BOOL BindDataSet( NDataModelDataSet * pDataSet , const CStringArray & aryNotBindCol ) ; 
	BOOL BindDataSet( NDataModelDataSet * pDataSet , const CUIntArray & aryNotBindCol ) ;
	 ; 
	/**
	* @brief  将绑定的数据提交给 数据库 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [21/6/2012]
	*/
	BOOL Update();

	/**
	* @brief  重新执行sql语句 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [21/6/2012]
	*/
	BOOL ResetSql();

	/**
	* @brief   根据记录集获取DataSet
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [2012-7-11]
	*/
	BOOL GetDataSet( CSQLDataSet * pDataSet ) ;
	void SetCurParmID(int iParmID)
	{
		m_ParmID = iParmID;
	}

private:

	/**
	* @brief  检测错误编号，并弹出提示 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [25/12/2012]
	*/
	BOOL CheckTableCtrl_ErrorCode(int iErrorCode);

	/**
	* @brief   字符串的读取需要特殊处理 /不对外公开 
	*
	*          
	* @note  : sqlite3_column_text16 返回的是UTF16 的编码
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [2012-7-27]
	*/
	BOOL	GetColAsString( const int iCol,CString & strValue , sqlite3_stmt * pStmt = NULL ) ;

	/**
	* @brief  尝试创建列名 map 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [5/7/2012]
	*/
	BOOL InitFieldNameMap();

	CPMSQLBaseCtrl * m_pSqlBaseCtrl;
	sqlite3_stmt * m_pStmt;//表绑定处理信息
	int m_ParmID;//当前序号 用于错误处理

	CMapStringToPtr m_MapNameField;//列名称队列，在有需要的时候才添加内容，value中对应的 名称所在列编号
};
#endif // !defined(AFX_PMSQLBASECTRL_H__5B8A1036_5F31_4D1A_B4DD_785B28F9D987__INCLUDED_)
