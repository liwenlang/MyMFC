// NDataModelDataSet.cpp: implementation of the NDataModelDataSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NDataModelDataSet.h"
#include "CDataSet_StringBuilder.h"

#include "CPub.h"
#include "NDataModelDataSet_Clone.h"
#include "NDataModelDataSet_ShareData.h"

#include "COXParser\OXParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE( NDataModelDataSet, CObject )

	NDataModelDataSet::NDataModelDataSet()
{
	m_PrimaryBuffers  = new NDataModelDataSetBuffer;
	m_DeleteBuffers  = new NDataModelDataSetBuffer;
	m_FilterBuffers  = new NDataModelDataSetBuffer;
	m_RecordBufferSize = 0;
	m_RecordSize = 0;
	m_PrimaryBuffers->SetOwner(this);
	m_DeleteBuffers->SetOwner(this);
	m_FilterBuffers->SetOwner(this);
	m_State = dmstNormal;
	m_IsModified = FALSE;
	m_pShardDataList = NULL;

	m_isGrouped = FALSE;
	//test
	//NDataFields m_fields1;
}

NDataModelDataSet::~NDataModelDataSet()
{
	Reset();
	delete m_PrimaryBuffers;
	delete m_DeleteBuffers;
	delete m_FilterBuffers;

	if (m_pShardDataList != NULL) 
	{
		//֪ͨ ���� ���ṹ�Ѿ�ɾ��
		for (int i = 0;i < m_pShardDataList->GetSize();i++)
		{
			((NDataModelDataSet_ShareData*)m_pShardDataList->GetAt(i))->OnSpyMainData_HasFree();
		}
		delete m_pShardDataList;
	}
	//�ͷŷ��������ڴ�
	FreeGroup();
}

BOOL	NDataModelDataSet::IsRowValid( const int iRow ) 
{
	/// 
	int iRowCount = GetRowCount() ; 

	/// 
	if (  iRow < 1 )
	{
		return FALSE ;
	}

	/// 
	if ( iRow > iRowCount )
	{
		return FALSE ;
	}

	return TRUE ;

}

void * NDataModelDataSet::AlloRecordBuffer()
{
	if (m_RecordBufferSize == 0) 
		return NULL;
	void * buff = new BYTE [m_RecordBufferSize];
	if (buff == NULL)
	{
		MessageBox(NULL,_T("�ڴ�����ʧ��!"),_T("����"),MB_OK);
	}
	ZeroMemory(buff,m_RecordBufferSize);
	return buff;
}

void NDataModelDataSet::InteralOpen()
{

	m_RecordSize = m_fields.GetRecordSize();
	m_RecordBufferSize = m_RecordSize + sizeof(RecInfo);

}

void NDataModelDataSet::CalFieldsSize()
{
	//m_CalcFieldsSize = m_fields->GetSize();
}

void NDataModelDataSet::FreeRecordBuffer(void *pBuffer)
{
	if (m_fields.m_HasBlob == TRUE) 
		FreeRecordBlobBuffer(pBuffer);

	FreeRecordOrigValueBuffer(pBuffer);
	delete []((BYTE*)pBuffer);
}
void NDataModelDataSet::FreeRecordBlobBuffer(void *pBuffer)
{
	if (m_fields.m_HasBlob == TRUE) 
	{
		NDataField *field;
		for(int i = 0 ; i < m_fields.m_Bloblist.GetSize() ; i++)
		{
			field = (NDataField *)m_fields.m_Bloblist[i];
			field->FreeBolbData(GetFieldDataPT(field,pBuffer,FALSE));
		}
	}
}
void NDataModelDataSet::FreeRecordOrigValueBuffer(void *pBuffer)
{
	if(GetRowModifiedInternal((char*)pBuffer))
	{
		int totalfield = GetFieldCount();
		//void *iteminfo;
		NDataField *field;
		for(int i = 0; i < totalfield ; i++)
		{
			//iteminfo = GetItemInfo(pBuffer,m_fields.GetField(i));
			//testwnd
			/*char * aa = (char *)iteminfo;
			BYTE byte = (BYTE)(aa--);
			CopyMemory(&byte,aa,sizeof(BYTE));*/
			//char * origptr;
			//CopyMemory(&origptr,iteminfo,sizeof(char*));
			//if((origptr) != NULL)
			//{
			//	   delete [](origptr);
			//}
			field = m_fields.GetField(i);
			field->FreeOrigValueBuff((char*)GetFieldDataPT(field,pBuffer,FALSE));
		}
	}
}
//
//eGetResult NDataModelDataSet::GetRecord(char *pbuff, eGetMode getmode)
//{
//	eGetResult result;
//	if (m_RecordCount < 1 ) 
//		result = grEOF;
//	else
//	{
//		result = grOK;
//		switch (getmode)
//		{
//		case gmNext: 
//		    if (m_CurrentRecord >= m_RecordCount -1)
//				result = grEOF;
//			else
//				m_CurrentRecord++;
//
//		case gmPrior:
//            if (m_CurrentRecord <=0 )
//				result = grBOF;
//			else
//				m_CurrentRecord--;
//
//		case gmCurrent:
//			if((m_CurrentRecord >= m_RecordCount)||(m_CurrentRecord <0 ))
//			  result = grError;
//		}	
//	}
//    //��ʼ��������װ��
//	if (result = grOK)
//	{
//		
//	}
//	return result;
//}

// BOOL NDataModelDataSet::SetFilter(LPSTR filterstr)
// {
// 	return FALSE;
// 
// }

int NDataModelDataSet::GetFieldOffset(NDataField *field) const
{
	ASSERT(AfxIsValidAddress(field,sizeof(NDataField)));	

	/*if (field->m_fieldNo<0||(field->m_fieldNo+1)>=m_fields.GetCount()) return 0;

	m_FieldOffset.GetAt(field->m_fieldNo);
	return 0;*/
	return m_fields.GetFieldOffset(field);

}

//ɾ��������ڴ�
void NDataModelDataSet::Reset()
{
	m_PrimaryBuffers->ResetBuffers();m_DeleteBuffers->ResetBuffers();
	m_FilterBuffers->ResetBuffers();
}

int NDataModelDataSet::LoadFromXML(BSTR xml)
{

	return _ImportXML( xml, FALSE, FALSE, FALSE);
}

//���ݴ�ȡ
//������Ҫ���иľ��������ͼ��ȡһ�������ڵ���Ӧ���׳�һ������
CString NDataModelDataSet::GetAsString(int Column,int row, eBufferFlag BufferFlag , BOOL originalvalue ) const
{
	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		return _T("");
	}
	return  GetAsString(Column,pTemRecordID,originalvalue);
}
double  NDataModelDataSet::GetAsFloat(int Column,int row, eBufferFlag BufferFlag , BOOL originalvalue ) const
{
	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		return 0;
	}
	return  GetAsFloat(Column,pTemRecordID,originalvalue);
}
int     NDataModelDataSet::GetAsInteger(int Column,int row, eBufferFlag BufferFlag , BOOL originalvalue ) const
{
	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		return 0;
	}
	return  GetAsInteger(Column,pTemRecordID,originalvalue);
}
VARIANT NDataModelDataSet::GetAsVariant(int Column,int row, eBufferFlag BufferFlag , BOOL originalvalue ) const
{
	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		VARIANT var;
		var.vt = VT_EMPTY;
		return var;
	}
	return  GetAsVariant(Column,pTemRecordID,originalvalue);
}

COleDateTime  NDataModelDataSet::GetAsDateTime(int Column,int row, eBufferFlag BufferFlag , BOOL originalvalue ) const
{
	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		COleDateTime dt;
		return dt;
	}
	return  GetAsDateTime(Column,pTemRecordID,originalvalue);
}

BOOL    NDataModelDataSet::GetAsBool(int Column,int row, eBufferFlag BufferFlag , BOOL originalvalue ) const
{
	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}
	return  GetAsBool(Column,pTemRecordID,originalvalue);
}

BOOL    NDataModelDataSet::SetAsString(int Column, int row ,const CString& val)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}
	return SetAsString(field,pTemRecordID,val);
}

BOOL    NDataModelDataSet::SetAsFloat(int Column, int row ,const double& val)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}
	return SetAsFloat(field,pTemRecordID,val);
}
BOOL    NDataModelDataSet::SetAsInteger(int Column, int row ,const int& val)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}

	return SetAsInteger(field,pTemRecordID,val);
}

BOOL    NDataModelDataSet::SetAsVariant(int Column, int row ,const VARIANT& val)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}

	return SetAsVariant(field,pTemRecordID,val);
}

BOOL    NDataModelDataSet::SetAsDateTime(int Column, int row ,const COleDateTime& val)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}

	return SetAsDateTime(field,pTemRecordID,val);
}

BOOL    NDataModelDataSet::SetAsBool(int Column, int row ,const BOOL & val)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}
	return SetAsBool(field,pTemRecordID,val);
}

long NDataModelDataSet::GetRowCount() const
{
	if (NULL == m_PrimaryBuffers)
	{
		return 0;
	}

	return m_PrimaryBuffers->GetRowCount();
}

int NDataModelDataSet::InsertRow(int iberforrow)
{
	iberforrow--;//ת��Ϊindex
	void * newbuff = AlloRecordBuffer();
	int newrow = m_PrimaryBuffers->InsertBuff(newbuff,iberforrow) +1;//���ص���index
	if (m_State == dmstNormal) {
		SetRowNewInternal(newbuff,TRUE);
		OnInsertedRow(newrow);
		if (m_pShardDataList != NULL) 
		{
			for(int i = 0 ; i < m_pShardDataList->GetSize(); i++)
			{
				((NDataModelDataSet_ShareData*)m_pShardDataList->GetAt(i))->OnParentInsert(newbuff);
			}
		}
	}
	//int newrow = m_PrimaryBuffers->InsertBuff(newbuff,iberforrow);

	return newrow;
}

NRecordID  NDataModelDataSet::GetRecordID(int row, eBufferFlag BufferFlag  ) const
{
	if (BufferFlag == Primary)
		return m_PrimaryBuffers->GetBuffPtr(row - 1);
	else 
		if(BufferFlag == Filter)
			return m_FilterBuffers->GetBuffPtr(row - 1);
		else
			return m_DeleteBuffers->GetBuffPtr(row - 1);
}

//ʹ��id�Ž������ݴ�ȡ
CString NDataModelDataSet::GetAsString(int Column,NRecordID id, BOOL originalvalue) const
{
	//	 void *ptr;
	NDataField *field = m_fields.GetField(Column);

	//if (field == NULL ) return FALSE;
	//ptr = GetFieldDataPT(field,id);
	//CString ss("this is test and new row");
	/*if (ptr == NULL) { return ss;
	}*/
	//return field->GetAsString(ptr);
	return GetAsString(field,id,originalvalue);
}
double  NDataModelDataSet::GetAsFloat(int Column,NRecordID id, BOOL originalvalue ) const
{
	//void *ptr;
	NDataField * field = m_fields.GetField(Column);
	//if (field == NULL ) return FALSE;
	/*ptr = GetFieldDataPT(field,id);
	return field->GetAsFloat(ptr);*/
	return GetAsFloat(field,id,originalvalue);
}

int     NDataModelDataSet::GetAsInteger(int Column,NRecordID id, BOOL originalvalue ) const
{
	// void *ptr;
	NDataField * field = m_fields.GetField(Column);
	//if (field == NULL ) return FALSE;
	/*ptr = GetFieldDataPT(field,id);
	return field->GetAsInteger(ptr);*/
	return GetAsInteger(field,id,originalvalue);
}

VARIANT NDataModelDataSet::GetAsVariant(int Column,NRecordID id, BOOL originalvalue ) const
{
	//void *ptr;
	NDataField * field = m_fields.GetField(Column);
	//if (field == NULL ) return FALSE;
	/*ptr = GetFieldDataPT(field,id);
	return field->GetAsVariant(ptr);*/
	return GetAsVariant(field,id,originalvalue);
}

COleDateTime  NDataModelDataSet::GetAsDateTime(int Column,NRecordID id, BOOL originalvalue) const
{
	//void *ptr;
	NDataField * field = m_fields.GetField(Column);
	//if (field == NULL ) return FALSE;
	/*ptr = GetFieldDataPT(field,id);
	return field->GetAsDateTime(ptr);*/
	return GetAsDateTime(field,id,originalvalue);
}

BOOL    NDataModelDataSet::GetAsBool(int Column,NRecordID id, BOOL originalvalue ) const
{
	//void *ptr;
	NDataField * field = m_fields.GetField(Column);
	//if (field == NULL ) return FALSE;
	/*ptr = GetFieldDataPT(field,id);
	return field->GetAsBoolean(ptr);*/
	return GetAsBool(field,id,originalvalue);
}


BOOL    NDataModelDataSet::SetAsString(int Column, NRecordID id ,const CString& val)
{
	NDataField * field = m_fields.GetField(Column);
	/*void * buffer = GetFieldDataPT(field ,id );
	return field->SetAsString(val,buffer);*/
	return SetAsString(field,id,val);
}

BOOL    NDataModelDataSet::SetAsFloat(int Column, NRecordID id ,const double& val)
{
	NDataField * field = m_fields.GetField(Column);
	/*void * buffer = GetFieldDataPT(field ,id );
	return field->SetAsFloat(val,buffer);*/
	return SetAsFloat(field,id,val);
}

BOOL    NDataModelDataSet::SetAsInteger(int Column, NRecordID id ,const int& val)
{
	NDataField * field = m_fields.GetField(Column);
	/*void * buffer = GetFieldDataPT(field ,id );
	return field->SetAsInteger(val,buffer);*/
	return SetAsInteger(field,id,val);
}

BOOL    NDataModelDataSet::SetAsVariant(int Column, NRecordID id ,const VARIANT& val)
{
	NDataField * field = m_fields.GetField(Column);
	/*void * buffer = GetFieldDataPT(field ,id );
	return field->SetAsVariant(val,buffer);*/
	return SetAsVariant(field,id,val);
}

BOOL    NDataModelDataSet::SetAsDateTime(int Column, NRecordID id ,const COleDateTime& val)
{
	NDataField * field = m_fields.GetField(Column);
	/*	void * buffer = GetFieldDataPT(field ,id );
	return field->SetAsDateTime(val,buffer);*/
	return SetAsDateTime(field,id,val);
}

BOOL    NDataModelDataSet::SetAsBool(int Column, NRecordID id ,const BOOL & val)
{
	NDataField * field = m_fields.GetField(Column);
	/*void * buffer = GetFieldDataPT(field ,id );
	return field->SetAsBoolean(val,buffer);*/
	return SetAsBool(field,id,val);
}


//ʹ���ַ��������������ݴ洢

CString NDataModelDataSet::GetAsString(const CString & Column,int row, eBufferFlag BufferFlag , BOOL originalvalue ) const
{
	CString sValue;
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL )
	{
		//		PMDataSetShowErr(_T("NDataModelDataSet::GetAsString����������,") + Column);
		return sValue;
	}

	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		return sValue;
	}

	return  GetAsString(field,pTemRecordID,originalvalue);
}

double  NDataModelDataSet::GetAsFloat(const CString & Column,int row, eBufferFlag BufferFlag , BOOL originalvalue ) const
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return 0;

	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		return 0;
	}
	return  GetAsFloat(field,pTemRecordID,originalvalue);
}

int     NDataModelDataSet::GetAsInteger(const CString & Column,int row, eBufferFlag BufferFlag , BOOL originalvalue ) const
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return 0;

	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		return 0;
	}

	return  GetAsInteger(field,pTemRecordID,originalvalue);
}

VARIANT NDataModelDataSet::GetAsVariant(const CString & Column,int row, eBufferFlag BufferFlag , BOOL originalvalue ) const
{
	VARIANT dresult;
	dresult.vt = VT_EMPTY;
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return dresult;

	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		return dresult;
	}
	return  GetAsVariant(field,pTemRecordID,originalvalue);
}

COleDateTime  NDataModelDataSet::GetAsDateTime(const CString & Column,int row, eBufferFlag BufferFlag , BOOL originalvalue ) const
{
	COleDateTime Result;
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return Result;

	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		return Result;
	}

	return  GetAsDateTime(field,pTemRecordID,originalvalue);
}

BOOL    NDataModelDataSet::GetAsBool(const CString & Column,int row, eBufferFlag BufferFlag , BOOL originalvalue ) const
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}

	return  GetAsBool(field,pTemRecordID,originalvalue);
}

BOOL    NDataModelDataSet::SetAsString(const CString & Column, int row ,const CString& val)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}

	return  SetAsString(field,pTemRecordID,val);
}

BOOL    NDataModelDataSet::SetAsFloat(const CString & Column, int row ,const double& val)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}

	return  SetAsFloat(field,pTemRecordID,val);
}

BOOL    NDataModelDataSet::SetAsInteger(const CString & Column, int row ,const int& val)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}

	return  SetAsInteger(field,pTemRecordID,val);
}

BOOL    NDataModelDataSet::SetAsVariant(const CString & Column, int row ,const VARIANT& val)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}

	return  SetAsVariant(field,pTemRecordID,val);
}

BOOL    NDataModelDataSet::SetAsDateTime(const CString & Column, int row ,const COleDateTime& val)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}

	return  SetAsDateTime(field,pTemRecordID,val);
}

BOOL    NDataModelDataSet::SetAsBool(const CString & Column, int row ,const BOOL & val)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return FALSE;
	}

	return  SetAsBool(field,pTemRecordID,val);
}

