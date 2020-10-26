// PM_LinkDataSet.cpp: implementation of the CPM_LinkDataSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PM_LinkDataSet.h"
#include "CDataSet_StringBuilder.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//IMPLEMENT_DYNCREATE( CPM_LinkDataSet,NDataModelDataSet )
//
//CPM_LinkDataSet::CNDataSetFieldIndexArr::CNDataSetFieldIndexArr(NDataModelDataSet * pDataSet)
//{
//	CArray<NDataField*,NDataField*> dataFields;
//	pDataSet->GetAllField(dataFields);
//
//	int iFieldSize = (int)dataFields.GetSize();
//	for (int i = 0;i < iFieldSize;i++)
//	{
//		NDataField * pField = dataFields.GetAt(i);
//		int iFieldPt = (int)pField;
//		CNDataSet2FieldIndex * pDataSet2FieldIndex = new CNDataSet2FieldIndex;
//		pDataSet2FieldIndex->AddFieldIndex(pDataSet,pDataSet->GetFieldIndex(pField->GetFieldName()));
//
//		m_NDataSet2FieldIndexMap[iFieldPt] = pDataSet2FieldIndex;
//	}
//}
//
//CPM_LinkDataSet::CNDataSetFieldIndexArr::~CNDataSetFieldIndexArr()
//{
//	for (FieldMapFieldIndex_T::iterator iter = m_NDataSet2FieldIndexMap.begin(); iter != m_NDataSet2FieldIndexMap.end(); ++iter)
//	{
//		CNDataSet2FieldIndex * pDataSetFieldIndex = iter->second;
//		delete pDataSetFieldIndex;
//	}
//	m_NDataSet2FieldIndexMap.clear();
//}
//	
//BOOL CPM_LinkDataSet::CNDataSetFieldIndexArr::AddDataFieldIndex(NDataField * pField,NDataModelDataSet * pDataSet,int i2FieldIndex)
//{
//	int iFieldPt = (int)pField;
//	FieldMapFieldIndex_T::iterator iter = m_NDataSet2FieldIndexMap.find(iFieldPt);
//	if (iter == m_NDataSet2FieldIndexMap.end())
//	{
//		return FALSE;
//	}
//	CNDataSet2FieldIndex * pDataSetFieldIndex = iter->second;
//	return pDataSetFieldIndex->AddFieldIndex(pDataSet,i2FieldIndex);
//}
//
//BOOL CPM_LinkDataSet::CNDataSetFieldIndexArr::AddDataFieldIndex(NDataModelDataSet * pDataSet)
//{
//	for (FieldMapFieldIndex_T::iterator iter = m_NDataSet2FieldIndexMap.begin(); iter != m_NDataSet2FieldIndexMap.end(); ++iter)
//		{
//		NDataField * pField = (NDataField*)iter->first;
//		int iFieldIndex = pDataSet->GetFieldIndex(pField->GetFieldName());
//		if(iFieldIndex <= -1)
//			continue;
//
//		CNDataSet2FieldIndex * pDataSetFieldIndex = iter->second;
//		pDataSetFieldIndex->AddFieldIndex(pDataSet,iFieldIndex);
//		}
//	return TRUE;
//}
//
//BOOL CPM_LinkDataSet::CNDataSetFieldIndexArr::FindDataFieldIndex(NDataField * pField,NDataModelDataSet * pDataSet,int & i2FieldIndex)
//{
//	int iFieldPt = (int)pField;
//	FieldMapFieldIndex_T::iterator iter = m_NDataSet2FieldIndexMap.find(iFieldPt);
//	if (iter == m_NDataSet2FieldIndexMap.end())
//	{
//		return FALSE;
//	}
//	CNDataSet2FieldIndex * pDataSetFieldIndex = iter->second;
//	return pDataSetFieldIndex->GetFieldIndex(pDataSet,i2FieldIndex);
//}
//
////////////////////////////////////////////////////////////////////////////
//
//CPM_LinkDataSet::CPM_LinkDataSet()
//{
//	m_pDataSetFieldIndexArr = NULL;
//	m_pLinkTempDataSet = NULL;
//	m_strLinkDataSetFieldName = _T("LinkDataSet");
//	m_strLinkBufferFieldName = _T("LinkBuffer");
//
//	AddField(m_strLinkDataSetFieldName,ftInteger,0);
//	AddField(m_strLinkBufferFieldName,ftInteger,0);
//
//	m_iLinkDataSetFieldIndex = GetFieldIndex(m_strLinkDataSetFieldName);
//	m_iLinkBufferFieldIndex = GetFieldIndex(m_strLinkBufferFieldName);
//}
//
//CPM_LinkDataSet::~CPM_LinkDataSet()
//{
//	m_pLinkTempDataSet = NULL;
//	delete m_pDataSetFieldIndexArr;
//}
//
//void CPM_LinkDataSet::LinkDataSet(NDataModelDataSet *pLinkDataSet)
//{
//	m_iLinkBufferFieldIndex = GetFieldIndex(m_strLinkBufferFieldName);
//	m_iLinkDataSetFieldIndex = GetFieldIndex(m_strLinkDataSetFieldName);
//
//	/********************************************************************
//	liwenlang
//		时间:2010:11:23   11:25
//		备注:将Fields设置 主DataSet标志
//	*********************************************************************/
//	int i = 0;
//	int iFieldCount = m_fields.GetCount();
//	for (i = 0;i < iFieldCount;i++)
//	{
//		m_fields.GetField(i)->SetSpecStatus(LinkMainField);
//	}
//
//	/********************************************************************
//	liwenlang
//		时间:2010:11:24   10:20
//		备注:判断链接的DataSet Field是合法
//	*********************************************************************/
//	if(NULL == m_pLinkTempDataSet)
//	{
//		m_pLinkTempDataSet = pLinkDataSet;
//		m_pDataSetFieldIndexArr = new CNDataSetFieldIndexArr(pLinkDataSet);
//	}
//	else
//	{
//		m_pDataSetFieldIndexArr->AddDataFieldIndex(pLinkDataSet);
//	}
//
//	int iRowCount = pLinkDataSet->GetRowCount();
//	int iDataSetMPos = (int)pLinkDataSet;
//	for (i = 1;i <= iRowCount;i++)
//	{
//		int iNewRow = this->InsertRow(0);
//		int iLinkBuffer = (int)pLinkDataSet->GetRecordID(i);
//
//		NDataModelDataSet::SetAsInteger(m_iLinkDataSetFieldIndex,iNewRow,iDataSetMPos);
//		NDataModelDataSet::SetAsInteger(m_iLinkBufferFieldIndex,iNewRow,iLinkBuffer);
//	}
//}
//
//void CPM_LinkDataSet::DisLinkDataSet()
//{
//	int iRowCount = this->GetRowCount();
//	int iNull = 0;
//	for (int i = 1;i <= iRowCount;i++)
//	{
//		NDataModelDataSet::SetAsInteger(m_iLinkDataSetFieldIndex,i,iNull);
//		NDataModelDataSet::SetAsInteger(m_iLinkBufferFieldIndex,i,iNull);
//	}
//	m_pLinkTempDataSet = NULL;
//
//	delete m_pDataSetFieldIndexArr;
//	m_pDataSetFieldIndexArr = NULL;
//}
//
//NDataField * CPM_LinkDataSet::GetField(int iIndex)
//{
//	NDataField * pTemField = NULL;
//	if(iIndex < 0)
//		return pTemField;
//	int iDataFieldCount = m_fields.GetCount();
//	if(iIndex < iDataFieldCount)
//		pTemField = m_fields.GetField(iIndex);
//	else if(NULL != m_pLinkTempDataSet)
//	{
//		iIndex -= iDataFieldCount;
//		pTemField = m_pLinkTempDataSet->GetField(iIndex);
//	}
//	return pTemField;
//}
//
//int CPM_LinkDataSet::GetFieldIndex(const CString & sFieldName)
//{
//	int iFieldIndex = m_fields.GetFieldIndex(sFieldName);
//	if(-1 == iFieldIndex && NULL != m_pLinkTempDataSet)
//	{
//		iFieldIndex = m_pLinkTempDataSet->GetFieldIndex(sFieldName);
//		iFieldIndex += m_fields.GetCount();
//	}
//	return iFieldIndex;
//}
//
//CString CPM_LinkDataSet::GetFieldName(int index)
//{
//	NDataField * pTemField = m_fields.GetField(index);
//	if(NULL == pTemField && NULL != m_pLinkTempDataSet)
//	{
//		pTemField = m_pLinkTempDataSet->GetField(index);
//		if(NULL == pTemField)
//			return _T("");
//	}
//	return pTemField->GetFieldName();
//}
//
//CString CPM_LinkDataSet::GetAsString(int Column,NRecordID id, BOOL originalvalue)
//{
//	NDataField * pField = GetField(Column);
//	if(Column >= m_fields.GetCount())
//	{
//		NDataModelDataSet * pLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,id);
//		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,id);
//		
//		return pLinkDataSet->GetAsString(Column - m_fields.GetCount(),pBuffer);
//	}
//	return NDataModelDataSet::GetAsString(pField,id);
//}
//
//double CPM_LinkDataSet::GetAsFloat(int Column,NRecordID id, BOOL originalvalue)
//{
//	NDataField * pField = GetField(Column);
//	if(Column >= m_fields.GetCount())
//	{
//		NDataModelDataSet * pLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,id);
//		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,id);
//		
//		return pLinkDataSet->GetAsFloat(Column - m_fields.GetCount(),pBuffer);
//	}
//	return NDataModelDataSet::GetAsFloat(pField,id);
//}
//
//int CPM_LinkDataSet::GetAsInteger(int Column,NRecordID id, BOOL originalvalue)
//{
//	NDataField * pField = GetField(Column);
//	if(Column >= m_fields.GetCount())
//	{
//		NDataModelDataSet * pLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,id);
//		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,id);
//		
//		return pLinkDataSet->GetAsInteger(Column - m_fields.GetCount(),pBuffer);
//	}
//	return NDataModelDataSet::GetAsInteger(pField,id);
//}
//
//BOOL CPM_LinkDataSet::GetAsBool(int Column,NRecordID id, BOOL originalvalue)
//{
//	NDataField * pField = GetField(Column);
//	if(Column >= m_fields.GetCount())
//	{
//		NDataModelDataSet * pLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,id);
//		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,id);
//		
//		return pLinkDataSet->GetAsBool(Column - m_fields.GetCount(),pBuffer);
//	}
//	return NDataModelDataSet::GetAsBool(pField,id);
//}
//
//BOOL CPM_LinkDataSet::SetAsString(int Column, NRecordID id ,const CString& val)
//{
//	NDataField * pField = GetField(Column);
//	if(Column >= m_fields.GetCount())
//	{
//		NDataModelDataSet * pLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,id);
//		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,id);
//		
//		return pLinkDataSet->SetAsString(Column - m_fields.GetCount(),pBuffer,val);
//	}
//	return NDataModelDataSet::SetAsString(pField,id,val);
//}
//
//BOOL CPM_LinkDataSet::SetAsFloat(int Column, NRecordID id ,const double& val)
//{
//	NDataField * pField = GetField(Column);
//	if(Column >= m_fields.GetCount())
//	{
//		NDataModelDataSet * pLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,id);
//		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,id);
//		
//		return pLinkDataSet->SetAsFloat(Column - m_fields.GetCount(),pBuffer,val);
//	}
//	return NDataModelDataSet::SetAsFloat(pField,id,val);
//}
//
//BOOL CPM_LinkDataSet::SetAsInteger(int Column, NRecordID id ,const int& val)
//{
//	NDataField * pField = GetField(Column);
//	if(Column >= m_fields.GetCount())
//	{
//		NDataModelDataSet * pLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,id);
//		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,id);
//		
//		return pLinkDataSet->SetAsInteger(Column - m_fields.GetCount(),pBuffer,val);
//	}
//	return NDataModelDataSet::SetAsInteger(pField,id,val);
//}
//
//BOOL CPM_LinkDataSet::SetAsBool(int Column, NRecordID id ,const BOOL & val)
//{
//	NDataField * pField = GetField(Column);
//	if(Column >= m_fields.GetCount())
//	{
//		NDataModelDataSet * pLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,id);
//		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,id);
//		
//		return pLinkDataSet->SetAsBool(Column - m_fields.GetCount(),pBuffer,val);
//	}
//	return NDataModelDataSet::SetAsBool(pField,id,val);
//}
//
//long  CPM_LinkDataSet::GetBlob(int Column, NRecordID id ,void *& ppBolb)
//{
//	NDataField * pField = GetField(Column);
//	if(Column >= m_fields.GetCount())
//	{
//		NDataModelDataSet * pLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,id);
//		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,id);
//		
//		return pLinkDataSet->GetBlob(Column - m_fields.GetCount(),pBuffer,ppBolb);
//	}
//	return NDataModelDataSet::GetBlob(pField,id,ppBolb);
//}
//
//BOOL CPM_LinkDataSet::SetBlob(int Column, NRecordID id ,void * ppBolb,long iSize)
//{
//	NDataField * pField = GetField(Column);
//	if(Column >= m_fields.GetCount())
//	{
//		NDataModelDataSet * pLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,id);
//		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,id);
//		
//		return pLinkDataSet->SetBlob(Column - m_fields.GetCount(),pBuffer,ppBolb,iSize);
//	}
//	return NDataModelDataSet::SetBlob(pField,id,ppBolb,iSize);
//}
//
////使用FieldIndex快速查找
//BOOL  CPM_LinkDataSet::SetAsString(int Column, int row ,const CString& val)
//{
//	return SetAsString(Column,GetRecordID(row),val);
//}
//
//BOOL  CPM_LinkDataSet::SetAsFloat(int Column, int row ,const double& val)
//{
//	return SetAsFloat(Column,GetRecordID(row),val);
//}
//
//BOOL  CPM_LinkDataSet::SetAsInteger(int Column, int row ,const int& val)
//{
//	return SetAsInteger(Column,GetRecordID(row),val);
//}
//// 
//// BOOL  CPM_LinkDataSet::SetAsVariant(int Column, int row ,VARIANT& val)
//// {
//// 	NDataField * pField = GetField(Column);
//// 	if(LinkDataField == pField->GetSpecStatus())
//// 	{
//// 		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,row);
//// 		return m_pLinkDataSet->SetAsVariant(Column - m_fields.GetCount(),pBuffer,val);
//// 	}
//// 	return NDataModelDataSet::SetAsVariant(pField,GetRecordID(row),val);
//// }
//// 
//// BOOL  CPM_LinkDataSet::SetAsDateTime(int Column, int row ,COleDateTime& val)
//// {
//// 	NDataField * pField = GetField(Column);
//// 	if(LinkDataField == pField->GetSpecStatus())
//// 	{
//// 		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,row);
//// 		return m_pLinkDataSet->SetAsDateTime(Column - m_fields.GetCount(),pBuffer,val);
//// 	}
//// 	return NDataModelDataSet::SetAsDateTime(pField,GetRecordID(row),val);
//// }
//
//BOOL  CPM_LinkDataSet::SetAsBool(int Column, int row ,const BOOL & val)
//{
//	return SetAsBool(Column,GetRecordID(row),val);
//}
//
//CString CPM_LinkDataSet::GetAsString(int Column,int row, eBufferFlag BufferFlag, BOOL originalvalue)
//{
//	return GetAsString(Column,GetRecordID(row));
//}
//
//double  CPM_LinkDataSet::GetAsFloat(int Column,int row, eBufferFlag BufferFlag, BOOL originalvalue)
//{
//	return GetAsFloat(Column,GetRecordID(row),originalvalue);
//}
//
//int CPM_LinkDataSet::GetAsInteger(int Column,int row, eBufferFlag BufferFlag, BOOL originalvalue)
//{
//	return GetAsInteger(Column,GetRecordID(row),originalvalue);
//}
//// 
//// VARIANT CPM_LinkDataSet::GetAsVariant(int Column,int row, eBufferFlag BufferFlag, BOOL originalvalue)
//// {
//// 	NDataField * pField = GetField(Column);
//// 	if(LinkDataField == pField->GetSpecStatus())
//// 	{
//// 		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,row);
//// 		return m_pLinkDataSet->GetAsVariant(Column - this->GetFieldCount(),pBuffer);
//// 	}
//// 	return NDataModelDataSet::GetAsVariant(pField,GetRecordID(row));
//// }
//// 
//// COleDateTime CPM_LinkDataSet::GetAsDateTime(int Col,int row, eBufferFlag BufferFlag, BOOL originalvalue)
//// {
//// 	NDataField * pField = GetField(Col);
//// 	if(LinkDataField == pField->GetSpecStatus())
//// 	{
//// 		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,row);
//// 		return m_pLinkDataSet->GetAsDateTime(Col - this->GetFieldCount(),pBuffer);
//// 	}
//// 	return NDataModelDataSet::GetAsDateTime(pField,GetRecordID(row));
//// }
//// 
//BOOL CPM_LinkDataSet::GetAsBool(int Col,int row, eBufferFlag BufferFlag, BOOL originalvalue)
//{
//	return GetAsBool(Col,GetRecordID(row),originalvalue);
//}
//
//long CPM_LinkDataSet::GetBlob(int iColumn, int row ,void *& ppBolb,eBufferFlag BufferFlag, BOOL originalvalue)
//{
//	return GetBlob(iColumn,GetRecordID(row),ppBolb);
//}
//
//BOOL CPM_LinkDataSet::SetBlob(int Column, int row ,void * ppBolb,long iSize)
//{
//	return SetBlob(Column,GetRecordID(row),ppBolb,iSize);
//}
//
//	//使用字符串列名进行数据存储
//CString CPM_LinkDataSet::GetAsString(const CString & Column,int row, eBufferFlag BufferFlag, BOOL originalvalue)
//{
//	return GetAsString(GetFieldIndex(Column),row,BufferFlag,originalvalue);
//}
//
//double  CPM_LinkDataSet::GetAsFloat(const CString & Column,int row, eBufferFlag BufferFlag, BOOL originalvalue)
//{
//	return GetAsFloat(GetFieldIndex(Column),row,BufferFlag,originalvalue);
//}
//
//int CPM_LinkDataSet::GetAsInteger(const CString & Column,int row, eBufferFlag BufferFlag, BOOL originalvalue)
//{
//	return GetAsInteger(GetFieldIndex(Column),row,BufferFlag,originalvalue);
//}
//
//// VARIANT CPM_LinkDataSet::GetAsVariant(CString Column,int row, eBufferFlag BufferFlag, BOOL originalvalue)
//// {
//// }
//// 
//// COleDateTime CPM_LinkDataSet::GetAsDateTime(CString Column,int row, eBufferFlag BufferFlag, BOOL originalvalue)
//// {
//// 
//// }
//	
//BOOL CPM_LinkDataSet::GetAsBool(const CString & Column,int row, eBufferFlag BufferFlag, BOOL originalvalue)
//{
//	return GetAsBool(GetFieldIndex(Column),row,BufferFlag,originalvalue);
//}
//
//BOOL CPM_LinkDataSet::SetAsString(const CString & Column, int row,const CString& val)
//{
//	return SetAsString(GetFieldIndex(Column),row,val);
//}
//
//BOOL CPM_LinkDataSet::SetAsFloat(const CString & Column, int row, const double& val)
//{
//	return SetAsFloat(GetFieldIndex(Column),row,val);
//}
//
//BOOL CPM_LinkDataSet::SetAsInteger(const CString & Column, int row,const int& val)
//{
//	return SetAsInteger(GetFieldIndex(Column),row,val);
//}
//// 
//// BOOL CPM_LinkDataSet::SetAsVariant(CString Column, int row, VARIANT& val)
//// {
//// }
//// 
//// BOOL CPM_LinkDataSet::SetAsDateTime(CString Column, int row, COleDateTime& val)
//// {
//// }
//
//BOOL CPM_LinkDataSet::SetAsBool(const CString & Column, int row,const BOOL & val)
//{
//	return SetAsBool(GetFieldIndex(Column),row,val);
//}
//		
//long CPM_LinkDataSet::GetBlob(const CString & Column, int row ,void *& ppBolb,eBufferFlag BufferFlag, BOOL originalvalue)
//{
//	return GetBlob(GetFieldIndex(Column),row,ppBolb,BufferFlag,originalvalue);
//}
//
//BOOL CPM_LinkDataSet::SetBlob(const CString & Column, int row ,void * ppBolb,long iSize)
//{
//	return SetBlob(GetFieldIndex(Column),row,ppBolb,iSize);
//}
//
//BOOL CPM_LinkDataSet::OnSortCompareField(NRecordID lowRecordID, NRecordID highRecordID,NDataField * pField,int & iResult)
//{
//	if(this == pField->GetOwner())
//	{//判断是否是 当前的DataSet的 Field
//		iResult = pField->Compare(GetFieldDataPT(pField,lowRecordID),GetFieldDataPT(pField,highRecordID));
//	}
//	else
//	{
//		if(NULL == m_pLinkTempDataSet)
//			return FALSE;
//
//		NDataModelDataSet * pLowLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,lowRecordID);
//		NRecordID pLowRowBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,lowRecordID);
//
//		NDataModelDataSet * phighLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,highRecordID);
//		NRecordID phighRowBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,highRecordID);
//
//		int iLowFieldIndex = -1;
//		m_pDataSetFieldIndexArr->FindDataFieldIndex(pField,pLowLinkDataSet,iLowFieldIndex);
//		if(iLowFieldIndex <= -1)
//			iLowFieldIndex = pLowLinkDataSet->GetFieldIndex(pField->GetFieldName());
//		if(iLowFieldIndex <= -1)
//			return FALSE;
//
//		int ihighFieldIndex = 0;
//		if(pLowLinkDataSet == phighLinkDataSet)
//			ihighFieldIndex = iLowFieldIndex;
//		else
//		{
//			m_pDataSetFieldIndexArr->FindDataFieldIndex(pField,phighLinkDataSet,ihighFieldIndex);
//			if(iLowFieldIndex <= -1)
//				ihighFieldIndex = phighLinkDataSet->GetFieldIndex(pField->GetFieldName());
//		}
//		if(iLowFieldIndex <= -1 && ihighFieldIndex > -1)
//		{
//			iResult = -1;
//			return TRUE;
//		}
//		else if(iLowFieldIndex > -1 && ihighFieldIndex <= -1)
//		{
//			iResult = 1;
//			return TRUE;
//	}
//		else if(iLowFieldIndex == -1 && ihighFieldIndex == -1)
//	{
//			iResult = 0;
//			return TRUE;
//		}
//
//
//		switch(pField->GetDataType())
//		{
//		case ftFloat:
//			{
//				double lowValue = pLowLinkDataSet->GetAsFloat(iLowFieldIndex,pLowRowBuffer);
//				double highValue = phighLinkDataSet->GetAsFloat(ihighFieldIndex,phighRowBuffer);
//				
//				iResult = pField->Compare(&lowValue,&highValue);
//			}
//			break;
//		case ftString:
//			{
//				CString lowValue = pLowLinkDataSet->GetAsString(iLowFieldIndex,pLowRowBuffer);
//				CString highValue = phighLinkDataSet->GetAsString(ihighFieldIndex,phighRowBuffer);
//				
//				iResult = pField->Compare(lowValue.GetBuffer(0),highValue.GetBuffer(0));
//			}
//			break;
//		case ftBlob:
//			{
//				LPVOID plowBlob = NULL;
//				LONG lLowBlobSize = pLowLinkDataSet->GetBlob(iLowFieldIndex,pLowRowBuffer,plowBlob);
//				
//				LPVOID phighBlob = NULL;
//				LONG lhighBlobSize = phighLinkDataSet->GetBlob(ihighFieldIndex,phighRowBuffer,phighBlob);
//				
//				iResult = pField->CompareValue(plowBlob,lLowBlobSize,phighBlob,lhighBlobSize);
//			}
//			break;
//		default:
//			{
//				int lowValue = pLowLinkDataSet->GetAsInteger(iLowFieldIndex,pLowRowBuffer);
//				int highValue = phighLinkDataSet->GetAsInteger(ihighFieldIndex,phighRowBuffer);
//				
//				iResult = pField->Compare(&lowValue,&highValue);
//			}
//			break;
//		}
//	}	
//	return TRUE;
//}
//
//BOOL CPM_LinkDataSet::OnRowFilter(int row)
//{
//	if(NULL == m_pLinkTempDataSet)
//		return TRUE;
//
//	NDataModelDataSet * pLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,row);
//	NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,row);
//
//	CArray<NDataField*,NDataField*> dataFields;
//	m_pLinkTempDataSet->GetAllField(dataFields);
//	int FieldCount = (int)dataFields.GetSize();
//	
//	for(int Col = 0 ;Col < FieldCount ;Col++)
//	{
//		NDataField * pField = dataFields.GetAt(Col);
//		if (pField != NULL && pField->HasFilterValue())
//		{
//			int iTemFieldIndex = -1;
//			m_pDataSetFieldIndexArr->FindDataFieldIndex(pField,pLinkDataSet,iTemFieldIndex);
//			if(iTemFieldIndex <= -1)
//				iTemFieldIndex = pLinkDataSet->GetFieldIndex(pField->GetFieldName());
//			if(iTemFieldIndex <= -1)
//				continue;
//
//			switch(pField->GetDataType())
//			{
//			case ftFloat:
//				{
//					double Value = pLinkDataSet->GetAsFloat(iTemFieldIndex,pBuffer);
//					
//					if (pField->FilterCompare(&Value) == FALSE) 
//						return FALSE;
//			}
//				break;
//			case ftString:
//				{
//					CString Value = pLinkDataSet->GetAsString(iTemFieldIndex,pBuffer);
//					
//					if (pField->FilterCompare(Value.GetBuffer(0)) == FALSE) 
//						return FALSE;
//				}
//				break;
//			case ftBlob:
//				{
//					LPVOID pBlob = NULL;
//					LONG lBlobSize = pLinkDataSet->GetBlob(iTemFieldIndex,pBuffer,pBlob);
//					
//					if (pField->FilterCompareValue(pBlob,lBlobSize) == FALSE) 
//						return FALSE;
//
//				}
//				break;
//			default:
//				{
//					int Value = pLinkDataSet->GetAsInteger(iTemFieldIndex,pBuffer);
//					
//					if (pField->FilterCompare(&Value) == FALSE) 
//						return FALSE;
//				}
//				break;
//			}
//		}
//		
//	}
//	return TRUE;
//}
//
//void CPM_LinkDataSet::ResetFilter(BOOL bResetExp)
//{
//	NDataModelDataSet::ResetFilter(bResetExp);
//	if(NULL != m_pLinkTempDataSet)
//		m_pLinkTempDataSet->ResetFilter(bResetExp);
//}
//
//void CPM_LinkDataSet::GetAllField(CArray<NDataField*,NDataField*> & dataFields)
//{
//	NDataModelDataSet::GetAllField(dataFields);
//	if(NULL != m_pLinkTempDataSet)
//		m_pLinkTempDataSet->GetAllField(dataFields);
//}
//
//BOOL CPM_LinkDataSet::BuildItemXML(CString tableName,int row,NRecordID recordID,CString &sbuff,CString &sXMLBuf,CString &sXMLBefore)
//{
////	UIBase::CTiming timing(_T("CPM_LargeFileDataSet::BuildItemXML"),TRUE);
//	
//	//先生成行信息
//	//sXMLBuf = "<"+m_TableName+GetRowModifiedInternal(recordID)?" hasChanges="+"\"modified\"":""+
//	//	GetRowNewInternal(recordID)?" IsNew="+"true":""+">";
//	
//	//开始生成每一个item
//	BOOL rowModified = GetRowModifiedInternal((char*)recordID);
//	BOOL rowNew = GetRowNewInternal(recordID);
//	CString Data/*,s,Orgs*/;
//	CDataSet_StringBuilder sInstance,sBefroe;
//	CString FieldName;
//	int datatype;
//	int i = 0 ;
//	for(i = 0 ; i < GetFieldCount(); i++)
//	{
//		Data = NDataModelDataSet::GetAsString(i,recordID);
//
//		///////////////////////////////////
//		///liwenlang 
//		///不将空的属性 保存 在LoadXML时 在读取Before数据时 没有办法 更新空数据库
//		///[25/12/2009 9:40]
//		//////////////////////////////////
////		if(Data.IsEmpty()) continue;
//
//		datatype=GetFieldType(i);
//		FieldName=GetFieldName(i);
//
//		//日期型特殊处理
//		if(datatype ==(int) ftDateTime) {
//			Data.Replace(_T('.'), _T('-'));
//			Data.Replace(_T(' '), _T('T'));
//		}
//		//字符型特殊处理
//		if(datatype==ftString || ftBlob) {
//			Data.Replace(_T("&"), _T("&amp;"));
//			Data.Replace(_T("<"), _T("&lt;"));
//			Data.Replace(_T(">"), _T("&gt;"));
//		}
////		s.Format(_T("<%s>%s</%s>"), FieldName, Data, FieldName);
//		sInstance.Append(_T("<"));
//		sInstance.Append(FieldName);
//		sInstance.Append(_T(">"));
//		sInstance.Append(Data);
//		sInstance.Append(_T("</"));
//		sInstance.Append(FieldName);
//		sInstance.Append(_T(">"));
//		//开始组织befor段的内容
//		if (rowModified && (!rowNew))
//		{
//			if (GetItemModifiedInternal(recordID,i)) 
//			{
//				Data = NDataModelDataSet::GetAsString(i,recordID,TRUE);
//				//日期型特殊处理
//				if(datatype ==(int) ftDateTime) {
//					Data.Replace(_T('.'), _T('-'));
//					Data.Replace(_T(' '), _T('T'));
//				}
//				//字符型特殊处理
//				if(datatype==ftString || ftBlob) {
//					Data.Replace(_T("&"), _T("&amp;"));
//					Data.Replace(_T("<"), _T("&lt;"));
//					Data.Replace(_T(">"), _T("&gt;"));
//				}
////				Orgs.Format(_T("<%s>%s</%s>"), FieldName, Data, FieldName);
//				sBefroe.Append(_T("<"));
//				sBefroe.Append(FieldName);
//				sBefroe.Append(_T(">"));
//				sBefroe.Append(Data);
//				sBefroe.Append(_T("</"));
//				sBefroe.Append(FieldName);
//				sBefroe.Append(_T(">"));
//			}
//		}
//	}
//
//	if(NULL != m_pLinkTempDataSet)
//	{
//		NDataModelDataSet * pLinkDataSet = (NDataModelDataSet*)NDataModelDataSet::GetAsInteger(m_iLinkDataSetFieldIndex,recordID);
//		NRecordID pBuffer = (NRecordID)NDataModelDataSet::GetAsInteger(m_iLinkBufferFieldIndex,recordID);
//
//		CArray<NDataField*,NDataField*> dataFields;
//		pLinkDataSet->GetAllField(dataFields);
//		for(i = 0 ; i < dataFields.GetSize(); i++)
//		{
//			NDataField * pTemField = dataFields.GetAt(i);
//
//			Data = pLinkDataSet->GetAsString(pLinkDataSet->GetFieldIndex(pTemField->GetFieldName()),pBuffer);
//			
//			///////////////////////////////////
//			///liwenlang 
//			///不将空的属性 保存 在LoadXML时 在读取Before数据时 没有办法 更新空数据库
//			///[25/12/2009 9:40]
//			//////////////////////////////////
//			//		if(Data.IsEmpty()) continue;
//			
//			datatype = pTemField->GetDataType();
//			FieldName = pTemField->GetFieldName();
//			
//			//日期型特殊处理
//			if(datatype ==(int) ftDateTime) {
//				Data.Replace(_T('.'), _T('-'));
//				Data.Replace(_T(' '), _T('T'));
//			}
//			//字符型特殊处理
//			if(datatype==ftString || ftBlob) {
//				Data.Replace(_T("&"), _T("&amp;"));
//				Data.Replace(_T("<"), _T("&lt;"));
//				Data.Replace(_T(">"), _T("&gt;"));
//			}
////			s.Format(_T("<%s>%s</%s>"), FieldName, Data, FieldName);
//			sInstance.Append(_T("<"));
//			sInstance.Append(FieldName);
//			sInstance.Append(_T(">"));
//			sInstance.Append(Data);
//			sInstance.Append(_T("</"));
//			sInstance.Append(FieldName);
//			sInstance.Append(_T(">"));
//			
//		}
//	}
//
//	CString strRow ;
//	strRow.Format( _T("%d") , row ) ;
//
//	//TCHAR cRows[20];
//	//_itot(row,cRows,10);
//	if (sBefroe.GetLength() > 0 )//为原始数据加上封装节点 
//	{
////		sXMLBefore.Format(_T("<%s rowid=\"%d\" buff=%s>%s</%s>"),tableName,row,sbuff,sBefroe.m_pMem,tableName);
//		CDataSet_StringBuilder sBuildXMLBefore;
//		sBuildXMLBefore.Append(_T("<"));
//		sBuildXMLBefore.Append(tableName);
//		sBuildXMLBefore.Append(_T(" rowid=\""));
//		sBuildXMLBefore.Append( strRow );
//		sBuildXMLBefore.Append(_T("\" buff="));
//		sBuildXMLBefore.Append(sbuff);
//		sBuildXMLBefore.Append(_T(">"));
//		sBuildXMLBefore.Append(sBefroe);
//		sBuildXMLBefore.Append(_T("<"));
//		sBuildXMLBefore.Append(_T("tableName"));
//		sBuildXMLBefore.Append(_T(">"));
//		sBuildXMLBefore.GetString(sXMLBefore);
//	}
//
//    CString sRowModify,sRownew;
//	sRowModify = rowModified&&(!rowNew) ? _T("hasChanges=\"modified\" "):_T("");
//	sRownew = rowNew?_T(" IsNew=\"true\" "):_T("");
////	sXMLBuf.Format(_T("<%s %s%s\"%d\" buff=%s>%s</%s>"),tableName , sRowModify+sRownew,_T("rowid="),row,sbuff,sInstance.m_pMem,tableName);
//	CDataSet_StringBuilder sBuildXMLBuff;
//	sBuildXMLBuff.Append(_T("<"));
//	sBuildXMLBuff.Append(tableName);
//	sBuildXMLBuff.Append(_T(" "));
//	sBuildXMLBuff.Append(sRowModify+sRownew);
//	sBuildXMLBuff.Append(_T("rowid="));
//	sBuildXMLBuff.Append(_T("\""));
//	sBuildXMLBuff.Append( strRow );
//	sBuildXMLBuff.Append(_T("\" buff="));
//	sBuildXMLBuff.Append(sbuff);
//	sBuildXMLBuff.Append(_T(">"));
//	sBuildXMLBuff.Append(sInstance);
//	sBuildXMLBuff.Append(_T("</"));
//	sBuildXMLBuff.Append(tableName);
//	sBuildXMLBuff.Append(_T(">"));
//	sBuildXMLBuff.GetString(sXMLBuf);
//
//	//sXMLBuf+="/"+m_TableName+">"
//	return TRUE;
//}
