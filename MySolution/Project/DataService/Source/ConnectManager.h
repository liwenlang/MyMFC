#pragma once

#include "DataBaseAndTableName.h"
/**
* @brief   ���ݿ����ӻ���
*
*          
* @note  : û����������� ʵ��
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [26/6/2012]
*/
class DataServiceAPI CConnectBase : public CObject
{
public:

	DECLARE_DYNCREATE( CConnectBase )

	CConnectBase();
	virtual ~CConnectBase();

	/// 
	virtual void CheckTableIsLive()
	{

	}


	/**
	* @brief  �޸� ��ǰ���ӵ� λ�� 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/6/2012]
	*/
	CString GetFilePath() const;

	/**
	* @brief  ��ǰ���� �ļ��� λ��
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/6/2012]
	*/
	CString GetFileDir() const;

	
	void	SetIniPath( const CString & strIniPath ) ; 

	CString	GetIniPath( ) const ; 

	/**
	* @brief  ���ڵ�ǰ���ӵ� ���ü��� 
	*			�������ü���
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/6/2012]
	*/
	bool AddUseCount();

	/**
	* @brief ���ڵ�ǰ���ӵ� ���ü��� 
	*			�������ü���
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/6/2012]
	*/
	bool SubUseCount();

	/**
	* @brief   ��������Դ
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/6/2012]
	*/
	virtual bool ConnectBase(const CString & strFilePath)
	{
		return false;
	}

	/**
	* @brief  �ͷ�����Դ 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/6/2012]
	*/
	virtual bool DisconnectBase()
	{
		return false;
	}

	/**
	* @brief  �Ƿ������ ���� 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [4/8/2012]
	*/
	BOOL CheckHasBakDone()
	{
		return m_bHasBak;
	}

	/**
	* @brief ���豸�ݱ��  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [4/8/2012]
	*/
	void HasBakDone()
	{
		m_bHasBak = TRUE;
	}

protected:

	BOOL m_bHasBak;			//�Ƿ��н��й�����
	
	CString m_strFilePath; //Ҫ���ӵ����ݿ� λ��

	/// 
	CString	m_strIniPath ; /// �����ļ��� ·�� *.pmsl , ֻ��projectʱ�������壬

	int m_iConnectUseCount;//��ǰ���ݵ� ���Ӵ���
};

/**
* @brief  ���ݿ����ӵ� sqlite�� ʵ�֡� 
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [26/6/2012]
*/ 
class DataServiceAPI CConnect_sqlite : public CConnectBase
{
public:

	DECLARE_DYNCREATE( CConnect_sqlite )

	CConnect_sqlite();
	virtual ~CConnect_sqlite();

	virtual void CheckTableIsLive()
	{

	}

	CPMSQLBaseCtrl * GetSQLBaseCtrl()
	{
		return m_pSQLBaseCtrl;
	}

	/**
	* @brief   ��������Դ
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/6/2012]
	*/
	virtual bool ConnectBase(const CString & strFilePath);

	/**
	* @brief  �ͷ�����Դ 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/6/2012]
	*/
	virtual bool DisconnectBase();

	/**
	* @brief   ������չ����Դ
	*
	*          
	* @note  : ����·���������ڲ�����
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/6/2012]
	*/
	virtual bool ConnectExternBase()
	{
		return true;
	}

	/**
	* @brief  �ͷ�����Դ 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/6/2012]
	*/
	virtual bool DisconnectExternBase()
	{
		return true;
	}

	/// 
	bool BackUpDataBase( const CString & strToFileName ) ;

	/**
	* @brief ��pTemDataSet�У�����SQL���ƶ���
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [13/7/2012]
	*/
	BOOL RegSQLCtrl(CSQLDataSet * pTemDataSet);

	/**
	* @brief  ����SQL��� 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [10/7/2012]
	*/
	static bool BuildSQL(CPMSQLBaseCtrl::CSQLType sqlType,CSQLDataSet * pDataSet,PmTString & sNewSQL);

