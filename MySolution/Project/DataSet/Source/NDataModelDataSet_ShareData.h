// NDataModelDataSet_ShareData.h: interface for the NDataModelDataSet_ShareData class.
// 说明：目前的功能仅仅对当前的数据窗口创建一个一样的快照，请不要在共享数据窗口中增加
//记录，应为共享数据窗口增加的记录不会通知期父窗口
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDATAMODELDATASET_SHAREDATA_H__FB07C8B0_2631_4682_B6E7_9CF07B526E13__INCLUDED_)
#define AFX_NDATAMODELDATASET_SHAREDATA_H__FB07C8B0_2631_4682_B6E7_9CF07B526E13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataModelDataSet.h"

/**
* @brief NDataModelDataSet_ShareData 实现了DataSet的行数据可以与其他格式相同的DataSet进行共享  
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

	//接受主DataSet 删除通知
	BOOL OnSpyMainData_HasFree();

//	virtual void DeleteRowInternal(int row,NDataModelDataSetBuffer *pBuff);

protected:
	virtual void OnParentReset();
	virtual void OnParentInsert(NRecordID pRecord);
    NDataModelDataSet *m_pParentDataSet;
};

#endif // !defined(AFX_NDATAMODELDATASET_SHAREDATA_H__FB07C8B0_2631_4682_B6E7_9CF07B526E13__INCLUDED_)
