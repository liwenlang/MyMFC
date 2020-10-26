// NDataModelDataSet_Clone.h: interface for the NDataModelDataSet_Clone class.
// ��������NDataModelDataSet�Ŀ�¡������Ҫ���ڴ洢�����ݶ���ĸ������û����ԶԸ�������
// �޸ģ����ȷ���Ƿ���Ҫ���������޸����õ����洢������
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDATAMODELDATASET_CLONE_H__C506D2C6_70C5_4A03_97C1_55DC20BC7DF3__INCLUDED_)
#define AFX_NDATAMODELDATASET_CLONE_H__C506D2C6_70C5_4A03_97C1_55DC20BC7DF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NDataModelDataSet.h"

/**
* @brief  NDataModelDataSet_Clone ��DataSet�Ŀ�¡�����࣬�����������ȫ������ 
*			ʹ��ApplyChanged���Խ���DataSet�������ͬ��
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class DataSetAPI NDataModelDataSet_Clone : public NDataModelDataSet  
{
public:
	BOOL ApplyChanged();
	NDataModelDataSet_Clone(NDataModelDataSet *pParentDataSet);
	virtual ~NDataModelDataSet_Clone();

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

    friend NDataModelDataSet;
protected:
	
	virtual void DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff);

	CDWordArray m_delRowBufferPtArr;//���ﱣ��Clone��ɾ����ָ���׵�ַ��������AppleChangeʱ�����Ժ��ܱ����ͬ����
									//ǿ������������������;�����ָ����Ұָ�룬�Ѿ�ɾ����

	NDataModelDataSet *m_pParentDataSet;
	//�����ӿ�¡�����е�ÿһ����¼���������ÿһ����¼�����ӹ�ϵ
	CMapPtrToPtr  m_MapCloneRecordToParent;
};

#endif // !defined(AFX_NDATAMODELDATASET_CLONE_H__C506D2C6_70C5_4A03_97C1_55DC20BC7DF3__INCLUDED_)
