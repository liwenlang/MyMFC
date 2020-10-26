// PM_LargeFileDataSet.h: interface for the CPM_LargeFileDataSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PM_LARGEFILEDATASET_H__033D161F_9E07_4AFC_AA09_0AB04A57B76F__INCLUDED_)
#define AFX_PM_LARGEFILEDATASET_H__033D161F_9E07_4AFC_AA09_0AB04A57B76F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PM_StorageDataSet.h"

/******************************************************************** 
功能描述：大型DataSet将 DataSet的主要内容 保存在文件中，以减轻内存压力。
参数：
返 回 值：
算法：
注意事项：
	1、	保存分为两部分，一个为保存的文件（在Project中有两条记录 表名_Index,表名_Data）。一个为临时文件（从Project中将表名_Data,复制到临时文件中）。所以保存函数也分为两个，一个是保存到临时文件中，一个为保存在永久文件。
	2、	重点修改在获取Field（GetField），得到指针偏移（GetFieldOffset，GetFieldDataPT）。
	3、	索引表 保存数据结构如下：
		a)	总索引表大小（4BYTE）+索引表DataSet信息（同CPM_StorageDataSet结构）+	字段信息大小（4BYTE）+字段信息+记录总行数（4BYTE）。(字段信息是 文件Field的信息)
		b)	Dataset的编码模式，DataSet版本，子版本在索引表中记录。
	4、为控制高效，可以将常用字段放在索引表中。
作    者：liwenlang
时    间：[2010:10:25]   20:20
*********************************************************************/ 
class DataSetAPI CPM_LargeFileDataSet : public CPM_StorageDataSet
{
	DECLARE_DYNCREATE(CPM_LargeFileDataSet) 
public:

	struct DataSetAPI CLargeFieldInfo
	{
	public:
		static const CString FileRowPos;//文件偏移位置
		static const CString MemRowPos;//内存偏移位置
		static const CString RowLength;//记录长度
		static const CString RowMode;//行模式 int 保存状态0：文件位置，1：FilePos中的内容是内存地址
									//2:内存中的记录有修改。
		
		static const CString ScrapFilePos;//碎片文件偏移位置
		static const CString ScrapLength;//碎片大小

		int	 FilePos_Index;
		int	 MemRowPos_Index;
		int	 RowLength_Index;
		int	 RowMode_Index;

		int ScrapFilePos_Index;
		int ScrapLength_Index;	
	};
	CLargeFieldInfo m_LargeFieldInfo;

	enum DataMode
	{
		InFile = 0,			//记录在文件中
		InMemory = 1,		//记录在内存中
		InMemory_Mdf = 2	//记录在内存中以修改
	};

	class DataSetAPI CStreamFileCtrl
	{
	public:
		CStreamFileCtrl();
		
		virtual ~CStreamFileCtrl();

		void SetStreamFilePath(CString strFilePath,CPM_LargeFileDataSet * pLargeFileDataSet);

		void DestroyAllStream();//释放所有流

		BOOL GetIndexRecordStream(CPMFileStreamCtrl *& pRecordStream);
		BOOL ReleaseIndexRecordSteam();

		BOOL GetFileRecordStream(CPMFileStreamCtrl *& pRecordStream);
		BOOL ReleaseFileRecordSteam();

		BOOL GetScrapRecordStream(CPMFileStreamCtrl *& pRecordStream);
		BOOL ReleaseScrapRecordSteam();

		BOOL GetRootStorage(CPMFileStorageCtrl *& pRootStorage);
		BOOL SaveRootStream();//保存根节点 流，只有他的保存时有效的
		BOOL ReleaseRootStorage();

	protected:
		
		CPM_LargeFileDataSet * m_pLargeFileDataSet;
		CString m_strFilePath;
		CPMFileStorageCtrl * m_piFileRootStr;
		int m_iFileRootCount;//文件夹 引用次数

		CPMFileStreamCtrl * m_pIndexRecordStream;//索引表
		int m_iIndexRecordCount;//索引表 引用次数

		CPMFileStreamCtrl * m_pFileRecordStream;//文件主表
		int m_iFileRecordCount;//文件表 引用次数

		CPMFileStreamCtrl * m_pScrapRecordStream;//文件碎片表
		int m_iScrapRecordCount;//碎片表 引用次数
	};

	class DataSetAPI CFileRecordStreamCtrl
	{
	public:

		CFileRecordStreamCtrl(CStreamFileCtrl * pStreamFileCtrl);
		
		virtual ~CFileRecordStreamCtrl();

		CPMFileStreamCtrl * GetFileRecordStream();

	protected:
		
		CStreamFileCtrl * m_pStreamFileCtrl;
		CPMFileStreamCtrl * m_pFileRecordStream;//文件主表
	};

	enum FieldMode
	{
		NullField = 0,
		IndexField = 1,	
		DataField = 2
	};

//	void BeginCacheMode(){m_IsInCacheMode = TRUE;}
//	void EndCacheMode();

	CPM_LargeFileDataSet();
	virtual ~CPM_LargeFileDataSet();
	
