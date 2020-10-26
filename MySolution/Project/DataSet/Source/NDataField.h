// NDataField.h: interface for the NGridColumn class.
//˵�� SetData �� ��SetBlob�Ǻ������ݿ����ú���,��ֱ�������ݴ洢λ�ý���ͨѶ�Ľӿ�,�κ��������ݿ�����ò����ƿ�������

//��ʷ������Bolb֧�����޸ĵ�����
//      1�����Ӻ���BOOL NDataField::SetBolbData(void *pRowbufferOffset, void *pBlob, long iSize)
//      2���޸���FreeOrigValueBuff����
//      3��������	����virtual long GetBlob(void *pRowbufferOffset,void *&pBlob);
	                //  virtual BOOL SetBlob(void *pRowbufferOffset,void *pBlob,long iBlockSize);
//      4���������κ����͵Ĺ���֧��(����Blob����) �ڽ�������SetFilterʱ���������뱾�ֶ�����һ�µ�ֵ��������
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NGRIDCOLUMN_H__A6A88B9D_A3A5_413E_8716_F47A0B436066__INCLUDED_)
#define AFX_NGRIDCOLUMN_H__A6A88B9D_A3A5_413E_8716_F47A0B436066__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum FieldType
{ftUnknown, ftString, ftSmallint, ftInteger, ftWord,
ftBoolean, ftFloat, ftCurrency, ftBCD, ftDate, ftTime, ftDateTime,
ftBytes, ftVarBytes, ftAutoInc, ftBlob, ftMemo, ftGraphic, ftFmtMemo,
ftParadoxOle, ftDBaseOle, ftTypedBinary, ftCursor, ftFixedChar, ftWideString,
ftLargeint, ftADT, ftArray, ftReference, ftDataSet, ftOraBlob, ftOraClob,
ftVariant, ftInterface, ftIDispatch, ftGuid, ftTimeStamp, ftFMTBcd
} ;

enum Alignment
{taLeftJustify, taRightJustify, taCenter
};

enum TextLayout
{
	tlTop, tlCenter, tlBottom
};

//#include "NDataModelBaseDataSet.h"
//
//class NDataModelBaseDataSet;

/**
* @brief DataSet �����л���  
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class DataSetAPI NDataField : public CObject  
{
public:

	LPVOID GetOwner(){return m_pOwner;}

	/**
	* @brief ��ȡ�ֶε� ���   
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [27/4/2012]
	*/
	inline int GetFieldNo() const
	{
		return m_fieldNo;
	}

	void SetFieldNo(int iFieldNo) {m_fieldNo = iFieldNo;}

	inline int GetSize()
	{
		return m_dataSize;
	}
// 
// 	virtual void DoDrawCell(CDC *pDC,CRect *rect,BOOL Selected, BOOL Focused,BOOL 
// 							 NewitemRow,BOOL RightEdge,CBrush *br,CString *text,COLORREF color,
// 							 CFont *font,Alignment alig,BOOL ADone);
	virtual void OnChanged();
	explicit NDataField(void *pOwner);

	virtual ~NDataField();

 	//������ֵ���бȽϲ���
	virtual int Compare(const void * pData1,const void * pData2);

	virtual int CompareValue(const void * pData1,const int iData1Size,const void * pData2,const int iData2Size){return 0;}


	// �Թ�����ʱ�������ݱȽ� [1/18/2003]
	virtual BOOL FilterCompare(const void * pData1);
	virtual BOOL FilterCompareValue(const void * pData1,const int iSize)
	{
		return TRUE;
	}

   virtual BOOL SetFilter(LPTSTR sOperator ,LPTSTR sValue);
	virtual BOOL FreeFilterValue();

	/******************************************************************** 
	������������ȡ�Ƿ��Field�й���
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:29]   10:39
	*********************************************************************/ 
	BOOL HasFilterValue()
	{
		if(NULL != m_pFilterValue)
			return TRUE;
		return FALSE;
	}

	//�����ݿ�֧��
	virtual long GetBlob(const void *pRowbufferOffset,void *&pBlob);
	virtual BOOL SetBlob(void *pRowbufferOffset,void *pBlob,long iBlockSize,BOOL alloOrigValueBuff = TRUE);
    void FreeBolbData(void *pRowbufferOffset);
	BOOL    IsUpdateAsBlobFiled()
	{
		return m_isUpdateAsBlobFiled;
	}
	//�����ֶ�
    void SetUpdateAsBlobFiled(BOOL b)
	{
		m_isUpdateAsBlobFiled = b;
	}
	void SetFieldName(CString name);
	void SetSize(int size);
	virtual void GetText(CString &cstr,void *buffer);
	void SetDataType(FieldType type);

	FieldType GetDataType( ) const;

	virtual CString GetAsString(const void *buffer);
	virtual int GetAsInteger(const void *buffer);
	virtual double GetAsFloat(const void *buffer);
	virtual BOOL GetAsBoolean(const void *buffer);
	virtual COleDateTime GetAsDateTime(const void *buffer);
	virtual VARIANT GetAsVariant(const void * buffer);

	virtual  BOOL SetAsString(const CString & val,void *buffer);
	virtual  BOOL SetAsInteger(const long & val,void *buffer);
	virtual  BOOL SetAsFloat(const double & val,void *buffer) ;
	virtual  BOOL SetAsBoolean(const BOOL & val,void *buffer) ;
	virtual  BOOL SetAsDateTime(const COleDateTime & val,void *buffer) ;
	virtual  BOOL SetAsVariant(const VARIANT & val,void *buffer) ;
    CString  GetFieldName( );
	//�޸� 
	void SetItemModified(char *pbuffer, BOOL bModified);
    BOOL GetItemModified(char *pbuffer);
	void FreeOrigValueBuff(void *buff, BOOL DeleteBuf = TRUE);
	void ClearModified(void *buff);
	//״̬

	BOOL IsPK()		  { return (m_Status & 0x08) ? TRUE:FALSE; }
	BOOL IsCanNull()  { return (m_Status & 0x10) ? TRUE:FALSE; }
	BOOL IsHasDefault()  { return (m_Status & 0x20) ? TRUE:FALSE; }
	BOOL IsPKAutoIncrease()		  { return (m_Status & 0x40) ? TRUE:FALSE; }
	BOOL IsUpdateAble()			  { return (m_Status & 0x80) ? TRUE:FALSE; }
	BOOL IsParseSQLAble()			  { return (m_Status & 0x100) ? TRUE:FALSE; }
	void SetPK(BOOL b=TRUE)		  { if(b) m_Status |= 0x08; else m_Status &= ~(0x08); }
	void SetCanNull(BOOL b=TRUE)  { if(b) m_Status |= 0x10; else m_Status &= ~(0x10); }
	void SetHasDefault(BOOL b=TRUE)		{ if(b) m_Status |= 0x20; else m_Status &= ~(0x20); }
	void SetPKAutoIncrease(BOOL b=TRUE)	{ if(b) m_Status |= 0x40; else m_Status &= ~(0x40); }
	void SetUpdateAble(BOOL b=TRUE)	{ if(b) m_Status |= 0x80; else m_Status &= ~(0x80); }
    void SetParseSQLAble(BOOL b=TRUE){ if(b) m_Status |= 0x100; else m_Status &= ~(0x100); }

	int GetSpecStatus()						{return m_iSpecStatus;}
	void SetSpecStatus(int iSpecStatus)		{m_iSpecStatus = iSpecStatus;}
	
