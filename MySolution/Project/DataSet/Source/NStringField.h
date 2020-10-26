// NStringField.h: interface for the NStringField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NSTRINGFIELD_H__D723B03B_2961_4495_89D8_6BEFBE0F4D5D__INCLUDED_)
#define AFX_NSTRINGFIELD_H__D723B03B_2961_4495_89D8_6BEFBE0F4D5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataField.h"


/**
* @brief  DataSet 数字数据列 字符串类派生  
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class DataSetAPI NStringField : public NDataField  
{
public:

	NStringField(void *pOwner);
	virtual ~NStringField();

//数据存储
public:
	virtual CString GetAsString(const void *buffer);
	virtual int GetAsInteger(const void *buffer) ;
	virtual double GetAsFloat(const void *buffer) ;
//	virtual BOOL GetAsBoolean(void *buffer) ;
	virtual COleDateTime GetAsDateTime(const void *buffer) ;
//	virtual VARIANT GetAsVariant(void * buffer) ;

	virtual  BOOL SetAsString(const CString & val,void *buffer) ;
	virtual  BOOL SetAsInteger(const long & val,void *buffer) ;
	virtual  BOOL SetAsFloat(const double & val,void *buffer) ;
//	virtual  BOOL SetAsBoolean(BOOL val,void *buffer) ;
//	virtual  BOOL SetAsDateTime(COleDateTime val,void *buffer) ;
//	virtual  BOOL SetAsVariant(VARIANT val,void *buffer) ;
	//排序比较
    virtual int Compare(const void * pData1,const void * pData2);
	virtual int CompareValue(const void * pData1,const int iData1Size,const void * pData2,const int iData2Size)
	{
		return Compare( pData1 , pData2 ) ; 
	}
	//过滤排序
	virtual BOOL FilterCompare(const void * pData1);
    virtual BOOL SetFilter(LPTSTR sOperator ,LPTSTR sValue);
	virtual BOOL FreeFilterValue();

	//比较函数
    BOOL FilterCompareEqual(const void * pData1,const void * pData2);
    BOOL FilterCompareLike(const void * pData1,const void * pData2);
    BOOL FilterCompareGreater(const void * pData1,const void * pData2);
    BOOL FilterCompareLess(const void * pData1,const void * pData2);
	//BOOL (NDataField::*m_pFuncompare) ( const void * pData1 ,const void * pData2);
protected:
	virtual BOOL  StringTranToThisType(CString sValue,void *& pValue,int &iSize);

};

#endif // !defined(AFX_NSTRINGFIELD_H__D723B03B_2961_4495_89D8_6BEFBE0F4D5D__INCLUDED_)
