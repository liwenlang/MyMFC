// NNumericField.h: interface for the NNumericField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NNUMERICFIELD_H__93598FA2_5A70_4B91_9CAF_8EC8A08C5E79__INCLUDED_)
#define AFX_NNUMERICFIELD_H__93598FA2_5A70_4B91_9CAF_8EC8A08C5E79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataField.h"


/**
* @brief DataSet 数字数据列  基类
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class DataSetAPI NNumericField : public NDataField  
{
public:
	NNumericField(void *pOwner);
	virtual ~NNumericField();

private:
    CString m_sDisplayFormat;
    CString m_sEditFormat;
};

#endif // !defined(AFX_NNUMERICFIELD_H__93598FA2_5A70_4B91_9CAF_8EC8A08C5E79__INCLUDED_)
