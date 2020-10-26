// NBlobField.h: interface for the NBlobField class.
// 
// 一般的字段采用SetData(buffer,str,size);函数进行赋值，而在
// 本字段中存储的仅仅是数据指针和数据块大小
// 本字段目前只是支持一种存取方法
//大数据的存储方法 void*(指向当前数据) long(数据块大小) 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NBLOBFIELD_H__555A4DC3_39DF_43BB_A8DE_42810F12A045__INCLUDED_)
#define AFX_NBLOBFIELD_H__555A4DC3_39DF_43BB_A8DE_42810F12A045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataField.h"

/**
* @brief  DataSet 数字数据列 大数据派生  
*
*          
* @note  : 这个数据长度可以不定
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class NBlobField : public NDataField
{
public:
	explicit NBlobField(void *pOwner);
	virtual ~NBlobField();
	//大数据块支持
	virtual long GetBlob(const void *pRowbufferOffset,void *&pBlob);
	virtual BOOL SetBlob(void *pRowbufferOffset,void *pBlob,long iBlockSize,BOOL alloOrigValueBuff = TRUE);
	virtual  BOOL SetAsString(const CString & val,void *buffer) ;
	virtual CString GetAsString(const void *buffer);
	//virtual void FreeBolbData(void *pRowbufferOffset);
	//对两个值进行比较操作
	virtual int Compare(const void * pData1,const void * pData2);
	virtual int CompareValue(const void * pData1,const int iData1Size,const void * pData2,const int iData2Size);

	virtual BOOL SetFilter(LPTSTR sOperator ,LPTSTR sValue);
	virtual BOOL FreeFilterValue();
	virtual BOOL FilterCompare(const void * pData1);

	virtual BOOL FilterCompareValue(const void * pData1,const int iSize);

	BOOL FilterCompareEqual(const void * pData1,const void * pData2);
    BOOL FilterCompareLike(const void * pData1,const void * pData2);
    BOOL FilterCompareGreater(const void * pData1,const void * pData2);
    BOOL FilterCompareLess(const void * pData1,const void * pData2);

};

#endif // !defined(AFX_NBLOBFIELD_H__555A4DC3_39DF_43BB_A8DE_42810F12A045__INCLUDED_)
