// NGuidField.h: interface for the NGuidField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NGUIDFIELD_H__649B5533_D17A_4B82_8215_A0928973CFC7__INCLUDED_)
#define AFX_NGUIDFIELD_H__649B5533_D17A_4B82_8215_A0928973CFC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataField.h"

/**
* @brief DataSet 数据列 GUID派生
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class NGuidField : public NDataField  
{
public:
	NGuidField(void *pOwner);
	virtual ~NGuidField();
//数据存储
public:
	virtual  CString GetAsString(const void *buffer);
	virtual  BOOL SetAsString(const CString & val,void *buffer);

	virtual  GUID GetAsGUID(const void *buffer);
	virtual  BOOL SetAsGUID(const GUID & val,void *buffer);
};

#endif // !defined(AFX_NGUIDFIELD_H__649B5533_D17A_4B82_8215_A0928973CFC7__INCLUDED_)
