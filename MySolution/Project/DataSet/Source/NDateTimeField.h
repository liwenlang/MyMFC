// NDateTimeField.h: interface for the NDateTimeField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDATETIMEFIELD_H__C6532B86_BE70_445F_85C2_393DCE7126B4__INCLUDED_)
#define AFX_NDATETIMEFIELD_H__C6532B86_BE70_445F_85C2_393DCE7126B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataField.h"


/**
* @brief DataSet 数字数据列 日期类派生
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class NDateTimeField : public NDataField  
{
public:
//	COleDateTime GetAsDateTime(void *buffer);
	void SetDisplayFormat(CString sFormat);
	NDateTimeField(void *pOwner);
	virtual ~NDateTimeField();
//数据存储
public:
	virtual CString GetAsString(const void *buffer);
//	virtual int GetAsInteger(void *buffer) ;
//	virtual double GetAsFloat(void *buffer) ;
//	virtual BOOL GetAsBoolean(void *buffer) ;
	virtual COleDateTime GetAsDateTime(const void *buffer) ;
//	virtual VARIANT GetAsVariant(void * buffer) ;

	virtual  BOOL SetAsString(const CString & val,void *buffer) ;
//	virtual  BOOL SetAsInteger(long val,void *buffer) ;
//	virtual  BOOL SetAsFloat(double val,void *buffer) ;
//	virtual  BOOL SetAsBoolean(BOOL val,void *buffer) ;
	virtual  BOOL SetAsDateTime(const COleDateTime & val,void *buffer) ;
//	virtual  BOOL SetAsVariant(VARIANT val,void *buffer) ;
protected:
	virtual BOOL  StringTranToThisType(CString sValue,void *& pValue,int &iSize);
private:
	 CString m_sDisplayFormat;
};

#endif // !defined(AFX_NDATETIMEFIELD_H__C6532B86_BE70_445F_85C2_393DCE7126B4__INCLUDED_)
