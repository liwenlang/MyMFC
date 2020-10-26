// NDataModelDataSet_Clone.cpp: implementation of the NDataModelDataSet_Clone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NDataModelDataSet_Clone.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NDataModelDataSet_Clone::NDataModelDataSet_Clone(NDataModelDataSet *pParentDataSet)
{
   m_pParentDataSet = pParentDataSet;
}

NDataModelDataSet_Clone::~NDataModelDataSet_Clone()
{

}

BOOL NDataModelDataSet_Clone::ApplyChanged()
{
	if (m_pParentDataSet == NULL) 
	{
		return FALSE;
	}

	//��ʼ���м����������
	int insertrow;
	NRecordID recordID,OrgrecordID;
	NRecordID  newrecordid;
	NDataField *field;


	/// ��ɾ���ƶ���ǰ��
	int i = 0;

	for (i = 0;i < m_delRowBufferPtArr.GetSize();i++)
	{
		NRecordID pTemRecordID = (NRecordID)m_delRowBufferPtArr.GetAt(i);
		if(m_MapCloneRecordToParent.Lookup(pTemRecordID,OrgrecordID) != 0)
		{
			m_pParentDataSet->DeleteRow(OrgrecordID);
		}
		/// ����Ҫ����map��ά��
		m_MapCloneRecordToParent.RemoveKey(pTemRecordID) ; 
	}

	m_delRowBufferPtArr.RemoveAll();

	
	for(i = 0 ; i < m_PrimaryBuffers->GetRowCount(); i++)
	{
		if (GetRowNew(i + 1)) 
		{
			insertrow = m_pParentDataSet->InsertRow(0);
			newrecordid = m_pParentDataSet->GetRecordID(insertrow);
			recordID = GetRecordID(i + 1);
			if (insertrow >0) 
			{
				for(int j = 0 ; j < m_fields.GetCount(); j++)
				{
					field = m_fields.GetField(j);
				    m_pParentDataSet->SetAsString(field->GetFieldName(),insertrow,GetAsString(field->GetFieldName(),i + 1));
				}
				m_MapCloneRecordToParent.SetAt(recordID,newrecordid);
			}
			continue;
		}
	    if(GetRowModified(i + 1))
		{ 
		       recordID = m_PrimaryBuffers->GetBuffPtr(i);
			   if(m_MapCloneRecordToParent.Lookup(recordID,OrgrecordID) != 0)
			   {
				   //��ʼ���и�ֵ
					   for(int j = 0 ; j < m_fields.GetCount(); j++)
					   {
						 if(!GetItemModified(i + 1, j)) continue;	//����δ�޸�
						 field = m_fields.GetField(j);
						 m_pParentDataSet->SetAsString(m_pParentDataSet->GetColNum(field->GetFieldName()),OrgrecordID,GetAsString(field->GetFieldName(),i + 1));
					   }
				   
			   }
	   }
	}
	/********************************************************************
	liwenlang
	ʱ��:2010:12:16   9:37
	��ע:ɾ��û�б��浽 Buffer��
	*********************************************************************/
	//��ʼ���ɾ����
// 	for(i = 0; i < DeletedCount(); i++) 
// 	{
// 		
// 		if(GetRowNew(i + 1,Delete)) continue;
// 		recordID = m_DeleteBuffers->GetBuffPtr(i);
// 		if(m_MapCloneRecordToParent.Lookup(recordID,OrgrecordID) != 0)
// 		{
// 			m_pParentDataSet->DeleteRow(OrgrecordID);
// 		}
// 	}
	
	
	ResetUpdate();
	return TRUE;
}

LONG NDataModelDataSet_Clone::GetNDataMemorySize()
{
	LONG lMemorySize = sizeof(*this);
	lMemorySize -= sizeof(*(NDataModelDataSet*)this);
	lMemorySize += NDataModelDataSet::GetNDataMemorySize();
	return lMemorySize;
}

void NDataModelDataSet_Clone::DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff)
{
	if (row < 0 || row > pBuff->GetRowCount()) 
	{
		return;
	}
	if (row == 0) 
	{
		row = pBuff->GetRowCount();
	}
	void * buff = pBuff->GetBuffPtr(row - 1);
	m_delRowBufferPtArr.Add((DWORD)buff);
	
	NDataModelDataSet::DeleteRowInternal(row,pBuff);
}
