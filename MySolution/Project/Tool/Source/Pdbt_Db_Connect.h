

#pragma once 

/**
 * @brief  数据库的连接 
 *
 *        
 * @author liwenlang
 * @version v1.0
 * @date  [2012-7-21]
 * @note  单连接,一次只能连接一个数据库
 */

class CDbConnect
{
public:
	enum
	{
		eMyDb =  CPmDataBase::Extern
	} ;

private:
	CDbConnect() ; 

public:
	virtual ~CDbConnect();
	static CDbConnect & Instance() ; 

public:

	BOOL	IsConnect() const ;

	BOOL	Connect( const CString& strFilePath ) ;

	BOOL	DisConnect( ) ;

	BOOL	ExecuteSQL( const CString & strSql ) ;

	BOOL	ExecuteSQL( const CString & strSql , CPMSQLTableCtrl *& pTableCtrl ) ;

	BOOL	ExecuteSQL( const CString & strSql , NDataModelDataSet *& pDataSet ) ;

	/// 测试sql执行效率
	BOOL	ExecuteSQL_Test( const CString & strSql , NDataModelDataSet *& pDataSet ) ;

	/// 压缩数据库
	BOOL	CompressDb() ; 

	/// 
	/// 将DataSet绑定到DbCtrl 
	/// 绑定后DataSet可以执行更新数据库
	/// 
	BOOL	BindRegDbCtrl( CSQLDataSet * pDataSet ) ;

	/// 
	BOOL	CreateTable( CSQLDataSet * pDataSet ) ;

	/// 开启事务
	void	StartTransaction() ; 

	/// 提交事务
	void	CommitTransaction() ; 

private:
	/// 已经连接上数据库
	BOOL	m_bConnect ; 
};