long NDataModelDataSet::GetBlob(const CString & Column, int row ,void *& ppBolb,eBufferFlag BufferFlag , BOOL originalvalue) const
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		ppBolb = NULL;
		return 0;
	}

	return GetBlob(field, pTemRecordID ,ppBolb);
}

long NDataModelDataSet::GetBlob(int iColumn, int row ,void *& ppBolb,eBufferFlag BufferFlag , BOOL originalvalue) const
{
	NDataField * field = m_fields.GetField(iColumn);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		ppBolb = NULL;
		return 0;
	}

	return GetBlob(field, pTemRecordID ,ppBolb);
}

BOOL NDataModelDataSet::SetBlob(const CString & Column, int row ,void * ppBolb,long iSize)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		return 0;
	}

	return SetBlob(field,pTemRecordID,ppBolb,iSize);
}

BOOL NDataModelDataSet::SetBlob(int Column, int row ,void * ppBolb,long iSize)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL ) return FALSE;

	NRecordID pTemRecordID = GetRecordID(row,Primary);
	if(NULL == pTemRecordID)
	{
		ppBolb = NULL;
		return 0;
	}

	return SetBlob(field,pTemRecordID,ppBolb,iSize);
}

BOOL NDataModelDataSet::SetBlob(int Column, NRecordID id ,void * ppBolb,long iSize)
{
	NDataField * field = m_fields.GetField(Column);
	if (field == NULL )
		return FALSE;
	return SetBlob(field,id,ppBolb,iSize);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
int NDataModelDataSet::LoadFromTextFile(LPTSTR sFileName,_TCHAR delim)
{
	SetState(dmstRetrieve);
	int row = 0;
	char aaa[10000];
	char aaa1[1000];

#ifdef _UNICODE
	USES_CONVERSION;
	LPSTR pTemp=W2A(sFileName);
	ifstream f(pTemp);
#else
	ifstream f(sFileName);
#endif

	int col;
	//f.getline(aaa,12);
	while (f.getline(aaa,10000)) /*,'\t')*/
	{
		int insertrow;
		insertrow = this->InsertRow(0);
		istrstream str(aaa);

		col = 0;
		while (str.getline(aaa1,1000,(char)delim))//'\t'))
		{
			if (col > m_fields.GetCount() - 1) {
				continue;
			}
			CString val(aaa1);
			this->SetAsString(col,insertrow,val);
			col++;
		}
		row ++;
	}
	SetState(dmstNormal);
	return row;
}

int NDataModelDataSet::LoadFromCString(CString &sbuff,char delim,BOOL isnewRecord)
{
	if(!isnewRecord)SetState(dmstRetrieve);
	int row = 0;
	char *aaa = new char[10000];
	char *aaa1 = new char[10000];
	COleDateTime tempDT;
	//ifstream f(sFileName);
	//istrstream f(sbuff.GetBuffer(0));
#ifdef _UNICODE
	USES_CONVERSION;
	LPTSTR strBuffer = sbuff.GetBuffer(0);
	LPSTR pTemp=W2A(strBuffer);
	istrstream f(pTemp);
#else
	istrstream f(sbuff.GetBuffer(0));
#endif
	int col;
	//f.getline(aaa,12);
	while (f.getline(aaa,10000)) /*,'\t')*/
	{
		int insertrow;
		insertrow = this->InsertRow(0);
		istrstream str(aaa);

		col = 0;
		while (str.getline(aaa1,1000,delim))//'\t'))
		{
			if (col > m_fields.GetCount() - 1) {
				continue;
			}
			CString val(aaa1);
			val.TrimRight();
			//����ComDB��������RecordSet���ڸ�ʽΪ"YYYY.MM.DD"���Խ��и�ʽת��
			if(GetFieldType(col) == ftDateTime)
			{
				tempDT.ParseDateTime(val);
				if ((tempDT.GetStatus() == COleDateTime::invalid)&&val.GetLength() > 0) 
				{
					tempDT = CPub::CString2COleDateTime(val);

					//tempDT = dt1;
					val = tempDT.Format();
				}
			}
			this->SetAsString(col,insertrow,val);
			col++;
		}
		row ++;
	}

	delete [] aaa;
	delete [] aaa1;

	if(!isnewRecord)SetState(dmstNormal);
	return row;
}

BOOL NDataModelDataSet::OnRowFilter(int row)
{
	return TRUE;
}

void NDataModelDataSet::FillFilterBuff(CPtrArray &filterBuffer)
{
	if (filterBuffer.GetSize()>0) {
		for(int i = 0; i < filterBuffer.GetSize(); i++)
		{
			m_FilterBuffers->AddBuff(filterBuffer[i]);
		}
		//m_FilterBuffers->m_Buffers.Append(filterBuffer);
	}
}

void NDataModelDataSet::FillFilterBuff(LPVOID *pBuffers,LONG lBufferCount)
{
	if(lBufferCount > 0)
	{
		for (int i = 0;i < lBufferCount;i++)
		{
			m_FilterBuffers->AddBuff(pBuffers[i]);
		}

		////////////////////////////////////////////
		/// liwenlang [2012:2:21   12:31]
		/// ˵��:
		/// ֱ�Ӷ� CArray�Ĵ�С�����趨������ʡ���ظ����ٵ�ַ�� ���ġ�
// 		int iOldFilterCount = m_FilterBuffers->m_Buffers.GetSize();
// 		int iOldBufferSize = sizeof(LPVOID) * iOldFilterCount;
// 		m_FilterBuffers->m_Buffers.SetSize(lBufferCount + iOldFilterCount);
// 		LPVOID * pBufferData = m_FilterBuffers->m_Buffers.GetData();
// 		if(iOldFilterCount > 0)
// 		{//�� ֮ǰ�� �������Ᵽ��
// 			CPtrArray temBuffers;
// 			temBuffers.Append(m_FilterBuffers->m_Buffers);
// 
// 			//����֮ǰ�Ĺ�����Ϣ
// 			memcpy(pBufferData,temBuffers.GetData(),iOldBufferSize);
// 		}
// 		memcpy(pBufferData + iOldBufferSize,pBuffers,sizeof(LPVOID) * lBufferCount);
	}
}

void NDataModelDataSet::DoFilter()
{
	m_PrimaryBuffers->Filter();
}

BOOL NDataModelDataSet::SetFilter(CString ColName,LPTSTR sOperator ,LPTSTR sValue)
{
	NDataField *field = GetField(GetFieldIndex(ColName));
	if (field != NULL) {
		return field->SetFilter(sOperator,sValue);
	}
	return FALSE;
}


BOOL NDataModelDataSet::SetFilter(int ColIndex,LPTSTR sOperator ,LPTSTR sValue)
{
	NDataField *field = GetField(ColIndex);
	if (field != NULL) {
		return field->SetFilter(sOperator,sValue);
	}
	return FALSE;
}


int NDataModelDataSet::RowsCopy(int startrow, int endrow,eBufferFlag copybuffer, NDataModelDataSet *TargetData, int beforerow,eBufferFlag targetbuffer)
{
	//�ж����ݴ洢���������ݽṹ�Ƿ�һ��
	NDataFields * TargetFields = TargetData->GetFieldsObj();
	int ifield = TargetFields->GetCount();
	if (m_fields.GetCount() != ifield) {
		return -1;
	}
	//�ж��ֶε����������Ƿ�һ��
	NDataField *field1,*field2;
	int i = 0;
	for(i = 0; i < ifield; i++)
	{
		field1 = m_fields.GetField(i);
		field2 = TargetFields->GetField(i);
		if (field1->GetDataType() != field2->GetDataType()) {
			return -1;
		}
		if (field1->GetSize() != field2->GetSize()) {
			return -1;
		}
	}
	//�������ݿ���
	int copyedrow = 0;
	NRecordID newrecord;

	int iInsertBeforeRow = beforerow;
	iInsertBeforeRow--;
	if ( iInsertBeforeRow < -1 )
	{
		iInsertBeforeRow = -1;
	}
	for(i = startrow; i <= endrow; i++)
	{
		newrecord = CloneRecord(i,copybuffer);
		switch (targetbuffer) {
		case Primary:
			TargetData->m_PrimaryBuffers->InsertBuff(newrecord,iInsertBeforeRow);
			break;
		case Delete:
			TargetData->m_DeleteBuffers->InsertBuff(newrecord,iInsertBeforeRow);
			break;
		case Filter:
			TargetData->m_FilterBuffers->InsertBuff(newrecord,iInsertBeforeRow);
			break;
		}

		///////////////////////////////////
		///liwenlang 
		///Ϊ�ڸ��ƺ���� ����Update Insert���
		///[20/7/2009]
		//////////////////////////////////
		SetRowNewInternal(newrecord,TRUE);

		/// ��Ҫ������������ʱ��++�ᵼ�´ӵڶ��п�ʼ���뵽����ǰ��
		if ( beforerow >= 0 )
		{
			iInsertBeforeRow++;
		}
		copyedrow ++;
	}
	//���
	return copyedrow;
}

//////////////////////////////////////////////////////////////////////////
//��¡һ����¼����
NRecordID NDataModelDataSet::CloneRecord(int row,eBufferFlag buffer)
{
	NRecordID record = NULL,TargetRecord = NULL;
	if (row <= 0) {
		return NULL;
	}
	switch (buffer)
	{
	case Primary:
		if (row > m_PrimaryBuffers->GetRowCount()) {
			return record;
		}
		record = AlloRecordBuffer();
		TargetRecord = m_PrimaryBuffers->GetBuffPtr(row - 1);
		break;
	case Delete:
		if (row > m_DeleteBuffers->GetRowCount()) {
			return record;
		}
		record = AlloRecordBuffer();
		TargetRecord = m_DeleteBuffers->GetBuffPtr(row - 1);
		break;
	case Filter:
		if (row > m_FilterBuffers->GetRowCount()) {
			return record;
		}
		record = AlloRecordBuffer();
		TargetRecord = m_FilterBuffers->GetBuffPtr(row - 1);
		break;
	}
	if(record && TargetRecord)
		CopyMemory(record,TargetRecord,m_RecordBufferSize);
	//��ʼclone blob����
	if (m_fields.m_HasBlob == TRUE) 
	{
		NDataField *field;
		void *pBlob;
		int iSize;
		for(int i = 0 ; i < m_fields.m_Bloblist.GetSize() ; i++)
		{
			field = (NDataField *)m_fields.m_Bloblist[i];

			pBlob = NULL;
			iSize = field->GetBlob(GetFieldDataPT(field,TargetRecord,FALSE),pBlob);
			if (pBlob != NULL && iSize>0) 
			{
				//�Ƚ���¼�еĴ��ֶ�ָ�����,�������գ�����������������ֵʱ���ڱȶ���ͬ���µ����ò��ɹ�
				void *pRowbufferOffset = GetFieldDataPT(field,record,FALSE);
				ZeroMemory(pRowbufferOffset,sizeof(void*));
				ZeroMemory((char*)pRowbufferOffset+sizeof(void*),sizeof(long));
				//��ӽ���07.3.3
				//�����setBlob������û�н���־λ�������ã���ˣ����ԭ���ݷ����仯��û�з����仯���������᲻ͬ��Ŀǰû�з��ֻ����ļ���
				//����Ҳ��һ���ķ���
				field->SetBlob(GetFieldDataPT(field,record,FALSE),pBlob,iSize,FALSE); //2008.3.4 ��ֹÿ���ֶ���4���ֽڵ��ڴ�й©

			}


		}
	}
	//��ʼ�������ñ�־2003.6.18
	NDataField *field = NULL;
	void *ptr;
	for(int i = 0 ; i < m_fields.GetCount(); i++)
	{
		field = m_fields.GetField(i);
		if (field != NULL) 
		{
			ptr = GetFieldDataPT(field,record);
			field->FreeOrigValueBuff(ptr,FALSE);
			field->SetItemModified((char*)ptr,FALSE);
		}
	}
	//void * GetFieldDataPT(NDataField *field,void *buff, BOOL originalvalue = FALSE);
	return record;
}

int NDataModelDataSet::GetColNum(CString ColName)
{
	return GetFieldIndex(ColName);
}

/*NRecordID NDataModelDataSet::GetRecordID(int row,eBufferFlag bufferflag)
{
if (bufferflag == Primary)
return m_PrimaryBuffers->m_Buffers.GetAt(row - 1);
else 
if(bufferflag == Filter)
return m_FilterBuffers->m_Buffers.GetAt(row - 1);
else
return m_DeleteBuffers->m_Buffers.GetAt(row - 1);
}*/

//////////////////////////////////////////////////////////////////////////
//     �������ݴ�ȡ����

inline CString NDataModelDataSet::GetAsString(NDataField * field,NRecordID id, BOOL originalvalue) const
{
	void *ptr = GetFieldDataPT(field,id,originalvalue);
	return field->GetAsString(ptr);
}
inline double  NDataModelDataSet::GetAsFloat(NDataField * field,NRecordID id, BOOL originalvalue ) const
{
	void *ptr = GetFieldDataPT(field,id,originalvalue);
	return field->GetAsFloat(ptr);
}
inline int     NDataModelDataSet::GetAsInteger(NDataField * field,NRecordID id, BOOL originalvalue ) const
{
	void *ptr = GetFieldDataPT(field,id,originalvalue);
	return field->GetAsInteger(ptr);
}
inline VARIANT NDataModelDataSet::GetAsVariant(NDataField * field,NRecordID id, BOOL originalvalue ) const
{
	void *ptr = GetFieldDataPT(field,id,originalvalue);
	return field->GetAsVariant(ptr);
}
inline COleDateTime  NDataModelDataSet::GetAsDateTime(NDataField * field,NRecordID id, BOOL originalvalue) const
{
	void *ptr = GetFieldDataPT(field,id,originalvalue);
	return field->GetAsDateTime(ptr);
}
inline BOOL    NDataModelDataSet::GetAsBool(NDataField * field,NRecordID id, BOOL originalvalue ) const
{
	void * ptr = GetFieldDataPT(field ,id ,originalvalue);
	return field->GetAsBoolean(ptr);
}

inline BOOL    NDataModelDataSet::SetAsString(NDataField * field, NRecordID id ,const CString& val)
{
	void * buffer = GetFieldDataPT(field ,id );
	return(field->SetAsString(val,buffer));

}
inline BOOL    NDataModelDataSet::SetAsFloat(NDataField * field, NRecordID id ,const double& val)
{
	void * buffer = GetFieldDataPT(field ,id );
	return( field->SetAsFloat(val,buffer));

}
inline BOOL    NDataModelDataSet::SetAsInteger(NDataField * field, NRecordID id ,const int& val)
{
	void * buffer = GetFieldDataPT(field ,id );
	return( field->SetAsInteger(val,buffer));

}
inline BOOL    NDataModelDataSet::SetAsVariant(NDataField * field, NRecordID id ,const VARIANT& val)
{
	void * buffer = GetFieldDataPT(field ,id );
	return( field->SetAsVariant(val,buffer));

}
inline BOOL    NDataModelDataSet::SetAsDateTime(NDataField * field, NRecordID id ,const COleDateTime& val)
{
	void * buffer = GetFieldDataPT(field ,id );
	return( field->SetAsDateTime(val,buffer));

}
inline BOOL    NDataModelDataSet::SetAsBool(NDataField * field, NRecordID id ,const BOOL & val)
{
	void * buffer = GetFieldDataPT(field ,id );
	return( field->SetAsBoolean(val,buffer));

}

long NDataModelDataSet::GetBlob(NDataField * field, NRecordID id ,void *& ppBolb) const
{
	if (field->GetDataType() != ftBlob) 
	{
		ppBolb = NULL;
		return 0;
	}
	void * buffer = GetFieldDataPT(field ,id);
	return field->GetBlob(buffer,ppBolb);
}

long NDataModelDataSet::GetBlob(int Column, NRecordID id ,void *& ppBolb) const
{
	NDataField * pBlobField = GetField(Column);
	if(NULL == pBlobField)
		return 0;
	return GetBlob(pBlobField,id,ppBolb);
}

BOOL NDataModelDataSet::SetBlob(NDataField * field, NRecordID id ,void * ppBolb,long iSize)
{
	////////////////////////////////////////////
	/// liwenlang [2011:12:2   10:19]
	/// ˵��:
	/// ��Ҫ��Blob�ֶ��ÿ�ֵ�����н⿪���� ppBolb == NULL
	if (field->GetDataType() != ftBlob /*|| ppBolb == NULL*/) 
	{
		return FALSE;
	}
	void * buffer = GetFieldDataPT(field ,id);
	return field->SetBlob(buffer,ppBolb,iSize);
}

//end

void NDataModelDataSet::SetRowModified(int row,BOOL bModified, eBufferFlag BufferFlag)
{
	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		return ;
	}

	SetRowModifiedInternal(pTemRecordID,bModified);
}

void NDataModelDataSet::SetRowModifiedInternal(void * buff,BOOL bModified)
{
	RecInfo *precinfo;
	precinfo = GetRowRecInfo(buff);
	if (precinfo != NULL) 
	{
		if(bModified) 
			precinfo->Status |= 0x02; 
		else 
			precinfo->Status &= ~(0x02);
		//m_RecordSize
	}
	//add by zhangym 2003.6.11
	if (m_IsModified == FALSE) 
	{
		m_IsModified = TRUE;
	}
}

