// NGuidField.cpp: implementation of the NGuidField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NGuidField.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NGuidField::NGuidField(void *pOwner): NDataField(pOwner)
{
	SetDataType(ftGuid);
	SetSize(sizeof(GUID));
}

NGuidField::~NGuidField()
{

}
CString NGuidField::GetAsString(const void *buffer)
{
//     CString sValue;
// 	GUID guid;
// 	guid = GetAsGUID(buffer);
// 	unsigned char *String;
// 	UuidToString(&guid,&String);
// 	sValue = String;
// 	RpcStringFree(&String);
// 	return sValue;

	//CString sValue;
	//GUID guid;
	//guid = GetAsGUID(buffer);
	//_TUCHAR *String;
	//UuidToString(&guid,String);
	//sValue = String;
	//RpcStringFree(&String);

	//wjy 为编译过 的特殊修改
	CString sValue;
#ifdef _UNICODE
	GUID guid;
	guid = GetAsGUID(buffer);
	unsigned short *String;
	UuidToString(&guid,&String);
	sValue = (TCHAR*)String;
	RpcStringFree(&String);
#else
	GUID guid;
	guid = GetAsGUID(buffer);
	unsigned char *String;
	UuidToString(&guid,&String);
	sValue = String;
	RpcStringFree(&String);
#endif
	return sValue;
}

BOOL NGuidField::SetAsString(const CString & val,void *buffer)
{
// 	GUID guid;
// 	if (UuidFromString((unsigned char *)val.GetBuffer(0),&guid) == RPC_S_OK)
// 	{
// 		SetAsGUID(guid,buffer);
// 		return TRUE;
// 	}
// 	return FALSE;
	GUID guid;
#ifdef _UNICODE
	if (UuidFromString((unsigned short *)(LPCTSTR)val,&guid) == RPC_S_OK)
	{
		SetAsGUID(guid,buffer);
		return TRUE;
	}
#else
	if (UuidFromString((unsigned char *)(LPCTSTR)val,&guid) == RPC_S_OK)
	{
		SetAsGUID(guid,buffer);
		return TRUE;
	}
#endif
	return FALSE;
}

GUID NGuidField::GetAsGUID(const void *buffer)
{
    GUID guid;
    guid = *(GUID*)buffer;
	return guid;
}

BOOL NGuidField::SetAsGUID(const GUID & val,void *buffer)
{
    return SetData(buffer,&(val),GetSize());
}
