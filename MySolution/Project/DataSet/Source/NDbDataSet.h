// NDbDataSet.h: interface for the NDbDataSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDBDATASET_H__6EFE5837_616F_4724_A895_895580A0B42C__INCLUDED_)
#define AFX_NDBDATASET_H__6EFE5837_616F_4724_A895_895580A0B42C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataModelDataSet.h"
//#include "..\comdb\DataServiceAbsolute.h"

#include "comdb\IDataFill.h"


#include "comdb\DataServiceAbsolute.h"

class CDataServiceAbsolute;

/**
* @brief  NDbDataSet 是对ACCESS数据库保存控制的封装   
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class DataSetAPI NDbDataSet : public NDataModelDataSet 
{
public:
	DECLARE_DYNCREATE(NDbDataSet)
		
	NDbDataSet();
	virtual ~NDbDataSet();

public:
	int FillData();
	void SetComDB(CDataServiceAbsolute *pComDB);
	CDataServiceAbsolute* GetComDB() { return m_pDataService; }
	
	void SetFieldParseSQLAble(int col,BOOL b = TRUE);
	BOOL GetRetrieved();
	int  RetrieveByXml();
	int  Update();

	void SetWhere(CString sWhere,BOOL bReplaceWhere = FALSE);
	CString GetSQL();
	//根据dataset的结构解析sql语句
	virtual BOOL ParseSQL();
	void SetSQL(CString sSQL);
	int Retrieve();

//实现填充的接口
	class CDataFill : public IDataFill
    {
    public:
		CDataFill()
		{
		}
	    ~CDataFill()
		{
		}
      	virtual BOOL SetValue(int colIndex,int rowIndex,CString &columnName,VARIANT &value);
	    virtual int  AddRow();

		BOOL SetBlobValue(int colIndex,int rowIndex,VARIANT &value);
    } m_xDataFill;
protected:
	//用于更新大字段的更新
	void UpdateBlobField();
	virtual void DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff);


	CString m_sql;
	CDataServiceAbsolute *m_pDataService;
	BOOL    m_isRetrieved;
};

#endif // !defined(AFX_NDBDATASET_H__6EFE5837_616F_4724_A895_895580A0B42C__INCLUDED_)