BOOL NDataModelDataSet::GetRowModified(int row, eBufferFlag BufferFlag)
{
	//RecInfo *precinfo;
	char * ptr =(char *) GetRecordID(row,BufferFlag);
	if(NULL == ptr)
		return FALSE;

	return GetRowModifiedInternal(ptr);

}

BOOL NDataModelDataSet::GetRowModifiedInternal(char * pbuff)
{
	RecInfo *precinfo = GetRowRecInfo(pbuff);
	if (precinfo != NULL) 
	{
		return (precinfo->Status & 0x02) ? TRUE:FALSE; 
	}
	return FALSE;
}

void NDataModelDataSet::SetItemModified(int row, int col, BOOL bModified, eBufferFlag BufferFlag)
{
	NRecordID pTemRecordID = GetRecordID(row,BufferFlag);
	if(NULL == pTemRecordID)
	{
		return ;
	}
	SetItemModifiedInternal(pTemRecordID,m_fields.GetField(col),bModified);
}

void NDataModelDataSet::SetItemModifiedInternal(NRecordID recordid, NDataField *field, BOOL bModified)
{
	field->SetItemModified((char*)GetFieldDataPT(field,recordid,FALSE),bModified);
}


BOOL NDataModelDataSet::GetItemModified(int row, int col, eBufferFlag BufferFlag)
{
	/*BYTE *iteminfo = (BYTE *)GetItemInfo(row,col);
	if (iteminfo != NULL) 
	{
	return ((*iteminfo) & 0x01) ? TRUE:FALSE; 
	}
	return FALSE;*/

	NRecordID recordid = GetRecordID(row,BufferFlag);
	if(NULL == recordid)
		return FALSE;

	return GetItemModifiedInternal( recordid,  col);

}
BOOL NDataModelDataSet::GetItemModifiedInternal(NRecordID recordid, int col)
{
	NDataField *field = m_fields.GetField(col);
	if(NULL == field)
		return FALSE;

	return field->GetItemModified((char*)GetFieldDataPT(field,recordid,FALSE));
}

void NDataModelDataSet::SetRowNew(int row, BOOL isnew, eBufferFlag BufferFlag)
{
	//RecInfo *precinfo;
	char * ptr =(char *) GetRecordID(row,BufferFlag);

	if(NULL == ptr)
		return;

	SetRowNewInternal(ptr,isnew);
}

inline void NDataModelDataSet::SetRowNewInternal(void * pbuff, BOOL isnew)
{
	RecInfo *precinfo;
	precinfo = GetRowRecInfo(pbuff);
	if (precinfo != NULL) 
	{
		if(isnew) 
			precinfo->Status |= 0x01; 
		else 
			precinfo->Status &= ~(0x01);
	}
	//add by zhangym 2003.6.11
	if (m_IsModified == FALSE) 
	{
		m_IsModified = TRUE;
	}
}

BOOL NDataModelDataSet::GetRowNew(int row, eBufferFlag BufferFlag)
{
	char * ptr =(char *) GetRecordID(row,BufferFlag);
	if(NULL == ptr)
		return FALSE;

	return GetRowNewInternal(ptr);
}

BOOL NDataModelDataSet::GetRowNewInternal(void * pbuff)
{
	RecInfo *precinfo;
	precinfo = GetRowRecInfo(pbuff);
	if (precinfo != NULL) 
	{
		return (precinfo->Status & 0x01) ? TRUE:FALSE; 
		//return TRUE;
	}
	return FALSE;
}
NDataModelDataSet::RecInfo * NDataModelDataSet::GetRowRecInfo(void *pBuff)
{
	RecInfo *precinfo = NULL;
	char * ptr =(char *) pBuff;
	if (ptr != NULL) 
	{
		ptr += m_RecordSize;
		precinfo = (RecInfo *)ptr;
	}
	return precinfo;
}

void* NDataModelDataSet::GetRowBuffPtr(NDataField *field,char *pBuff)
{
	//�Ȼص���Buff�Ŀ�ʼ��
	int iFieldOffset;
	iFieldOffset = GetFieldOffset(field);
	(pBuff) -=iFieldOffset;
	//ptr -= iFieldOffset;
	return pBuff;
}

/*void* NDataModelDataSet::GetItemInfo(int row, int col)
{
char *pbuff =(char *)  GetRecordID(row);
NDataField * field = m_fields.GetField(col);
return GetItemInfo((void *)pbuff,field);
}

void* NDataModelDataSet::GetItemInfo(NRecordID recordid, NDataField *field)
{
char *pbuff =(char *) recordid;
if (field == NULL || recordid == NULL)
return NULL;
int iFieldOffset;
iFieldOffset = GetFieldOffset(field);
pbuff+= iFieldOffset + field->GetSize() + 1;//1BYTE ��״̬λ��;
return pbuff;
}
*/

inline eDataModelDataSetState NDataModelDataSet::GetState()
{
	return m_State;
}

inline void NDataModelDataSet::SetState(eDataModelDataSetState newstate)
{
	m_State = newstate;
}

BOOL NDataModelDataSet::Update(CString &TableName, BOOL bByPK, BOOL bAutoCommit)
{
	CStringArray arrSQLDelete, arrSQLUpdate, arrSQLInsert;
	return Update(TableName,bByPK,bAutoCommit,arrSQLDelete,arrSQLUpdate,arrSQLInsert);
}

BOOL NDataModelDataSet::Update(CString &TableName, BOOL bByPK, BOOL bAutoCommit,CStringArray &arrSQLDelete,CStringArray &arrSQLUpdate,CStringArray &arrSQLInsert)
{


	CString s, statement, where, insertCol;
	int row, col, i;
	//long affect;
	//P.K.������
	CStringArray arrPKFieldName;
	CUIntArray arrPKCol;
	//���ɵ�SQL���
	//CStringArray arrSQLDelete, arrSQLUpdate, arrSQLInsert;
	//ѭ��5�����
	int CountLoop, MaxLoop;
	CountLoop=MaxLoop=0;

	//////////////////////////////////////////////////////////
	//Step1: ��ʼ����
	//////////////////////////////////////////////////////////
	//����Ƿ���SetDBMS( )
	/*if(m_pOleDb==NULL) {
	Error(_T("DBMS Not set yet before Update()!	"));
	return FALSE;
	}*/

	//����Ϊ�գ�ȡ��һ������
	/*if(TableName.IsEmpty())
	for(col=0; col<ColCount(); col++) {
	TableName=GetColTableName(col);
	if(!TableName.IsEmpty()) break;
	}*/

	if (TableName.IsEmpty()) {
		TableName = m_TableName;
	}
	//д��SQLLog
	//if (m_pOleDb!= NULL) m_pOleDb->m_SQLog.RecordUpdateStart(TableName);

	//����нṹ�Ƿ��ѳ�ʼ
	if(GetFieldCount()==0) {
		ErrorLog(_T("Column structure not set yet at Update()!	"));
		return FALSE;
	}

	//���PK����
	for(col=0; col < GetFieldCount(); col++) {
		if(!IsFieldPK(col)) continue;
		//if(TableName.CompareNoCase(GetColTableName(col))) continue;
		arrPKCol.Add(col);
		arrPKFieldName.Add(GetFieldName(col));
	}
	if(arrPKCol.GetSize()==0) {
		ErrorLog(_T("Pimary Key Column not found!"));
		return FALSE;
	}

	//���м���CanNull��
	for(row=0; row < GetRowCount(); row++) {
		if(!GetRowModified(row + 1)) continue;			//����δ�޸ģ������
		//���д���
		for(col=0; col<GetFieldCount(); col++) {
			if(!IsFieldUpdateAble(col)) continue;		//�в��ɴ���
			if(IsFieldCanNull(col)) continue;			//������Ϊ��
			if(!GetItemModified(row + 1, col)) continue;	//����δ�޸�

			//if(TableName.CompareNoCase(GetColTableName(col))) continue;		//�Ǳ���

			s=GetAsString(col,row + 1);//CDataTableStatic::GetData(row, col);
			if(!s.IsEmpty()) continue;				//���ݷǿ�
			if(IsFieldPK(col) && IsFieldPKAutoIncrease(col)) continue;			//�Զ�������������

			s.Format(_T("Field %s at row %d is Empty."), GetFieldName(col), row);
			ErrorLog((LPTSTR)(LPCTSTR)s);
			return FALSE;
		}
	}

	//��ֹfilter�е�����û�б�����2008.5.8
	const int filterBufferCount = CopyFilterToPrimaryBuffer();//m_FilterBuffers->m_Buffers.GetSize();
	//if(PrimaryBufferCount >0 ) (m_PrimaryBuffers->m_Buffers).Append(m_FilterBuffers->m_Buffers);


	/////////////////////////////////////////////////////////////////
	//Step2: ����Delete��SQL���
	/////////////////////////////////////////////////////////////////
	for(row = 0; row < DeletedCount(); row++) {
		if(GetRowNew(row + 1,Delete)) continue;	//������ɾ��������
		statement.Empty();
		for(i=0; i < arrPKCol.GetSize(); i++) {
			col=arrPKCol.GetAt(i);
			if(!statement.IsEmpty()) statement+=_T(" and ");
			statement+=arrPKFieldName.GetAt(i);

			s=GetAsString(col,row + 1,Delete,TRUE);//m_arrRowDelete.GetAt(row)->GetOriginData(col);
			if(s.IsEmpty())
				statement+=_T(" is null ");
			else {
				if(GetFieldDataType(col)==ftInteger || GetFieldDataType(col)==ftFloat)
					statement+=_T("=") +s;
				else
					statement+=_T("='") +Value2SQLValue(s) +_T("'");
			}
		}
		if(statement.IsEmpty()) continue;

		//SQL�����뵽����
		s.Format(_T("delete from %s where %s"), TableName, statement);
		arrSQLDelete.Add(s);
	}

	/////////////////////////////////////////////////////////////////
	//Step3: ����Update��SQL���
	/////////////////////////////////////////////////////////////////
	//����update��set ??=??, ??=??����
	for(row=0; row<GetRowCount(); row++) {
		if(GetRowNew(row + 1)) continue;					//new���в���Update����
		if(!GetRowModified(row + 1)) continue;			//����δ�޸�
		statement.Empty();
		for(col=0; col < GetFieldCount(); col++) {
			if(!IsFieldUpdateAble(col)) continue;		//�в��ɴ���
			if(!GetItemModified(row + 1, col)) continue;	//������δ��
			if (IsUpdateAsBlobFiled(col) == TRUE) continue;         //����Ǵ��ֶ��򲻽�������2008.1.10
			//if(TableName.CompareNoCase(GetColTableName(col))) continue;		//�Ǳ���

			s=GetAsString(col,row + 1);//CDataTableStatic::GetData(row, col);
			if(!statement.IsEmpty()) statement+=_T(",");
			statement+=GetFieldName(col);
			if(s.IsEmpty())
				statement+=_T("=null");
			else {
				if(GetFieldDataType(col)==ftInteger || GetFieldDataType(col)==ftFloat)
					statement+=_T("=") +s;
				else
					statement+=_T("='") +Value2SQLValue(s)+ _T("'");
			}
		}
		if(statement.IsEmpty()) continue;

		//����where�Ӿ�
		where.Empty();
		for(i=0; i < arrPKCol.GetSize(); i++) {
			col=arrPKCol.GetAt(i);
			if(i) where+= _T(" and ");
			where += arrPKFieldName.GetAt(i);

			s=GetAsString(col,row + 1,Primary,TRUE);//CDataTableStatic::GetOriginData(row, col);
			if(s.IsEmpty())
			where+=_T("=null");
			else {
				if(GetFieldDataType(col)==ftInteger || GetFieldDataType(col)==ftFloat)
				where+=_T("=") +s;
				else
				where+=_T("='") +Value2SQLValue(s)+ _T("'");
			}
		}

		//���뵽����
		s.Format(_T("update %s set %s where %s"), TableName, statement, where);
		arrSQLUpdate.Add(s);
	}

	/////////////////////////////////////////////////////////////////
	//Step4: ����Insert���, �Զ����ɵ�������ҪLoop 5��
	/////////////////////////////////////////////////////////////////
	//����insert���Ĺ̶��ֶ�������
	for(col=0; col < GetFieldCount(); col++) {
		if(!IsFieldUpdateAble(col)) continue;
		if (IsUpdateAsBlobFiled(col) == TRUE) continue;         //������Ǵ��ֶ��򲻽�������2008.1.10
		//if(TableName.CompareNoCase(GetColTableName(col))) continue;
		if(!insertCol.IsEmpty()) insertCol+=_T(",");
		insertCol+=GetFieldName(col);
	}

	//5��ѭ�������￪ʼ
	//Loop_Insert:

	//�Զ�����AutoIncrease��
	for(col=0; col<GetFieldCount(); col++) {
		if(!(IsFieldPK(col) && IsFieldPKAutoIncrease(col))) continue;
		statement.Format(_T("select max(%s) from %s"), GetFieldName(col), TableName);
		//if (m_pOleDb!= NULL)
		//	int aa = 0;
		//m_pOleDb->Select1Value(_bstr_t(statement), s);
		//else//û��OLEDb��״�� 
		{
			s = GetMaxFieldValue(col);
		}
		i=_ttoi(s);
		//���м�1
		for(row=0; row<GetRowCount(); row++) {
			if(!GetRowNew(row + 1)) continue;	//�ǲ�����
			s.Format(_T("%d"), ++i);
			//�ַ�����Ҫ����0��
			if(GetFieldDataType(col)==ftString /*|| GetColDataType(col)==DATATYPE_NVARCHAR*/)
				while(s.GetLength() < GetFieldMaxLen(col)) s=_T("0")+s;
			SetAsString(row, col, s);//CDataTableStatic::SetData(row, col, s);
			if(MaxLoop==0) MaxLoop=5;		//ȷʵ��Ҫѭ��
		}
	}

	//��������
	arrSQLInsert.RemoveAll();
	for(row=0; row<GetRowCount(); row++) {
		if(!GetRowNew(row + 1)) continue;				//�ǲ�����
		statement.Empty();
		for(col=0; col<GetFieldCount(); col++) {
			if(!IsFieldUpdateAble(col)) continue;
			if (IsUpdateAsBlobFiled(col) == TRUE) continue;         //����Ǵ��ֶ��򲻽�������2008.1.10
			//if(TableName.CompareNoCase(GetColTableName(col))) continue;
			if(!statement.IsEmpty()) statement+=_T(",");

			s=GetAsString(col,row + 1);//CDataTableStatic::GetData(row, col);
			if(s.IsEmpty())
				statement+=_T("null");
			else {
				if(GetFieldDataType(col)==ftInteger || GetFieldDataType(col)==ftFloat)
					statement+=s;
				else
					statement+=_T("'") +Value2SQLValue(s)+ _T("'");
			}
		}

		//��������
		s.Format(_T("insert into %s (%s) values (%s)"), TableName, insertCol, statement);
		arrSQLInsert.Add(s);
	}
	// 	//��ֹfilter�е�����û�б�����2008.5.8�ָ����˵�����2008.5.8 
	//2008.10.20�޸ģ�������resetupdateʱ���޷�����filter�����е����ݵ����⣬�����Ƶ����
	// 	if (PrimaryBufferCount >0)
	// 	{
	// 		m_PrimaryBuffers->m_Buffers.RemoveAt(m_PrimaryBuffers->m_Buffers.GetSize() - PrimaryBufferCount,PrimaryBufferCount);
	// 	}

	/*if (m_pOleDb!= NULL)
	{
	/////////////////////////////////////////////////////////////////
	//Step5: �������ݿ�
	/////////////////////////////////////////////////////////////////
	//���û�������򴴽�
	if(!m_pOleDb->IsInTrans())
	if(!m_pOleDb->BeginTrans()) {
	ErrorLog((LPTSTR)(LPCTSTR)m_pOleDb->LastError());
	return FALSE;
	}

	//Delete
	for(i=0; i<arrSQLDelete.GetSize(); i++)
	if(!m_pOleDb->ExecuteNonQuery(arrSQLDelete.GetAt(i), ADODB::adCmdText|ADODB::adExecuteNoRecords, &affect)) {
	Error(m_pOleDb->LastError());
	m_pOleDb->RollBack();		//ʧ�ܾͻع�
	return FALSE;
	}
	//Update
	for(i=0; i<arrSQLUpdate.GetSize(); i++)
	if(!m_pOleDb->ExecuteNonQuery(arrSQLUpdate.GetAt(i), ADODB::adCmdText|ADODB::adExecuteNoRecords, &affect)) {
	Error(m_pOleDb->LastError());
	m_pOleDb->RollBack();		//ʧ�ܾͻع�
	return FALSE;
	}
	//Insert
	for(i=0; i<arrSQLInsert.GetSize(); i++)
	if(!m_pOleDb->ExecuteNonQuery(arrSQLInsert.GetAt(i), ADODB::adCmdText|ADODB::adExecuteNoRecords, &affect)) {
	m_pOleDb->RollBack();		//ʧ�ܾͻع�
	CountLoop++;				//��ѭ��������1
	if(CountLoop<MaxLoop) goto Loop_Insert;
	else {
	Error(m_pOleDb->LastError());
	return FALSE;
	}
	}

	/////////////////////////////////////////////////////////////////
	//Step6: �ύ����
	/////////////////////////////////////////////////////////////////
	//����Ҫ�ύ
	if(!bAutoCommit) return TRUE;

	//��Ҫ�ύ
	m_pOleDb->m_SQLog.ResetTime();
	if(m_pOleDb->Commit()) {
	//�ύ�ɹ�����ʱ���״̬��־
	ResetUpdate();
	m_pOleDb->m_SQLog.RecordUpdateCommit();
	return TRUE;
	}
	else {
	ErrorLog((LPTSTR)(LPCTSTR)m_pOleDb->LastError());
	return FALSE;
	}
	}*/
	ResetUpdate(FALSE);
	//��ֹfilter�е�����û�б�����2008.5.8�ָ����˵�����2008.5.8
	// 	if (PrimaryBufferCount >0)
	// 	{
	// 		m_PrimaryBuffers->m_Buffers.RemoveAt(m_PrimaryBuffers->m_Buffers.GetSize() - PrimaryBufferCount,PrimaryBufferCount);
	// 	}
	RemoveCopyedFilterToPrimaryBuffer(filterBufferCount);
	return TRUE;
}

