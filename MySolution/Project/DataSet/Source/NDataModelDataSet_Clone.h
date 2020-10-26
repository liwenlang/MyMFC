// NDataModelDataSet_Clone.h: interface for the NDataModelDataSet_Clone class.
// 本对象是NDataModelDataSet的克隆对象，主要用于存储主数据对象的副本，用户可以对副本进行
// 修改，最后确定是否需要将副本的修改饮用到主存储对象中
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDATAMODELDATASET_CLONE_H__C506D2C6_70C5_4A03_97C1_55DC20BC7DF3__INCLUDED_)
#define AFX_NDATAMODELDATASET_CLONE_H__C506D2C6_70C5_4A03_97C1_55DC20BC7DF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NDataModelDataSet.h"

/**
* @brief  NDataModelDataSet_Clone 是DataSet的克隆对象类，里面的数据完全独立。 
*			使用ApplyChanged可以进行DataSet间的数据同步
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
	功能描述：获取DataSet的内存大小
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:9:26]   13:41
	*********************************************************************/ 
	virtual LONG GetNDataMemorySize();

    friend NDataModelDataSet;
protected:
	
	virtual void DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff);

	CDWordArray m_delRowBufferPtArr;//这里保存Clone表删除的指针首地址，用于在AppleChange时，可以和总表进行同步。
									//强调：不可以做其他用途，这个指针是野指针，已经删除过

	NDataModelDataSet *m_pParentDataSet;
	//建立从克隆对象中的每一条记录到父对象的每一条记录的连接关系
	CMapPtrToPtr  m_MapCloneRecordToParent;
};

#endif // !defined(AFX_NDATAMODELDATASET_CLONE_H__C506D2C6_70C5_4A03_97C1_55DC20BC7DF3__INCLUDED_)
