// NDataModelDataSet_ShareData.h: interface for the NDataModelDataSet_ShareData class.
// ˵����Ŀǰ�Ĺ��ܽ����Ե�ǰ�����ݴ��ڴ���һ��һ���Ŀ��գ��벻Ҫ�ڹ������ݴ���������
//��¼��ӦΪ�������ݴ������ӵļ�¼����֪ͨ�ڸ�����
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDATAMODELDATASET_SHAREDATA_H__FB07C8B0_2631_4682_B6E7_9CF07B526E13__INCLUDED_)
#define AFX_NDATAMODELDATASET_SHAREDATA_H__FB07C8B0_2631_4682_B6E7_9CF07B526E13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataModelDataSet.h"

/**
* @brief NDataModelDataSet_ShareData ʵ����DataSet�������ݿ�����������ʽ��ͬ��DataSet���й���  
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class DataSetAPI NDataModelDataSet_ShareData : public NDataModelDataSet  
{
public:
	friend NDataModelDataSet;
	NDataModelDataSet_ShareData(NDataModelDataSet *pParentDataSet);
	virtual ~NDataModelDataSet_ShareData();

	virtual void OnParentDelete(NRecordID pRecord);

	//������DataSet ɾ��֪ͨ
	BOOL OnSpyMainData_HasFree();

//	virtual void DeleteRowInternal(int row,NDataModelDataSetBuffer *pBuff);

protected:
	virtual void OnParentReset();
	virtual void OnParentInsert(NRecordID pRecord);
    NDataModelDataSet *m_pParentDataSet;
};

#endif // !defined(AFX_NDATAMODELDATASET_SHAREDATA_H__FB07C8B0_2631_4682_B6E7_9CF07B526E13__INCLUDED_)
