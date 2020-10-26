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
	//注意共享的数据存储对象不再负责数据的释放空间的责任
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
		时间:2010:12:13   20:47
		备注:由于现在没有 把信息放在DeleteBuffer中，而是直接删除，所以映像表只要移除指针
	*********************************************************************/
	//   DeleteRow(pRecord);
	int row = GetRowOf(pRecord);
	if (row > 0) 
	{
//		DeleteRow(row);
		m_PrimaryBuffers->GetBufferPtr()->RemoveAt(row - 1);
	}
	else//在主存中找不到在filter中查找
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
