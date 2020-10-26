// NDataModelDataSetBuffer.h: interface for the NDataModelDataSetBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDATAMODELDATASETBUFFER_H__E184EC8D_2313_494E_B03B_F23815889967__INCLUDED_)
#define AFX_NDATAMODELDATASETBUFFER_H__E184EC8D_2313_494E_B03B_F23815889967__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataFields.h"
#include "NDataModelBaseDataSet.h"
#include "NDataModelDataSet.h"
//���ڹ���data�еĻ���
//class NDataModelDataSet;

class DataSetAPI NDataModelDataSetBuffer  
{
public:
    friend NDataModelDataSet;

	/******************************************************************** 
	������������ȡDataSet���ڴ��С
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:9:26]   13:41
	*********************************************************************/ 
	virtual LONG GetBufferMemorySize();

//	inline  void * GetFieldDataPT(NDataField *field,int row, BOOL originalvalue = FALSE );//����AlloOrigBuff��ʶ�ڽ���ȡ��ָ��ʱ���û��Ϊԭֵ�����ڴ�����з��䣬������ǰֵ���Ƶ�ԭֵ��
//	inline  void * GetFieldDataPT(NDataField *field,void *buff, BOOL originalvalue = FALSE );
//	void SetFieldData(NDataField *field,int row,void * buffer);
//	BOOL GetFieldData(NDataField *Field,int row,void *buffer);
	int GetRowCount();
//	void QSort();
	BOOL SetSort(NDataFields sortfield);
	inline void SetFieldData(NDataField *field, int row ,void *buffer);
	inline BOOL GetFieldData(NDataField *Field,int row,  void *buffer);
	BOOL SetRow(int currentrow);
	NDataModelDataSetBuffer();
	virtual ~NDataModelDataSetBuffer();
 //   void test();
	//���ݴ洢
public:
	CPMPtrArray * GetBufferPtr();

	int MoveRow(int row, int beforRow);

	void * GetBuffPtr(int index) const;

	int InsertBuff(void *buff, int beforRowIndex);
	
	void Filter();
	BOOL Sort(int low, int high);
	void ResetSortFields();
	void AddSortField(NDataField * sortfield ,int order = 1);
	void ResetBuffers();
	void SetOwner(NDataModelDataSet *owner);
//	void RemoveBuff(int index);
	void AddBuff(void * buff);

	/*CString GetAsString(NDataField *field,int row, BOOL originalvalue = FALSE);
	double  GetAsFloat(NDataField *field,int row, BOOL originalvalue = FALSE);
	int     GetAsInteger(NDataField *field,int row, BOOL originalvalue = FALSE);
	VARIANT GetAsVariant(NDataField *field,int row, BOOL originalvalue = FALSE);
	COleDateTime  GetAsDateTime(NDataField *field,int row, BOOL originalvalue = FALSE);
	BOOL    GetAsBool(NDataField *field,int row, BOOL originalvalue = FALSE);

	BOOL    SetAsString(NDataField *field, int row ,CString& val);
	BOOL    SetAsFloat(NDataField *field, int row ,double& val);
	BOOL    SetAsInteger(NDataField *field, int row ,int& val);
	BOOL    SetAsVariant(NDataField *field, int row ,VARIANT& val);
	BOOL    SetAsDateTime(NDataField *field, int row ,COleDateTime& val);
	BOOL    SetAsBool(NDataField *field, int row ,BOOL & val);*/
	
protected:
	virtual int OnSortCompare(int low,int high);
    BOOL OnRowFilter(int row);

    char * InitFieldMem(NDataField *field);


	//�����б�
//	CPtrArray m_Buffers;
	CPMPtrArray m_Buffers;

	int m_CurrentRow;
	CPtrArray m_Sortdfields;
//	int m_SortColCount;
	CArray<int,int> m_SortColAscending;
	//�����Ժ�Ļ����б�  
	//CPtrArray m_SortedBuffers;
	//����
//	BOOL m_isGrouped;
//	CPtrArray m_Groupfields;
	CPtrArray m_GroupBuffers;
	NDataModelDataSet *m_owner;
private:
	BOOL QSort(int low, int high);
};

#endif // !defined(AFX_NDATAMODELDATASETBUFFER_H__E184EC8D_2313_494E_B03B_F23815889967__INCLUDED_)
