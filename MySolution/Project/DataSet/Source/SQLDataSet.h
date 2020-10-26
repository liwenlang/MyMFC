// SQLDataSet.h: interface for the CSQLDataSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLDATASET_H__B1AE734E_5E51_4D03_A3C4_9134EFA0553F__INCLUDED_)
#define AFX_SQLDATASET_H__B1AE734E_5E51_4D03_A3C4_9134EFA0553F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataModelDataSet.h"

#ifdef  UNICODE                    
#	ifndef _PM_SQLSTRING_DEFINED
		/// 		
		typedef	std::wstring	PmTSQLString;
		#define _PM_SQLSTRING_DEFINED
#	endif
#else
#	ifndef _PM_SQLSTRING_DEFINED
		typedef	std::string		PmTSQLString ;
		#define _PM_SQLSTRING_DEFINED
#	endif /* !_TCHAR_DEFINED */
#endif /*UNICODE*/

class CPMSQLBaseCtrl;

/**
* @brief  SQL���ݱ�  ,��װSQL��ȡ������ӿ�
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [8/6/2012]
*/
class DataSetAPI CSQLDataSet : public NDataModelDataSet
{
public:
	DECLARE_DYNCREATE(CSQLDataSet)

	CSQLDataSet();
	CSQLDataSet(CPMSQLBaseCtrl * pBaseCtrl);
	virtual ~CSQLDataSet();

	void SetSqlBaseCtrl(CPMSQLBaseCtrl * pBaseCtrl);

	bool DoSqlUpdate();
	
	/**
	* @brief  ����������ͳһ�ӿ� 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/9/2012]
	*/
	virtual BOOL GetCreateIndexSQL(CStringArray & strFieldNameArr,CString & strIndexName);
	
	/**
	* @brief  ����sql��� select�е� where ������Ϣ 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [6/7/2012]
	*/
	void SetWhere(const CString & strWhere)
	{
		m_strWhere = strWhere;
	}

	CString GetWhere(){return m_strWhere;}

protected:
	
	/**
	* @brief ִ��ɾ�� ����  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [21/6/2012]
	*/
	bool DoDeleteActive(CUIntArray & arrPKCol);

	bool DoInsertActive();

	bool DoMdfActive(CUIntArray & arrPKCol);

	void StringFormat( PmTSQLString & str , const TCHAR *_format, ... ) ;

	virtual void DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff);

	CPMSQLBaseCtrl * m_pBaseCtrl;

	CString m_strWhere;//Sql�Ĺ��˲���
};

#endif // !defined(AFX_SQLDATASET_H__B1AE734E_5E51_4D03_A3C4_9134EFA0553F__INCLUDED_)