BOOL NDataModelDataSet::IsFieldUpdateAble(int col)
{
	NDataField *field = GetField(col);
	if (field != NULL) 
	{
		return  field->IsUpdateAble();
	}
	return FALSE;
}

BOOL NDataModelDataSet::IsFieldCanNull(int col)
{
	NDataField *field = GetField(col);
	if (field != NULL) 
	{
		return  field->IsCanNull();
	}
	return FALSE;
}

// 
// BOOL NDataModelDataSet::IsFieldBlob(int col)
// {
//    NDataField *field = GetField(col);
//    if (field != NULL) 
//    {
// 	   return  field->IsBlobField();
//    }
//    return FALSE;
// }

BOOL NDataModelDataSet::IsUpdateAsBlobFiled(int col)
{
	NDataField *field = GetField(col);
	if (field != NULL) 
	{
		return  field->IsUpdateAsBlobFiled();
	}
	return FALSE;
}
void NDataModelDataSet::SetUpdateAsBlobFiled(int col,BOOL b)
{
	NDataField *field = GetField(col);
	if (field != NULL) 
	{
		field->SetUpdateAsBlobFiled(b);
	}
}
BOOL NDataModelDataSet::IsUpdateAsBlobFiled(CString colName)
{
	int index = GetFieldIndex(colName);
	if (index >=0 )
	{
		return IsUpdateAsBlobFiled(index);
	}
	return FALSE;
}
void NDataModelDataSet::SetUpdateAsBlobFiled(CString colName,BOOL b)
{

	int index = GetFieldIndex(colName);
	if (index >=0 )
	{
		SetUpdateAsBlobFiled(index,b);
	}

}


BOOL NDataModelDataSet::IsFieldPKAutoIncrease(int col)
{
	NDataField *field = GetField(col);
	if (field != NULL) 
	{
		return  field->IsPKAutoIncrease();
	}
	return FALSE;
}

int NDataModelDataSet::DeletedCount()
{
	return m_DeleteBuffers->GetRowCount();
}

int NDataModelDataSet::FilterCount()
{
	return m_FilterBuffers->GetRowCount();
}

FieldType NDataModelDataSet::GetFieldDataType(int col)
{
	NDataField *field = GetField(col);
	if (field != NULL) 
	{
		return  field->GetDataType();
	}
	return ftUnknown;
}

int NDataModelDataSet::GetFieldMaxLen(int col)
{
	NDataField *field = GetField(col);
	if (field != NULL) 
	{
		return  field->GetSize();
	}
	return 0;
}

BOOL NDataModelDataSet::IsFieldPK(int col)
{
	NDataField *field = GetField(col);
	if (field != NULL) 
	{
		return  field->IsPK();
	}
	return FALSE;
}
void NDataModelDataSet::SetFieldPK(int col,BOOL b)
{
	NDataField *field = GetField(col);
	if (field != NULL) 
	{
		field->SetPK(b);
	}
}
void NDataModelDataSet::SetFieldCanNull(int col,BOOL b)
{
	NDataField *field = GetField(col);
	if (field != NULL) 
	{
		field->SetCanNull(b);
	}
}
void NDataModelDataSet::SetFieldPKAutoIncrease(int col,BOOL b)
{
	NDataField *field = GetField(col);
	if (field != NULL) 
	{
		field->SetPKAutoIncrease(b);
	}
}
void NDataModelDataSet::SetFieldUpdateAble(int col,BOOL b)
{
	NDataField *field = GetField(col);
	if (field != NULL) 
	{
		field->SetUpdateAble(b);
	}
}

void NDataModelDataSet::ErrorLog(CString error)
{

}

void NDataModelDataSet::ResetUpdate(BOOL resetBlobField)
{
	int  row;
	/*for(i=0; i<m_arrRowDelete.GetSize(); i++)	delete m_arrRowDelete.GetAt(i);
	m_arrRowDelete.RemoveAll();*/
	m_DeleteBuffers->ResetBuffers();
	if (resetBlobField == TRUE)
	{
		for(row = 0; row<GetRowCount(); row++) {
			if(GetRowNew(row + 1)) SetRowNew(row + 1, FALSE);
			//if(GetRowModified(row)) m_arrRow.GetAt(row)->ClearModified(TRUE);
			for(int col = 0; col < GetFieldCount(); col++)
			{
				NDataField *field = m_fields.GetField(col);
				NRecordID recordid = GetRecordID(row + 1);
				field->ClearModified((char*)GetFieldDataPT(field,recordid,FALSE));
			}

		}
	}
	else
	{
		for(row = 0; row<GetRowCount(); row++) {
			if(GetRowNew(row + 1)) SetRowNew(row + 1, FALSE);
			//if(GetRowModified(row)) m_arrRow.GetAt(row)->ClearModified(TRUE);
			for(int col = 0; col < GetFieldCount(); col++)
			{
				NDataField *field = m_fields.GetField(col);
				if (field->IsUpdateAsBlobFiled() == TRUE) //���������������Ҫ�����汣��һ��
				{
					continue;
				}
				NRecordID recordid = GetRecordID(row + 1);
				field->ClearModified((char*)GetFieldDataPT(field,recordid,FALSE));
			}

		}
	}	

	m_IsModified = FALSE;
}

CString NDataModelDataSet::GetMaxFieldValue(int col)
{
	NDataField *field;
	CString result;
	field = GetField(col);
	void *MaxBuffer,*buffer;
	int maxrow = -1;
	if (field != NULL && GetRowCount() > 0) {
		MaxBuffer = GetFieldDataPT(field,GetRecordID(1));
		maxrow = 1;
		for(int i = 1 ; i < GetRowCount(); i++)
		{
			buffer = GetFieldDataPT(field,GetRecordID(i + 1));
			if(field->Compare(MaxBuffer,buffer) < 0)
			{
				MaxBuffer = buffer;
				maxrow = i + 1;
			}
		}
	}
	if (maxrow >0 ) 
	{
		result = GetAsString(col,maxrow);
	}
	return result;
}

void NDataModelDataSet::DeleteRow(int row)
{
	/*if (row <= 0 || row > GetRowCount()) 
	{
	return;
	}
	void * buff = m_PrimaryBuffers->GetBuffPtr(row - 1);
	m_PrimaryBuffers->m_Buffers.RemoveAt(row - 1);
	m_DeleteBuffers->AddBuff(buff);*/
	DeleteRowInternal(row,m_PrimaryBuffers);
}

int NDataModelDataSet::GetRowOf(NRecordID recordid,eBufferFlag BufferFlag)
{
	NDataModelDataSetBuffer *pcurrentbuff = NULL;
	switch(BufferFlag)
	{
	case Primary:
		pcurrentbuff = m_PrimaryBuffers;
		break;
	case Filter:
		pcurrentbuff = m_FilterBuffers;
		break;
	case Delete:
		pcurrentbuff = m_DeleteBuffers;
		break;
	default:
		return -1;
	}
	int row = 0;
	for(int i = 0;  i < pcurrentbuff->GetRowCount(); i++)
	{
		if (pcurrentbuff->GetBuffPtr(i) == recordid) 
		{
			return i + 1;
		}
	}
	return row;
}

int NDataModelDataSet::MoveRow(int row, int beforRow,eBufferFlag BufferFlag)
{
	NDataModelDataSetBuffer *pcurrentbuff;
	switch(BufferFlag)
	{
	case Primary:
		pcurrentbuff = m_PrimaryBuffers;
		break;
	case Filter:
		pcurrentbuff = m_FilterBuffers;
		break;
	case Delete:
		pcurrentbuff = m_DeleteBuffers;
		break;
	}
	return pcurrentbuff->MoveRow(row,beforRow);
}

void NDataModelDataSet::DeleteRow(NRecordID recordid)
{
	int row = GetRowOf(recordid);
	if (row > 0) 
	{
		DeleteRow(row);
	}
	else//���������Ҳ�����filter�в���
	{
		row = GetRowOf(recordid,Filter);
		if (row > 0) 
		{
			DeleteRowInternal(row,m_FilterBuffers);
		}
	}
}

void NDataModelDataSet::DeleteRowInternal(int row,NDataModelDataSetBuffer *pBuff)
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
	pBuff->m_Buffers.RemoveAt(row - 1);

	//���ٹ������
	if (m_pShardDataList != NULL) 
	{
		for(int i = 0 ; i < m_pShardDataList->GetSize(); i++)
		{
			((NDataModelDataSet_ShareData*)m_pShardDataList->GetAt(i))->OnParentDelete(buff);
		}
	}
	//   m_DeleteBuffers->AddBuff(buff);
	/********************************************************************
	liwenlang
	ʱ��:2010:12:9   10:57
	��ע:ȥ��ɾ������
	*********************************************************************/
	FreeRecordBuffer(buff);

	if (m_IsModified == FALSE) 
	{
		m_IsModified = TRUE;
	}

}

void NDataModelDataSet::GetUpdateSql(CStringArray &sqlarray)
{
	CStringArray arrSQLDelete, arrSQLUpdate, arrSQLInsert;
	Update(GetTableName(),TRUE,FALSE,arrSQLDelete,arrSQLUpdate,arrSQLInsert);
	sqlarray.Append(arrSQLDelete);
	sqlarray.Append(arrSQLUpdate);
	sqlarray.Append(arrSQLInsert);
}

int NDataModelDataSet::InitFieldStr()
{
	return 0;
}

FieldType NDataModelDataSet::GetFieldType(int iFieldIndex)
{
	NDataField* field = GetField(iFieldIndex);
	if (field != NULL) 
	{
		return field->GetDataType();
	}
	return ftUnknown;
}

void NDataModelDataSet::ResetFilter(BOOL bResetExp)
{
	(m_PrimaryBuffers->m_Buffers).Append(m_FilterBuffers->m_Buffers);
	m_FilterBuffers->m_Buffers.RemoveAll();
	//�����õ�����ȫ�����

	if (bResetExp) 
	{
		NDataField *pFiled;
		for(int i = 0 ; i < GetFieldCount() ; i++)
		{
			pFiled = GetField(i);
			if (pFiled != NULL) 
			{
				pFiled->FreeFilterValue();
			}
		}
	}

}

void NDataModelDataSet::SaveAsText(CString &sBuff)
{
	sBuff.Empty();
	CString s;
	for(int row=0; row<GetRowCount(); row++) {
		for(int col=0; col<GetFieldCount(); col++) {

			s=GetAsString(col,row + 1);//CDataTableStatic::GetData(row, col);
			/*if(s.IsEmpty())
			statement+=_T("null");
			else {
			if(GetFieldDataType(col)==ftInteger || GetFieldDataType(col)==ftFloat)
			statement+=s;
			else
			statement+=_T("'") +CADODataType::Value2SQLValue(s)+ _T("'");
			}*/
			sBuff =sBuff+ s + (col == GetFieldCount() - 1?_T("\r\n"):_T("\t"));
		}
	}
}

BOOL NDataModelDataSet::GetModified()
{
	return m_IsModified;
}
void NDataModelDataSet::SetModified(BOOL isModified)
{
	m_IsModified = isModified;
}
BOOL NDataModelDataSet::_ImportXML(BSTR xml, BOOL bFromFile, BOOL bAppend, BOOL bAsNew)
{
	return FALSE;
	//	if(!bAsNew)SetState(dmstRetrieve);
	//
	//	//���ݲ���
	//
	//
	//	//���ݲ��Խ���*/
	//	
	////	HRESULT hr;
	//	COleDateTime tempDT; 
	//	//CStringArray arrTable;
	//	CString s, TableName, FieldName, Data;
	//	int row, col/*, datatype*/;
	//
	//	//����нṹ
	//	if(bAppend==FALSE) Reset();
	//	if(!GetFieldCount()) {
	//		//Error(_T("Column structure not create yet."));
	//		return FALSE;
	//	}
	//	//Ŀǰֻ��֧��һ�ű�
	//	//GetColTableNames(arrTable);
	//	COXParser markUpxml;
	//
	//	//load from file or buf
	//	if(bFromFile) {
	//	/*	if(!markUpxml.LoadFromFile(filename,pmcommon::XMLFILE_ENCODING_ASNI)) {
	//			s.Format(_T("XML File %s not found"), (_bstr_t)filename);
	//			Error(s);*/
	//			SetState(dmstNormal);
	//			return FALSE;
	////		}
	//	}
	//	else 
	//		markUpxml.Parse_C(xml);
	//
	//	//��Ԫ��
	//	COXParserElement * pRootMarkUpRoot = markUpxml.Root();
	//	if(NULL == pRootMarkUpRoot)
	//	{
	//		SetState(dmstNormal);
	//		return TRUE;
	//	}
	//
	//	for( pmcommon::CMarkupNode NodeTable = rootMarkUpRoot.GetChild() ; NodeTable.IsValid(); NodeTable = NodeTable.GetSibling() ) 
	//	{
	//		TableName = NodeTable.GetName();
	//	//	TableName=(LPCTSTR)NodeTable->nodeName;
	//		//if(CPub::ArrayCompareNoCase(arrTable, TableName)==-1) continue;
	//		row=InsertRow(bAsNew);
	//		//����ֶ�
	//		for(pmcommon::CMarkupNode NodeField=NodeTable.GetChild(); NodeField.IsValid(); NodeField=NodeField.GetSibling()) 
	//		{
	////			FieldName=(LPCTSTR)NodeField->nodeName;
	//			FieldName=(LPCTSTR)NodeField.GetName();
	//			if(FieldName.Find(_T("."))==-1 && TableName.IsEmpty()==FALSE)
	//				FieldName=/*TableName +_T(".")+*/ FieldName;
	//			col=GetFieldIndex(FieldName);
	//			if(col==-1) continue;
	//
	//			//Data
	//			//datatype=GetColDataType(col);
	////			Data=(LPCTSTR)NodeField->text;
	//			Data=NodeField.GetValue();
	//			/*if(datatype==DATATYPE_DATETIME) {
	//				Data.Replace(_T('T'), _T(' '));
	//				if(Data.GetLength()>19) Data=Data.Left(19);
	//			}
	//			CDataTableStatic::SetData(row, col, Data);*/
	//			if(GetFieldType(col) == ftDateTime)
	//			{
	//				tempDT.ParseDateTime(Data);
	//				if ((tempDT.GetStatus() == COleDateTime::invalid)&&Data.GetLength() > 0) 
	//				{
	//					tempDT = CPub::CString2COleDateTime(Data);
	//					
	//					//tempDT = dt1;
	//					Data = tempDT.Format();
	//				}
	//			}
	//			this->SetAsString(col,row,Data);
	//		}
	//	}
	//	//doc=NULL;
	//	//ele=NULL;
	//	//Node=NULL;
	//	//NodeTable=NULL;
	//	//NodeField=NULL;
	//
	//	SetState(dmstNormal);
	//	return TRUE;
}

NDataModelDataSet * NDataModelDataSet::Clone()
{
	//������¡����
	NDataModelDataSet_Clone *pCloneDataStore;
	pCloneDataStore = new NDataModelDataSet_Clone(this);
	//PMRegMemory(pCloneDataStore);
	//��ʼ������ͬ�ı�ṹ
	NDataField *field;
	int i = 0;
	for(i = 0; i < m_fields.GetCount(); i++)
	{
		field = m_fields.GetField(i);
		pCloneDataStore->AddField(field->GetFieldName(),field->GetDataType(),field->GetDataType() == ftString?((field->GetSize())/sizeof(_TCHAR) - 1):field->GetSize());
	}    
	//��ʼ�������ݵĿ���
	//RowsCopy(1,GetRowCount(),Primary,pCloneDataStore,0,Primary);
	int beforerow = -1;

	NRecordID newrecord,OrgRecord;
	for(i = 1; i <= GetRowCount(); i++)
	{
		OrgRecord = m_PrimaryBuffers->GetBuffPtr(i - 1);
		newrecord = CloneRecord(i,Primary);
		pCloneDataStore->m_PrimaryBuffers->InsertBuff(newrecord,beforerow);
		//���ÿ�¡�ļ�¼Ϊ�������У�û�н��й��޸�
		SetRowNewInternal(newrecord,FALSE);
		SetRowModifiedInternal(newrecord,FALSE);
		//��ʼ����ӳ��
		pCloneDataStore->m_MapCloneRecordToParent.SetAt(newrecord,OrgRecord);
		//beforerow ++;
	}

	return pCloneDataStore;
}