	/****************************************************************
	  功能描述：	 获取文件流控制类
		参数：
		返 回 值：
		算法：
		注意事项：在外部逻辑 控制很多行时,在外部逻辑中 多使用一次逻辑记数,
			就是使用 CFileRecordStreamCtrl对象锁定文件流,否则多行控制时非常慢
		作    者：liwenlang
		时    间：[2011:2:25   14:09]
	*****************************************************************/
	CPM_LargeFileDataSet::CStreamFileCtrl * GetStreamFileCtrl(){ return m_pStreamFileCtrl;}

	virtual void Reset();

	virtual void DoFilter();

	virtual void Sort();

	BOOL AddFileField(CString sFieldName, FieldType fieldtype, int size);
	virtual void InteralOpen();
	void SetFileTableName(CString strFileTableName){m_strFileTableName = strFileTableName;}
	CString GetFileTableName(){return m_strFileTableName;}

	virtual int GetFieldIndex(const CString & sFieldName) const;
	virtual NDataField * GetField(int index) const;
	virtual CString GetFieldName(int index) const;

	virtual void GetAllField(CArray<NDataField*,NDataField*> & dataFields);

	virtual NDataModelDataSet * Clone();

	virtual int   RowsCopy(int startrow,int endrow,eBufferFlag copybuffer,NDataModelDataSet *TargetData,int beforerow, eBufferFlag	targetbuffer);

	/******************************************************************** 
	功能描述：   获取 用户数据起始FieldIndex 和 终止FieldIndex
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:11:23]   9:15
	*********************************************************************/ 
	BOOL GetUserDataField_Range(int & iBegin,int & iEnd);

	/******************************************************************** 
	功能描述：在完成表结构创建 必须调用
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:10:26]   10:23
	*********************************************************************/ 
	virtual void RelCalFieldIndex();
	
	/******************************************************************** 
	功能描述：以Field 获取偏移量
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:10:26]   14:37
	*********************************************************************/ 
	virtual int GetFieldOffset(NDataField * pField) const;

	
	///////////////////////////////////////////////////////////////////////////
	///数据控制类函数开始

	/******************************************************************** 
	功能描述：获取文件中的一行记录
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:10:26]   13:20
	*********************************************************************/ 
	BOOL GetFileRowRecord(int row, NRecordID &pBuffer,eBufferFlag BufferFlag = Primary) const;

	/******************************************************************** 
	功能描述：通过索引表 索取主表内容
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:11:4]   10:59
	*********************************************************************/ 
	BOOL GetFileRowRecord(NRecordID pIndexTableRowInfo, NRecordID &pBuffer) const;

	/******************************************************************** 
	功能描述：获取一条记录
	参数：
	返 回 值：0：调用失败 1：索引表 2：主表
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:10:26]   13:22
	*********************************************************************/ 
	CPM_LargeFileDataSet::FieldMode GetRowRecord(int iCol,int row,NRecordID &pBuffer) const;

	CPM_LargeFileDataSet::FieldMode GetRowRecord(NDataField * pField,int row,NRecordID &pBuffer) const;

	CPM_LargeFileDataSet::FieldMode GetRowRecord(int iCol,NRecordID pIndexBuffer,NRecordID &pBuffer) const;

	CPM_LargeFileDataSet::FieldMode GetRowRecord(NDataField * pField,NRecordID pIndexBuffer,NRecordID &pBuffer) const;

	virtual int InsertRow(int iberforrow);

