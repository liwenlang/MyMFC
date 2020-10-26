// NDataModelDataSet.h: interface for the NDataModelDataSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDATAMODELDATASET_H__FA146F4B_3B2B_4B74_B879_4BC879BAE7A4__INCLUDED_)
#define AFX_NDATAMODELDATASET_H__FA146F4B_3B2B_4B74_B879_4BC879BAE7A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataField.h"	// Added by ClassView
#include "NDataFields.h"
#include "NDataModelBaseDataSet.h"

#include "NDataModelDataSetBuffer.h"
#include "NCGroup.h"
//class NDataFields;

#define SINGLEQUATE		_T('\'')

enum eGetMode
{
	gmCurrent , gmNext , gmPrior
};

enum eGetResult
{
	grOK , grBOF , grEOF ,grError
};

enum eDataModelDataSetState
{ dmstNormal,dmstRetrieve};
//class NDataFields;
class NDataModelDataSetBuffer;

/**
* @brief  NDataModelDataSet ��DataSet����Ҫ�����࣬�����Ѿ�������������Ϣ��  
*		Ҳ�кܶ�ҵ���ܺ���  
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class DataSetAPI NDataModelDataSet : public NDataModelBaseDataSet  
{
	DECLARE_DYNCREATE(NDataModelDataSet) 
public:
	friend NDataField;
	friend NDataModelDataSetBuffer;
   enum eBookmarkFlag
   {
	  bfCurrent, bfBOF, bfEOF, bfInserted
   };
   enum eBufferState
   {
	  Selected ,Normal
   };

   enum eBufferFlag
   {
	  Primary, Delete, Filter
   };
      
	typedef struct tagRecInfo
	{
		unsigned Status; //��״̬
		/*long RecordNumber;
		eBookmarkFlag BookmarkFlag;
		eBufferFlag BufferFlag ;
		eBufferState BufferState;
		BYTE Attribute;*/
	}RecInfo;

public:

	/**
		* @brief ��Ӻ����ж� �к� �Ƿ���Ч
		*
		*          
		* @note  : 
		* @param : iRow �Ǵ� 1 ��ʼ�����
		* @defenc: 
		* @return: 
		* @author: liwenlang
		* @date  : 2013��9��2��   16:44
	*/
	BOOL	IsRowValid( const int iRow ) ; 

	/**
	* @brief ��DataSet��Ϣ������Arr��  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [28/3/2013]
	*/
	BOOL DataSetRows2BufferArr(CPtrArray & rowPtArr) const;

	/**
	* @brief  ��Arr������Ϣ ������ DataSet�� 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [28/3/2013]
	*/
	BOOL BufferArr2DataSetRows(const CPtrArray & rowPtArr);

	/**
	* @brief  ��������Ϣ ����ΪBuffer 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [28/3/2013]
	*/
	BOOL BuildSimRow2Buffer(NRecordID pRowRecordID,LPVOID & pBuffer,ULONG & lBufferSize) const;

	/**
	* @brief ��Buffer��ȡ��DataSet�У���������  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [28/3/2013]
	*/
	BOOL ReadBuffer2SimRow(LPVOID pRowBuffer,ULONG lRowBufferSize);

	/**
	* @brief  ����ָ���� DataSet��ĳ�� 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [17/1/2013]
	*/
	BOOL FilterDataSetRow(int iRow);

	void OnSpyShareDataFree(NDataModelDataSet *pShareData);
	
	BOOL GetModified();
	void SetModified(BOOL isModified);
	int  LoadFromXML(BSTR xml);
	virtual void Reset();

	virtual int GetFieldOffset(NDataField *field) const;

	//BOOL SetFilter(LPSTR filterstr);
	void FreeRecordBuffer(void *pBuffer);
	void FreeRecordOrigValueBuffer(void *pBuffer);
	void FreeRecordBlobBuffer(void *pBuffer);
	void CalFieldsSize();
	void * AlloRecordBuffer();
	NDataModelDataSet();
	virtual ~NDataModelDataSet();

	NDataModelDataSetBuffer *m_PrimaryBuffers ,*m_DeleteBuffers ,*m_FilterBuffers;