NDataModelDataSet * NDataModelDataSet::CloneStruct()
{
	//������¡����
	NDataModelDataSet *pCloneDataStore;
	pCloneDataStore = new NDataModelDataSet();
	//��ʼ������ͬ�ı�ṹ
	NDataField *field;
	for(int i = 0; i < m_fields.GetCount(); i++)
	{
		field = m_fields.GetField(i);
		pCloneDataStore->AddField(field->GetFieldName(),field->GetDataType(),field->GetDataType() == ftString?field->GetSize() - 1:field->GetSize());
	}  
	return pCloneDataStore;
}
//����һ����ͬ�ṹ�Ķ��󣬶���������ȫ��ͬ�����ԶԸ����Ժ�Ķ�����з��顢����Ȳ���������Ӱ��
//ԭ����������ƶ�������ݽ������޸ģ����Է�ӳ��ԭ����
NDataModelDataSet * NDataModelDataSet::GetShardData(BOOL bSpyChanged,BOOL PrimaryOnly)
{
	//������¡����
	NDataModelDataSet *pCloneDataStore;
	pCloneDataStore = NewShareDataObj();
	if (pCloneDataStore == NULL) 
	{
		return NULL;
	}
	//��ʼ������ͬ�ı�ṹ
	NDataField *field;
	for(int i = 0; i < m_fields.GetCount(); i++)
	{
		field = m_fields.GetField(i);
		pCloneDataStore->AddField(field->GetFieldName(),field->GetDataType(),field->GetDataType() == ftString?(field->GetSize() / sizeof(TCHAR)) - 1:field->GetSize());
	}
	//��ʼ���д洢����ֱ�Ӹ���
	CPMPtrArray* pArray,*pArray1;
	pArray = m_PrimaryBuffers->GetBufferPtr();
	pArray1 = pCloneDataStore->m_PrimaryBuffers->GetBufferPtr();
	pArray1->Append(*pArray);
	if(!PrimaryOnly)
	{
		pArray = m_DeleteBuffers->GetBufferPtr();
		pArray1 = pCloneDataStore->m_DeleteBuffers->GetBufferPtr();
		pArray1->Append(*pArray);

		pArray = m_FilterBuffers->GetBufferPtr();
		pArray1 = pCloneDataStore->m_FilterBuffers->GetBufferPtr();
		pArray1->Append(*pArray);
	}
	if ( bSpyChanged) 
	{
		CheckShardDataList();
		m_pShardDataList->Add(pCloneDataStore);
	}
	return pCloneDataStore;
}

//�Լٶ��Ѿ�������������ֶ�sFieldName�����۰����
//����������
int NDataModelDataSet::Bisearch(void *pBuff, CString sFieldName, eBufferFlag BufferFlag)
{
	int nIndex = GetFieldIndex( sFieldName ) ; 

	return Bisearch( pBuff , nIndex , BufferFlag ) ; 
	// 	//�洢����
	// 	void *buffer;
	// 	NDataField *field = m_fields.GetField(sFieldName);
	// 	if (field == NULL) 
	// 	{
	// 		return -1;
	// 	}
	//  ///
	//  int  L, H, I, C;
	// 
	//   int Result = -1;
	// 
	//   L = 0;
	// 
	//   //H = FListNodes.GetSize() - 1;
	//  	if (BufferFlag == Primary)
	// 		H = m_PrimaryBuffers->GetRowCount() - 1;
	// 	else 
	// 	{
	// 		if(BufferFlag == Filter)
	// 		  H = m_FilterBuffers->GetRowCount() - 1;
	// 		else
	// 			H = m_DeleteBuffers->GetRowCount() - 1; 
	// 	}
	// 
	//   while (L <= H) 
	//   {
	//     I = (L + H) >> 1;
	// 	buffer = GetFieldDataPT(field,GetRecordID(I + 1,BufferFlag));
	//     C = field->Compare(buffer,pBuff);//int(FListNodes[I]) - int(Node);
	//     if (C == 0)
	//     {
	//       Result = I;
	//       break;
	//     }
	//     else
	// 	{
	//       if (C < 0)  
	// 		  L = I + 1;
	//       else H = I - 1;
	// 	}
	//   }
	// 	  return Result;
}

int NDataModelDataSet::Bisearch(const CArray<void *,void *> &findValues,const CStringArray &fieldNames,const eBufferFlag BufferFlag )
{
	/// ���� Index 
	CUIntArray iFieldIndexArr ; 

	int i = 0 ; 
	for ( ; i < fieldNames.GetSize() ; i++ )
	{
		int nIndex =  GetFieldIndex( fieldNames[i] ) ; 
		iFieldIndexArr.Add( nIndex ) ; 
	}

	return Bisearch( findValues , iFieldIndexArr , BufferFlag ) ; 
	//�洢����
	//     int findCount = fieldNames.GetSize();
	// 	
	// 	/********************************************************************
	// 	liwenlang
	// 	ʱ��:2010:9:27   9:02
	// 	��ע:��ѭ����ͨ��FieldNameȡFieldָ��ܷ�ʱ��
	// 	*********************************************************************/
	// 	
	// 	void *buffer = NULL;
	// 	NDataField *field = NULL/*m_fields.GetField(sFieldName)*/;
	// 	CArray<NDataField*,NDataField*&> fieldPtrArr;
	// 	for (int x = 0;x < findCount;x++)
	// 	{
	// 		field = m_fields.GetField(fieldNames[x]);
	// 		fieldPtrArr.Add(field);
	// 	}
	// 	// 	if (field == NULL) 
	// 	// 	{
	// 	// 		return -1;
	// 	// 	}
	// 		///
	// 		int  L, H, I, C;
	// 		
	// 		int Result = -1;
	// 		
	// 		L = 0;
	// 		
	// 		//H = FListNodes.GetSize() - 1;
	// 		if (BufferFlag == Primary)
	// 			H = m_PrimaryBuffers->GetRowCount() - 1;
	// 		else 
	// 	{
	// 			if(BufferFlag == Filter)
	// 		{
	// 				H = m_FilterBuffers->GetRowCount() - 1;
	// 		}
	// 			else
	// 		{
	// 				H = m_DeleteBuffers->GetRowCount() - 1; 
	// 		}
	// 	}
	// 			
	// 			
	// 			while (L <= H) 
	// 				
	// 			{
	// 				I = (L + H) >> 1;
	// 				for (int i = 0 ; i < findCount ; i++)
	// 				{
	// 			/********************************************************************
	// 			liwenlang
	// 				ʱ��:2010:9:27   9:15
	// 				��ע:������ȡFieldָ���е��˷�
	// 			*********************************************************************/
	// 			//					field = m_fields.GetField(fieldNames[i]);
	// 			field = fieldPtrArr.GetAt(i);
	// 					buffer = GetFieldDataPT(field,GetRecordID(I + 1,BufferFlag));
	// 					
	// 					void *psourceBuff = findValues[i];
	// 					long size1 = 0 , size2 = 0 ; 
	// 					/// blob ���⴦��
	// 					/// blob �ֶε� buffer��һ�� Recod �ļ�¼
	// 					/// blob ��ֵ��Ҫ ͨ�� GetBlob ȥȡ
	// 					if ( ftBlob == field->GetDataType() )
	// 					{
	// 						void *pBlob1 ;
	// 						size1 = field->GetBlob(buffer,pBlob1);
	// 						buffer = pBlob1 ; 
	// 
	// 						size2 = sizeof( (BYTE *)psourceBuff) ; 
	// 					}
	// 					
	// 					C = field->CompareValue(buffer , size1 ,psourceBuff , size2 );//C = int(FListNodes[I]) - int(Node);
	// 					
	// 					if (C != 0)
	// 					{
	// 						break;
	// 					}
	// 				}
	// 				if (C == 0)
	// 				{
	// 					Result = I;
	// 					break;
	// 				}
	// 				else
	// 				{
	// 					if (C < 0)  
	// 						L = I + 1;
	// 					else H = I - 1;
	// 				}
	// 			}
	// 	  return Result;
}

//�Լٶ��Ѿ�������������ֶ�sFieldName�����۰����,��ȷ�����յ�ǰ��������Ҫ�����λ��
//ע�⣬���ص���������
int NDataModelDataSet::FindNearest(void *pBuff, CString sFieldName, eBufferFlag BufferFlag)
{
	//�洢����
	void *buffer;
	NDataField *field = m_fields.GetField(sFieldName);
	if (field == NULL) 
	{
		return -1;
	}
	int L, H, I, C;
	
	L = 0;

	if (BufferFlag == Primary)
		H = m_PrimaryBuffers->GetRowCount() - 1;
	else 
		if(BufferFlag == Filter)
			H = m_FilterBuffers->GetRowCount() - 1;
		else
			H = m_DeleteBuffers->GetRowCount() - 1; 


	while (L <= H)
	{
		I = ((L + H) >> 1);

		buffer = GetFieldDataPT(field,GetRecordID(I + 1,BufferFlag));
		C = field->Compare(buffer,pBuff);//C = int(FListNodes[I]) - int(Node);

		if (C < 0) 
		{
			L = I + 1;
		}
		else
		{
			H = I - 1;
			if (C == 0)
			{
				L = I;
			}
		}
	}

	return L;
}

int NDataModelDataSet::Bisearch(void* pBuff, int iFieldIndex,eBufferFlag BufferFlag)
{
	//�洢����
	void *buffer;
	NDataField *field = m_fields.GetField(iFieldIndex);
	if (field == NULL) 
	{
		return -1;
	}
	///
	int  L, H, I, C;

	int Result = -1;

	L = 0;

	//H = FListNodes.GetSize() - 1;
	if (BufferFlag == Primary)
		H = m_PrimaryBuffers->GetRowCount() - 1;
	else 
	{
		if(BufferFlag == Filter)
			H = m_FilterBuffers->GetRowCount() - 1;
		else
			H = m_DeleteBuffers->GetRowCount() - 1;
	}

	while (L <= H) 		
	{
		I = (L + H) >> 1;
		buffer = GetFieldDataPT(field,GetRecordID(I + 1,BufferFlag));

		long size1 = 0 , size2 = 0 ; 
		{
			/// blob ���⴦��
			/// blob �ֶε� buffer��һ�� Recod �ļ�¼
			/// blob ��ֵ��Ҫ ͨ�� GetBlob ȥȡ
			if ( ftBlob == field->GetDataType() )
			{
				void *pBlob1 ;
				size1 = field->GetBlob(buffer,pBlob1);
				buffer = pBlob1 ; 

				/// �ֽ���
				size2 = lstrlen((TCHAR *)pBuff) * sizeof( TCHAR ); 
				/*size2 = sizeof( (BYTE *)psourceBuff) ; */
			}

		}

		C = field->CompareValue(buffer , size1 ,pBuff , size2 );//C = int(FListNodes[I]) - int(Node);

		/*C = field->Compare(buffer,pBuff);//int(FListNodes[I]) - int(Node);*/
		if (C == 0)
		{
			Result = I;
			break;
		}
		else
		{
			if (C < 0)  
				L = I + 1;
			else H = I - 1;
		}
	}
	return Result;
}

int NDataModelDataSet::Bisearch(const CArray<void *,void *> &findValues,const CUIntArray & iFieldIndexArr,const eBufferFlag BufferFlag)
{
	//�洢����
	int findCount = (int)iFieldIndexArr.GetSize();

	/********************************************************************
	liwenlang
	ʱ��:2010:9:27   9:02
	��ע:��ѭ����ͨ��FieldNameȡFieldָ��ܷ�ʱ��
	*********************************************************************/

	void *buffer = NULL;
	NDataField *field = NULL/*m_fields.GetField(sFieldName)*/;
	CArray<NDataField*,NDataField*&> fieldPtrArr;
	for (int x = 0;x < findCount;x++)
	{
		field = m_fields.GetField(iFieldIndexArr[x]);
		fieldPtrArr.Add(field);
	}

	int  L, H, I, C = -1;	
	int Result = -1;	
	L = 0;

	//H = FListNodes.GetSize() - 1;
	if (BufferFlag == Primary)
		H = m_PrimaryBuffers->GetRowCount() - 1;
	else 
	{
		if(BufferFlag == Filter)
		{
			H = m_FilterBuffers->GetRowCount() - 1;
		}
		else
		{
			H = m_DeleteBuffers->GetRowCount() - 1;
		}
	}


	while (L <= H) 		
	{
		I = (L + H) >> 1;
		for (int i = 0 ; i < findCount ; i++)
		{
			/********************************************************************
			liwenlang
			ʱ��:2010:9:27   9:15
			��ע:������ȡFieldָ���е��˷�
			*********************************************************************/
			//					field = m_fields.GetField(fieldNames[i]);
			field = fieldPtrArr.GetAt(i);
			buffer = GetFieldDataPT(field,GetRecordID(I + 1,BufferFlag));

			void *psourceBuff = findValues[i];
			long size1 = 0 , size2 = 0 ; 
			/// blob ���⴦��
			/// blob �ֶε� buffer��һ�� Recod �ļ�¼
			/// blob ��ֵ��Ҫ ͨ�� GetBlob ȥȡ
			if ( ftBlob == field->GetDataType() )
			{
				void *pBlob1 ;
				size1 = field->GetBlob(buffer,pBlob1);
				buffer = pBlob1 ; 

				size2 = lstrlen((TCHAR *)psourceBuff) * sizeof(TCHAR); 
				/*size2 = sizeof( (BYTE *)psourceBuff) ; */

				C = field->CompareValue(buffer , size1 ,psourceBuff , size2 );//C = int(FListNodes[I]) - int(Node);
			}
			else
				C = field->Compare(buffer,psourceBuff);//C = int(FListNodes[I]) - int(Node);

			if (C != 0)
			{
				break;
			}
		}
		if (C == 0)
		{
			Result = I;
			break;
		}
		else
		{
			if (C < 0) {
				L = I + 1; }
			else{
				H = I - 1; }
		}
	}
	return Result;
}

void NDataModelDataSet::OnInsertedRow(int newrow)
{

}
//���������ӵĹ������ݴ洢����
void NDataModelDataSet::OnSpyShareDataFree(NDataModelDataSet *pShareData)
{
	if (m_pShardDataList != NULL) 
	{
		for(int i = 0; i < m_pShardDataList->GetSize() ; i++)
		{
			if (pShareData == (m_pShardDataList->GetAt(i))) 
			{
				m_pShardDataList->RemoveAt(i);
				break;
			}
		}
	}
}

void NDataModelDataSet::CheckShardDataList()
{
	if (m_pShardDataList == NULL) {
		m_pShardDataList = new CArray <NDataModelDataSet *,NDataModelDataSet*>; 
	}

}

NDataModelDataSet * NDataModelDataSet::NewShareDataObj()
{
	return  new NDataModelDataSet_ShareData(this);
}