	virtual void DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff);

	virtual CString GetAsString(int Column,NRecordID id, BOOL originalvalue = FALSE) const;
	virtual double  GetAsFloat(int Column,NRecordID id, BOOL originalvalue = FALSE) const;
	virtual int     GetAsInteger(int Column,NRecordID id, BOOL originalvalue = FALSE) const;
	virtual BOOL    GetAsBool(int Column,NRecordID id, BOOL originalvalue = FALSE) const;

	virtual BOOL    SetAsString(int Column, NRecordID id ,const CString& val);
	virtual BOOL    SetAsFloat(int Column, NRecordID id ,const double& val);
	virtual BOOL    SetAsInteger(int Column, NRecordID id ,const int& val);
	virtual BOOL    SetAsBool(int Column, NRecordID id ,const BOOL & val);

	//使用FieldIndex快速查找
	virtual BOOL    SetAsString(int Column, int row ,const CString& val);
	virtual BOOL    SetAsFloat(int Column, int row ,const double& val);
	virtual BOOL    SetAsInteger(int Column, int row ,const int& val);
	virtual BOOL    SetAsVariant(int Column, int row ,const VARIANT& val);
	virtual BOOL    SetAsDateTime(int Column, int row ,const COleDateTime& val);
	virtual BOOL    SetAsBool(int Column, int row ,const BOOL & val);

	virtual CString GetAsString(int Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual double  GetAsFloat(int Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual int     GetAsInteger(int Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual VARIANT GetAsVariant(int Column,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual COleDateTime  GetAsDateTime(int Col,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual BOOL    GetAsBool(int Col,int row, eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;

	virtual long GetBlob(int iColumn, int row ,void *& ppBolb,eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual long GetBlob(int Column, NRecordID id ,void *& ppBolb) const;

	virtual BOOL SetBlob(int Column, int row ,void * ppBolb,long iSize);
	virtual BOOL SetBlob(int Column, NRecordID id ,void * ppBolb,long iSize);

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
		
	virtual long GetBlob(const CString & Column, int row ,void *& ppBolb,eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual BOOL SetBlob(const CString & Column, int row ,void * ppBolb,long iSize);

	virtual BOOL OnRowFilter(int row);
	
	virtual BOOL OnSortCompareField(NRecordID lowRecordID, NRecordID highRecordID,NDataField * pField,int & iResult);

	virtual void  SetRowModifiedInternal(void * buff,BOOL bModified);

	///数据控制类函数结束
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	///打开文件，保存文件 函数开始

	/******************************************************************** 
	功能描述：打开到零时文件中
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:10:25]   20:35
	*********************************************************************/ 
	BOOL OpenLargeFile(CString strFilename);
	
	/******************************************************************** 
	功能描述：保存到临时文件中
	参数：
	返 回 值：
	算法：
	注意事项：将缓存修改数据保存到 临时文件中
	作    者：liwenlang
	时    间：[2010:10:25]   20:33
	*********************************************************************/ 
	BOOL SaveMemory2DataFile();
	
	/**
	* @brief  重排超大DataSet 内容，用于整理碎片 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [21/2/2012]
	*/
	BOOL ReSortLargeFileDataSetFile();

	/******************************************************************** 
	功能描述：保存到永久文件中
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:10:25]   20:33
	*********************************************************************/ 
//	BOOL Save2File();
	
	CString GetScrapDataSetXML();

	///打开文件，保存文件 函数结束
	//////////////////////////////////////////////////////////////////////////

	virtual void ResetFilter(BOOL bResetExp = TRUE);

protected:

	BOOL UpdataFileDataSet_Field(short iOpeningDataCode,int iRow,LPVOID pFormBuffer,CPM_StorageDataSet * pTemFileDataSet,NDataField * pDataField);

	/******************************************************************** 
	功能描述：升级FileDataSet
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2011:1:4]   10:48
	*********************************************************************/ 
	BOOL UpdataFileDataSet(CPM_StorageDataSet * pTemFileFieldDataSet,short iOpeningDataCode);

	virtual BOOL BuildItemXML(CString tableName,int row,NRecordID recordID,CString &sbuff,CString &sXMLBuf,CString &sXMLBefore);

	//////////////////////////////////////////////////////////////////////////
	///碎片表控制函数 开始

	/******************************************************************** 
	功能描述：重排碎片Table
	参数：iSortMode：1 文件位置排序 2 碎片大小排序
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:10:27]   11:31
	*********************************************************************/ 
	void ReSortScrapTable(int iSortMode);

	/******************************************************************** 
	功能描述：将文件碎片 信息 写入 碎片表中
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:10:27]   11:29
	*********************************************************************/ 
	void AddScrapFileInfo(int iFilePos,int iLength,BOOL bNeedReSort = FALSE);

	/******************************************************************** 
	功能描述：获取最合适的 文件碎片 位置 与 大小
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:10:27]   15:34
	*********************************************************************/ 
	BOOL GetScrapFileInfo(NDataModelDataSet * pScrapDataSet,int iCheckLength,int & iFilePos,int & iLength,BOOL bNeedReSort = FALSE);

	///碎片表控制函数 结束
	//////////////////////////////////////////////////////////////////////////
	void FreeFileRecordBuffer(void *pBuffer);

	LPVOID AddFileRecord2Mem(int iRow,eBufferFlag BufferFlag = Primary);
	
	/******************************************************************** 
	功能描述：注意这里分配的是 主表的内存，不能和索引表混用
	参数：
	返 回 值：
	算法：
	注意事项：
	作    者：liwenlang
	时    间：[2010:10:26]   9:32
	*********************************************************************/ 
	void * AlloFileRecordBuffer();

	CPM_StorageDataSet * m_pFileScrapDataSet;//文件碎片表
	
//	NDataModelDataSetBuffer * m_pFileBuffers;
	NDataFields m_Filefields;
	CString m_strFileTableName;//文件表名字，重要用来对主内容进行定位
	CString m_strLargeFilePath;//文件所在位置

	int m_FileRecordSize;
	int m_FileRecordBufferSize;

// 	IStorage* m_piFileRootStr;//用于保存文件索引表接口，类释放时要Release
// 	IStream* m_pFileRecordStream;//类释放时要Release
	CStreamFileCtrl * m_pStreamFileCtrl;

//	BOOL m_IsInCacheMode;//在这个模式下从文件读取后的信息，将保存在内存中。
};

#endif // !defined(AFX_PM_LARGEFILEDATASET_H__033D161F_9E07_4AFC_AA09_0AB04A57B76F__INCLUDED_)
