// NDataModelDataSet_ShareData.cpp: implementation of the NDataModelDataSet_ShareData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NDataModelDataSet_ShareData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NDataModelDataSet_ShareData::NDataModelDataSet_ShareData(NDataModelDataSet *pParentDataSet)
{
   m_pParentDataSet = pParentDataSet;
}

NDataModelDataSet_ShareData::~NDataModelDataSet_ShareData()
{
	//ע�⹲������ݴ洢�����ٸ������ݵ��ͷſռ������
   CPMPtrArray* pPreArray;
   pPreArray = m_PrimaryBuffers->GetBufferPtr();
   pPreArray->RemoveAll();
   pPreArray = m_DeleteBuffers->GetBufferPtr();
   pPreArray->RemoveAll();
   pPreArray = m_FilterBuffers->GetBufferPtr();
   pPreArray->RemoveAll();
   //m_DeleteBuffers;
   //m_FilterBuffers;
   if (m_pParentDataSet !=NULL) 
   {
	   m_pParentDataSet->OnSpyShareDataFree(this);
   }
}

BOOL NDataModelDataSet_ShareData::OnSpyMainData_HasFree()
{
	CPMPtrArray* pPreArray;
	pPreArray = m_PrimaryBuffers->GetBufferPtr();
	pPreArray->RemoveAll();
	pPreArray = m_DeleteBuffers->GetBufferPtr();
	pPreArray->RemoveAll();
	pPreArray = m_FilterBuffers->GetBufferPtr();
	pPreArray->RemoveAll();

	m_pParentDataSet = NULL;
	return TRUE;
}

void NDataModelDataSet_ShareData::OnParentInsert(NRecordID pRecord)
{
    m_PrimaryBuffers->InsertBuff(pRecord,-1);
}

void NDataModelDataSet_ShareData::OnParentDelete(NRecordID pRecord)
{
	/********************************************************************
	liwenlang
		ʱ��:2010:12:13   20:47
		��ע:��������û�� ����Ϣ����DeleteBuffer�У�����ֱ��ɾ��������ӳ���ֻҪ�Ƴ�ָ��
	*********************************************************************/
	//   DeleteRow(pRecord);
	int row = GetRowOf(pRecord);
	if (row > 0) 
	{
//		DeleteRow(row);
		m_PrimaryBuffers->GetBufferPtr()->RemoveAt(row - 1);
	}
	else//���������Ҳ�����filter�в���
	{
		row = GetRowOf(pRecord,Filter);
		if (row > 0) 
		{
			m_FilterBuffers->GetBufferPtr()->RemoveAt(row - 1);
//			DeleteRowInternal(row,m_FilterBuffers);
		}
	}
}

void NDataModelDataSet_ShareData::OnParentReset()
{
   CPMPtrArray* pPreArray;
   pPreArray = m_PrimaryBuffers->GetBufferPtr();
   pPreArray->RemoveAll();
   pPreArray = m_DeleteBuffers->GetBufferPtr();
   pPreArray->RemoveAll();
   pPreArray = m_FilterBuffers->GetBufferPtr();
   pPreArray->RemoveAll();  
}
