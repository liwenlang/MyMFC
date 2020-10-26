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
* @brief  NDataModelDataSet 是DataSet的主要派生类，里面已经加入表格内容信息。  
*		也有很多业务功能函数  
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
		unsigned Status; //行状态
		/*long RecordNumber;
		eBookmarkFlag BookmarkFlag;
		eBufferFlag BufferFlag ;
		eBufferState BufferState;
		BYTE Attribute;*/
	}RecInfo;

public:

	/**
		* @brief 添加函数判断 行号 是否有效
		*
		*          
		* @note  : 
		* @param : iRow 是从 1 开始计算的
		* @defenc: 
		* @return: 
		* @author: liwenlang
		* @date  : 2013年9月2日   16:44
	*/
	BOOL	IsRowValid( const int iRow ) ; 

	/**
	* @brief 将DataSet信息串化到Arr中  
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
	* @brief  将Arr队列信息 串化到 DataSet中 
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
	* @brief  将单行信息 创建为Buffer 
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
	* @brief 将Buffer读取到DataSet中，并建立行  
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
	* @brief  过滤指定的 DataSet的某行 
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

	//将filter中的数据刷新到主存储区,返回移动的缓存的大小
	int  CopyFilterToPrimaryBuffer();
	void RemoveCopyedFilterToPrimaryBuffer(const int bufferSize);

	/******************************************************************** 
	功能描述：获取DataSet的内存大小
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:9:26]   13:41
	*********************************************************************/ 
	virtual LONG GetNDataMemorySize();

	/****************************************************************
	  功能描述：	 获取每行Buffer大小
		参数：
		返 回 值：
		算法：
		注意事项：
		作    者：liwenlang
		时    间：[2011:3:4   9:45]
	*****************************************************************/
	int GetRowBufferSize()
	{
		return m_RecordBufferSize;
	}

	virtual void Sort();
	
	void ResetSortField();
	void AddSortField(CString sField,BOOL Ascending);

	/******************************************************************** 
	功能描述：	  检查是否需要 重排DataSet
	参数：strFieldNameArr：重排段 iSortAscendingArr：排序方式 checkFilter：是否要检查Filter段
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:12:7]   14:19
	*********************************************************************/ 
	BOOL CheckIsNeedSort(CStringArray & strFieldNameArr,CArray<int,int> & iSortAscendingArr,BOOL checkFilter = TRUE);

	/******************************************************************** 
	功能描述：用指定FieldIndex来完成 排序可以减少查CString的Map 会快点
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:9:27]   16:13
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
	 功能描述：读取XML 但是不读取DataBefore信息
	 参数：
	 返 回 值：
	 算法：
	 注意事项：慎用
	 作    者：liwenlang
	 时    间：[25/12/2009]
	*********************************************************************/ 
	BOOL LoadDataSetXMLBuf_NoDataBefore(const CString & strXMLBuf,BOOL bAppend);

	virtual BOOL LoadDataSetXMLBuf(const CString &sXMLBuf, BOOL bAppend);
	int Bisearch(void* pBuff, CString sFieldName,eBufferFlag BufferFlag = Primary);
	int FindNearest(void *pBuff, CString sFieldName, eBufferFlag BufferFlag);
	int Bisearch(const CArray<void *,void *> &findValues,const CStringArray &fieldNames,const eBufferFlag BufferFlag = Primary);
	
	/******************************************************************** 
	功能描述：针对目前提供的折半查找方式，只通过FieldIndex来去Field指针效率更高
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:9:27]   9:18
	*********************************************************************/ 
	int Bisearch(void* pBuff, int iFieldIndex,eBufferFlag BufferFlag = Primary);
	int Bisearch(const CArray<void *,void *> &findValues,const CUIntArray & iFieldIndexArr,const eBufferFlag BufferFlag = Primary);

	//得到共享当前数据的对象,同时可以监视是否原数据发生的编码
	NDataModelDataSet * GetShardData(BOOL bSpyChanged = FALSE,BOOL PrimaryOnly = FALSE);
	NDataModelDataSet * CloneStruct();
	//Clone一个一样结构一样数据的对象
	virtual NDataModelDataSet * Clone();
	void SaveAsText(CString &sBuff);
	virtual void ResetFilter(BOOL bResetExp = TRUE);
	FieldType GetFieldType(int iFieldIndex);
	//初始化列结构
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
	//字段属性设置
	BOOL IsFieldPK(int col);
	BOOL IsFieldPKAutoIncrease(int col);
	BOOL IsFieldCanNull(int col);
	BOOL IsFieldUpdateAble(int col);
	
	//新增函数08.1.15
	//BOOL IsFieldBlob(int col); 禁用采用下面函数替代
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
	//主要用于字段对象的反调
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
	功能描述：让DataSet的派生类 有可能可以修改 Sort的函数
	参数：
	返 回 值：FALSE:按 默认实现 TRUE：不用使用默认实现
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:10:29]   11:06
	*********************************************************************/ 
	virtual BOOL OnSortCompareField(NRecordID lowRecordID, NRecordID highRecordID,NDataField * pField,int & iResult)
	{
		return FALSE;
	}

	int   GetColNum(CString ColName);
	NRecordID CloneRecord(int row,eBufferFlag buffer = Primary);

	/**
	* @brief  将DataSet内容 拷贝到另一个DataSet 中，格式要求一样 
	*
	*          
	* @note  : 原DataSet 不受影响
	enum eBufferFlag
	{
	Primary：目前显示的记录, Delete：删除的记录, Filter：在被过滤的记录
	};
	* @param :int startrow,原DataSet的 复制开始行
	*			int endrow,原DataSet的 复制终止行
	*			eBufferFlag copybuffer,复制的 记录类型
	*			NDataModelDataSet *TargetData:目标DataSet
	*			int beforerow：目标DataSet插入的行位置
	*			eBufferFlag	targetbuffer ：目标的记录类型
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

	//数据存取
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

    //使用id号进行数据存取
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

	//大数据支持
	long GetBlob(NDataField * field, NRecordID id ,void *& ppBolb) const;
	virtual long GetBlob(int Column, NRecordID id ,void *& ppBolb) const;
	
	//使用字符串列名进行数据存储
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
	
	//大数据支持
	virtual long GetBlob(const CString & Column, int row ,void *& ppBolb,eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	
	/******************************************************************** 
	功能描述：用指定FieldIndex来完成 排序可以减少查CString的Map 会快点
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:9:27]   16:22
	*********************************************************************/ 
	virtual long GetBlob(int iColumn, int row ,void *& ppBolb,eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	
	virtual BOOL SetBlob(const CString & Column, int row ,void * ppBolb,long iSize);
	virtual BOOL SetBlob(int Column, int row ,void * ppBolb,long iSize);
	virtual BOOL SetBlob(int Column, NRecordID id ,void * ppBolb,long iSize);
	
    NRecordID  GetRecordID(int row, eBufferFlag BufferFlag = Primary) const;
	
protected:
	//本函数已经废弃，在NDataField对象中实现本功能
	void AlloOrigValueBuff(void *buff/*,BOOL AlloOrigBuff*/,int offset,int fieldsize)
	{
		char * ptr = (char *)buff;
		ptr+= (offset + fieldsize + 1);
		//ptr = (char*)(*ptr);
		if (((char*)(*ptr)) == NULL) //尚未分配内存
		{
			char * newptr = new char[fieldsize];
			/*ptr = *ptr;
			(*ptr) =(char*) newptr;*/
			//将新的指针复制到字段中
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
		//是否需要创建原值
		//if(bAlloOrigBuff) AlloOrigValueBuff(buff,iFieldOffset,field->GetSize());
		//是否是原值进行判断
		if (originalvalue) 
		{
			//取得原值的指针
			ptr+= iFieldOffset + field->GetSize() + 1;//1BYTE 的状态位置;
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
	//使用id号和字段进行数据存取
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
   //用于存放缓存指针列表

//   int m_BookmarkSize;
   //计算出的所有字段的大小
//   int m_CalcFieldsSize;
   //过滤字符串
   CString m_FilterText;

   int m_RecordBufferSize;
   int m_RecordSize;
//   long m_RecordCount;
//   long m_CurrentRecord;
   
   //存放当前的状态
   eDataModelDataSetState m_State;

   //保存是否datawindow发生过修改
   BOOL m_IsModified;

   //存放shardData的数据窗口对象
   CArray <NDataModelDataSet *,NDataModelDataSet*> *m_pShardDataList; 
protected:
	virtual NDataModelDataSet * NewShareDataObj();
	void CheckShardDataList();

	virtual void OnInsertedRow(int newrow);

	virtual void DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff);
	

//	virtual eGetResult GetRecord(char * pbuff , eGetMode getmode );
	//初始化信息
	virtual void InteralOpen();

protected: //分组功能支持
	CString Value2SQLValue(CString &sValue);
	CPtrArray    m_GroupList;

	BOOL         m_isGrouped;
	CPtrArray    m_Groupfields;
	CArray<int,int> m_GroupSortColAscending;
	  
};




#endif // !defined(AFX_NDATAMODELDATASET_H__FA146F4B_3B2B_4B74_B879_4BC879BAE7A4__INCLUDED_)
