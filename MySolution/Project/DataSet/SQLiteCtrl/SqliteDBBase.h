#pragma once
#include "sqlite3.h"
#include <map>

// 简单封装的Sqlite数据库对象
class DataSetAPI CSqliteDataBase
{
public:
	explicit CSqliteDataBase(const CString& strSqliteDBPath);
	~CSqliteDataBase(void);
public:
	/**
	*  @brief : 获取数据库对象指针
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/31 9:07
	*/
	sqlite3* GetSqliteDb();

	/**
	*  @brief : 加载数据库或者保存数据库(在两个main数据库中进行操作)
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/8/4 13:55
	*/
	int LoadOrSaveDb(const CString& strFilePath,BOOL bIsSave);

	/**
	*  @brief :  加载数据库或者保存数据库(可指定任意数据库)
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/8/4 13:56
	*/
	int LoadOrSaveDb(const CString& strFilePath,const CString& strDesDbName,const CString& strSrcDbName,BOOL bIsSave);
	
private:
	/**
	*  @brief : 打开数据库,如果不存在,则创建
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:47
	*/
	int Open(const CString& strSqliteDBPath);
	int Open(const CString& strSqliteDBPath,sqlite3*& pDb);
	/**
	*  @brief : 关闭数据库
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:47
	*/
	int Close(sqlite3* pDb);
private:
	CSqliteDataBase(const CSqliteDataBase&);
	CSqliteDataBase& operator = (const CSqliteDataBase&);
public:
	int Close();
public:
	/**
	*  @brief : 注册自定义函数
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2016/5/16 16:52
	*/
	int Create_function(const CString& strFunctionName,const int nArg,void (*pFunc)(void* ctx,int nArg,void** values),void *pApp = NULL);
	/**
	*  @brief : 获取自定义函数传递的值
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2016/5/17 9:57
	*/
	static int Value_Int(void* pValue);
	static __int64 Value_Int64(void* pValue);
	static double Value_Double(void* pValue);
	static CString Value_Text(void* pValue);
	static int Value_Bytes(void* pValue);
	static const void* Value_Blob(void* pValue);
	/**
	*  @brief : 设置自定义函数的返回值
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2016/5/17 10:16
	*/
	static void Result_int(void* ctx,int nValue);
	static void Result_int64(void* ctx,__int64 nValue);
	static void Result_Double(void* ctx,double dblValue);
	static void Result_Text(void* ctx,const CString& strValue);
	static void Result_Blob(void* ctx,const void* pBuffer,const int nByte);

	/**
	*  @brief : 创建自定义聚合函数
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2016/5/18 9:21
	*/
	int Create_Aggregation_function(const CString& strFunctionName,const int nArg,void (*pStep)(void* ctx,int nArg,void** values),
									void (*pFinal)(void* ctx),void *pApp = NULL);

	/**
	*  @brief : 聚合上下文,用来存储聚合SQL语句的状态
	*
	*
	*  @note	首次调用为聚合分配内存,之后每次调用返回首次分配的内存地址
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2016/5/18 9:37
	*/
	static void* Aggregate_Context(void* ctx,int nBytes);
	/**
	*  @brief : 内存管理函数
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2016/5/18 10:41
	*/
	static void* Malloc(int nBytes);						// 分配内存
	static void* Malloc64(__int64 nBytes);					// 分配内存
	static void* Realloc(void* pPriorMem,int nBytes);		// 重置内存大小
	static void* Realloc64(void* pPriorMem,__int64 nBytes);	// 重置内存大小
	static void Free(void* pMem);							// 释放内存
	static __int64 Msize(void* pMem);						// 获取内存大小
	/**
	*  @brief : 创建自定义排序
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2016/5/18 13:56
	*/
	int Create_Collation(const CString& strFunctionName,int(*xCompare)(void*,int,const void*,int,const void*),void *pArg = NULL);
private:
	sqlite3* m_pDb;					// 数据库指针对象
};

// 简单封装的Sqlite语句对象
class DataSetAPI CSqliteDBStmt
{
public:
	explicit CSqliteDBStmt(CSqliteDataBase* sqliteDB);
	~CSqliteDBStmt(void);

	/**
	*  @brief : 开启事务
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/8/4 15:43
	*/
	void Begin();
	/**
	*  @brief : 回滚事务
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/8/25 16:00
	*/
	void RollBack();
	/**
	*  @brief : 提交事务
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/8/4 15:43
	*/
	void Commit();
	/**
	*  @brief : 准备编译语句
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:48
	*/
	int Prepare(const CString& strSQL);

	/**
	*  @brief : 执行语句
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:48
	*/
	int Step();

	/**
	*  @brief : 完成语句
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:50
	*/
	int Finalize();

