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
* @brief ��Ϊ SQL���ݿ�� ���ӳ�  
*
*          
* @note  : ����࣬һ��ֻ���� һ��SQLite�ļ����ڲ����Զ� һ���ļ��� �����в�ѯ
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

	enum CSQLType//SQL ���� ����
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

		// ����ع�
		bool RollBack();

	protected:
		
		CPMSQLBaseCtrl * m_pSqlBaseCtrl;
	};

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
	static bool BuildSQL(CSQLType sqlType,CSQLDataSet * pDataSet,PmTSQLString & sNewSQL);

	/**
	* @brief  ���� SQL ���ݿ� 
	*
	*          
	* @note  : ִ�� ������� SQL�� ���ݱ��û�� ����
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [8/6/2012]
	*/
	bool ConnectSQLDataBase(const CString & strFilePath);

	/**
	* @brief  �Ͽ�SQL ���ݿ� 
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
	* @brief ִ��SQL��� ��ȡ DataSet 
	*
	*          
	* @note  : �����Ҫ���棬ִ�� DataSet.UpdateSQL()����
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [8/6/2012]
	*/
	bool GetDataSet(LPCTSTR strSQL,CSQLDataSet * pDataSet);

	/**
	* @brief   ��ӵ�һ���м亯��, �Ӽ�¼��ת��DataSet
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
	* @brief ��ȡ ����һ������  
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

	//���� ��������������� ʹ��
	BOOL AddSQLTransationCount();
	void SubSQLTransationCount();

protected:
	
	LPVOID m_pSQlite3;//SQL�Ŀ���ָ��

	CString m_strDBFilePath;//���ӵ� SQL�ļ�λ��

	int		m_iSQLTransationCount;//��ǰ�������
//	CSQLTableInfoArr m_SQLTableInfoArr;//SQL ����Ϣ ����
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
	* @brief  �Ƶ������һ�� 
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
	* @brief ��ȡָ��λ�õ� ������  
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
	//���� ������ʼ
	//ֻ���� ִ�� Insert Update���ʱ ����Ч����Sql����е� �ʺţ����ֽ������ݰ�
	//��Ҫ���ڸ��� Blob����
	BOOL BindAsInterger(int iCol,int iValue);
	BOOL BindAsDouble(int iCol,double dValue);
	BOOL BindAsString(int iCol,const CString & strValue);
	BOOL BindAsBlob( const int iCol,const LPVOID & pBuffer,const long lBufferSize);

	

	/**
	* @brief   ��װ������DataSet�İ󶨡� ����ʵ�ֵ�ʱ����һ���ֶ�һ���ֶ�ȥ����
	*
	*          
	* @note  : ע�� aryNotBindCol �Ǽ�¼���е��±�
	* @param : aryNotBindCol ����Ҫ�󶨵��ֶ�
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [2012-7-19]
	*/
	BOOL BindDataSet( NDataModelDataSet * pDataSet , const CStringArray & aryNotBindCol ) ; 
	BOOL BindDataSet( NDataModelDataSet * pDataSet , const CUIntArray & aryNotBindCol ) ;
	 ; 
	/**
	* @brief  ���󶨵������ύ�� ���ݿ� 
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
	* @brief  ����ִ��sql��� 
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
	* @brief   ���ݼ�¼����ȡDataSet
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
	* @brief  �������ţ���������ʾ 
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
	* @brief   �ַ����Ķ�ȡ��Ҫ���⴦�� /�����⹫�� 
	*
	*          
	* @note  : sqlite3_column_text16 ���ص���UTF16 �ı���
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [2012-7-27]
	*/
	BOOL	GetColAsString( const int iCol,CString & strValue , sqlite3_stmt * pStmt = NULL ) ;

	/**
	* @brief  ���Դ������� map 
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
	sqlite3_stmt * m_pStmt;//��󶨴�����Ϣ
	int m_ParmID;//��ǰ��� ���ڴ�����

	CMapStringToPtr m_MapNameField;//�����ƶ��У�������Ҫ��ʱ���������ݣ�value�ж�Ӧ�� ���������б��
};
#endif // !defined(AFX_PMSQLBASECTRL_H__5B8A1036_5F31_4D1A_B4DD_785B28F9D987__INCLUDED_)
