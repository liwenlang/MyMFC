// PM_LinkDataSet.h: interface for the CPM_LinkDataSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PM_LINKDATASET_H__31B9F955_08BC_442A_9780_04EE6A334BE8__INCLUDED_)
#define AFX_PM_LINKDATASET_H__31B9F955_08BC_442A_9780_04EE6A334BE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataModelDataSet.h"

/******************************************************************** 
功能描述：做为超链 DataSet 将一个DataSet的内容 用地址指针方式 保存在这个DataSet中
	在获取数据时 进行翻译，使DataSet控制 一体化（跟一个DataSet一样）
参数：
返 回 值：
算法：
注意事项：现在是静态 超链，在链接DataSet 增加行，删除行时，这个DataSet不会反应。
	而且 很危险，为尽快实现，将这个问题，暂不考虑 由外部逻辑 维护。

	链接一定要在，这个DataSet没有插入数据之前 进行。
		链接DataSet，在这个DataSet没有删除前不能删除。
作    者：liwenlang
时    间：[2010:11:23]   11:01
*********************************************************************/ 
//class DataSetAPI CPM_LinkDataSet : public NDataModelDataSet
//{
//public:
//
//	class CNDataSet2FieldIndex
//	{
//	public:
//		CNDataSet2FieldIndex(){}
//		virtual ~CNDataSet2FieldIndex(){}
//
//		BOOL AddFieldIndex(NDataModelDataSet * pDataSet,int iFieldIndex)
//		{
//			int iDataSetSize = (int)m_pDataSetArr.GetSize();
//			for (int i = 0;i < iDataSetSize;i++)
//			{
//				if(pDataSet == m_pDataSetArr.GetAt(i))
//				{
//					return FALSE;
//				}
//			}
//			m_pDataSetArr.Add(pDataSet);
//			m_iFieldIndexArr.Add(iFieldIndex);
//			return TRUE;
//		}
//
//		BOOL GetFieldIndex(NDataModelDataSet * pDataSet,int & iFieldIndex)
//		{
//			int iDataSetSize = (int)m_pDataSetArr.GetSize();
//			for (int i = 0;i < iDataSetSize;i++)
//			{
//				if(pDataSet == m_pDataSetArr.GetAt(i))
//				{
//					iFieldIndex = m_iFieldIndexArr.GetAt(i);
//					return TRUE;
//				}
//			}
//			return FALSE;
//		}
//
//	protected:
//		
//		CArray<NDataModelDataSet *,NDataModelDataSet*&> m_pDataSetArr;
//		CArray<int,int&> m_iFieldIndexArr;
//	};
//
//	class CNDataSetFieldIndexArr
//	{
//	public:
//
//		CNDataSetFieldIndexArr(NDataModelDataSet * pDataSet);
//
//		virtual ~CNDataSetFieldIndexArr();
//		
//		BOOL AddDataFieldIndex(NDataField * pField,NDataModelDataSet * pDataSet,int i2FieldIndex);
//
//		BOOL AddDataFieldIndex(NDataModelDataSet * pDataSet);
//
//		BOOL FindDataFieldIndex(NDataField * pField,NDataModelDataSet * pDataSet,int & i2FieldIndex);
//
//	protected:
//		
//		typedef std::map<int,CNDataSet2FieldIndex*> FieldMapFieldIndex_T;
//		FieldMapFieldIndex_T m_NDataSet2FieldIndexMap;
//		
//	};
//	enum LinkFieldMode
//	{
//		LinkMainField = 3,
//		LinkDataField = 0
//	};
//
//	void LinkDataSet(NDataModelDataSet * pLinkDataSet);
//	void DisLinkDataSet();
//
//	DECLARE_DYNCREATE(CPM_LinkDataSet) 
//
//	CPM_LinkDataSet();
//	virtual ~CPM_LinkDataSet();
//
//	virtual BOOL BuildItemXML(CString tableName,int row,NRecordID recordID,CString &sbuff,CString &sXMLBuf,CString &sXMLBefore);
//
//	virtual int GetFieldIndex(const CString & sFieldName);
//	virtual NDataField * GetField(int index);
//	virtual CString GetFieldName(int index);
//
//	//使用FieldIndex快速查找
//	virtual BOOL    SetAsString(int Column, int row ,const CString& val);
//	virtual BOOL    SetAsFloat(int Column, int row ,const double& val);
//	virtual BOOL    SetAsInteger(int Column, int row ,const int& val);
//	virtual BOOL    SetAsBool(int Column, int row ,const BOOL & val);
//
//	virtual CString GetAsString(int Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE);
//	virtual double  GetAsFloat(int Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE);
//	virtual int     GetAsInteger(int Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE);
//	virtual BOOL    GetAsBool(int Col,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE);
//
//	virtual long GetBlob(int iColumn, int row ,void *& ppBolb,eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE);
//	virtual BOOL SetBlob(int Column, int row ,void * ppBolb,long iSize);
//
//	//NRecordID 
//	virtual CString GetAsString(int Column,NRecordID id, BOOL originalvalue = FALSE);
//	virtual double  GetAsFloat(int Column,NRecordID id, BOOL originalvalue = FALSE);
//	virtual int     GetAsInteger(int Column,NRecordID id, BOOL originalvalue = FALSE);
//	virtual BOOL    GetAsBool(int Column,NRecordID id, BOOL originalvalue = FALSE);
//
//	virtual BOOL    SetAsString(int Column, NRecordID id ,const CString& val);
//	virtual BOOL    SetAsFloat(int Column, NRecordID id ,const double& val);
//	virtual BOOL    SetAsInteger(int Column, NRecordID id ,const int& val);
//	virtual BOOL    SetAsBool(int Column, NRecordID id ,const BOOL & val);
//
//	virtual long GetBlob(int Column, NRecordID id ,void *& ppBolb);
//	virtual BOOL SetBlob(int Column, NRecordID id ,void * ppBolb,long iSize);
//
//	//使用字符串列名进行数据存储
//	virtual CString GetAsString(const CString & Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE);
//	virtual double  GetAsFloat(const CString & Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE);
//	virtual int     GetAsInteger(const CString & Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE);
//	virtual BOOL    GetAsBool(const CString & Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE);
//
//
//	virtual BOOL    SetAsString(const CString & Column, int row,const CString& val);
//	virtual BOOL    SetAsFloat(const CString & Column, int row, const double& val);
//	virtual BOOL    SetAsInteger(const CString & Column, int row,const int& val);
//	virtual BOOL    SetAsBool(const CString & Column, int row,const BOOL & val);
//		
//	virtual long GetBlob(const CString & Column, int row ,void *& ppBolb,eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE);
//	virtual BOOL SetBlob(const CString & Column, int row ,void * ppBolb,long iSize);
//
//	virtual BOOL OnSortCompareField(NRecordID lowRecordID, NRecordID highRecordID,NDataField * pField,int & iResult);
//
//	virtual BOOL OnRowFilter(int row);
//
//	virtual void ResetFilter(BOOL bResetExp = TRUE);
//
//	virtual void GetAllField(CArray<NDataField*,NDataField*> & dataFields);
//
//protected:
//
//	CNDataSetFieldIndexArr * m_pDataSetFieldIndexArr;
//	NDataModelDataSet * m_pLinkTempDataSet;
//
//	CString m_strLinkDataSetFieldName;
//	int m_iLinkDataSetFieldIndex;
//
//	CString m_strLinkBufferFieldName;
//	int m_iLinkBufferFieldIndex;
//};

#endif // !defined(AFX_PM_LINKDATASET_H__31B9F955_08BC_442A_9780_04EE6A334BE8__INCLUDED_)
