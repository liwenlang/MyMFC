// NDataFields.h: interface for the NDataColumns class.
// �޸���ʷ�������˶Դ����ݵ�֧�� 
//           �޸ĺ��� CreateField
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDATACOLUMNS_H__31D78D3E_8162_43F5_AB12_8E35E2D13288__INCLUDED_)
#define AFX_NDATACOLUMNS_H__31D78D3E_8162_43F5_AB12_8E35E2D13288__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "NDataModelDataSet.h"
#include "NDataField.h"


class NDataModelDataSet;
class CPM_SharebufferDataSet;

class DataSetAPI NDataFields : public CObject  
{
public:
/*	typedef  struct tagFieldInfo 
	{
		BYTE      Status;
		void      *OrgValuePointer;
	} FieldInfo;*/

public:

	BOOL HasBlobField() const{return m_HasBlob;}

	const CPtrArray * GetBlobFieldArr() const{return & m_Bloblist;}

	/******************************************************************** 
	������������ȡDataSet���ڴ��С
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:9:26]   13:41
	*********************************************************************/ 
	virtual LONG GetFieldsMemorySize();

	friend NDataModelDataSet;
	friend CPM_SharebufferDataSet;
	
	int GetFieldInfoSize( );
	int GetFieldIndex(const CString & fieldName) const;

	int GetRecordSize();
	inline int GetFieldOffset(NDataField *field) const
	{
		return m_FieldOffset.GetAt(field->GetFieldNo() -1);
	}

	void Clear();
	BOOL CreateField(void *pOwner,CString name,FieldType fieldtype,int size);

	inline NDataField *GetField(int index) const
	{
		if(index <= -1)
			return NULL;
		return (NDataField *)m_list.GetAt(index);
	}

	inline NDataField *GetField(const CString & sColname) const;
	
	void SetField(int Index ,NDataField *Field);

	int GetCount() const;

	NDataFields();
	virtual ~NDataFields();
	void RefreshFieldOffset();

protected:
	CPtrArray m_list;
	//NDataModelDataSet *m_dataset;
    //�洢���ֶ��ڻ����е�ƫ����
    //int m_BufferOffset;
    CArray<int,int&> m_FieldOffset;
	CMapStringToPtr m_MapNameField;
	//Blob ֧��
	CPtrArray       m_Bloblist;
	BOOL            m_HasBlob;
};

#endif // !defined(AFX_NDATACOLUMNS_H__31D78D3E_8162_43F5_AB12_8E35E2D13288__INCLUDED_)