BOOL NDataModelDataSet::SaveDataSetXMLBuf(CString &sXMLBuf)
{
	CString tableName = m_TableName;
	if (tableName.IsEmpty())
	{
		tableName = _T("table");
	}
	CDataSet_StringBuilder sb/*(StringBuilder_NodeSize * 10)*/;
	sXMLBuf.Empty();
	if(GetFieldCount() <= 0) {
		ErrorLog(_T("Column structure not create yet."));
		return FALSE;
	}

	//��ʼ
	CString s, FieldName, Data;
	int  row,   cols,rowcount = GetRowCount();

	cols=GetFieldCount();

	//����XML
	//	sb+=(_T("<?xml version=\"1.0\" standalone=\"yes\" ?>"));
	///////////////////////////////////
	///liwenlang 
	///�������DataSet ֧������
	///[25/12/2009]
	//////////////////////////////////
	sb.Append(_T("<?xml version=\"1.0\" encoding=\"GB2312\" ?>"));
	sb.Append(_T("<!-- Generated by DataSet 1.0 Component  -->"));
	sb.Append(_T("<Data TableName='"+tableName+"'>")); //����table����
	//��ʼ����������
	//˵�������ݷ�Ϊ������Ҫ�Ľڵ� DataInstance��Databefore�ֱ�洢��ǰ�����ݺ����ݴ��ڸı���ǰ������
	CDataSet_StringBuilder sDataInstance/*(StringBuilder_NodeSize * 10)*/,sDatabefore/*(StringBuilder_NodeSize * 10)*/;
	CString sbuff = _T("\"primary\"");
	CString sXMLRow,sXMLRowBefroe;
	NRecordID recordID;
	for(int i = 0 ; i <3 ;i++)
	{
		switch(i) {
		case 0:
			sbuff = _T("\"primary\"");
			//rowcount = 
			break;
		case 2:
			rowcount = m_FilterBuffers->GetRowCount();
			sbuff = _T("\"filter\"");
			break;
		default:
			rowcount = m_DeleteBuffers->GetRowCount();
			sbuff = _T("\"delete\"");
		}
		for(row = 1; row <= rowcount; row++) 
		{
			sXMLRow = _T("");
			sXMLRowBefroe = _T("");
			recordID = GetRecordID(row,(eBufferFlag)i);
			BuildItemXML(tableName,row,recordID,sbuff,sXMLRow,sXMLRowBefroe);
			sDataInstance.Append(sXMLRow);
			if(!sXMLRowBefroe.IsEmpty())
			{
				sDatabefore.Append(sXMLRowBefroe);
			}
		}
	}

	sb.Append(_T("<DataInstance>"));
	sb.Append(sDataInstance);
	sb.Append(_T("</DataInstance>"));

	sb.Append(_T("<Databefore>"));
	sb.Append(sDatabefore);
	sb.Append(_T("</Databefore>"));

	sb.Append(_T("</Data>"));

	sb.GetString(sXMLBuf);
	return TRUE;
}
// 
// BOOL NDataModelDataSet::SaveDataSetXMLBuf(CString &sXMLBuf)
// {
// 
// 	CString tableName = m_TableName;
//     if (tableName.IsEmpty())
//     {
// 		tableName = _T("table");
//     }
// 	CString sb;
// 	sXMLBuf.Empty();
// 	if(GetFieldCount() <= 0) {
// 		ErrorLog(_T("Column structure not create yet."));
// 		return FALSE;
// 	}
// 
// 	//��ʼ
// 	CString s, FieldName, Data;
// 	int  row,   cols,rowcount = GetRowCount();
// 
// 	cols=GetFieldCount();
// 
// 	//����XML
// //	sb+=(_T("<?xml version=\"1.0\" standalone=\"yes\" ?>"));
// 	///////////////////////////////////
// 	///liwenlang 
// 	///�������DataSet ֧������
// 	///[25/12/2009]
// 	//////////////////////////////////
// 	sb+=(_T("<?xml version=\"1.0\" encoding=\"GB2312\" ?>"));
// 	sb+=(_T("<!-- Generated by DataSet 1.0 Component  -->"));
// 	sb+=(_T("<Data TableName='"+tableName+"'>")); //����table����
// 	//��ʼ����������
// 	//˵�������ݷ�Ϊ������Ҫ�Ľڵ� DataInstance��Databefore�ֱ�洢��ǰ�����ݺ����ݴ��ڸı���ǰ������
// 	CString sDataInstance,sDatabefore;
// 	CString sbuff = _T("\"primary\"");
// 	CString sXMLRow,sXMLRowBefroe;
// 	NRecordID recordID;
// 	for(int i = 0 ; i <3 ;i++)
// 	{
// 		switch(i) {
// 		case 0:
// 			sbuff = _T("\"primary\"");
// 			//rowcount = 
// 			break;
// 		case 2:
// 			rowcount = m_FilterBuffers->GetRowCount();
// 			sbuff = _T("\"filter\"");
// 			break;
// 		default:
// 			rowcount = m_DeleteBuffers->GetRowCount();
// 			sbuff = _T("\"delete\"");
// 		}
// 		for(row = 1; row <= rowcount; row++) 
// 		{
// 			sXMLRow = _T("");
// 			sXMLRowBefroe = _T("");
// 			recordID = GetRecordID(row,(eBufferFlag)i);
// 			BuildItemXML(tableName,row,recordID,sbuff,sXMLRow,sXMLRowBefroe);
// 			sDataInstance+=sXMLRow;
//             if(!sXMLRowBefroe.IsEmpty())
// 			{
// 			  sDatabefore+=sXMLRowBefroe;
// 			}
// 		}
// 	}
// 
// 	sDataInstance = _T("<DataInstance>") + sDataInstance +_T("</DataInstance>");
// 	sDatabefore = _T("<Databefore>") + sDatabefore +_T("</Databefore>");
// 	sb+=sDataInstance + sDatabefore+(_T("</Data>"));
// 	sXMLBuf = sb;
// //	CPub::CString2FileW(xml, _T("D:\\11.txt"));
// 	//�������ݲ���
// //	LoadDataSetXMLBuf(sXMLBuf,TRUE);
// 	return TRUE;
// }

BOOL NDataModelDataSet::SaveDataSetXMLBuf_NoDelete(CString &sXMLBuf)
{
	CString tableName = m_TableName;
	if (tableName.IsEmpty())
	{
		tableName = _T("table");
	}
	CDataSet_StringBuilder sb/*(StringBuilder_NodeSize * 10)*/;
	sXMLBuf.Empty();
	if(GetFieldCount() <= 0) {
		ErrorLog(_T("Column structure not create yet."));
		return FALSE;
	}

	//��ʼ
	CString s, FieldName, Data;
	int  row,   cols,rowcount = GetRowCount();

	cols=GetFieldCount();

	//����XML
	//	sb+=(_T("<?xml version=\"1.0\" standalone=\"yes\" ?>"));
	///////////////////////////////////
	///liwenlang 
	///�������DataSet ֧������
	///[25/12/2009]
	//////////////////////////////////
	sb.Append(_T("<?xml version=\"1.0\" encoding=\"GB2312\" ?>"));
	sb.Append(_T("<!-- Generated by DataSet 1.0 Component  -->"));
	sb.Append(_T("<Data TableName='"+tableName+"'>")); //����table����
	//��ʼ����������
	//˵�������ݷ�Ϊ������Ҫ�Ľڵ� DataInstance��Databefore�ֱ�洢��ǰ�����ݺ����ݴ��ڸı���ǰ������
	CDataSet_StringBuilder sDataInstance/*(StringBuilder_NodeSize * 10)*/,sDatabefore/*(StringBuilder_NodeSize * 10)*/ ;
	CString sbuff = _T("\"primary\"");
	CString sXMLRow,sXMLRowBefroe;
	NRecordID recordID;
	for(int i = 0 ; i <3 ;i++)
	{
		BOOL bNeedCal = TRUE;
		switch(i) {
		case 0:
			sbuff = _T("\"primary\"");
			//rowcount = 
			break;
		case 2:
			rowcount = m_FilterBuffers->GetRowCount();
			sbuff = _T("\"filter\"");
			break;
		default:
			rowcount = m_DeleteBuffers->GetRowCount();
			sbuff = _T("\"delete\"");
			bNeedCal = FALSE;
		}

		///////////////////////////////////
		///liwenlang 
		///������ɾ������Ϣ
		///[4/9/2008]
		//////////////////////////////////
		if(!bNeedCal)
			continue;

		for(row = 1; row <= rowcount; row++) 
		{
			sXMLRow = _T("");
			sXMLRowBefroe = _T("");
			recordID = GetRecordID(row,(eBufferFlag)i);
			BuildItemXML(tableName,row,recordID,sbuff,sXMLRow,sXMLRowBefroe);
			sDataInstance.Append(sXMLRow);
			if(!sXMLRowBefroe.IsEmpty())
			{
				sDatabefore.Append(sXMLRowBefroe);
			}
		}
	}

	sb.Append(_T("<DataInstance>"));
	sb.Append(sDataInstance);
	sb.Append(_T("</DataInstance>"));

	sb.Append(_T("<Databefore>"));
	sb.Append(sDatabefore);
	sb.Append(_T("</Databefore>"));

	sb.Append(_T("</Data>"));

	sb.GetString(sXMLBuf);
	return TRUE;
}
// 
// BOOL NDataModelDataSet::SaveDataSetXMLBuf_NoDelete(CString &sXMLBuf)
// {
// 	sXMLBuf.Empty();
// 
// 	//��ʼ
// 	CString s, FieldName, Data;
// 	int  row,   cols,rowcount = GetRowCount();
// 	if(rowcount <= 0)
// 		return TRUE;
// 
// 	CString tableName = m_TableName;
//     if (tableName.IsEmpty())
//     {
// 		tableName = _T("table");
//     }
// 	CString sb;
// 	if(GetFieldCount() <= 0) {
// 		ErrorLog(_T("Column structure not create yet."));
// 		return FALSE;
// 	}
// 
// 	cols=GetFieldCount();
// 
// 	//����XML
// //	sb+=(_T("<?xml version=\"1.0\" standalone=\"yes\" ?>"));
// 	///////////////////////////////////
// 	///liwenlang 
// 	///�������DataSet ֧������
// 	///[25/12/2009]
// 	//////////////////////////////////
// 	sb+=(_T("<?xml version=\"1.0\" encoding=\"GB2312\" ?>"));
// 	sb+=(_T("<!-- Generated by DataSet 1.0 Component  -->"));
// 	sb+=(_T("<Data TableName='"+tableName+"'>")); //����table����
// 	//��ʼ����������
// 	//˵�������ݷ�Ϊ������Ҫ�Ľڵ� DataInstance��Databefore�ֱ�洢��ǰ�����ݺ����ݴ��ڸı���ǰ������
// 	CString sDataInstance,sDatabefore;
// 	CString sbuff = _T("\"primary\"");
// 	CString sXMLRow,sXMLRowBefroe;
// 	NRecordID recordID;
// 	for(int i = 0 ; i <3 ;i++)
// 	{
// 		BOOL bNeedCal = TRUE;
// 		switch(i) {
// 		case 0:
// 			sbuff = _T("\"primary\"");
// 			//rowcount = 
// 			break;
// 		case 2:
// 			rowcount = m_FilterBuffers->GetRowCount();
// 			sbuff = _T("\"filter\"");
// 			break;
// 		default:
// 			rowcount = m_DeleteBuffers->GetRowCount();
// 			sbuff = _T("\"delete\"");
// 			bNeedCal = FALSE;
// 		}
// 
// 		///////////////////////////////////
// 		///liwenlang 
// 		///������ɾ������Ϣ
// 		///[4/9/2008]
// 		//////////////////////////////////
// 		if(!bNeedCal)
// 			continue;
// 
// 		for(row = 1; row <= rowcount; row++) 
// 		{
// 			sXMLRow = _T("");
// 			sXMLRowBefroe = _T("");
// 			recordID = GetRecordID(row,(eBufferFlag)i);
// 			BuildItemXML(tableName,row,recordID,sbuff,sXMLRow,sXMLRowBefroe);
// 			sDataInstance+=sXMLRow;
//             if(!sXMLRowBefroe.IsEmpty())
// 			{
// 			  sDatabefore+=sXMLRowBefroe;
// 			}
// 		}
// 	}
// 
// 	sDataInstance = _T("<DataInstance>") + sDataInstance +_T("</DataInstance>");
// 	sDatabefore = _T("<Databefore>") + sDatabefore +_T("</Databefore>");
// 	sb+=sDataInstance + sDatabefore+(_T("</Data>"));
// 	sXMLBuf = sb;
// //	CPub::CString2FileW(xml, _T("D:\\11.txt"));
// 	//�������ݲ���
// //	LoadDataSetXMLBuf(sXMLBuf,TRUE);
// 	return TRUE;
// }

BOOL NDataModelDataSet::BuildItemXML(CString tableName,int row,NRecordID recordID,CString &sbuff,CString &sXMLBuf,CString &sXMLBefore)
{
	//����������Ϣ
	//sXMLBuf = "<"+m_TableName+GetRowModifiedInternal(recordID)?" hasChanges="+"\"modified\"":""+
	//	GetRowNewInternal(recordID)?" IsNew="+"true":""+">";

	//��ʼ����ÿһ��item
	BOOL rowModified = GetRowModifiedInternal((char*)recordID);
	BOOL rowNew = GetRowNewInternal(recordID);
	CString Data/*,s,Orgs*/;
	CDataSet_StringBuilder sInstance,sBefroe;
	CString FieldName;
	int datatype;
	for(int i = 0 ; i < GetFieldCount(); i++)
	{
		Data=GetAsString(i,recordID);

		///////////////////////////////////
		///liwenlang 
		///�����յ����� ���� ��LoadXMLʱ �ڶ�ȡBefore����ʱ û�а취 ���¿����ݿ�
		///[25/12/2009 9:40]
		//////////////////////////////////
		//		if(Data.IsEmpty()) continue;

		datatype=GetFieldType(i);
		FieldName=GetFieldName(i);

		//���������⴦��
		if(datatype ==(int) ftDateTime) {
			Data.Replace(_T('.'), _T('-'));
			Data.Replace(_T(' '), _T('T'));
		}
		//�ַ������⴦��
		if(datatype==ftString || datatype==ftBlob) {
			Data.Replace(_T("&"), _T("&amp;"));
			Data.Replace(_T("<"), _T("&lt;"));
			Data.Replace(_T(">"), _T("&gt;"));
		}
		//		s.Format(_T("<%s>%s</%s>"), FieldName, Data, FieldName);
		sInstance.Append(_T("<"));
		sInstance.Append(FieldName);
		sInstance.Append(_T(">"));
		sInstance.Append(Data);
		sInstance.Append(_T("</"));
		sInstance.Append(FieldName);
		sInstance.Append(_T(">"));
		//��ʼ��֯befor�ε�����
		if (rowModified && (!rowNew))
		{
			if (GetItemModifiedInternal(recordID,i)) 
			{
				Data=GetAsString(i,recordID,TRUE);
				//���������⴦��
				if(datatype ==(int) ftDateTime) {
					Data.Replace(_T('.'), _T('-'));
					Data.Replace(_T(' '), _T('T'));
				}
				//�ַ������⴦��
				if(datatype==ftString || datatype==ftBlob) {
					Data.Replace(_T("&"), _T("&amp;"));
					Data.Replace(_T("<"), _T("&lt;"));
					Data.Replace(_T(">"), _T("&gt;"));
				}
				//				Orgs.Format(_T("<%s>%s</%s>"), FieldName, Data, FieldName);
				sBefroe.Append(_T("<"));
				sBefroe.Append(FieldName);
				sBefroe.Append(_T(">"));
				sBefroe.Append(Data);
				sBefroe.Append(_T("</"));
				sBefroe.Append(FieldName);
				sBefroe.Append(_T(">"));
			}
		}
	}

	CString strRow ;
	strRow.Format( _T("%d") , row ) ; 

	// 	TCHAR cRows[20];
	// 	_itot(row,cRows,10);
	if (sBefroe.GetLength() > 0)//Ϊԭʼ���ݼ��Ϸ�װ�ڵ� 
	{
		//		sXMLBefore.Format(_T("<%s rowid=\"%d\" buff=%s>%s</%s>"),tableName,row,sbuff,sBefroe,tableName);
		CDataSet_StringBuilder sBuildXMLBefore;
		sBuildXMLBefore.Append(_T("<"));
		sBuildXMLBefore.Append(tableName);
		sBuildXMLBefore.Append(_T(" rowid=\""));
		sBuildXMLBefore.Append( strRow );
		sBuildXMLBefore.Append(_T("\" buff="));
		sBuildXMLBefore.Append(sbuff);
		sBuildXMLBefore.Append(_T(">"));
		sBuildXMLBefore.Append(sBefroe);
		sBuildXMLBefore.Append(_T("</"));
		sBuildXMLBefore.Append(tableName);
		sBuildXMLBefore.Append(_T(">"));

		sBuildXMLBefore.GetString(sXMLBefore);
	}

	CString sRowModify,sRownew;
	sRowModify = rowModified&&(!rowNew) ? _T("hasChanges=\"modified\" "):_T("");
	sRownew = rowNew?_T(" IsNew=\"true\" "):_T("");
	//	sXMLBuf.Format(_T("<%s %s%s\"%d\" buff=%s>%s</%s>"),tableName , sRowModify+sRownew,_T("rowid="),row,sbuff,sInstance.m_pMem,tableName);
	CDataSet_StringBuilder sBuildXMLBuff;
	sBuildXMLBuff.Append(_T("<"));
	sBuildXMLBuff.Append(tableName);
	sBuildXMLBuff.Append(_T(" "));
	sBuildXMLBuff.Append(sRowModify+sRownew);
	sBuildXMLBuff.Append(_T("rowid="));
	sBuildXMLBuff.Append(_T("\""));
	sBuildXMLBuff.Append( strRow );
	sBuildXMLBuff.Append(_T("\" buff="));
	sBuildXMLBuff.Append(sbuff);
	sBuildXMLBuff.Append(_T(">"));
	sBuildXMLBuff.Append(sInstance);
	sBuildXMLBuff.Append(_T("</"));
	sBuildXMLBuff.Append(tableName);
	sBuildXMLBuff.Append(_T(">"));

	sBuildXMLBuff.GetString(sXMLBuf);

	//sXMLBuf+="/"+m_TableName+">"
	return TRUE;
}

