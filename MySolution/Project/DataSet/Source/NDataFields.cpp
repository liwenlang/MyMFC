 // NDataFields.cpp: implementation of the NDataColumns class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NDataFields.h"
#include "NDateTimeField.h"
#include "NIntegerField.h"
#include "NStringField.h"
#include "NFloatField.h"
#include "NBlobField.h"
#include "NGuidField.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NDataFields::NDataFields()
{
   m_HasBlob = FALSE;
}

NDataFields::~NDataFields()
{
	Clear();
}

int NDataFields::GetCount() const
{
	return (int)m_list.GetSize();
}

void NDataFields::SetField(int Index ,NDataField *Field)
{
	//POSITION pos = m_list.FindIndex(Index);
	m_list.InsertAt(Index, Field);
}

/*NDataField *NDataFields::GetField(int index)
{
	//POSITION pos = m_list.FindIndex(index);
	//if (pos == NULL) return NULL;
	return (NDataField *)m_list.GetAt(index);
}

NDataField *NDataFields::GetField(CString sColname)
{
	NDataField *field;
	if (m_MapNameField.Lookup(sColname,(void *&)field) ==0)
		return NULL;
	return field;
}*/

BOOL NDataFields::CreateField(void *pOwner,CString name, FieldType fieldtype, int size)
{
	//POSITION pos = m_list.GetTailPosition();
	NDataField *newfield;
	switch(fieldtype) {
	case ftString:
		newfield=new NStringField(pOwner);
        newfield->SetSize(/*size+1*/(size + 1)*sizeof(_TCHAR)); //存储结束符号
		break;
	case ftInteger:
		newfield=new NIntegerField(pOwner);
		break;
	case ftFloat:
		newfield=new NFloatField(pOwner);
		break;
	case ftDateTime:
		newfield=new NDateTimeField(pOwner);
		break;
    case ftBlob:
		newfield=new NBlobField(pOwner);
		m_HasBlob = TRUE;
		m_Bloblist.Add(newfield);
		break;
    case ftGuid:
		newfield=new NGuidField(pOwner);
		break;
	default:
		MessageBox(NULL,_T("不支持的数据字段类型！"),_T("提示!"),MB_OK);
		return FALSE;
	}
//	name.MakeLower();
	newfield->SetFieldName(name);
	newfield->SetDataType(fieldtype);
	m_list.Add(newfield); 
	m_MapNameField.SetAt(name.GetBuffer(0),newfield);
	newfield->SetFieldNo((int)m_list.GetSize());
	RefreshFieldOffset();
    return TRUE;
}

NDataField * NDataFields::GetField(const CString & sColname) const
{
	NDataField *field;
//	CString strTemColName = sColname;
//	sColname.MakeLower();
	if (m_MapNameField.Lookup(sColname,(void *&)field) ==0)
		return NULL;
	return field;

}

void NDataFields::Clear()
{
	NDataField *field;
	int iTotalObj = (int)m_list.GetSize();
	//POSITION pos = m_list.GetHeadPosition();
	for(int i = 0 ;i <= iTotalObj - 1 ; i++)
	{
		field = (NDataField *)m_list.GetAt(i);
		delete field;
	}
	m_list.RemoveAll();

	/********************************************************************
	liwenlang
		时间:2010:9:26   13:48
		备注:Blob列表没有清空
	*********************************************************************/
	m_Bloblist.RemoveAll();

	m_MapNameField.RemoveAll();
	RefreshFieldOffset();
}

/*int NDataFields::GetFieldOffset(NDataField *field)
{
	return m_FieldOffset.GetAt(field->m_fieldNo -1);
}*/

void NDataFields::RefreshFieldOffset()
{
	m_FieldOffset.RemoveAll();
	int offset = 0;
	NDataField *field;
	int iTotalObj = (int)m_list.GetSize();
	//POSITION pos = m_list.GetHeadPosition();
	for(int i = 0 ;i <= iTotalObj - 1 ; i++)
	{
		m_FieldOffset.Add(offset);
		field = (NDataField *)m_list.GetAt(i);
		field->SetFieldNo(i + 1);
		offset +=field->GetSize() + GetFieldInfoSize();
	}
}

int NDataFields::GetRecordSize()
{
	int recordsize = 0;
	NDataField *field;
	int iTotalObj = (int)m_list.GetSize();
	//POSITION pos = m_list.GetHeadPosition();
	for(int i = 0 ;i <= iTotalObj - 1 ; i++)
	{
		field = (NDataField *)m_list.GetAt(i);
		recordsize +=field->GetSize() + GetFieldInfoSize();
	}
	return recordsize;
}

int NDataFields::GetFieldIndex(const CString & fieldName) const
{
   	NDataField *field;
 //   CString strTemFieldName = fieldName;
//	fieldName.MakeLower();
	if (m_MapNameField.GetCount() > 0) 
	{
		if (m_MapNameField.Lookup(fieldName,(void *&)field) ==0)
			return -1;
	}
	else
	{
		return -1;
	}
	for(int i = 0; i < m_list.GetSize(); i++)
	{
		if (m_list[i] == field) {
			return i;
		}
	}
    return -1;
}

inline int NDataFields::GetFieldInfoSize()
{                //状态位     指向原始的数据位置的指针
	return sizeof(BYTE) + sizeof(void*);
   //return sizeof(FieldInfo);//sizeof(BYTE) + sizeof(long*);
}

LONG NDataFields::GetFieldsMemorySize()
{
	LONG lMemorySize = 0;
	NDataField * pField = NULL;
	int lListSize = (int)m_list.GetSize();
	for (int i = 0;i < lListSize;i++)
	{
		pField = (NDataField *)m_list.GetAt(i);
		lMemorySize += sizeof(*pField);
	}
	return sizeof(*this) + lMemorySize;
}
