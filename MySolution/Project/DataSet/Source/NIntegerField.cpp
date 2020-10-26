// NIntegerField.cpp: implementation of the NIntegerField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NIntegerField.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NIntegerField::NIntegerField(void *pOwner) : NNumericField(pOwner)
{
	SetDataType(ftInteger);
	SetSize(sizeof(int));
	//m_iFilterValue = 0;
}

NIntegerField::~NIntegerField()
{
   FreeFilterValue();
}

int NIntegerField::GetAsInteger(const void *buffer)
{
	int *result;
	result = (int *)buffer;
    return *result;
}

BOOL NIntegerField::SetAsString(const CString &val,void *buffer)
{
	LPCTSTR pdata = (LPCTSTR)val;
	//CopyMemory(buffer,pdata,field->GetSize());
	//(*(int *)buffer) = atoi(pdata);
	int iVal = /*atoi*/_ttoi(pdata);
	return SetData(buffer,&iVal,GetSize());
	//return TRUE;
}

CString NIntegerField::GetAsString(const void *buffer)
{
	int itemp = GetAsInteger(buffer);
	CString cs(_T(""));
	cs.Format(_T("%d"),itemp);
	return cs;
}

double NIntegerField::GetAsFloat(const void *buffer) 
{
	return (double)GetAsInteger(buffer);
}

/*VARIANT NIntegerField::GetAsVariant(void * buffer) 
{
	COleVariant var;
	var.Attach(int)buffer
    return var;
	//VARIANT var = (VARIANT)(*buffer);
}*/

BOOL NIntegerField::SetAsInteger(const long & val,void *buffer) 
{
	//(*(long *)buffer) = val;
	return SetData(buffer,&val,GetSize());
	//return TRUE;
}

BOOL NIntegerField::SetAsFloat(const double & val,void *buffer) 
{
	int iVal = int((val));
	return SetData(buffer,&iVal,GetSize());
	//return SetAsInteger(int((val)),buffer);
}


/*BOOL NIntegerField::SetAsVariant(VARIANT val,void *buffer) 
{
}*/

int NIntegerField::Compare(const void * pData1,const void * pData2)
{
	int *int1,*int2;
	int1 = (int *)pData1;
	int2 = (int *)pData2;
    if ((*int1)>(*int2))
		return 1;
	else 
		if((*int1)<(*int2))
			return -1;

	return 0;
}

/*
BOOL NIntegerField::FilterCompare(const void * pData1)
{
	if (m_pFilterValue == NULL) {
		return TRUE;
	}
	return m_pFilterFuncompare(pData1,m_pFilterValue,m_dataSize);
}
BOOL NIntegerField::SetFilter(LPSTR sOperator ,LPSTR sValue)
{
	FreeFilterValue();
	//NDataField::SetFilter(sOperator,sValue);
	CString sop = sOperator;
	if (sop == "<") {
		m_pFilterFuncompare = (IsLess);
	}
	else if (sop == ">") {
		m_pFilterFuncompare = (IsGreater);
	}
	else if (sop == "=") {
		m_pFilterFuncompare = (IsEqual);
	}
	else  {
		return FALSE;
	}
	int val = _ttoi(sValue);
	m_pFilterValue = new BYTE [sizeof(int)];
	memcpy(m_pFilterValue,&val,sizeof(int));
	m_FilterOperator = sOperator;
	return TRUE;
}*/

BOOL NIntegerField::FreeFilterValue()
{
	if (m_pFilterValue != NULL) {
		delete [] m_pFilterValue;
		m_pFilterValue = NULL;
	}
	return TRUE;
}


BOOL  NIntegerField::StringTranToThisType(CString sValue,void *& pValue,int &iSize)
{
    int iVal =  _ttoi(sValue.GetBuffer(0));//atoi(sValue.GetBuffer(0));
	pValue = new char [GetSize()];
	//pValue = odt.m_dt;
	iSize = GetSize();
	CopyMemory(pValue,&(iVal),iSize);
	return TRUE;
}
