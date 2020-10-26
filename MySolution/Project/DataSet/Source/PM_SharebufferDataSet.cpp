// PM_SharebufferDataSet.cpp: implementation of the CPM_SharebufferDataSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PM_SharebufferDataSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE( CPM_SharebufferDataSet,CPM_StorageDataSet )

CPM_SharebufferDataSet::CPM_SharebufferDataSet()
{
	m_pPrimaryDataSet = NULL;
	m_bHasShareBuffer = FALSE;
}

CPM_SharebufferDataSet::CPM_SharebufferDataSet(NDataModelDataSet * pPrimaryDataSet)
{
	m_bHasShareBuffer = TRUE;
	m_pPrimaryDataSet = pPrimaryDataSet;
	
	if ( m_pPrimaryDataSet->IsKindOf( RUNTIME_CLASS(CPM_StorageDataSet) ))
	{
		short iPriVersion = 0;
		((CPM_StorageDataSet*)m_pPrimaryDataSet)->GetVersionID(iPriVersion) ; 
		SetVersionID(iPriVersion);
	}
	/********************************************************************
	liwenlang
		时间:2010:10:9   9:49
		备注:建立Field的共享信息
	*********************************************************************/
	NDataField *field;
	int i = 0;
	for(i = 0; i < pPrimaryDataSet->GetFieldCount(); i++)
	{
		field = pPrimaryDataSet->GetField(i);
//		this->AddField(field->GetFieldName(),field->GetDataType(),field->GetDataType() == ftString?field->GetSize() - 1:field->GetSize());
	
		if(ftBlob == field->GetDataType())
		{
			this->m_fields.m_HasBlob = TRUE;
			this->m_fields.m_Bloblist.Add(field);
		}
		this->m_fields.m_list.Add(field);
		this->m_fields.m_MapNameField.SetAt(field->GetFieldName(),field);

		this->m_fields.RefreshFieldOffset();
	}
	this->InteralOpen();

	//开始进行存储区的直接复制
	CPMPtrArray* pArray,*pArray1;
	pArray = pPrimaryDataSet->m_PrimaryBuffers->GetBufferPtr();
	pArray1 = m_PrimaryBuffers->GetBufferPtr();
	pArray1->Append(*pArray);
    
	pArray = pPrimaryDataSet->m_DeleteBuffers->GetBufferPtr();
	pArray1 = m_DeleteBuffers->GetBufferPtr();
	pArray1->Append(*pArray);
	
	pArray = pPrimaryDataSet->m_FilterBuffers->GetBufferPtr();
	pArray1 = m_FilterBuffers->GetBufferPtr();
	pArray1->Append(*pArray);
}

CPM_SharebufferDataSet::CPM_SharebufferDataSet(CUIntArray & iRowIDArr,NDataModelDataSet * pPrimaryDataSet)
{
	m_bHasShareBuffer = TRUE;
	m_pPrimaryDataSet = pPrimaryDataSet;

	if ( m_pPrimaryDataSet->IsKindOf( RUNTIME_CLASS(CPM_StorageDataSet) ))
	{
		short iPriVersion = 0;
		((CPM_StorageDataSet*)m_pPrimaryDataSet)->GetVersionID(iPriVersion) ; 
		SetVersionID(iPriVersion);
	}
	/********************************************************************
	liwenlang
	时间:2010:10:9   9:49
	备注:建立Field的共享信息
	*********************************************************************/
	NDataField *field;
	int i = 0;
	for(i = 0; i < pPrimaryDataSet->GetFieldCount(); i++)
	{
		field = pPrimaryDataSet->GetField(i);
		//		this->AddField(field->GetFieldName(),field->GetDataType(),field->GetDataType() == ftString?field->GetSize() - 1:field->GetSize());

		if(ftBlob == field->GetDataType())
		{
			this->m_fields.m_HasBlob = TRUE;
			this->m_fields.m_Bloblist.Add(field);
		}
		this->m_fields.m_list.Add(field);
		this->m_fields.m_MapNameField.SetAt(field->GetFieldName(),field);

		this->m_fields.RefreshFieldOffset();
	}
	this->InteralOpen();

	//开始进行存储区的直接复制
	CPMPtrArray* pYuanArray,*pNewArray1;
	pYuanArray = pPrimaryDataSet->m_PrimaryBuffers->GetBufferPtr();
	pNewArray1 = m_PrimaryBuffers->GetBufferPtr();

	int iRowIDSize = (int)iRowIDArr.GetSize();
//	pNewArray1->SetSize(iRowIDSize);
	pNewArray1->SetMemorySize(iRowIDSize);
	for (i = 0;i < iRowIDSize;i++)
	{
		int iTemYuanRowID = iRowIDArr.GetAt(i);
		if(iTemYuanRowID < 0 || iTemYuanRowID > pYuanArray->GetSize())
			continue;

		NRecordID pTemRecordID = pYuanArray->GetAt(iTemYuanRowID - 1);
		pNewArray1->Add(pTemRecordID);
	}
}