	/**
	* @brief  ������ṹ 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [10/7/2012]
	*/
	bool DoCreateSQLTable(CSQLDataSet * pTableDataSet);

	/**
	* @brief ��ȡ DataSet,���յ�ǰDataSet�Ľṹ������װ�� 
	*
	*          
	* @note  : �����Ҫ���棬ִ�� DataSet.UpdateSQL()����
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [8/6/2012]
	*/
	bool GetDataSet(CSQLDataSet * pDataSet);

	/**
	* @brief  ִ��sql ��װ��DataSet 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [28/6/2012]
	*/
	bool GetDataSet(LPCTSTR strSQL,CSQLDataSet * pDataSet);

	/**
	* @brief  ��ȡ ִ��sql֮��� ����� 
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
	* @brief  ͨ���Զ��ϳɵ� sql ��ȡ ����� 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [28/6/2012]
	*/
	bool GetSQLTableCtrl(CStringArray & strSelectArr,CStringArray & strWhereKeyArr,CStringArray & strWhereValueArr,const CString & strTableName,CPMSQLTableCtrl *& pTableCtrl);

	/**
	* @brief ִ��SQL ���  
	*
	*          
	* @note  : ��Ϊ DataSet �����Ψһ ִ�нӿ�
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [8/6/2012]
	*/
	bool ExecuteSQL(LPCTSTR sSQL);

	/**
	* @brief  ��ȡָ����� ���ֵ 
	*
	*          
	* @note  : ����ID�� ����
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [11/7/2012]
	*/
	bool GetTable_ColMaxValue(const CString & strColName,const CString & strTableName,int & iMaxID);

	bool GetTable_ColMaxValue(const CString & strColName,const CString & strTableName, CString & strMaxID);

	/**
	* @brief   ��ȡָ����� ������
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [2012-7-23]
	*/
	bool GetTable_RowCount( const CString & strTableName,int & iRowCount ) ;

	BOOL  IsTableExsit( const CString & strTableName ) ;

	/**
	*  @brief : �������ݿ⵽��ǰ���ݿ�
	*
	*
	*  @note
	*  @param : 
	*  @defenc: 
	*  @return: 
	*  @author: yjc
	*  @date  : 2015/7/31 9:27
	*/
	int Attach(const CString& strtDBPath,const CString& strDBName);

	/**
	*  @brief : ���븽�����ݿ�
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

protected:

	CPMSQLBaseCtrl * m_pSQLBaseCtrl;
};

class DataServiceAPI CConnectManager
{
public:
	
	static CConnectManager * Instance()
	{
		if(NULL == m_pInstance)
		{
			m_pInstance = PMNew(CConnectManager);
		}
		return m_pInstance;
	}

	static void Release()
	{
		if(NULL != m_pInstance)
		{
			PMDelete(m_pInstance);
			m_pInstance = NULL;
		}
	}

public:

	//ע�������
	void RegisterConnectBase(CPmDataBase::DataBaseName dataBaseName,CRuntimeClass *pRuntimeClass);

	//��ȡ����
	CConnect_sqlite * GetConnectCtrl(CPmDataBase::DataBaseName dataBaseName);

	CConnectBase * GetConnectCtrl_Base(CPmDataBase::DataBaseName dataBaseName);

	//��������
	bool ConnectDataBase(CPmDataBase::DataBaseName dataBaseName,const CString & strFilePath);

	//��������
	bool ConnectDataBase( CPmDataBase::DataBaseName dataBaseName, CConnectBase * pNewConnect ,const CString & strFilePath);

	//�Ͽ�����
	bool DisConnectDataBase( CPmDataBase::DataBaseName dataBaseName);

	/// 
	bool	BackUpDataBase( CPmDataBase::DataBaseName dataBaseName,const CString & strToFilePath ) ;

	/// ������ݿ��������
	bool	CkeckProjectIntegrity( ) ;

	bool	ConnectSysDataDataBase();
	/// 
	bool	CkeckDataBaseIntegrity( CPmDataBase::DataBaseName dataBaseName , const CString & strFilePath ) ;

	/**
	* @brief  �޸� ��ǰ���ӵ� λ�� 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/6/2012]
	*/
	CString GetFilePath( const CPmDataBase::DataBaseName dataBaseName ) ;

