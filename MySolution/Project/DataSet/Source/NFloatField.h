// NFloatField.h: interface for the NFloatField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NFLOATFIELD_H__4919BF9A_552F_442F_8A3E_C719F7F8FFA8__INCLUDED_)
#define AFX_NFLOATFIELD_H__4919BF9A_552F_442F_8A3E_C719F7F8FFA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NNumericField.h"


/**
* @brief DataSet 数字数据列 浮点派生
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class NFloatField : public NNumericField  
{
public:
	double GetAsFloat(char *buff);
	NFloatField(void *pOwner);
	virtual ~NFloatField();
//数据存储
public:

	
	virtual BOOL FreeFilterValue() ;


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
    virtual int Compare(const void * pData1,const void * pData2);
	
	virtual int CompareValue(const void * pData1,const int iData1Size,const void * pData2,const int iData2Size)
	{
		return Compare( pData1 , pData2 ) ; 
	}

protected:
	virtual BOOL  StringTranToThisType(CString sValue,void *& pValue,int &iSize);
private:
//	double m_MinValue;
//    double m_MaxValue;
};

#endif // !defined(AFX_NFLOATFIELD_H__4919BF9A_552F_442F_8A3E_C719F7F8FFA8__INCLUDED_)