protected:
	//�ȽϺ���
    BOOL IsEqual(const void * pData1,const void * pData2);
	//static BOOL FilterCompareLike(const void * pData1,const void * pData2);
    BOOL IsGreater(const void * pData1,const void * pData2);
    BOOL IsLess(const void * pData1,const void * pData2);
	//BOOL (*m_pFilterFuncompare) ( const void * pData1 ,const void * pData2,int size);


	BOOL (NDataField::*m_pFuncompare) ( const void * pData1 ,const void * pData2);
	//typedef BOOL (*FUNTEST)( const void *  ,const void * );
	//FUNTEST m_pFuncompare;
public:
	virtual BOOL  StringTranToThisType(CString sValue,void *& pValue,int &iSize);   //���ַ�������ת��Ϊ��ǰ�������͵�����

	//alloOrigValueBuff ��clone������²���Ҫ����ԭʼ����
	BOOL SetBolbData(void *pRowbufferOffset,void *pBlob,long iSize,BOOL alloOrigValueBuff = TRUE);

protected:	
	
	//BOOL GetData(int row,void * buffer);
	//����Set���ݺ������õ���������ֵ������pbuffer�ǵ�ǰ
	BOOL SetData(void *pbuffer,const void * Sourcebuffer,int size);
    inline void AlloOrigValueBuff(void *buff);

	char* GetItemInfo(char *pbuffer);

	//�ֶ����
	int m_fieldNo; 

	CString m_FieldName;

    //�е�����
//    int m_iSortedOrder;

//    BOOL m_DisableFilter;
//    int m_iMaxLength;
	//���ֶε���󳤶�
    int m_dataSize;
//    void * m_ValueBuffer;
    //���ݹ���
	FieldType m_DataType;
	//�Ƚ�ʱ�������
	CString m_FilterOperator;
	//�Ƚ�ʱ��ֵ
	void * m_pFilterValue;
    int    m_iFilerValueSize;
	//NDataModelBaseDataSet *m_pOwner;
	void *m_pOwner;
	//
	WORD m_Status;
	//���մ��ֶεķ�ʽ���и���,��Щ��Ȼ���ô��ֶδ洢���Ǹ��·�ʽ���ǿ��԰���ֱ����update��insert����и���
	//���ڶ������ļ���������ô��ֶη�ʽ����
	BOOL m_isUpdateAsBlobFiled;

	int m_iSpecStatus;//�ɾ�������DataSet���庬�壬
					//��CPM_LargeFileDataSet�У�1ΪIndexField��2ΪDataField
					//��CPM_LinkDataSet�У�3Ϊ��DataSet
};

#endif // !defined(AFX_NGRIDCOLUMN_H__A6A88B9D_A3A5_413E_8716_F47A0B436066__INCLUDED_)
