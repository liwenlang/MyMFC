// NIntegerField.h: interface for the NIntegerField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NINTEGERFIELD_H__D1B3CB18_FE5A_442D_95EC_05C3F3E21B9B__INCLUDED_)
#define AFX_NINTEGERFIELD_H__D1B3CB18_FE5A_442D_95EC_05C3F3E21B9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NNumericField.h"


/**
* @brief DataSet 数字数据列 整形派生
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class DataSetAPI NIntegerField : public NNumericField  
{
public:
	NIntegerField(void *pOwner);
	virtual ~NIntegerField();
//数据存储
public:
	virtual CString GetAsString(const void *buffer);
	virtual int GetAsInteger(const void *buffer) ;
	virtual double GetAsFloat(const void *buffer) ;
//	virtual BOOL GetAsBoolean(void *buffer) ;
//	virtual COleDateTime GetAsDateTime(void *buffer) ;
//	virtual VARIANT GetAsVariant(void * buffer) ;

	virtual  BOOL SetAsString(const CString & val,void *buffer) ;
	virtual  BOOL SetAsInteger(const long & val,void *buffer) ;
	virtual  BOOL SetAsFloat(const double & val,void *buffer) ;
//	virtual  BOOL SetAsBoolean(BOOL val,void *buffer) ;
//	virtual  BOOL SetAsDateTime(COleDateTime val,void *buffer) ;
//	virtual  BOOL SetAsVariant(VARIANT val,void *buffer) ;
   
public:
	virtual int Compare(const void * pData1,const void * pData2);
	
	virtual int CompareValue(const void * pData1,const int iData1Size,const void * pData2,const int iData2Size)
	{
		return Compare( pData1 , pData2 ) ; 
	}

	// 对过滤行时进行数据比较 [1/18/2003]
	//virtual BOOL FilterCompare(const void * pData1);
    //virtual BOOL SetFilter(LPSTR sOperator ,LPSTR sValue);
	virtual BOOL FreeFilterValue();
	//比较函数
//	static BOOL FilterCompareEqual(const void * pData1,const void * pData2);
//	static BOOL FilterCompareLike(const void * pData1,const void * pData2);
//	static BOOL FilterCompareGreater(const void * pData1,const void * pData2);
//	static BOOL FilterCompareLess(const void * pData1,const void * pData2);
//	BOOL (*m_pFuncompare) ( const void * pData1 ,const void * pData2);
	
protected:
	virtual BOOL  StringTranToThisType(CString sValue,void *& pValue,int &iSize);
//	int m_iFilterValue;
};

#endif // !defined(AFX_NINTEGERFIELD_H__D1B3CB18_FE5A_442D_95EC_05C3F3E21B9B__INCLUDED_)