	/**
	*  @brief : 重置语句
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:50
	*/
	int Reset();
	/**
	*  @brief : 清空绑定值为null
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:50
	*/
	int Clear_Bindings();
	/**
	*  @brief : 获取SELECT语句的字段个数
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:50
	*/
	int Column_Count();
	/**
	*  @brief : 获取记录的字段个数,只有语句句柄具有活动游标才可以工作
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:50
	*/
	int Data_Count();

	/**
	*  @brief : 获取字段名称
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:52
	*/
	CString Column_Name(int iCol);
	/**
	*  @brief : 获取列的值类型
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2016/8/19 17:31
	*/
	int Column_Type(const CString& strColName);
	/**
	*  @brief : 获取字段值为字符
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:52
	*/
	CString Column_Text(int iCol);
	BOOL Column_Text(const CString& strColName,CString& strColValue);
	/**
	*  @brief : 获取字段值为整型
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:53
	*/
	int Column_Int(int iCol);
	BOOL Column_Int(const CString& strColName,int& iColValue);
	/**
	*  @brief : 获取字段值为浮点型
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:54
	*/
	double Column_Double(int iCol);
	BOOL Column_Double(const CString& strColName,double& dblColValue);

	/**
	*  @brief : 绑定大字段
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:54
	*/
	const void* Column_Blob(int iCol);
	BOOL Column_Blob(const CString& strColName,const void*& pBuffer);
	BOOL Column_Blob( const CString& strColName,const void*& pBuffer,long & lBufferSize);

	/**
	*  @brief : 获取大字段的大小
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/9/10 16:19
	*/
	long Column_Blob_Size( const int iCol ); 

	/**
	*  @brief : 获取绑定名称对应的索引号
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/8/4 9:14
	*/
	int Bind_Parameter_Index(const CString& strColName);
	/**
	*  @brief : 绑定整型值
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:54
	*/
	int Bind_Int(int iIndex,int iValue);
	int Bind_Int(const CString& strColName,int iValue);
	int Bind_Int64(int iIndex, __int64 llValue);
	int Bind_Int64(const CString& strColName, __int64 llValue);
	/**
	*  @brief : 绑定浮点型值
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:55
	*/
	int Bind_Double(int iIndex,double dblValue);
	int Bind_Double(const CString& strColName,double dblValue);
	/**
	*  @brief : 绑定字符型值
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:55
	*/
	int Bind_Text(int iIndex,const CString& strValue);
	int Bind_Text(const CString& strColName,const CString& strValue);
	/**
	*  @brief : 绑定二进制
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 16:55
	*/
	int Bind_Blob(int iIndex,const void* pBuffer,const int nByte);
	int Bind_Blob(const CString& strColName,const void* pBuffer,const int nByte);
	/**
	*  @brief : 获取最近的错误信息
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/22 17:21
	*/
	CString ErrMsg();
	/**
	*  @brief : 执行SQL语句
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/23 10:06
	*/
	int Exec(const CString& strSQL);
	/**
	*  @brief : 附加数据库到当前数据库
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2016/8/4 9:15
	*/
	int Attach(const CString& strtDBPath,const CString& strDBName);
	/**
	*  @brief : 分离附加数据库
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/8/5 11:10
	*/
	int Detach(const CString& strDBName);
	/**
	*  @brief : 删除指定数据库的指定表
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/8/7 15:56
	*/
	int DropTable(const CString& strTableName,const CString& strDbName = _T("main"));
	/**
	*  @brief : 删除视图
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2016/5/19 19:15
	*/
	int DropView(const CString& strTableName,const CString& strDbName = _T("main"));
	/**
	*  @brief : 判断表是否存在
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/8/7 15:01
	*/
	BOOL IsTableExist(const CString& strTableName,const CString& strDbName = _T("main"));
	/**
	*  @brief : 判断是否存在指定视图
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2016/5/19 19:13
	*/
	BOOL IsViewExist(const CString& strTableName,const CString& strDbName = _T("main"));
	/**
	*  @brief : 判断表是否存在
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/8/7 15:01
	*/
	BOOL IsFieldExist(const CString& strTableName,const CString& strFieldName,const CString& strDbName = _T("main"));

	/**
	*  @brief : 复制表，从源数据库复制表到目标数据库
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2019/11/13 11:36
	*/
	int CopyTable(const CString& strSrcDbName,const CString& strSrcTableName,const CString& strDesDbName,const CString& strDesTableName);

	/**
	*  @brief : 获取数据库语句对象指针
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/31 9:30
	*/
	sqlite3_stmt* GetSqliteStmt();

	/**
	*  @brief : 初始化列名和列序号的对应关系
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/8/3 14:50
	*/
	void InitColName2ColIndex();
private:
	typedef std::map<CString,int> MapStrName2ICol;
	typedef MapStrName2ICol::iterator MapStrName2IColIter;		
	MapStrName2ICol m_mapstrName2iCol;							// 列名和列序号Map
	sqlite3_stmt* m_pStmt;										// 语句对象指针
	CSqliteDataBase* m_pSqliteDataBase;							// Sqlite基类对象指针
};
