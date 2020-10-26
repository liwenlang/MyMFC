// NBlobField.cpp: implementation of the NBlobField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NBlobField.h"
#include "NDataModelDataSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NBlobField::NBlobField(void *pOwner): NDataField(pOwner)
{
	SetDataType(ftBlob);
	SetSize(sizeof(void*) + sizeof(long));
	//m_isBlobFiled = TRUE;
}

NBlobField::~NBlobField()
{

}

long NBlobField::GetBlob(const void *pRowbufferOffset,void *&pBlob)
{
  	//void *ptr;
	long iSize;
    CopyMemory(&iSize,(char*)(pRowbufferOffset)+sizeof(void*),sizeof(long));
	CopyMemory(&pBlob,pRowbufferOffset,sizeof(void*));
	return iSize;
}

BOOL NBlobField::SetBlob(void *pRowbufferOffset,void *pBlob,long iBlockSize,BOOL alloOrigValueBuff)
{
	void *ptr = NULL;
	if(iBlockSize >0 )
	{
		ptr = new char[iBlockSize];
		memcpy(ptr,pBlob,(UINT)iBlockSize);
	}
	return SetBolbData(pRowbufferOffset, ptr, iBlockSize,alloOrigValueBuff);
}

/*void NBlobField::FreeBolbData(void *pRowbufferOffset)
{
	void *ptr;
   	//long iSize;
    //CopyMemory(&iSize,(char*)(pRowbufferOffset)+sizeof(void*),sizeof(long));
	CopyMemory(&ptr,pRowbufferOffset,sizeof(void*));
	if (ptr != NULL) {
		delete [](char*)ptr;
	}
	//将指针清空
	memset(pRowbufferOffset,0,GetSize());
}*/

BOOL NBlobField::SetAsString(const CString & val,void *buffer) 
{
//   return SetBlob(buffer,val.GetBuffer(0),val.GetLength()*sizeof(_TCHAR) /*+ 1*/); 
	return SetBlob(buffer,(LPVOID)(LPCTSTR)val,val.GetLength()*sizeof(_TCHAR) /*+ 1*/); 
   //考虑了GetAsString 时的采用GetBufferSetLength函数，大字段中不在保存结束字符窜 \0
}

CString NBlobField::GetAsString(const void *buffer)
{
	if(IsUpdateAsBlobFiled())
	{
		CString strErrMsg;
		strErrMsg.Format(_T("NBlobField::GetAsString 取值错误，大字段不能使用GetAsString	%s"),this->GetFieldName());
		CDataSetErrorInfo::Instance()->PrintInfo(strErrMsg,CDataSetErrorInfo::DataSetError);
		return _T("");
	}

	void *pBlob;
    UINT iSize = GetBlob(buffer,pBlob);
	CString sResult;
	if (iSize >0)
	{
		LPTSTR pTembuffer = sResult.GetBufferSetLength(iSize/sizeof(_TCHAR));
        //memset(buffer,iSize + 1,0);
		memcpy(pTembuffer,pBlob,iSize);
		//int test = sResult.GetLength();
	}
	//sResult = (char*)pBlob;
	return sResult;
}

int NBlobField::CompareValue(const void * pData1,const int iData1Size,const void * pData2,const int iData2Size)
{
	TCHAR * pCValue1 = (TCHAR*)pData1;
	UINT iCValue1Size = iData1Size/sizeof(TCHAR);
	TCHAR * pCValue2 = (TCHAR*)pData2;
	UINT iCValue2Size = iData2Size/sizeof(TCHAR);
	UINT iMinSize = min(iCValue1Size,iCValue2Size);

	int ret = 0 ;
	UINT iCurPos = 0;
	while(iCurPos < iMinSize)
	{
		ret = (int)(*pCValue1 - *pCValue2);
		if(ret != 0)
			break;

		++pCValue1, ++pCValue2;
		iCurPos++;
	}
	if(0 == ret)
	{
		if(iCValue1Size > iCValue2Size)
			ret = 1;
		else if(iCValue1Size < iCValue2Size)
			ret = -1;
	}

	if ( ret < 0 )
		ret = -1 ;
	else if ( ret > 0 )
		ret = 1 ;

	return( ret );
	//long minsize = min(iData2Size,iData1Size);
	//int compresult = memcmp(pData1,pData2,minsize);
	//if (compresult == 0) //当返回值有大小，或者没有大小，但是但是内存大小相等
	//{
	//	if (iData1Size > minsize)
	//	{
	//		return 1;//pData1 > pData2
	//	}
	//	else if(iData1Size == iData2Size)
	//	{
	//		return 0;
	//	}
	//	else
	//		return -1;
	//}
	//
	//return compresult;
}

