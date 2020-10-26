// NFloatField.cpp: implementation of the NFloatField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NFloatField.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NFloatField::NFloatField(void *pOwner) : NNumericField(pOwner)
{
	SetDataType(ftFloat);
	SetSize(sizeof(double));
}

NFloatField::~NFloatField()
{

}

double NFloatField::GetAsFloat(const void *buff)
{
	double *result;
	result = (double *)buff;
    return *result;
}

int NFloatField::GetAsInteger(const void *buffer)
{
	int result;
	result = (int)GetAsFloat(buffer);
    return result;
}

CString NFloatField::GetAsString(const void *buffer)
{
	double itemp = GetAsFloat(buffer);
	CString cs(_T(""));
	cs.Format(_T("%.3f"),itemp + 0.00001);
	return cs;
}

BOOL NFloatField::SetAsString(const CString & val,void *buffer)
{
	LPCTSTR pdata = (LPCTSTR)val;
	//CopyMemory(buffer,pdata,field->GetSize());
	//(*(double *)buffer) = atof(pdata);
	double dval = _tcstod(pdata,NULL);//double dval = atof(pdata);
	return SetData(buffer,&dval,GetSize());
	//return TRUE;
}

BOOL NFloatField::SetAsInteger(const long & val,void *buffer) 
{
	//(*(double *)buffer) = val;
	double dval = val;
	return  SetData(buffer,&dval,GetSize());
	//return TRUE;
}

BOOL NFloatField::SetAsFloat(const double & val,void *buffer) 
{
	//*(double*)buffer = val;
	return SetData(buffer,&val,GetSize());
	//return TRUE;
}

int NFloatField::Compare(const void * pData1,const void * pData2)
{
	double *Float1,*Float2;
	Float1 = (double *)pData1;
	Float2 = (double *)pData2;
    if ((*Float1)>(*Float2))
		return 1;
	else 
		if((*Float1)<(*Float2))
			return -1;

	return 0;
}

BOOL  NFloatField::StringTranToThisType(CString sValue,void *& pValue,int &iSize)
{
    double iVal = /*atof*/_tcstod(sValue.GetBuffer(0),NULL);
	pValue = new char [GetSize()];
	//pValue = odt.m_dt;
	iSize = GetSize();
	CopyMemory(pValue,&(iVal),iSize);
	return TRUE;
}

BOOL NFloatField::FreeFilterValue()
{
	if (m_pFilterValue != NULL) {
		delete [] m_pFilterValue;
		m_pFilterValue = NULL;
	}
	return TRUE;
}
