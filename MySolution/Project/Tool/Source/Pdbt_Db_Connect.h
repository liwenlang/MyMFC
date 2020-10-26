

#pragma once 

/**
 * @brief  ���ݿ������ 
 *
 *        
 * @author liwenlang
 * @version v1.0
 * @date  [2012-7-21]
 * @note  ������,һ��ֻ������һ�����ݿ�
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

	/// ����sqlִ��Ч��
	BOOL	ExecuteSQL_Test( const CString & strSql , NDataModelDataSet *& pDataSet ) ;

	/// ѹ�����ݿ�
	BOOL	CompressDb() ; 

	/// 
	/// ��DataSet�󶨵�DbCtrl 
	/// �󶨺�DataSet����ִ�и������ݿ�
	/// 
	BOOL	BindRegDbCtrl( CSQLDataSet * pDataSet ) ;

	/// 
	BOOL	CreateTable( CSQLDataSet * pDataSet ) ;

	/// ��������
	void	StartTransaction() ; 

	/// �ύ����
	void	CommitTransaction() ; 

private:
	/// �Ѿ����������ݿ�
	BOOL	m_bConnect ; 
};

