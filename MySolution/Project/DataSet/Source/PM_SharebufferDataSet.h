// PM_SharebufferDataSet.h: interface for the CPM_SharebufferDataSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PM_SHAREBUFFERDATASET_H__2366C49A_FA69_4D79_91AF_6035DF23E72E__INCLUDED_)
#define AFX_PM_SHAREBUFFERDATASET_H__2366C49A_FA69_4D79_91AF_6035DF23E72E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PM_StorageDataSet.h"

/******************************************************************** 
������������ָ��DataSet��Buffer�� ���������DataSet���й�������ʹ��ͬһ��Buffer��
	���ڲ������޸�ʱ�����޸ĵ����ݽ����µ�Buffer��������ά����־λ��
������
�� �� ֵ��
�㷨��
ע�����
		  SetAsxxxxx(int Column, NRecordID id ,CString& val) ������ʽ��ֹ���ã��ٶȷǳ���

  
��    �ߣ�liwenlang
ʱ    �䣺[2010:10:4]   21:32
*********************************************************************/ 
class DataSetAPI CPM_SharebufferDataSet : public CPM_StorageDataSet
{
public:
	DECLARE_DYNCREATE(CPM_SharebufferDataSet) 

	friend NDataField;

	virtual BOOL GetVersionID(short & iVersion);

	BOOL GetMdfBufferArr(CPtrArray & pRecordIDArr);

	/**
	* @brief  ��� �����DataSet�Ƿ��� ��DataSet
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [3/8/2012]
	*/
	BOOL CheckIsPrimaryDataSet(NDataModelDataSet * pCheckDataSet);

	CPM_SharebufferDataSet();

	/**
	* @brief �����ݱ���  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [18/5/2012]
	*/
	CPM_SharebufferDataSet(NDataModelDataSet * pPrimaryDataSet);

	/**
	* @brief  ָ�������� ���� 
	*
	*          
	* @note  : ֻ�ܴ��� ���������ݣ�m_PrimaryBuffers��
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [18/5/2012]
	*/
	CPM_SharebufferDataSet(CUIntArray & iRowIDArr,NDataModelDataSet * pPrimaryDataSet);

	virtual ~CPM_SharebufferDataSet();
	
	virtual int InsertRow(int iberforrow);

	virtual BOOL SetBlob(const CString & Column, int row ,void * ppBolb,long iSize);
	virtual BOOL SetBlob(int Column, int row ,void * ppBolb,long iSize);

	BOOL	SetBlob_Field(NDataField * field,int iRow,void * ppBolb,long iSize);

	BOOL    SetAsString_Field(NDataField * field, int iRow ,const CString& val);
	BOOL    SetAsFloat_Field(NDataField * field, int iRow ,const double& val);
	BOOL    SetAsInteger_Field(NDataField * field, int iRow ,const int& val);
	BOOL    SetAsVariant_Field(NDataField * field, int iRow ,const VARIANT& val);
	BOOL    SetAsDateTime_Field(NDataField * field, int iRow ,const COleDateTime& val);
	BOOL    SetAsBool_Field(NDataField * field, int iRow ,const BOOL & val);

	/******************************************************************** 
	�����������ٶ��� ��Ҫ����
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:8]   17:08
	*********************************************************************/ 
	virtual BOOL    SetAsString(int Column, NRecordID id ,const CString& val);
	virtual BOOL    SetAsFloat(int Column, NRecordID id ,const double& val);
	virtual BOOL    SetAsInteger(int Column, NRecordID id ,const int& val);
	virtual BOOL    SetAsVariant(int Column, NRecordID id ,const VARIANT& val);
	virtual BOOL    SetAsDateTime(int Column, NRecordID id ,const COleDateTime& val);
	virtual BOOL    SetAsBool(int Column, NRecordID id ,const BOOL & val);

	virtual BOOL    SetAsString(const CString & Column, int row,const CString& val);
	virtual BOOL    SetAsFloat(const CString & Column, int row, const double& val);
	virtual BOOL    SetAsInteger(const CString & Column, int row,const int& val);
	virtual BOOL    SetAsVariant(const CString & Column, int row, const VARIANT& val);
	virtual BOOL    SetAsDateTime(const CString & Column, int row, const COleDateTime& val);
	virtual BOOL    SetAsBool(const CString & Column, int row, const BOOL & val);

	virtual BOOL    SetAsString(int Column, int row ,const CString& val);
	virtual BOOL    SetAsFloat(int Column, int row ,const double& val);
	virtual BOOL    SetAsInteger(int Column, int row ,const int& val);
	virtual BOOL    SetAsVariant(int Column, int row ,const VARIANT& val);
	virtual BOOL    SetAsDateTime(int Column, int row ,const COleDateTime& val);
	virtual BOOL    SetAsBool(int Column, int row ,const BOOL & val);
	
protected:

	virtual void DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff);

	BOOL ReplaceBuffer(int iRow,NRecordID id,NRecordID newID);

	NRecordID BuildNewNRecordID(NRecordID id);
	
	NDataModelDataSet * m_pPrimaryDataSet;
	BOOL m_bHasShareBuffer;//�Ƿ�ʼ�ڴ湲��

	typedef std::map<LPVOID,UINT> Address2MdfMode;
	Address2MdfMode m_address2MdfMode;//��¼�Ƿ�����Ϣ���޸ģ�0:û���޸ģ�1�����޸�
};

#endif // !defined(AFX_PM_SHAREBUFFERDATASET_H__2366C49A_FA69_4D79_91AF_6035DF23E72E__INCLUDED_)
