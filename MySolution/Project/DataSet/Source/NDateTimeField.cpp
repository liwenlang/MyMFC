// NDateTimeField.cpp: implementation of the NDateTimeField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NDateTimeField.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NDateTimeField::NDateTimeField(void *pOwner) : NDataField(pOwner)
{
	SetDataType(ftDateTime);
	SetSize(sizeof(DATE));
	
	/*int displayFormatSize =  GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SLONGDATE,0,0);//GetDateFormat((LOCALE_USER_DEFAULT),NULL,NULL,NULL,NULL,0);
	if (displayFormatSize>0) 
	{
		char *pbuff = new char[displayFormatSize + 2];
		//m_sDisplayFormat.AllocBuffer(displayFormatSize);
		//GetDateFormat((LOCALE_USER_DEFAULT),DATE_LONGDATE,NULL,NULL,pbuff,displayFormatSize + 2);
		GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SLONGDATE,pbuff,displayFormatSize);
		m_sDisplayFormat = "yyyyƒÍMM‘¬d»’";//pbuff;
		delete []pbuff;
	}*/

}

NDateTimeField::~NDateTimeField()
{

}

void NDateTimeField::SetDisplayFormat(CString sFormat)
{
	m_sDisplayFormat = sFormat;
}

COleDateTime NDateTimeField::GetAsDateTime(const void *buffer)
{
    COleDateTime odt;
    odt.m_dt = *(DATE*)buffer;
	return odt;
}

CString NDateTimeField::GetAsString(const void *buffer)
{
	COleDateTime dt = GetAsDateTime(buffer);
//	CString sdt = dt.Format("%yyyy-%MM-%d");
	//sdt = dt.Format(LOCALE_NOUSEROVERRIDE);
	return  dt.Format(VAR_DATEVALUEONLY);
}


BOOL NDateTimeField::SetAsDateTime(const COleDateTime & val,void *buffer)
{
	/*int size = GetSize();
	CopyMemory(buffer,&(val.m_dt),size);
	return TRUE;*/
	return SetData(buffer,&(val.m_dt),GetSize());
}

BOOL NDateTimeField::SetAsString(const CString & val,void *buffer)
{
    COleDateTime odt;
	LPCTSTR str = (LPCTSTR)val;
    odt.ParseDateTime(str);
	/*CopyMemory(buffer,&(odt.m_dt), GetSize());
	return TRUE;*/
	return SetAsDateTime(odt,buffer);
}

BOOL  NDateTimeField::StringTranToThisType(CString sValue,void *& pValue,int &iSize)
{
    COleDateTime odt;
	PTSTR str = sValue.GetBuffer(0);
    odt.ParseDateTime(str);
	pValue = new char [sizeof(double)];
	//pValue = odt.m_dt;
	iSize = GetSize();
	CopyMemory(pValue,&(odt.m_dt),iSize);
	return TRUE;
}