	/**
	* @brief  ��ǰ���� �ļ��� λ��
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/6/2012]
	*/
	CString GetFileDir( const CPmDataBase::DataBaseName dataBaseName ) ;

	CString	GetIniPath( const CPmDataBase::DataBaseName dataBaseName ) ;

	void	SetIniPath( const CPmDataBase::DataBaseName dataBaseName , const CString & strIniPath ) ;

protected:

	CConnectManager(void);
	virtual ~CConnectManager(void);

	static CConnectManager * m_pInstance;

	typedef std::map<int,CConnectBase*> CConnectBaseCtrlMap_T;
	CConnectBaseCtrlMap_T m_ConnenctBaseMap;

	typedef std::map<int,CRuntimeClass*> CConnectBaseFactoryMap_T;
	CConnectBaseFactoryMap_T m_ConnenctBaseFactoryMap;
};

class DataServiceAPI CProjectConnectTransaction
{
public:
	CProjectConnectTransaction()
	{
		CConnect_sqlite * pConnectSqlite = CConnectManager::Instance()->GetConnectCtrl(CPmDataBase::Project);
		if(NULL == pConnectSqlite)
		{
			m_pSQLTransaction = NULL;
			return;
		}
		m_pSQLTransaction = PMNew_Parm(CPMSQLBaseCtrl::CSQLTransaction,(pConnectSqlite->GetSQLBaseCtrl()));

	}

	virtual ~CProjectConnectTransaction()
	{
		if(NULL != m_pSQLTransaction)
		{
			PMDelete(m_pSQLTransaction);
		}
	}

	BOOL Disable(bool disable)
	{
		if(true == disable)
		{
			PMDelete(m_pSQLTransaction);
		}
		else
		{
			if(m_pSQLTransaction)
				PMDelete(m_pSQLTransaction);
			CConnect_sqlite * pConnectSqlite = CConnectManager::Instance()->GetConnectCtrl(CPmDataBase::Project);
			if(NULL == pConnectSqlite)
			{
				m_pSQLTransaction = NULL;
				return FALSE;
			}
			m_pSQLTransaction = PMNew_Parm(CPMSQLBaseCtrl::CSQLTransaction,(pConnectSqlite->GetSQLBaseCtrl()));
		}
		return TRUE;
	}

protected:

	CPMSQLBaseCtrl::CSQLTransaction * m_pSQLTransaction;
};


/**
* @brief	��ʱ���ݿ�����ӣ�û�����������ʵ��
*
*          
* @note  :	
* @param :	
* @defenc:	
* @return:	
* @author:	liwenlang
* @date  :	
*/
class DataServiceAPI CTempDataBaseConnectOpr
{
public:
	CTempDataBaseConnectOpr();
	virtual ~CTempDataBaseConnectOpr();

	//����
	virtual BOOL ConnectDataBase(const CString& strFilePath);

	//�Ͽ�
	virtual void DisConnect();

	//��ȡ�ļ�λ��
	CString GetDataBasePath();

	//��ȡ����
	CConnectBase* GetConnectCtrl();

protected:
	CConnectBase* m_pConnectCtrl;
};

/**
* @brief	��ʱ���ݿ��sqlite����
*
*          
* @note  :	
* @param :	
* @defenc:	
* @return:	
* @author:	liwenlang
* @date  :	
*/
class DataServiceAPI CTempSqliteConnect : public CTempDataBaseConnectOpr
{
public:
	CTempSqliteConnect();
	virtual ~CTempSqliteConnect();

	//����
	virtual BOOL ConnectDataBase(const CString& strFilePath);

	//�Ͽ�
	virtual void DisConnect();
};