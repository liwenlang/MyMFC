// NDataModelBaseDataSet.cpp: implementation of the NDataModelBaseDataSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NDataModelBaseDataSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NDataModelBaseDataSet::NDataModelBaseDataSet()
{
//   m_pOleDb = NULL;
}

NDataModelBaseDataSet::~NDataModelBaseDataSet()
{

}


NDataFields * NDataModelBaseDataSet::GetFieldsObj()
{
   return &m_fields;
}

BOOL NDataModelBaseDataSet::AddField(CString sFieldName, FieldType fieldtype, int size)
{
	BOOL bresult;
	bresult = m_fields.CreateField(this,sFieldName, fieldtype ,size);
	if (bresult) InteralOpen();
	return bresult;
}

int NDataModelBaseDataSet::GetFieldIndex(const CString & sFieldName) const
{
   return m_fields.GetFieldIndex(sFieldName);
}

NDataField * NDataModelBaseDataSet::GetField(int index) const
{
	if(index < 0)
		return NULL;
   return m_fields.GetField(index);
}

int NDataModelBaseDataSet::GetFieldCount() const
{
   return m_fields.GetCount();
}

BOOL NDataModelBaseDataSet::OnRowFilter(int row)
{
   return TRUE;
}

void NDataModelBaseDataSet::FillFilterBuff(CPtrArray &filterBuffer)
{

}

CString NDataModelBaseDataSet::GetFieldName(int index) const 
{
	CString sFieldName;
	NDataField *pField = m_fields.GetField(index);
	if (pField != NULL) 
	{
		sFieldName = pField->GetFieldName();
	}
	return sFieldName;
}


void NDataModelBaseDataSet::SetTableName(CString stablename)
{
   m_TableName = stablename;
}

CString NDataModelBaseDataSet::GetTableName() const
{
   return m_TableName;
}

void NDataModelBaseDataSet::GetAllField(CArray<NDataField*,NDataField*> & dataFields)
{
	int iFieldCount = m_fields.GetCount();
	for (int i = 0;i < iFieldCount;i++)
	{
		dataFields.Add(m_fields.GetField(i));
	}
}

void NDataModelBaseDataSet::Error(CString &Error)
{

}

LONG NDataModelBaseDataSet::GetNDataMemorySize()
{
	LONG lMemorySize = 0;
	NObserver *ob;
	POSITION pos;
	for( pos = m_ObserverList.GetHeadPosition(); pos != NULL; )
	{
		ob = (NObserver *)m_ObserverList.GetNext( pos );
		lMemorySize += sizeof(*ob);
	}
	lMemorySize += m_fields.GetFieldsMemorySize();
	lMemorySize -= sizeof(m_fields);
	lMemorySize += sizeof(*this);
	return lMemorySize;
}
