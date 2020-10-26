// NDataField.h: interface for the NGridColumn class.
//说明 SetData 和 和SetBlob是核心数据块设置函数,是直接与数据存储位置进行通讯的接口,任何设置数据块的设置不得绕开本函数

//历史：增加Bolb支持所修改的内容
//      1、增加函数BOOL NDataField::SetBolbData(void *pRowbufferOffset, void *pBlob, long iSize)
//      2、修改了FreeOrigValueBuff函数
//      3、增加了	函数virtual long GetBlob(void *pRowbufferOffset,void *&pBlob);
	                //  virtual BOOL SetBlob(void *pRowbufferOffset,void *pBlob,long iBlockSize);
//      4、增加了任何类型的过滤支持(除了Blob类型) 在进行设置SetFilter时可以设置与本字段类型一致的值进行设置
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
* @brief DataSet 数据列基类  
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
	* @brief 获取字段的 序号   
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

 	//对两个值进行比较操作
	virtual int Compare(const void * pData1,const void * pData2);

	virtual int CompareValue(const void * pData1,const int iData1Size,const void * pData2,const int iData2Size){return 0;}


	// 对过滤行时进行数据比较 [1/18/2003]
	virtual BOOL FilterCompare(const void * pData1);
	virtual BOOL FilterCompareValue(const void * pData1,const int iSize)
	{
		return TRUE;
	}

   virtual BOOL SetFilter(LPTSTR sOperator ,LPTSTR sValue);
	virtual BOOL FreeFilterValue();

	/******************************************************************** 
	功能描述：获取是否此Field有过滤
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:10:29]   10:39
	*********************************************************************/ 
	BOOL HasFilterValue()
	{
		if(NULL != m_pFilterValue)
			return TRUE;
		return FALSE;
	}

	//大数据块支持
	virtual long GetBlob(const void *pRowbufferOffset,void *&pBlob);
	virtual BOOL SetBlob(void *pRowbufferOffset,void *pBlob,long iBlockSize,BOOL alloOrigValueBuff = TRUE);
    void FreeBolbData(void *pRowbufferOffset);
	BOOL    IsUpdateAsBlobFiled()
	{
		return m_isUpdateAsBlobFiled;
	}
	//设置字段
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
	//修改 
	void SetItemModified(char *pbuffer, BOOL bModified);
    BOOL GetItemModified(char *pbuffer);
	void FreeOrigValueBuff(void *buff, BOOL DeleteBuf = TRUE);
	void ClearModified(void *buff);
	//状态

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
	//比较函数
    BOOL IsEqual(const void * pData1,const void * pData2);
	//static BOOL FilterCompareLike(const void * pData1,const void * pData2);
    BOOL IsGreater(const void * pData1,const void * pData2);
    BOOL IsLess(const void * pData1,const void * pData2);
	//BOOL (*m_pFilterFuncompare) ( const void * pData1 ,const void * pData2,int size);


	BOOL (NDataField::*m_pFuncompare) ( const void * pData1 ,const void * pData2);
	//typedef BOOL (*FUNTEST)( const void *  ,const void * );
	//FUNTEST m_pFuncompare;
public:
	virtual BOOL  StringTranToThisType(CString sValue,void *& pValue,int &iSize);   //将字符串类型转换为当前数据类型的数据

	//alloOrigValueBuff 再clone的情况下不需要分配原始数据
	BOOL SetBolbData(void *pRowbufferOffset,void *pBlob,long iSize,BOOL alloOrigValueBuff = TRUE);

protected:	
	
	//BOOL GetData(int row,void * buffer);
	//所有Set数据函数调用的设置数据值函数，pbuffer是当前
	BOOL SetData(void *pbuffer,const void * Sourcebuffer,int size);
    inline void AlloOrigValueBuff(void *buff);

	char* GetItemInfo(char *pbuffer);

	//字段序号
	int m_fieldNo; 

	CString m_FieldName;

    //列的排序
//    int m_iSortedOrder;

//    BOOL m_DisableFilter;
//    int m_iMaxLength;
	//本字段的最大长度
    int m_dataSize;
//    void * m_ValueBuffer;
    //数据过滤
	FieldType m_DataType;
	//比较时的运算符
	CString m_FilterOperator;
	//比较时的值
	void * m_pFilterValue;
    int    m_iFilerValueSize;
	//NDataModelBaseDataSet *m_pOwner;
	void *m_pOwner;
	//
	WORD m_Status;
	//按照大字段的方式进行更新,有些虽然采用大字段存储但是更新方式还是可以按照直接在update、insert语句中更新
	//对于二进制文件，必须采用大字段方式更新
	BOOL m_isUpdateAsBlobFiled;

	int m_iSpecStatus;//由具体派生DataSet定义含义，
					//在CPM_LargeFileDataSet中，1为IndexField，2为DataField
					//在CPM_LinkDataSet中，3为主DataSet
};

#endif // !defined(AFX_NGRIDCOLUMN_H__A6A88B9D_A3A5_413E_8716_F47A0B436066__INCLUDED_)