BOOL NDataModelDataSet::LoadDataSetXMLBuf(const CString &sXMLBuf, BOOL bAppend)
{
	SetState(dmstRetrieve);
	CArray<int ,int > ModifyFieldArray;
	//	HRESULT hr;
	COleDateTime tempDT; 
	//CStringArray arrTable;
	CString s, TableName, FieldName, Data,sAttri;
	NRecordID recordID;
	int row, col/*, datatype*/;

	//����нṹ
	if(bAppend == FALSE) Reset();
	if(!GetFieldCount()) {
		//Error(_T("Column structure not create yet."));
		return FALSE;
	}
	if(sXMLBuf.GetLength() <= 0)
		return TRUE;

	//��ʼMSXML����ָ��

	//load from  buf
	//	BSTR bs = sXMLBuf.AllocSysString();
	//	doc->loadXML(/*sXMLBuf.GetBuffer(0)*/bs);
	//	SysFreeString(bs);
	COXParser oxParser;
	if(!oxParser.Parse_C(sXMLBuf))
		return FALSE;

	//��Ԫ��
	//	ele=doc->documentElement;
	COXParserElement* pEle = oxParser.Root();
	if(NULL == pEle)
	{
		SetState(dmstNormal);
		return TRUE;
	}

	//	NodeDataInstance = ele->firstChild;;
	//	NodeDatabefore = NodeDataInstance->nextSibling;
	//	NodeRowbefore = NodeDatabefore->firstChild;
	COXParserElement * pNodeDataInstance = pEle->Object_Ele(0);
	COXParserElement * pNodeDatabefore = pNodeDataInstance->Object_Ele(1);
	COXParserElement * pNodeRowbefore = pNodeDatabefore->Object_Ele(0);
	pNodeDataInstance = pNodeDataInstance->Object_Ele(0);//��ȡ����Ԫ��һ��

	int rowid;
	BOOL isNew = FALSE;

	//����н������ݽ���
	//	for(COXParserElement * pNodeRow = pNodeDataInstance.Object_Ele(0); NodeRow.IsValid(); NodeRow=NodeRow.GetSibling()) 
	for(int i = 0;i < pNodeDataInstance->NumObjects();i++)
	{
		COXParserElement * pNodeRow = pNodeDataInstance->Object_Ele(i);
		ModifyFieldArray.RemoveAll();
		row=InsertRow(0);
		rowid = row;
		//���ݻ����������е��ƶ�
		//ȡ�ô洢��
		//attri = NodeRow->getAttributeNode("buff");
		//		attributes = NodeRow->attributes;
		//		attri = attributes->getNamedItem(_T("buff"));
		sAttri = pNodeRow->GetAttributeValue(_T("buff"));
		if(sAttri.GetLength() <= 0)
		{
			sAttri = _T("filter");
		}

		recordID = GetRecordID(row);
		if (sAttri != _T("primary")) 
		{
			if(sAttri == _T("filter"))
			{
				m_FilterBuffers->m_Buffers.Add(m_PrimaryBuffers->m_Buffers.GetAt(row - 1));
				m_PrimaryBuffers->m_Buffers.RemoveAt(row - 1);
				rowid = m_FilterBuffers->GetRowCount();
			}
			else if(sAttri == _T("delete"))
			{
				m_DeleteBuffers->m_Buffers.Add(m_PrimaryBuffers->m_Buffers.GetAt(row - 1));
				m_PrimaryBuffers->m_Buffers.RemoveAt(row - 1);
				rowid = m_DeleteBuffers->GetRowCount();
			}
		}
		//ȡ����id
		//attri = NodeRow->getAttributeNode("rowid");

		//ȡ���Ƿ�������
		//attri = NodeRow->getAttributeNode("IsNew");
		//		attributes = NodeRow->attributes;
		//		attri = attributes->getNamedItem(_T("IsNew"));
		sAttri = pNodeRow->GetAttributeValue(_T("IsNew"));

		if (sAttri == _T("true")) {
			//isNew = TRUE;
			SetRowNewInternal(recordID,TRUE);
		}

		//ȡ���Ƿ��Ѿ������仯
		//attri = NodeRow->getAttributeNode("hasChanges");
		//		attributes = NodeRow->attributes;
		//		attri = attributes->getNamedItem(_T("hasChanges"));
		sAttri = pNodeRow->GetAttributeValue(_T("hasChanges"));

		//sAttri = (LPCTSTR)attri->text;
		if (sAttri == _T("modified") ) //����ԭʼֵ
		{
			//�ж�id�Ƿ�һ��
			//��ʼ����仯��ԭʼ����
			if (pNodeRowbefore) {

				for(int j = 0; j < pNodeRowbefore->NumObjects(); j++) 
				{
					COXParserElement * pNodeField = pNodeRowbefore->Object_Ele(j);
					//					FieldName=(LPCTSTR)NodeField->nodeName;
					FieldName = pNodeField->GetName();
					if(FieldName.Find(_T("."))==-1 && TableName.IsEmpty()==FALSE)
						FieldName=/*TableName +_T(".")+*/ FieldName;
					col=GetFieldIndex(FieldName);
					if(col==-1) continue;
					//					Data=(LPCTSTR)NodeField->text;
					Data = pNodeField->GetElementValue();

					if(GetFieldType(col) == ftDateTime)
					{
						tempDT.ParseDateTime(Data);
						if ((tempDT.GetStatus() == COleDateTime::invalid)&&Data.GetLength() > 0) 
						{
							tempDT = CPub::CString2COleDateTime(Data);
							Data = tempDT.Format();
						}
					}
					ModifyFieldArray.Add(col);
					this->SetAsString(col,recordID,Data);					
				}
				//	NodeRowbefore = NodeRowbefore.GetSibling();
				//SetState(dmstNormal);//��һ����¼������ֵ������
			}
		}
		else
		{
			//SetState(dmstRetrieve);
		}

		//		for(pmcommon::CMarkupNode NodeField=NodeRow.GetChild(); NodeField.IsValid(); NodeField = NodeField.GetSibling())  //������ֵ
		for(int j = 0; j < pNodeRow->NumObjects(); j++)  //������ֵ
		{
			COXParserElement * pNodeField = pNodeRow->Object_Ele(j);
			//			FieldName=(LPCTSTR)NodeField->nodeName;
			FieldName = pNodeField->GetName();
			if(FieldName.Find(_T("."))==-1 && TableName.IsEmpty()==FALSE)
				FieldName=/*TableName +_T(".")+*/ FieldName;
			col=GetFieldIndex(FieldName);
			if(col==-1) continue;
			//			Data=(LPCTSTR)NodeField->text;
			Data = pNodeField->GetElementValue();

			if(GetFieldType(col) == ftDateTime)
			{
				tempDT.ParseDateTime(Data);
				if ((tempDT.GetStatus() == COleDateTime::invalid)&&Data.GetLength() > 0) 
				{
					tempDT = CPub::CString2COleDateTime(Data);
					Data = tempDT.Format();
				}
			}
			if (ModifyFieldArray.GetSize()>0) //����б仯���ֶ����ڽ������õ�ǰֵʱ��������
			{
				for(int iMFIndex = 0 ; iMFIndex < ModifyFieldArray.GetSize(); iMFIndex++)
				{
					if (ModifyFieldArray[iMFIndex] == col) 
					{
						SetState(dmstNormal);
						continue;
					}
				}
			}
			this->SetAsString(col,recordID,Data);
			SetState(dmstRetrieve);
		}
		//�������ڵĴ洢�������ƶ��洢��
	}

	//doc=NULL;
	//ele=NULL;
	//attributes = NULL;
	////Node=NULL;
	////NodeTable=NULL;
	//NodeField=NULL;
	//   attri = NULL;
	//NodeRow = NULL;
	//NodeField=NULL;
	//NodeDataInstance = NULL;
	//NodeDatabefore = NULL;
	//   NodeRowbefore = NULL;
	SetState(dmstNormal);
	return TRUE;
}


//	MSXML::IXMLDOMNamedNodeMapPtr attributes = NULL;
//	MSXML::IXMLDOMDocumentPtr doc=NULL;
//	MSXML::IXMLDOMElementPtr  ele=NULL;
//	//MSXML::IXMLDOMAttributePtr attri = NULL;      
//	MSXML::IXMLDOMNodePtr     attri = NULL,/*Node=NULL, NodeTable=NULL,*/ NodeRow = NULL,NodeField=NULL,NodeDataInstance = NULL,NodeDatabefore = NULL,NodeRowbefore = NULL;

void NDataModelDataSet::DoGroup()
{
	FreeGroup();

	if (m_Groupfields.GetSize() <=0 || m_PrimaryBuffers->GetRowCount() <=0) 
	{
		return;
	}
	//���Ȱ��շ����������
	int i = 0;
	for(i = 0 ; i < m_Groupfields.GetSize(); i++)
	{
		m_PrimaryBuffers->AddSortField((NDataField *)m_Groupfields.GetAt(i),m_GroupSortColAscending.GetAt(i));
	}
	m_isGrouped = TRUE;
	m_PrimaryBuffers->Sort(1,m_PrimaryBuffers->GetRowCount());

	//���ɷ���Ľṹ
	int iGroupCount = (int)m_Groupfields.GetSize();
	NDataField * field;
	int iResult;
	int irowcount = m_PrimaryBuffers->GetRowCount();
	void *pBuff1,*pBuff2;
	NCGroup *pCurrentGroup,*pCurrentLeafGroup;;
	NCGroup *pNewGroup;
	//�������ɵ�һ������
	int Col = 0;
	for(Col = 0 ;Col < iGroupCount ;Col++)
	{   
		pNewGroup = new NCGroup;
		pNewGroup->SetGroupField(m_Groupfields.GetAt(Col));
		if (Col == 0) 
		{
			m_GroupList.Add(pNewGroup);
			pCurrentGroup = pNewGroup;
		}
		else
		{
			pCurrentGroup->AddChildGroup(pNewGroup,Col);
		}
		pCurrentLeafGroup = pNewGroup;
	}
	pBuff1 = m_PrimaryBuffers->GetBuffPtr(0);
	pCurrentLeafGroup->AddItem(pBuff1);

	//��ʼ�������з���
	for( i = 0 ; i < irowcount - 1; i++) //ÿ�ε�ǰ������һ�н������ݱȽ�
	{
		pBuff1 = m_PrimaryBuffers->GetBuffPtr(i);
		pBuff2 = m_PrimaryBuffers->GetBuffPtr(i + 1);
		for(Col = 0 ;Col < iGroupCount ;Col++)
		{
			//���бȽ�
			field =(NDataField *) m_Groupfields.GetAt(Col);
			iResult = field->Compare(GetFieldDataPT(field,pBuff1),GetFieldDataPT(field,pBuff2));
			if (iResult != 0) //��Ҫ�����ɷ������
			{
				for(int j = Col; j < iGroupCount; j++)//���½��ķ��������뵽��ǰ�����j������
				{
					pNewGroup = new NCGroup;
					pNewGroup->SetGroupField(m_Groupfields.GetAt(j));

					if (j == 0) 
					{
						m_GroupList.Add(pNewGroup);
						pCurrentGroup = pNewGroup;
					}
					else
					{
						pCurrentGroup->AddChildGroup(pNewGroup,j);
					}
					pCurrentLeafGroup = pNewGroup;
				}
				//�����ݷ������һ������
				pCurrentLeafGroup->AddItem(pBuff2);	
				break;
			}
			else if (Col == iGroupCount - 1) //���һ������
			{
				pCurrentLeafGroup->AddItem(pBuff2);
			}

		}
	}
}

void NDataModelDataSet::FreeGroup()
{
	m_isGrouped = FALSE;
	//��ʼ�ͷ�group�Ŀռ�
	NCGroup *pGroup;
	for(int i = 0 ; i < m_GroupList.GetSize(); i++)
	{
		pGroup =(NCGroup *) m_GroupList.GetAt(i);
		pGroup->FreeGroup();
		delete pGroup;
	}
	m_GroupList.RemoveAll();
}

BOOL NDataModelDataSet::AddGroupField(CString sField,int order)
{
	NDataField* pfield = GetField(GetFieldIndex(sField));
	if (pfield == NULL) 
	{
		return FALSE;
	}
	int iTotal = (int)m_Groupfields.GetSize();
	for (int i=0 ;i<iTotal ;i++)
		if(m_Groupfields.GetAt(i) == (void*)pfield)
			return  TRUE;

	m_GroupSortColAscending.Add(order);
	m_Groupfields.Add(pfield);

	return TRUE;
	//	m_pDataSetFilterExp->m_PrimaryBuffers->AddSortField(m_pDataSetFilterExp->GetField(m_pDataSetFilterExp->GetFieldIndex("sortfield")));
}

void NDataModelDataSet::ResetGroupField()
{
	m_Groupfields.RemoveAll();
	m_GroupSortColAscending.RemoveAll();
}

CString NDataModelDataSet::GetGroupString(NCGroup *pGroup)
{
	CString sResult;
	if (pGroup != NULL) 
	{
		void* RecordID = pGroup->GetRecordID();
		if(pGroup->GetGroupField() != NULL && RecordID!= NULL)
		{
			void *ptr = GetFieldDataPT(((NDataField *)pGroup->GetGroupField()),RecordID);
			return ((NDataField *)(pGroup->GetGroupField()))->GetAsString(ptr);
		}
	}
	return sResult;
}

BOOL NDataModelDataSet::IsGroup()
{
	return m_isGrouped;
}

NCGroup* NDataModelDataSet::GetGroupObj(int index)
{
	return (NCGroup*)m_GroupList[index];
}

int NDataModelDataSet::GetGroupCount()
{
	return (int)m_GroupList.GetSize();
} 

void NDataModelDataSet::AddSortField(CString sField, BOOL Ascending)
{
	NDataField * field = GetField(GetFieldIndex(sField));
	if (field == NULL ) return;
	m_PrimaryBuffers->AddSortField(field,Ascending);
}

void NDataModelDataSet::AddSortField(int iFieldIndex,BOOL Ascending)
{
	NDataField * field = GetField(iFieldIndex);
	if (field == NULL ) return;
	m_PrimaryBuffers->AddSortField(field,Ascending);
}

void NDataModelDataSet::ResetSortField()
{
	m_PrimaryBuffers->ResetSortFields();
}

void NDataModelDataSet::Sort()
{
	if(m_PrimaryBuffers->GetRowCount()<=1)
	{
		return;
	}
	m_PrimaryBuffers->Sort(1,m_PrimaryBuffers->GetRowCount());
}


//���ַ�����ת����SQL��ʽ
//ע��������ת��Ϊ�ɶ�
CString NDataModelDataSet::Value2SQLValue(CString& sValue)
{
	CString s;
	//int i;
	//TCHAR chr;
	if(sValue.IsEmpty()) return s;
	if(sValue.Find(SINGLEQUATE)==-1) {
		s=sValue;
		return s;
	}
	s = sValue;
	CString oldstr = _T("'");
	CString newstr = _T("''");
	s.Replace(oldstr,newstr);
	// 	for(i=0; i<str.GetLength(); i++) {
	// 		chr=str.GetAt(i);
	// 		if(chr==SINGLEQUATE) s+=SINGLEQUATE;
	// 		s+=chr;
	// 	}
	return s;
}

int  NDataModelDataSet::CopyFilterToPrimaryBuffer()
{
	const int filterBufferCount = (int)m_FilterBuffers->m_Buffers.GetSize();
	if(filterBufferCount >0 ) (m_PrimaryBuffers->m_Buffers).Append(m_FilterBuffers->m_Buffers);
	return filterBufferCount;
}
void NDataModelDataSet::RemoveCopyedFilterToPrimaryBuffer(const int bufferSize)
{
	if (bufferSize >0)
	{
		m_PrimaryBuffers->m_Buffers.RemoveAt((UINT)m_PrimaryBuffers->m_Buffers.GetSize() - bufferSize,bufferSize);
	}
}

BOOL NDataModelDataSet::LoadDataSetXMLBuf_NoDataBefore(const CString & strXMLBuf, BOOL bAppend)
{
	SetState(dmstRetrieve);
	CArray<int ,int > ModifyFieldArray;
	//	HRESULT hr;
	COleDateTime tempDT; 
	//CStringArray arrTable;
	CString s, TableName, FieldName, Data,sAttri;
	NRecordID recordID;
	int row, col/*, datatype*/;

	//����нṹ
	if(bAppend == FALSE) Reset();
	if(!GetFieldCount()) {
		//Error(_T("Column structure not create yet."));
		return FALSE;
	}
	if(strXMLBuf.GetLength() <= 0)
		return TRUE;

	//��ʼMSXML����ָ��

	//load from  buf
	COXParser markUpXml;

	if(!markUpXml.Parse_C(strXMLBuf))
		return TRUE;

	//��Ԫ��
	//	ele=doc->documentElement;
	COXParserElement * pEle = markUpXml.Root();
	if(NULL == pEle)
	{
		SetState(dmstNormal);
		return TRUE;
	}

	COXParserElement * pNodeDataInstance = pEle->Object_Ele(0)->Object_Ele(0);
	COXParserElement * pNodeDatabefore = pEle->Object_Ele(0)->Object_Ele(1);
	COXParserElement * pNodeRowbefore = pNodeDatabefore->Object_Ele(0);
	//TableName=(LPCTSTR)NodeTable->nodeName;
	//if(CPub::ArrayCompareNoCase(arrTable, TableName)==-1) continue;

	int rowid;
	BOOL isNew = FALSE;
	//����н������ݽ���
	//	for(pmcommon::CMarkupNode NodeRow=NodeDataInstance.GetChild(); NodeRow.IsValid(); NodeRow=NodeRow.GetSibling()) 
	for(int i = 0; i < pNodeDataInstance->NumElement(); i++) 
	{
		COXParserElement * pNodeRow = pNodeDataInstance->Object_Ele(i);
		ModifyFieldArray.RemoveAll();
		row=InsertRow(0);
		rowid = row;
		//���ݻ����������е��ƶ�
		//ȡ�ô洢��
		//		attributes = NodeRow->attributes;
		//		attri = attributes->getNamedItem(_T("buff"));
		sAttri = pNodeRow->GetAttributeValue(_T("buff"));

		if(sAttri.GetLength() <= 0)
		{
			sAttri = _T("filter");
		}

		recordID = GetRecordID(row);
		if (sAttri != _T("primary")) 
		{
			if(sAttri == _T("filter"))
			{
				m_FilterBuffers->m_Buffers.Add(m_PrimaryBuffers->m_Buffers.GetAt(row - 1));
				m_PrimaryBuffers->m_Buffers.RemoveAt(row - 1);
				rowid = m_FilterBuffers->GetRowCount();
			}
			else if(sAttri == _T("delete"))
			{
				m_DeleteBuffers->m_Buffers.Add(m_PrimaryBuffers->m_Buffers.GetAt(row - 1));
				m_PrimaryBuffers->m_Buffers.RemoveAt(row - 1);
				rowid = m_DeleteBuffers->GetRowCount();
			}
		}
		//ȡ����id
		//attri = NodeRow->getAttributeNode("rowid");

		//ȡ���Ƿ�������
		//attri = NodeRow->getAttributeNode("IsNew");
		//		attributes = NodeRow->attributes;
		//		attri = attributes->getNamedItem(_T("IsNew"));
		sAttri = pNodeRow->GetAttributeValue(_T("IsNew"));

		if (sAttri == _T("true")) {
			//isNew = TRUE;
			SetRowNewInternal(recordID,TRUE);
		}

		//ȡ���Ƿ��Ѿ������仯
		//attri = NodeRow->getAttributeNode("hasChanges");
		//		attributes = NodeRow->attributes;
		//		attri = attributes->getNamedItem(_T("hasChanges"));
		sAttri = pNodeRow->GetAttributeValue(_T("hasChanges"));

		SetState(dmstRetrieve);
		//		for(pmcommon::CMarkupNode NodeField=NodeRow.GetChild(); NodeField.IsValid(); NodeField=NodeField.GetSibling())  //������ֵ
		for(int j = 0; j < pNodeRow->NumElement(); j++)  //������ֵ
		{
			COXParserElement * pNodeField = pNodeRow->Object_Ele(j);
			//			FieldName=(LPCTSTR)NodeField->nodeName;
			FieldName = pNodeField->GetName();
			if(FieldName.Find(_T("."))==-1 && TableName.IsEmpty()==FALSE)
				FieldName=/*TableName +_T(".")+*/ FieldName;
			col=GetFieldIndex(FieldName);
			if(col==-1) continue;
			//			Data=(LPCTSTR)NodeField->text;
			Data = pNodeField->GetElementValue();

			if(GetFieldType(col) == ftDateTime)
			{
				tempDT.ParseDateTime(Data);
				if ((tempDT.GetStatus() == COleDateTime::invalid)&&Data.GetLength() > 0) 
				{
					tempDT = CPub::CString2COleDateTime(Data);
					Data = tempDT.Format();
				}
			}

			this->SetAsString(col,recordID,Data);
			SetState(dmstRetrieve);
		}
		//�������ڵĴ洢�������ƶ��洢��
	}
	//}
	
//	doc=NULL;
//	ele=NULL;
//	attributes = NULL;
//	NodeField=NULL;
//	attri = NULL;
//	NodeRow = NULL;
//	NodeField=NULL;
//	NodeDataInstance = NULL;
//	NodeDatabefore = NULL;
//	NodeRowbefore = NULL;
	SetState(dmstNormal);
	ResetUpdate(FALSE);
	return TRUE;
}