CPM_SharebufferDataSet::~CPM_SharebufferDataSet()
{
	if(FALSE == m_bHasShareBuffer)
		return;

	Address2MdfMode::iterator iter = m_address2MdfMode.begin() ;
	for ( ; iter != m_address2MdfMode.end() ; iter++ )
	{
		FreeRecordBuffer((LPVOID)iter->first);		
	}

	/********************************************************************
	liwenlang
		时间:2010:10:9   9:49
		备注:清除Field的共享信息
	*********************************************************************/
	this->m_fields.m_list.RemoveAll();
	this->m_fields.m_Bloblist.RemoveAll();

	this->m_fields.m_MapNameField.RemoveAll();
	this->m_fields.RefreshFieldOffset();

	m_PrimaryBuffers->GetBufferPtr()->RemoveAll();
	m_FilterBuffers->GetBufferPtr()->RemoveAll();
	m_DeleteBuffers->GetBufferPtr()->RemoveAll();
	
	m_address2MdfMode.clear();
	m_pPrimaryDataSet = NULL;
}

int CPM_SharebufferDataSet::InsertRow(int iberforrow)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::InsertRow(iberforrow);

	iberforrow--;//转化为index
	void * newbuff = AlloRecordBuffer();
	int newrow = m_PrimaryBuffers->InsertBuff(newbuff,iberforrow) +1;//返回的是index
	if (m_State == dmstNormal) {
		SetRowNewInternal(newbuff,TRUE);
		OnInsertedRow(newrow);
		
	}

	/********************************************************************
	liwenlang
		时间:2010:10:8   8:44
		备注:添加修改标志
	*********************************************************************/
	m_address2MdfMode[(LPVOID)newbuff] = 1;
	return newrow;
}

BOOL CPM_SharebufferDataSet::SetBlob(const CString & Column, int row ,void * ppBolb,long iSize)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetBlob(Column,row,ppBolb,iSize);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetBlob_Field(pTemField,row,ppBolb,iSize);
}
	
BOOL CPM_SharebufferDataSet::SetBlob(int Column, int row ,void * ppBolb,long iSize)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetBlob(Column,row,ppBolb,iSize);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetBlob_Field(pTemField,row,ppBolb,iSize);
}