public:

	//��filter�е�����ˢ�µ����洢��,�����ƶ��Ļ���Ĵ�С
	int  CopyFilterToPrimaryBuffer();
	void RemoveCopyedFilterToPrimaryBuffer(const int bufferSize);

	/******************************************************************** 
	������������ȡDataSet���ڴ��С
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:9:26]   13:41
	*********************************************************************/ 
	virtual LONG GetNDataMemorySize();

	/****************************************************************
	  ����������	 ��ȡÿ��Buffer��С
		������
		�� �� ֵ��
		�㷨��
		ע�����
		��    �ߣ�liwenlang
		ʱ    �䣺[2011:3:4   9:45]
	*****************************************************************/
	int GetRowBufferSize()
	{
		return m_RecordBufferSize;
	}

	virtual void Sort();
	
	void ResetSortField();
	void AddSortField(CString sField,BOOL Ascending);

	/******************************************************************** 
	����������	  ����Ƿ���Ҫ ����DataSet
	������strFieldNameArr�����Ŷ� iSortAscendingArr������ʽ checkFilter���Ƿ�Ҫ���Filter��
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:12:7]   14:19
	*********************************************************************/ 
	BOOL CheckIsNeedSort(CStringArray & strFieldNameArr,CArray<int,int> & iSortAscendingArr,BOOL checkFilter = TRUE);

	/******************************************************************** 
	������������ָ��FieldIndex����� ������Լ��ٲ�CString��Map ����
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:9:27]   16:13
	*********************************************************************/ 
	void AddSortField(int iFieldIndex,BOOL Ascending);

	int GetGroupCount();
	NCGroup* GetGroupObj(int index);
	BOOL IsGroup();
	CString GetGroupString(NCGroup *pGroup);
	void ResetGroupField();
	BOOL AddGroupField(CString sField,int order = 1);
	void FreeGroup();
	void DoGroup();
	virtual BOOL SaveDataSetXMLBuf_NoDelete(CString &sXMLBuf);
	BOOL SaveDataSetXMLBuf(CString &sXMLBuf);

	/******************************************************************** 
	 ������������ȡXML ���ǲ���ȡDataBefore��Ϣ
	 ������
	 �� �� ֵ��
	 �㷨��
	 ע���������
	 ��    �ߣ�liwenlang
	 ʱ    �䣺[25/12/2009]
	*********************************************************************/ 
	BOOL LoadDataSetXMLBuf_NoDataBefore(const CString & strXMLBuf,BOOL bAppend);

	virtual BOOL LoadDataSetXMLBuf(const CString &sXMLBuf, BOOL bAppend);
	int Bisearch(void* pBuff, CString sFieldName,eBufferFlag BufferFlag = Primary);
	int FindNearest(void *pBuff, CString sFieldName, eBufferFlag BufferFlag);
	int Bisearch(const CArray<void *,void *> &findValues,const CStringArray &fieldNames,const eBufferFlag BufferFlag = Primary);
	
	/******************************************************************** 
	�������������Ŀǰ�ṩ���۰���ҷ�ʽ��ֻͨ��FieldIndex��ȥFieldָ��Ч�ʸ���
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:9:27]   9:18
	*********************************************************************/ 
	int Bisearch(void* pBuff, int iFieldIndex,eBufferFlag BufferFlag = Primary);
	int Bisearch(const CArray<void *,void *> &findValues,const CUIntArray & iFieldIndexArr,const eBufferFlag BufferFlag = Primary);

	//�õ�����ǰ���ݵĶ���,ͬʱ���Լ����Ƿ�ԭ���ݷ����ı���
	NDataModelDataSet * GetShardData(BOOL bSpyChanged = FALSE,BOOL PrimaryOnly = FALSE);
	NDataModelDataSet * CloneStruct();
	//Cloneһ��һ���ṹһ�����ݵĶ���
	virtual NDataModelDataSet * Clone();
	void SaveAsText(CString &sBuff);
	virtual void ResetFilter(BOOL bResetExp = TRUE);
	FieldType GetFieldType(int iFieldIndex);
	//��ʼ���нṹ
	virtual int InitFieldStr();
	void GetUpdateSql(CStringArray &sqlarray);
	void DeleteRow(NRecordID recordid);
	int  GetRowOf(NRecordID recordid, eBufferFlag BufferFlag = Primary);
	void DeleteRow(int row);
	int  MoveRow(int row, int beforRow,eBufferFlag BufferFlag = Primary);
	CString GetMaxFieldValue(int col);
	void ResetUpdate(BOOL resetBlobField = TRUE);
	void ErrorLog(CString error);

	int GetFieldMaxLen( int col);
	FieldType GetFieldDataType(int col);
	int FilterCount();
	int DeletedCount();
	//�ֶ���������
	BOOL IsFieldPK(int col);
	BOOL IsFieldPKAutoIncrease(int col);
	BOOL IsFieldCanNull(int col);
	BOOL IsFieldUpdateAble(int col);
	
	//��������08.1.15
	//BOOL IsFieldBlob(int col); ���ò������溯�����
	BOOL IsUpdateAsBlobFiled(int col);

    virtual void SetUpdateAsBlobFiled(int col,BOOL b);

	BOOL IsUpdateAsBlobFiled(CString colName);
    void SetUpdateAsBlobFiled(CString colName,BOOL b);
    
	void SetFieldPK(int col,BOOL b=TRUE);
	void SetFieldCanNull(int col,BOOL b=TRUE);
	void SetFieldPKAutoIncrease(int col,BOOL b=TRUE);
	void SetFieldUpdateAble(int col,BOOL b=TRUE);

    BOOL Update(CString &TableName, BOOL bByPK, BOOL bAutoCommit);
	BOOL Update(CString& TableName, BOOL bByPK, BOOL bAutoCommit,CStringArray &arrSQLDelete,CStringArray &arrSQLUpdate,CStringArray &arrSQLInsert);
	inline void SetState(eDataModelDataSetState newstate);
	inline eDataModelDataSetState GetState();
	//void* GetItemInfo(NRecordID recordid,NDataField *field);
	//void* GetItemInfo(int row,int col);
	RecInfo * GetRowRecInfo(void *pBuff);
	//��Ҫ�����ֶζ���ķ���
	void* GetRowBuffPtr(NDataField *field,char *pBuff);
	BOOL  GetRowNew(int row, eBufferFlag BufferFlag = Primary);
	void  SetRowNew(int row,BOOL isnew, eBufferFlag BufferFlag = Primary);
	void  SetRowNewInternal(void * pbuff, BOOL isnew);
	BOOL  GetRowNewInternal(void * pbuff);
	BOOL  GetItemModified(int row,int col , eBufferFlag BufferFlag = Primary);
	BOOL  GetItemModifiedInternal(NRecordID recordid, int col);
	void  SetItemModified(int row,int col,BOOL bModified , eBufferFlag BufferFlag = Primary);
	void  SetItemModifiedInternal(NRecordID recordid, NDataField *field, BOOL bModified);
	BOOL  GetRowModified(int row, eBufferFlag BufferFlag = Primary);
	BOOL  GetRowModifiedInternal(char * pbuff);
	void  SetRowModified( int row,BOOL bModified, eBufferFlag BufferFlag = Primary);

	virtual void  SetRowModifiedInternal(void * buff,BOOL bModified);

	virtual BOOL OnRowFilter(int row);

	/******************************************************************** 
	������������DataSet�������� �п��ܿ����޸� Sort�ĺ���
	������
	�� �� ֵ��FALSE:�� Ĭ��ʵ�� TRUE������ʹ��Ĭ��ʵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:29]   11:06
	*********************************************************************/ 
	virtual BOOL OnSortCompareField(NRecordID lowRecordID, NRecordID highRecordID,NDataField * pField,int & iResult)
	{
		return FALSE;
	}

	int   GetColNum(CString ColName);
	NRecordID CloneRecord(int row,eBufferFlag buffer = Primary);

	/**
	* @brief  ��DataSet���� ��������һ��DataSet �У���ʽҪ��һ�� 
	*
	*          
	* @note  : ԭDataSet ����Ӱ��
	enum eBufferFlag
	{
	Primary��Ŀǰ��ʾ�ļ�¼, Delete��ɾ���ļ�¼, Filter���ڱ����˵ļ�¼
	};
	* @param :int startrow,ԭDataSet�� ���ƿ�ʼ��
	*			int endrow,ԭDataSet�� ������ֹ��
	*			eBufferFlag copybuffer,���Ƶ� ��¼����
	*			NDataModelDataSet *TargetData:Ŀ��DataSet
	*			int beforerow��Ŀ��DataSet�������λ��
	*			eBufferFlag	targetbuffer ��Ŀ��ļ�¼����
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [3/3/2012]
	*/
	virtual int   RowsCopy(int startrow,int endrow,eBufferFlag copybuffer,NDataModelDataSet *TargetData,int beforerow, eBufferFlag	targetbuffer);

	BOOL  SetFilter(CString ColName,LPTSTR sOperator ,LPTSTR sValue);
    BOOL  SetFilter(int ColIndex,LPTSTR sOperator ,LPTSTR sValue);

	virtual void DoFilter();
	virtual void FillFilterBuff(CPtrArray &filterBuffer);
	virtual void FillFilterBuff(LPVOID *pBuffers,LONG lBufferCount);

	int LoadFromTextFile(LPTSTR sFileName,_TCHAR delim = '\t');
	int LoadFromCString(CString &sbuff,char delim = '\t',BOOL isnewRecord = FALSE);
	BOOL _ImportXML(BSTR xml, BOOL bFromFile, BOOL bAppend, BOOL bAsNew);

	virtual int InsertRow(int iberforrow);

	long GetRowCount() const;

	//���ݴ�ȡ
	virtual CString GetAsString(int Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual double  GetAsFloat(int Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual int     GetAsInteger(int Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual VARIANT GetAsVariant(int Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual COleDateTime  GetAsDateTime(int Col,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual BOOL    GetAsBool(int Col,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;

	virtual BOOL    SetAsString(int Column, int row ,const CString& val);
	virtual BOOL    SetAsFloat(int Column, int row ,const double& val);
	virtual BOOL    SetAsInteger(int Column, int row ,const int& val);
	virtual BOOL    SetAsVariant(int Column, int row ,const VARIANT& val);
	virtual BOOL    SetAsDateTime(int Column, int row ,const COleDateTime& val);
	virtual BOOL    SetAsBool(int Column, int row ,const BOOL & val);

    //ʹ��id�Ž������ݴ�ȡ
	virtual CString GetAsString(int Column,NRecordID id, BOOL originalvalue = FALSE) const;
	virtual double  GetAsFloat(int Column,NRecordID id, BOOL originalvalue = FALSE) const;
	virtual int     GetAsInteger(int Column,NRecordID id, BOOL originalvalue = FALSE) const;
	virtual BOOL    GetAsBool(int Column,NRecordID id, BOOL originalvalue = FALSE) const;
	VARIANT GetAsVariant(int Column,NRecordID id, BOOL originalvalue = FALSE) const;
	COleDateTime  GetAsDateTime(int Column,NRecordID id, BOOL originalvalue = FALSE) const;

	virtual BOOL    SetAsString(int Column, NRecordID id ,const CString& val);
	virtual BOOL    SetAsFloat(int Column, NRecordID id ,const double& val);
	virtual BOOL    SetAsInteger(int Column, NRecordID id ,const int& val);
	virtual BOOL    SetAsBool(int Column, NRecordID id ,const BOOL & val);
	virtual BOOL    SetAsVariant(int Column, NRecordID id ,const VARIANT& val);
	virtual BOOL    SetAsDateTime(int Column, NRecordID id ,const COleDateTime& val);

	//������֧��
	long GetBlob(NDataField * field, NRecordID id ,void *& ppBolb) const;
	virtual long GetBlob(int Column, NRecordID id ,void *& ppBolb) const;
	
	//ʹ���ַ��������������ݴ洢
	virtual CString GetAsString(const CString & Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual double  GetAsFloat(const CString & Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual int     GetAsInteger(const CString & Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual VARIANT GetAsVariant(const CString & Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual COleDateTime  GetAsDateTime(const CString & Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual BOOL    GetAsBool(const CString & Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;

	virtual BOOL    SetAsString(const CString & Column, int row,const CString& val);
	virtual BOOL    SetAsFloat(const CString & Column, int row, const double& val);
	virtual BOOL    SetAsInteger(const CString & Column, int row,const int& val);
	virtual BOOL    SetAsVariant(const CString & Column, int row, const VARIANT& val);
	virtual BOOL    SetAsDateTime(const CString & Column, int row, const COleDateTime& val);
	virtual BOOL    SetAsBool(const CString & Column, int row, const BOOL & val);
	
	//������֧��
	virtual long GetBlob(const CString & Column, int row ,void *& ppBolb,eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	
	/******************************************************************** 
	������������ָ��FieldIndex����� ������Լ��ٲ�CString��Map ����
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:9:27]   16:22
	*********************************************************************/ 
	virtual long GetBlob(int iColumn, int row ,void *& ppBolb,eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	
	virtual BOOL SetBlob(const CString & Column, int row ,void * ppBolb,long iSize);
	virtual BOOL SetBlob(int Column, int row ,void * ppBolb,long iSize);
	virtual BOOL SetBlob(int Column, NRecordID id ,void * ppBolb,long iSize);
	
    NRecordID  GetRecordID(int row, eBufferFlag BufferFlag = Primary) const;
	
protected:
	//�������Ѿ���������NDataField������ʵ�ֱ�����
	void AlloOrigValueBuff(void *buff/*,BOOL AlloOrigBuff*/,int offset,int fieldsize)
	{
		char * ptr = (char *)buff;
		ptr+= (offset + fieldsize + 1);
		//ptr = (char*)(*ptr);
		if (((char*)(*ptr)) == NULL) //��δ�����ڴ�
		{
			char * newptr = new char[fieldsize];
			/*ptr = *ptr;
			(*ptr) =(char*) newptr;*/
			//���µ�ָ�븴�Ƶ��ֶ���
			CopyMemory((void*)ptr,&(newptr),sizeof(void*));
			buff=(char *)buff + offset;
			CopyMemory((void*)newptr,buff,fieldsize);

			delete []newptr;
		}
	}
public:
    
	inline  void * GetFieldDataPT(NDataField *field,void *buff, BOOL originalvalue = FALSE) const
	{
		if (buff == NULL) {
			return NULL;
		}
		/*ASSERT(buff != NULL);
		int iFieldOffset;
		iFieldOffset = GetFieldOffset(field);
		char * ptr = (char *)buff;
		ptr += iFieldOffset;
		return ptr;*/
		ASSERT(buff != NULL);
		int iFieldOffset;
		iFieldOffset = GetFieldOffset(field);
		char * ptr = (char *)buff;
		//�Ƿ���Ҫ����ԭֵ
		//if(bAlloOrigBuff) AlloOrigValueBuff(buff,iFieldOffset,field->GetSize());
		//�Ƿ���ԭֵ�����ж�
		if (originalvalue) 
		{
			//ȡ��ԭֵ��ָ��
			ptr+= iFieldOffset + field->GetSize() + 1;//1BYTE ��״̬λ��;
			//ptr = (char*)(*ptr);
			CopyMemory(&ptr,&(*ptr),sizeof(void*));
			if (ptr == NULL)
			{
				ptr = (char *)buff;
				ptr += iFieldOffset;
			}
		}
		else
		{
		   ptr += iFieldOffset;
		}
		return ptr;
	}
protected:
	virtual BOOL BuildItemXML(CString tableName,int row,NRecordID recordID,CString &sbuff,CString &sXMLBuf,CString &sXMLBefore);
	//ʹ��id�ź��ֶν������ݴ�ȡ
	CString GetAsString(NDataField * field,NRecordID id, BOOL originalvalue = FALSE) const;
	double  GetAsFloat(NDataField * field,NRecordID id, BOOL originalvalue = FALSE) const;
	int     GetAsInteger(NDataField * field,NRecordID id, BOOL originalvalue = FALSE) const;
	VARIANT GetAsVariant(NDataField * field,NRecordID id, BOOL originalvalue = FALSE) const;
	COleDateTime  GetAsDateTime(NDataField * field,NRecordID id, BOOL originalvalue = FALSE) const;
	BOOL    GetAsBool(NDataField * field,NRecordID id, BOOL originalvalue = FALSE) const;

	virtual BOOL SetBlob(NDataField * field, NRecordID id ,void * ppBolb,long iSize);

	virtual BOOL    SetAsString(NDataField * field, NRecordID id ,const CString& val);
	virtual BOOL    SetAsFloat(NDataField * field, NRecordID id ,const double& val);
	virtual BOOL    SetAsInteger(NDataField * field, NRecordID id ,const int& val);
	virtual BOOL    SetAsVariant(NDataField * field, NRecordID id ,const VARIANT& val);
	virtual BOOL    SetAsDateTime(NDataField * field, NRecordID id ,const COleDateTime& val);
	virtual BOOL    SetAsBool(NDataField * field, NRecordID id ,const BOOL & val);
	
protected:
   eBookmarkFlag *PBookmarkFlag;
   //���ڴ�Ż���ָ���б�

//   int m_BookmarkSize;
   //������������ֶεĴ�С
//   int m_CalcFieldsSize;
   //�����ַ���
   CString m_FilterText;

   int m_RecordBufferSize;
   int m_RecordSize;
//   long m_RecordCount;
//   long m_CurrentRecord;
   
   //��ŵ�ǰ��״̬
   eDataModelDataSetState m_State;

   //�����Ƿ�datawindow�������޸�
   BOOL m_IsModified;

   //���shardData�����ݴ��ڶ���
   CArray <NDataModelDataSet *,NDataModelDataSet*> *m_pShardDataList; 
protected:
	virtual NDataModelDataSet * NewShareDataObj();
	void CheckShardDataList();

	virtual void OnInsertedRow(int newrow);

	virtual void DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff);
	

//	virtual eGetResult GetRecord(char * pbuff , eGetMode getmode );
	//��ʼ����Ϣ
	virtual void InteralOpen();

protected: //���鹦��֧��
	CString Value2SQLValue(CString &sValue);
	CPtrArray    m_GroupList;

	BOOL         m_isGrouped;
	CPtrArray    m_Groupfields;
	CArray<int,int> m_GroupSortColAscending;
	  
};




#endif // !defined(AFX_NDATAMODELDATASET_H__FA146F4B_3B2B_4B74_B879_4BC879BAE7A4__INCLUDED_)
