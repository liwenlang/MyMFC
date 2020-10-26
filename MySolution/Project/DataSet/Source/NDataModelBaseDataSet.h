// NDataModelBaseDataSet.h: interface for the NDataModelBaseDataSet class.
// �޸���ʷ ���Ӷ�Blob�������ݵ�֧��
//          �޸���AddField����
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDATAMODELBASEDATASET_H__043E7E1D_509B_4197_8230_761E22939A2A__INCLUDED_)
#define AFX_NDATAMODELBASEDATASET_H__043E7E1D_509B_4197_8230_761E22939A2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataModelBase.h"
#include "NDataField.h"
#include "NDataFields.h"
//#include "..\actx\ACTX\COleDb\COleDb.h"


class NDataField;

/**
* @brief DataSet�Ļ��࣬����������Field����  
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class DataSetAPI NDataModelBaseDataSet : public NDataModelBase  
{
public:
	NDataModelBaseDataSet();
	virtual ~NDataModelBaseDataSet();

public:

	/******************************************************************** 
	������������ȡDataSet���ڴ��С
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:9:26]   13:41
	*********************************************************************/ 
	virtual LONG GetNDataMemorySize();
	
	virtual CString GetFieldName(int index) const ;
	virtual void FillFilterBuff(CPtrArray &filterBuffer);
	int GetFieldCount() const;
	virtual int GetFieldIndex(const CString & sFieldName) const;
	virtual NDataField * GetField(int index) const;
	BOOL AddField(CString sFieldName, FieldType fieldtype, int size);
	virtual void InteralOpen() = 0;
	NDataFields * GetFieldsObj();
	virtual int GetFieldOffset(NDataField * field) const = 0 ;
    virtual BOOL OnRowFilter(int row);

	virtual void GetAllField(CArray<NDataField*,NDataField*> & dataFields);

public:
	void Error(CString &Error);
	CString GetTableName() const;
	void SetTableName(CString stablename);
	//COleDb *m_pOleDb;
protected:
	   NDataFields m_fields;
	   CString m_TableName;
};

#endif // !defined(AFX_NDATAMODELBASEDATASET_H__043E7E1D_509B_4197_8230_761E22939A2A__INCLUDED_)