int NBlobField::Compare(const void * pData1,const void * pData2)
{
// 	void *pBlob1,*pBlob2;
//     long size = GetBlob(pData1,pBlob1);
// 	size = min(GetBlob(pData2,pBlob2),size);
//      
// 	//return 0;
// 	//进行内存比较
// 	return memcmp(pBlob1,pBlob2,size);
	void *pBlob1,*pBlob2;
    long size1 = GetBlob(pData1,pBlob1);
	long size2 = GetBlob(pData2,pBlob2);

	return CompareValue(pBlob1,size1,pBlob2,size2);

	//long minsize = min(size2,size1);
	/////[2010年2月6日]                                  
 //   ///int compresult = memcmp(pBlob1,pBlob2,size);
	//int compresult = memcmp(pBlob1,pBlob2,minsize);
	//if (compresult == 0) //当返回值有大小，或者没有大小，但是但是内存大小相等
	//{
	//	if (size1 > minsize)
	//	{
	//		return 1;//pData1 > pData2
	//	}
	//	else if(size1 == size2)
	//	{
	//		return 0;
	//	}
	//	else
	//		return -1;
	//}
	//return compresult;
}

BOOL NBlobField::FilterCompare(const void * pData1)
{
	if (m_pFilterValue == NULL) {
		return TRUE;
	}
	if(TRUE == m_isUpdateAsBlobFiled)
		return TRUE;

	CString strTemValue = GetAsString(pData1);
	return (this->*m_pFuncompare)(strTemValue.GetBuffer(0),m_pFilterValue);
	//return FALSE;
}

BOOL NBlobField::FilterCompareValue(const void * pData1,const int iSize)
{
	if (m_pFilterValue == NULL) {
		return TRUE;
	}
	if(TRUE == m_isUpdateAsBlobFiled)
		return TRUE;

	return (this->*m_pFuncompare)(pData1,m_pFilterValue);
}

BOOL NBlobField::FilterCompareEqual(const void * pData1,const void * pData2)
{
	return _tcscmp((LPTSTR)pData2,(LPTSTR)pData1) == 0;
}

BOOL NBlobField::FilterCompareLike(const void * pData1,const void * pData2)
{
	return _tcscspn((LPTSTR )pData1,(LPTSTR )pData2) >= 0;
}

BOOL NBlobField::FilterCompareGreater(const void * pData1,const void * pData2)
{
	//BOOL test = strcmp((char *)pData1,(char *)pData2) > 0;
	return _tcscmp((LPTSTR )pData1,(LPTSTR )pData2) > 0;
}

BOOL NBlobField::FilterCompareLess(const void * pData1,const void * pData2)
{
	return _tcscmp((LPTSTR )pData1,(LPTSTR )pData2) < 0;
}

BOOL NBlobField::SetFilter(LPTSTR sOperator ,LPTSTR sValue)
{
	FreeFilterValue();
	//NDataField::SetFilter(sOperator,sValue);
	CString sop = sOperator;
	if (sop == _T("<")) {
		m_pFuncompare = (BOOL (NDataField::*) ( const void *  ,const void * ))(&NBlobField::FilterCompareLess);
	}
	else if (sop == _T(">")) {
		m_pFuncompare = (BOOL (NDataField::*) ( const void *  ,const void * ))(&NBlobField::FilterCompareGreater);
	}
	else if (sop == _T("=")) {
		m_pFuncompare = (BOOL (NDataField::*) ( const void *  ,const void * ))(&NBlobField::FilterCompareEqual);
	}
	else if (sop.CompareNoCase(_T("like"))) {
		m_pFuncompare = (BOOL (NDataField::*) ( const void *  ,const void * ))(&NBlobField::FilterCompareLike);
	}
	
	m_pFilterValue = new BYTE [(_tcslen(sValue) + 1)*sizeof(_TCHAR)] ;
	
	memset(m_pFilterValue,0,(_tcslen(sValue) + 1)*sizeof(_TCHAR));
	
	_tcscpy_s((LPTSTR)m_pFilterValue, strlen((char*)m_pFilterValue), sValue);
	m_FilterOperator = sOperator;
	return TRUE;
	//return TRUE;
}

BOOL NBlobField::FreeFilterValue()
{
	if (m_pFilterValue != NULL) {
		delete [] m_pFilterValue;
		m_pFilterValue = NULL;
	}
	return TRUE;
}