BOOL CPM_SharebufferDataSet::SetBlob_Field(NDataField * field,int iRow,void * ppBolb,long iSize)
{
	if(NULL == field)
		return FALSE;
	NRecordID id = GetRecordID(iRow);
	if(NULL == id)
		return FALSE;

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(iRow,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	return CPM_StorageDataSet::SetBlob(field,newid,ppBolb,iSize);
}

NRecordID CPM_SharebufferDataSet::BuildNewNRecordID(NRecordID id)
{
	NRecordID pNewBuffer = AlloRecordBuffer();
	if(NULL == pNewBuffer)
		return NULL;

	CopyMemory(pNewBuffer,id,m_RecordBufferSize);

	int iFieldCount = m_fields.GetCount();
	int i = 0;
	for (i = 0;i < iFieldCount;i++)
	{
		NDataField * pTemField = m_fields.GetField(i);
		if(NULL == pTemField)
			continue;

		if(ftBlob == pTemField->GetDataType())
		{
			void * buffer = GetFieldDataPT(pTemField ,id);

			LPVOID pBlobBuffer = NULL;
			LONG BlobSize = pTemField->GetBlob(buffer,pBlobBuffer);

			LPVOID pTemBuffer = GetFieldDataPT(pTemField,pNewBuffer);
			if(pTemBuffer)
			{
				ZeroMemory(pTemBuffer,pTemField->GetSize());
				pTemField->SetBlob(pTemBuffer,pBlobBuffer,BlobSize);
			}
		}
	}

	NDataField *field = NULL;
	void *ptr;
	for(i = 0 ; i < m_fields.GetCount(); i++)
	{
		field = m_fields.GetField(i);
		if (field != NULL) 
		{
			ptr = GetFieldDataPT(field,pNewBuffer);
			field->FreeOrigValueBuff(ptr,FALSE);
			field->SetItemModified((char*)ptr,FALSE);
		}
	}
	return pNewBuffer;
}

BOOL CPM_SharebufferDataSet::ReplaceBuffer(int iRow,NRecordID id,NRecordID newID)
{	
	int iBufferID = iRow - 1;
	if(id != m_PrimaryBuffers->GetBuffPtr(iBufferID))
	{
		int iBufferCount = m_PrimaryBuffers->GetRowCount();
		int i = 0;
		for(i = 0;i < iBufferCount;i++)
		{
			if(m_PrimaryBuffers->GetBuffPtr(i) == id)
			{
				m_PrimaryBuffers->GetBufferPtr()->SetAt(i,newID);
				
				m_address2MdfMode[(LPVOID)newID] = 1;
				return TRUE;
			}
		}
	}
	else
	{
		m_PrimaryBuffers->GetBufferPtr()->SetAt(iBufferID,newID);
		m_address2MdfMode[(LPVOID)newID] = 1;
		return TRUE;
	}
	return FALSE;
}

BOOL CPM_SharebufferDataSet::GetMdfBufferArr(CPtrArray &pRecordIDArr)
{
	Address2MdfMode::iterator iter = m_address2MdfMode.begin() ;
	for ( ; iter != m_address2MdfMode.end() ; iter++ )
	{
		pRecordIDArr.Add((LPVOID)iter->first);		
	}
	return TRUE;
}

BOOL CPM_SharebufferDataSet::SetAsString(int Column, NRecordID id ,const CString& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsString(Column,id,val);

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(1,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	NDataField * pTemField = m_fields.GetField(Column);
	return CPM_StorageDataSet::SetAsString(pTemField,newid,val);
}

BOOL CPM_SharebufferDataSet::SetAsFloat(int Column, NRecordID id ,const double& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsFloat(Column,id,val);

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(1,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	NDataField * pTemField = m_fields.GetField(Column);
	return CPM_StorageDataSet::SetAsFloat(pTemField,newid,val);
}

BOOL CPM_SharebufferDataSet::SetAsInteger(int Column, NRecordID id ,const int& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsInteger(Column,id,val);

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(1,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	NDataField * pTemField = m_fields.GetField(Column);
	return CPM_StorageDataSet::SetAsInteger(pTemField,newid,val);
}

BOOL CPM_SharebufferDataSet::SetAsVariant(int Column, NRecordID id ,const VARIANT& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsVariant(Column,id,val);

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(1,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	NDataField * pTemField = m_fields.GetField(Column);
	return CPM_StorageDataSet::SetAsVariant(pTemField,newid,val);
}

BOOL CPM_SharebufferDataSet::SetAsDateTime(int Column, NRecordID id ,const COleDateTime& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsDateTime(Column,id,val);

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(1,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	NDataField * pTemField = m_fields.GetField(Column);
	return CPM_StorageDataSet::SetAsDateTime(pTemField,newid,val);
}
	
BOOL CPM_SharebufferDataSet::SetAsBool(int Column, NRecordID id ,const BOOL & val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsBool(Column,id,val);

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(1,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	NDataField * pTemField = m_fields.GetField(Column);
	return CPM_StorageDataSet::SetAsBool(pTemField,newid,val);
}

BOOL CPM_SharebufferDataSet::SetAsString(const CString & Column, int row,const CString& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsString(Column,row,val);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetAsString_Field(pTemField,row,val);
}

BOOL CPM_SharebufferDataSet::SetAsFloat(const CString & Column, int row, const double& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsFloat(Column,row,val);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetAsFloat_Field(pTemField,row,val);
}

BOOL CPM_SharebufferDataSet::SetAsInteger(const CString & Column, int row,const int& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsInteger(Column,row,val);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetAsInteger_Field(pTemField,row,val);
}

BOOL CPM_SharebufferDataSet::SetAsVariant(const CString & Column, int row, const VARIANT& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsVariant(Column,row,val);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetAsVariant_Field(pTemField,row,val);
}

BOOL CPM_SharebufferDataSet::SetAsDateTime(const CString & Column, int row, const COleDateTime& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsDateTime(Column,row,val);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetAsDateTime_Field(pTemField,row,val);
}

BOOL CPM_SharebufferDataSet::SetAsBool(const CString & Column, int row,const BOOL & val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsBool(Column,row,val);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetAsBool_Field(pTemField,row,val);
}

BOOL CPM_SharebufferDataSet::SetAsString(int Column, int row ,const CString& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsString(Column,row,val);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetAsString_Field(pTemField,row,val);
}

BOOL CPM_SharebufferDataSet::SetAsFloat(int Column, int row ,const double& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsFloat(Column,row,val);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetAsFloat_Field(pTemField,row,val);
}

BOOL CPM_SharebufferDataSet::SetAsInteger(int Column, int row ,const int& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsInteger(Column,row,val);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetAsInteger_Field(pTemField,row,val);
}

BOOL CPM_SharebufferDataSet::SetAsVariant(int Column, int row ,const VARIANT& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsVariant(Column,row,val);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetAsVariant_Field(pTemField,row,val);
}

BOOL CPM_SharebufferDataSet::SetAsDateTime(int Column, int row ,const COleDateTime& val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsDateTime(Column,row,val);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetAsDateTime_Field(pTemField,row,val);
}

BOOL CPM_SharebufferDataSet::SetAsBool(int Column, int row ,const BOOL & val)
{
	if(FALSE == m_bHasShareBuffer)
		return CPM_StorageDataSet::SetAsBool(Column,row,val);

	NDataField * pTemField = m_fields.GetField(Column);
	return SetAsBool_Field(pTemField,row,val);
}

BOOL CPM_SharebufferDataSet::SetAsString_Field(NDataField * field, int iRow ,const CString& val)
{
	if(NULL == field)
		return FALSE;

	NRecordID id = GetRecordID(iRow);
	if(NULL == id)
		return FALSE;
 
	CString strOldValue = GetAsString(field,id);
	if((strOldValue.GetLength() == val.GetLength()) && 0 == _tcscmp(strOldValue,val))
		return FALSE;

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(iRow,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	return CPM_StorageDataSet::SetAsString(field,newid,val);
}

BOOL CPM_SharebufferDataSet::SetAsFloat_Field(NDataField * field, int iRow ,const double& val)
{
	if(NULL == field)
		return FALSE;

	NRecordID id = GetRecordID(iRow);
	if(NULL == id)
		return FALSE;

	double dOldValue = GetAsFloat(field,id);
	if(dOldValue == val)
		return FALSE;

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(iRow,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	return CPM_StorageDataSet::SetAsFloat(field,newid,val);
}

BOOL CPM_SharebufferDataSet::SetAsInteger_Field(NDataField * field, int iRow ,const int& val)
{
	if(NULL == field)
		return FALSE;

	NRecordID id = GetRecordID(iRow);
	if(NULL == id)
		return FALSE;

	int iOldValue = GetAsInteger(field,id);
	if(iOldValue == val)
		return FALSE;

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(iRow,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	return CPM_StorageDataSet::SetAsInteger(field,newid,val);
}

BOOL CPM_SharebufferDataSet::SetAsVariant_Field(NDataField * field, int iRow ,const VARIANT& val)
{
	if(NULL == field)
		return FALSE;

	NRecordID id = GetRecordID(iRow);
	if(NULL == id)
		return FALSE;

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(iRow,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	return CPM_StorageDataSet::SetAsVariant(field,newid,val);
}
BOOL CPM_SharebufferDataSet::SetAsDateTime_Field(NDataField * field, int iRow ,const COleDateTime& val)
{
	if(NULL == field)
		return FALSE;

	NRecordID id = GetRecordID(iRow);
	if(NULL == id)
		return FALSE;

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(iRow,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	return CPM_StorageDataSet::SetAsDateTime(field,newid,val);
}

BOOL CPM_SharebufferDataSet::SetAsBool_Field(NDataField * field, int iRow ,const BOOL & val)
{
	if(NULL == field)
		return FALSE;
	NRecordID id = GetRecordID(iRow);
	if(NULL == id)
		return FALSE;

	BOOL bOldValue = GetAsBool(field,id);
	if(bOldValue == val)
		return FALSE;

	NRecordID newid = id;
	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)id);
	if(iter == m_address2MdfMode.end())
	{
		/********************************************************************
		liwenlang
			时间:2010:10:8   11:07
			备注:创建新的地址存放 新的信息
		*********************************************************************/
		newid = BuildNewNRecordID(id);
		if(!ReplaceBuffer(iRow,id,newid))
		{
			delete [](BYTE*)newid;
			return FALSE;
		}
	}

	return CPM_StorageDataSet::SetAsBool(field,newid,val);
}

void CPM_SharebufferDataSet::DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff)
{
	NRecordID pRecordInfo = pBuff->GetBuffPtr(row - 1);

	Address2MdfMode::iterator iter = m_address2MdfMode.find((LPVOID)pRecordInfo);
	if(iter != m_address2MdfMode.end())
	{
		FreeRecordBuffer(pRecordInfo);
		m_address2MdfMode.erase(iter);
		pBuff->GetBufferPtr()->RemoveAt(row - 1);
		return;
	}

	pBuff->GetBufferPtr()->RemoveAt(row - 1);
}

BOOL CPM_SharebufferDataSet::CheckIsPrimaryDataSet(NDataModelDataSet * pCheckDataSet)
{
	if(m_pPrimaryDataSet == pCheckDataSet)
		return TRUE;
	return FALSE;
}

BOOL CPM_SharebufferDataSet::GetVersionID(short & iVersion)
{
	iVersion = m_iVersion;
	return TRUE;
}