LONG NDataModelDataSet::GetNDataMemorySize()
{
	LONG lMemorySize = sizeof(*this);
	lMemorySize -= sizeof(*(NDataModelBaseDataSet*)this);
	lMemorySize += NDataModelBaseDataSet::GetNDataMemorySize();

	if(NULL != m_PrimaryBuffers)
		lMemorySize += m_PrimaryBuffers->GetBufferMemorySize();

	if(NULL != m_FilterBuffers)
		lMemorySize += m_FilterBuffers->GetBufferMemorySize();

	if(NULL != m_DeleteBuffers)
		lMemorySize += m_DeleteBuffers->GetBufferMemorySize();

	return lMemorySize;
}

BOOL NDataModelDataSet::FilterDataSetRow(int iRow)
{
	if(iRow <= 0 || iRow > GetRowCount())
		return FALSE;

	int iTemIndex = iRow - 1;
	//���� ��ָ��
	void * pTemBuffer = m_PrimaryBuffers->GetBuffPtr(iTemIndex);
	//�Ƴ�
	m_PrimaryBuffers->m_Buffers.RemoveAt(iTemIndex);

	//���� ��ָ��
	m_FilterBuffers->AddBuff(pTemBuffer);
	return TRUE;
}

BOOL NDataModelDataSet::CheckIsNeedSort(CStringArray & strFieldNameArr,CArray<int,int> & iSortAscendingArr,BOOL checkFilter)
{
	if(TRUE == checkFilter)
	{
		int iFilterRowCount = m_FilterBuffers->GetRowCount();
		if(iFilterRowCount > 0)
			return TRUE;
	}

	int iCheckFieldSize = (int)strFieldNameArr.GetSize();
	int iCheckSortAscendSize = (int)iSortAscendingArr.GetSize();
	if(iCheckFieldSize != iCheckSortAscendSize)
		return TRUE;

	CPtrArray checkFieldPtrArr;
	int i = 0;
	for(i = 0; i < strFieldNameArr.GetSize();)
	{
		NDataField * pTemField = GetField(GetFieldIndex(strFieldNameArr.GetAt(i)));
		if(NULL == pTemField)
		{
			strFieldNameArr.RemoveAt(i);
			iSortAscendingArr.RemoveAt(i);
			continue;
		}
		checkFieldPtrArr.Add(pTemField);
		i++;
	}
	if(checkFieldPtrArr.GetSize() != m_PrimaryBuffers->m_Sortdfields.GetSize())
		return TRUE;
	for (i = 0; i < checkFieldPtrArr.GetSize();i++)
	{
		if(checkFieldPtrArr.GetAt(i) == m_PrimaryBuffers->m_Sortdfields.GetAt(i))
		{
			if(iSortAscendingArr.GetAt(i) == m_PrimaryBuffers->m_SortColAscending.GetAt(i))
				continue;
		}
		return TRUE;
	}

	////////////////////////////////////////////
	/// liwenlang [2012:2:29   15:35]
	/// ˵��:
	/// ����� ���˵Ļ�����������
	if(0 < m_FilterBuffers->GetRowCount())
		return TRUE;;

	return FALSE;
}

BOOL NDataModelDataSet::BuildSimRow2Buffer(NRecordID pRowRecordID,LPVOID & pBuffer,ULONG & lBufferSize) const
{//�ڲ�������Ϣʹ��ACSII������
	if(NULL == pRowRecordID)
		return FALSE;

	lBufferSize = sizeof(long);//��ͷλ�ñ�������Ϣ���ȱ��

	int iFieldCount = m_fields.GetCount();
	LPVOID * pFieldValueArr = new LPVOID[iFieldCount];
	long * pFieldSizeArr = new long[iFieldCount];
	int i = 0;
	for (i = 0;i < iFieldCount;i++)
	{
		NDataField * pTemField = m_fields.GetField(i);

		LPVOID pTemFieldValue = NULL;
		int iTemFieldSize = 0;

		if(ftBlob == pTemField->GetDataType())
		{
			iTemFieldSize = pTemField->GetBlob(GetFieldDataPT(pTemField,pRowRecordID,FALSE),pTemFieldValue);
			if(NULL == pTemFieldValue || iTemFieldSize < 0)
			{
				pTemFieldValue = NULL;
				iTemFieldSize = 0;
			}
			if(pTemFieldValue)
			{
				if(pTemField->IsUpdateAsBlobFiled())
				{
					pFieldValueArr[i] = pTemFieldValue;
					pFieldSizeArr[i] = iTemFieldSize;//����TCHARΪ��Blob�ֶ��е��ַ����󱣴�ʱ���н�����־
				}
				else
				{
					TCHAR * pTemInfo = new TCHAR[iTemFieldSize + 1];
					memset(pTemInfo,0,sizeof(TCHAR) * iTemFieldSize + sizeof(TCHAR));
					memcpy(pTemInfo,pTemFieldValue,iTemFieldSize);

					LPSTR pTemSTR = CPub::PM_T2A((LPCTSTR)pTemInfo);
					delete []pTemInfo;

					iTemFieldSize = strlen(pTemSTR) + 1;//��֤β����1�����ֽ���Ϊ ����

					pFieldValueArr[i] = pTemSTR;
					pFieldSizeArr[i] = iTemFieldSize;//
				}
			}
			else
			{
				pFieldValueArr[i] = pTemFieldValue;
				pFieldSizeArr[i] = iTemFieldSize;
			}
			iTemFieldSize += sizeof(long);
		}
		else if(ftString == pTemField->GetDataType())
		{
			pTemFieldValue = GetFieldDataPT(pTemField,pRowRecordID,FALSE);
			//		int iTemsfef = strlen((char*)pTemFieldValue) + sizeof(TCHAR);
			iTemFieldSize = pTemField->GetSize();

			TCHAR * pTemInfo = new TCHAR[iTemFieldSize];
			memset(pTemInfo,0,sizeof(TCHAR) * iTemFieldSize);
			memcpy(pTemInfo,pTemFieldValue,iTemFieldSize);

			LPSTR pTemSTR = CPub::PM_T2A((LPCTSTR)pTemInfo);
			delete []pTemInfo;

			iTemFieldSize = strlen(pTemSTR) + 1;//��֤β����1�����ֽ���Ϊ ����
			pFieldValueArr[i] = pTemSTR;
			pFieldSizeArr[i] = iTemFieldSize;//

			iTemFieldSize += sizeof(long);
		}
		else
		{
			pTemFieldValue = GetFieldDataPT(pTemField,pRowRecordID,FALSE);
			iTemFieldSize = pTemField->GetSize();

			pFieldValueArr[i] = pTemFieldValue;
			pFieldSizeArr[i] = iTemFieldSize;
		}

		lBufferSize += iTemFieldSize;
	}

	pBuffer = new BYTE [lBufferSize];
	if (pBuffer == NULL)
	{
		delete []pFieldValueArr;
		delete []pFieldSizeArr;

		CString strTemMsg = _T("CPM_StorageDataSet::BuildSimRowBuffer,������ �ڴ�����ʧ��!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	ZeroMemory(pBuffer,lBufferSize);
	CopyMemory(pBuffer,&lBufferSize,sizeof(long));

	LPVOID pCurFieldValue = (BYTE*)pBuffer + sizeof(long);
	///////////////////////////////////
	///liwenlang 
	///д�� Field����
	///[28/5/2010]
	//////////////////////////////////
	for (i = 0;i < iFieldCount;i++)
	{
		NDataField * pTemField = m_fields.GetField(i);
		if(ftBlob == pTemField->GetDataType())
		{
			CopyMemory(pCurFieldValue,&pFieldSizeArr[i],sizeof(long));
			CopyMemory((BYTE*)pCurFieldValue + sizeof(long),pFieldValueArr[i],pFieldSizeArr[i]);//��ȥTCHARΪ�˲��´��������Ϣ��
			pCurFieldValue = (BYTE*)pCurFieldValue + pFieldSizeArr[i] + sizeof(long);

			if(!pTemField->IsUpdateAsBlobFiled())
				delete []pFieldValueArr[i];
		}
		else if(ftString == pTemField->GetDataType())
		{
			////////////////////////////////////////////
			/// liwenlang [2011:7:28   21:27]
			/// ˵��:
			/// ����֮ǰ�汾�ڸ��Ƶ�ʱ���ѱ�עλ������� Unicode �Ḵ��һ�����ֵ�� �ļ��С����� Field����Ҫ��TCHAR����
			int iTemFieldSize = pFieldSizeArr[i];
			CopyMemory(pCurFieldValue,&iTemFieldSize,sizeof(long));
			CopyMemory((BYTE*)pCurFieldValue + sizeof(long),pFieldValueArr[i],iTemFieldSize);
			pCurFieldValue = (BYTE*)pCurFieldValue + pFieldSizeArr[i] + sizeof(long);

			delete []pFieldValueArr[i];
		}
		else
		{
			CopyMemory((BYTE*)pCurFieldValue,pFieldValueArr[i],pFieldSizeArr[i]);
			pCurFieldValue = (BYTE*)pCurFieldValue + pFieldSizeArr[i];
		}
	}

	delete []pFieldValueArr;
	delete []pFieldSizeArr;
	return TRUE;
}

BOOL NDataModelDataSet::ReadBuffer2SimRow(LPVOID pRowBufferInfo,ULONG lRowBufferSize)
{
	///////////////////////////////////
	///liwenlang 
	///���� ��ͨ �ֶ�����
	///[1/6/2010]
	//////////////////////////////////
	LPVOID pRowBuffer = AlloRecordBuffer();
	int iFieldCount = m_fields.GetCount();
	LPVOID pStreamBuffer = (BYTE*)pRowBufferInfo + sizeof(long);
	int i = 0;
	for (i = 0;i < iFieldCount;i++)
	{
		NDataField * pField = m_fields.GetField(i);
		if(ftBlob == pField->GetDataType())
		{
			void *pRowbufferOffset = GetFieldDataPT(pField,pRowBuffer,FALSE);
			ZeroMemory(pRowbufferOffset,sizeof(void*));
			ZeroMemory((char*)pRowbufferOffset+sizeof(void*),sizeof(long));

			long lSize;
			CopyMemory(&lSize,pStreamBuffer,sizeof(long));
			if(lRowBufferSize < lSize || lSize < 0)
			{
				CDataSetErrorInfo::Instance()->PrintDataSetInfo(this,pRowBuffer,pField);
				CString strTemMsg;
				strTemMsg.Format(_T("CPM_StorageDataSet::ReadRowFStream_SVersion	BlobFieldSize:%d����RowBufferSize:%d"),lSize,lRowBufferSize);
				CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
				return FALSE;
			}

			LPVOID pCurBlob = (BYTE*)pStreamBuffer + sizeof(long);

			/********************************************************************
			liwenlang
			ʱ��:2010:9:29   16:04
			��ע:���ݱ��뷽ʽת������ lSize����0 ���ô���
			*********************************************************************/
			if(FALSE == pField->IsUpdateAsBlobFiled() && lSize > 0)
			{//ֻ���� �ַ����� Blob
				LPTSTR pTemBuffer = CPub::PM_A2T((LPSTR)pCurBlob);
				pField->SetBlob(pRowbufferOffset,pTemBuffer,_tcslen(pTemBuffer) * sizeof(TCHAR),FALSE);
				delete []pTemBuffer;
			}
			else
				pField->SetBlob(pRowbufferOffset,pCurBlob,lSize,FALSE);


			///////////////////////////////////
			///liwenlang 
			///��ָ�� �ƶ��� �¸�λ��
			///[1/6/2010]
			//////////////////////////////////
			pStreamBuffer = (BYTE*)pStreamBuffer + sizeof(long) + lSize;
		}
		else if(ftString == pField->GetDataType())
		{
			void *pRowbufferOffset = GetFieldDataPT(pField,pRowBuffer,FALSE);
			long lSize;
			CopyMemory(&lSize,pStreamBuffer,sizeof(long));

			if(lRowBufferSize < lSize)
			{
				CDataSetErrorInfo::Instance()->PrintDataSetInfo(this,pRowBuffer,pField);
				CString strTemMsg;
				strTemMsg.Format(_T("CPM_StorageDataSet::ReadRowFStream_SVersion	StringFieldSize:%d����RowBufferSize:%d"),lSize,lRowBufferSize);
				CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
				return FALSE;
			}

			LPVOID pTemFieldValue = (BYTE*)pStreamBuffer + sizeof(long);

			LPTSTR pTemBuffer = CPub::PM_A2T((LPSTR)pTemFieldValue);
			CopyMemory(pRowbufferOffset,pTemBuffer,_tcslen(pTemBuffer) * sizeof(TCHAR));
			delete []pTemBuffer;

			///////////////////////////////////
			///liwenlang 
			///��ָ�� �ƶ��� �¸�λ��
			///[1/6/2010]
			//////////////////////////////////
			pStreamBuffer = (BYTE*)pStreamBuffer + sizeof(long) + lSize;
		}
		else
		{
			int iTemFieldSize = pField->GetSize();
			void *pRowbufferOffset = GetFieldDataPT(pField,pRowBuffer,FALSE);
			ZeroMemory(pRowbufferOffset,iTemFieldSize + m_fields.GetFieldInfoSize());

			CopyMemory(pRowbufferOffset,pStreamBuffer,iTemFieldSize);

			pStreamBuffer = (BYTE*)pStreamBuffer + iTemFieldSize;
		}
	}

	//��ʼ�������ñ�־2003.6.18
	NDataField *field = NULL;
	void *ptr;
	for(i = 0 ; i < m_fields.GetCount(); i++)
	{
		field = m_fields.GetField(i);
		if (field != NULL) 
		{
			ptr = GetFieldDataPT(field,pRowBuffer);
			field->FreeOrigValueBuff(ptr,FALSE);
			field->SetItemModified((char*)ptr,FALSE);
		}
	}

	SetRowNewInternal(pRowBuffer,FALSE);
	SetRowModifiedInternal(pRowBuffer,FALSE);
	m_PrimaryBuffers->InsertBuff(pRowBuffer,-1);
	return TRUE;
}

BOOL NDataModelDataSet::DataSetRows2BufferArr(CPtrArray & rowPtArr) const
{
	int iRowCount = m_PrimaryBuffers->GetRowCount();
	rowPtArr.SetSize(iRowCount);

	for (int i = 0;i < iRowCount;i++)
	{
		NRecordID temRecordID = m_PrimaryBuffers->GetBuffPtr(i);

		LPVOID pbuffer = NULL;
		ULONG lBufferSize = 0;
		if(!BuildSimRow2Buffer(temRecordID,pbuffer,lBufferSize))
		{
			return FALSE;
		}

		rowPtArr.SetAt(i,pbuffer);
	}
	return TRUE;
}

BOOL NDataModelDataSet::BufferArr2DataSetRows(const CPtrArray & rowPtArr)
{
	for(int i = 0;i < rowPtArr.GetSize();i++)
	{
		NRecordID pTemRecordID = rowPtArr.GetAt(i);

		long lRowSize = 0;
		memcpy(&lRowSize,pTemRecordID,sizeof(long));

		if(!ReadBuffer2SimRow(pTemRecordID,lRowSize))
			return FALSE;
	}

	return TRUE;
}