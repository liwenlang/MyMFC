// NBlobField.h: interface for the NBlobField class.
// 
// һ����ֶβ���SetData(buffer,str,size);�������и�ֵ������
// ���ֶ��д洢�Ľ���������ָ������ݿ��С
// ���ֶ�Ŀǰֻ��֧��һ�ִ�ȡ����
//�����ݵĴ洢���� void*(ָ��ǰ����) long(���ݿ��С) 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NBLOBFIELD_H__555A4DC3_39DF_43BB_A8DE_42810F12A045__INCLUDED_)
#define AFX_NBLOBFIELD_H__555A4DC3_39DF_43BB_A8DE_42810F12A045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataField.h"

/**
* @brief  DataSet ���������� ����������  
*
*          
* @note  : ������ݳ��ȿ��Բ���
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
	//�����ݿ�֧��
	virtual long GetBlob(const void *pRowbufferOffset,void *&pBlob);
	virtual BOOL SetBlob(void *pRowbufferOffset,void *pBlob,long iBlockSize,BOOL alloOrigValueBuff = TRUE);
	virtual  BOOL SetAsString(const CString & val,void *buffer) ;
	virtual CString GetAsString(const void *buffer);
	//virtual void FreeBolbData(void *pRowbufferOffset);
	//������ֵ���бȽϲ���
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
