// NStringField.cpp: implementation of the NStringField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NStringField.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NStringField::NStringField(void *pOwner) : NDataField(pOwner)
{
	SetDataType(ftString);
   
}

NStringField::~NStringField()
{
   FreeFilterValue();
}

CString NStringField::GetAsString(const void *buffer)
{
	CString str((_TCHAR *)buffer);
	return str;
}


int NStringField::GetAsInteger(const void *buffer)
{
	return _ttoi(GetAsString(buffer));;//atoi(GetAsString(buffer));
}


double NStringField::GetAsFloat(const void *buffer)
{
	return /*atof*/_ttoi(GetAsString(buffer));
}

COleDateTime NStringField::GetAsDateTime(const void *buffer)
{
	COleDateTime datetime;
	datetime.ParseDateTime(GetAsString(buffer));
	return datetime;
}

BOOL NStringField::SetAsString(const CString & val,void *buffer)
{
	int size;
	if ((GetSize()/sizeof(_TCHAR)) >= val.GetLength()+1)
		size = val.GetLength()+1;
	else
		size = GetSize()/sizeof(_TCHAR);
	
	LPCTSTR str = (LPCTSTR)val;
	//CopyMemory(buffer,str,size);
	return SetData(buffer,(LPVOID)str,size*sizeof(_TCHAR));
	//return TRUE;
}

BOOL NStringField::SetAsInteger(const long & val,void *buffer)
{
	//itoa(val,(char*)buffer,10);
// 	TCHAR Valbuffer[50];
// 	_itot(val,Valbuffer,10);//itoa(val,(char*)Valbuffer,10);
// 	CString sval = Valbuffer;

	CString sval ; 
	sval.Format( _T("%d") , val ) ; 

	return SetAsString(sval,buffer);
}

BOOL NStringField::SetAsFloat(const double & val,void *buffer)
{
	//char Valbuffer[50];
	//_gcvt(val,10,(char*)Valbuffer);
	//CString sval = Valbuffer;
	CString sval;
	sval.Format(_T("%f"),val);
	return SetAsString(sval,buffer);
	//return SetData(buffer,Valbuffer,GetSize());
	//_gcvt(val,10,(char*)buffer);
	//return TRUE;
}

/*BOOL NStringField::SetAsDateTime(COleDateTime val,void *buffer)
{
	val.Format("");
	return TRUE;
}*/

int NStringField::Compare(const void * pData1,const void * pData2)
{
	LPTSTR str1,str2;
	str1 = (LPTSTR) pData1;
	str2 = (LPTSTR) pData2;
    return _tcscmp(str1 ,str2);
}

BOOL NStringField::FilterCompare(const void * pData1)
{
	if (m_pFilterValue == NULL) {
		return TRUE;
	}
	return (this->*m_pFuncompare)(pData1,m_pFilterValue);
	//return FALSE;
}

BOOL NStringField::FilterCompareEqual(const void * pData1,const void * pData2)
{
	return _tcscmp((LPTSTR)pData2,(LPTSTR)pData1) == 0;
}

BOOL NStringField::FilterCompareLike(const void * pData1,const void * pData2)
{
	return _tcscspn((LPTSTR )pData1,(LPTSTR )pData2) >= 0;
}

BOOL NStringField::FilterCompareGreater(const void * pData1,const void * pData2)
{
	//BOOL test = strcmp((char *)pData1,(char *)pData2) > 0;
	return _tcscmp((LPTSTR )pData1,(LPTSTR )pData2) > 0;
}

BOOL NStringField::FilterCompareLess(const void * pData1,const void * pData2)
{
	return _tcscmp((LPTSTR )pData1,(LPTSTR )pData2) < 0;
}

BOOL NStringField::SetFilter(LPTSTR sOperator ,LPTSTR sValue)
{
	FreeFilterValue();
	//NDataField::SetFilter(sOperator,sValue);
	CString sop = sOperator;
	if (sop == _T("<")) {
		m_pFuncompare = (BOOL (NDataField::*) ( const void *  ,const void * ))(&NStringField::FilterCompareLess);
	}
	else if (sop == _T(">")) {
		m_pFuncompare = (BOOL (NDataField::*) ( const void *  ,const void * ))(&NStringField::FilterCompareGreater);
	}
	else if (sop == _T("=")) {
		m_pFuncompare = (BOOL (NDataField::*) ( const void *  ,const void * ))(&NStringField::FilterCompareEqual);
	}
	else if (sop.CompareNoCase(_T("like"))) {
		m_pFuncompare = (BOOL (NDataField::*) ( const void *  ,const void * ))(&NStringField::FilterCompareLike);
	}
	
// 	m_pFilterValue = new BYTE [_tcslen(sValue) + 1];
// 	
// 	memset(m_pFilterValue,0,_tcslen(sValue) + 1);
// 
// 	_tcscpy((LPTSTR)m_pFilterValue,sValue);
// 	m_FilterOperator = sOperator;
	m_pFilterValue = new BYTE [(_tcslen(sValue) + 1)*sizeof(_TCHAR)] ;
	
	memset(m_pFilterValue,0,(_tcslen(sValue) + 1)*sizeof(_TCHAR));
	
	_tcscpy((LPTSTR)m_pFilterValue,sValue);
	m_FilterOperator = sOperator;
	return TRUE;
	//return TRUE;
}

BOOL NStringField::FreeFilterValue()
{
	//NDataField::FreeFilterValue1();
	if (m_pFilterValue != NULL) {
		delete [] m_pFilterValue;
		m_pFilterValue = NULL;
	}
	return TRUE;
}

BOOL  NStringField::StringTranToThisType(CString sValue,void *& pValue,int &iSize)
{
//	pValue = new char [sValue.GetLength() + 1];
//	//pValue = odt.m_dt;
//	iSize = sValue.GetLength();
//	CopyMemory(pValue,sValue.GetBuffer(0),iSize);
//	return TRUE;

	pValue = new char [(sValue.GetLength() + 1)*sizeof(_TCHAR)];
	//pValue = odt.m_dt;
	memset(pValue,0,(sValue.GetLength() + 1)*sizeof(_TCHAR));
	iSize = sValue.GetLength()*sizeof(_TCHAR);
	CopyMemory(pValue,sValue.GetBuffer(0),iSize);
	return TRUE;
}
