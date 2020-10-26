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
��������������DataSet�� DataSet����Ҫ���� �������ļ��У��Լ����ڴ�ѹ����
������
�� �� ֵ��
�㷨��
ע�����
	1��	�����Ϊ�����֣�һ��Ϊ������ļ�����Project����������¼ ����_Index,����_Data����һ��Ϊ��ʱ�ļ�����Project�н�����_Data,���Ƶ���ʱ�ļ��У������Ա��溯��Ҳ��Ϊ������һ���Ǳ��浽��ʱ�ļ��У�һ��Ϊ�����������ļ���
	2��	�ص��޸��ڻ�ȡField��GetField�����õ�ָ��ƫ�ƣ�GetFieldOffset��GetFieldDataPT����
	3��	������ �������ݽṹ���£�
		a)	���������С��4BYTE��+������DataSet��Ϣ��ͬCPM_StorageDataSet�ṹ��+	�ֶ���Ϣ��С��4BYTE��+�ֶ���Ϣ+��¼��������4BYTE����(�ֶ���Ϣ�� �ļ�Field����Ϣ)
		b)	Dataset�ı���ģʽ��DataSet�汾���Ӱ汾���������м�¼��
	4��Ϊ���Ƹ�Ч�����Խ������ֶη����������С�
��    �ߣ�liwenlang
ʱ    �䣺[2010:10:25]   20:20
*********************************************************************/ 
class DataSetAPI CPM_LargeFileDataSet : public CPM_StorageDataSet
{
	DECLARE_DYNCREATE(CPM_LargeFileDataSet) 
public:

	struct DataSetAPI CLargeFieldInfo
	{
	public:
		static const CString FileRowPos;//�ļ�ƫ��λ��
		static const CString MemRowPos;//�ڴ�ƫ��λ��
		static const CString RowLength;//��¼����
		static const CString RowMode;//��ģʽ int ����״̬0���ļ�λ�ã�1��FilePos�е��������ڴ��ַ
									//2:�ڴ��еļ�¼���޸ġ�
		
		static const CString ScrapFilePos;//��Ƭ�ļ�ƫ��λ��
		static const CString ScrapLength;//��Ƭ��С

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
		InFile = 0,			//��¼���ļ���
		InMemory = 1,		//��¼���ڴ���
		InMemory_Mdf = 2	//��¼���ڴ������޸�
	};

	class DataSetAPI CStreamFileCtrl
	{
	public:
		CStreamFileCtrl();
		
		virtual ~CStreamFileCtrl();

		void SetStreamFilePath(CString strFilePath,CPM_LargeFileDataSet * pLargeFileDataSet);

		void DestroyAllStream();//�ͷ�������

		BOOL GetIndexRecordStream(CPMFileStreamCtrl *& pRecordStream);
		BOOL ReleaseIndexRecordSteam();

		BOOL GetFileRecordStream(CPMFileStreamCtrl *& pRecordStream);
		BOOL ReleaseFileRecordSteam();

		BOOL GetScrapRecordStream(CPMFileStreamCtrl *& pRecordStream);
		BOOL ReleaseScrapRecordSteam();

		BOOL GetRootStorage(CPMFileStorageCtrl *& pRootStorage);
		BOOL SaveRootStream();//������ڵ� ����ֻ�����ı���ʱ��Ч��
		BOOL ReleaseRootStorage();

	protected:
		
		CPM_LargeFileDataSet * m_pLargeFileDataSet;
		CString m_strFilePath;
		CPMFileStorageCtrl * m_piFileRootStr;
		int m_iFileRootCount;//�ļ��� ���ô���

		CPMFileStreamCtrl * m_pIndexRecordStream;//������
		int m_iIndexRecordCount;//������ ���ô���

		CPMFileStreamCtrl * m_pFileRecordStream;//�ļ�����
		int m_iFileRecordCount;//�ļ��� ���ô���

		CPMFileStreamCtrl * m_pScrapRecordStream;//�ļ���Ƭ��
		int m_iScrapRecordCount;//��Ƭ�� ���ô���
	};

	class DataSetAPI CFileRecordStreamCtrl
	{
	public:

		CFileRecordStreamCtrl(CStreamFileCtrl * pStreamFileCtrl);
		
		virtual ~CFileRecordStreamCtrl();

		CPMFileStreamCtrl * GetFileRecordStream();

	protected:
		
		CStreamFileCtrl * m_pStreamFileCtrl;
		CPMFileStreamCtrl * m_pFileRecordStream;//�ļ�����
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
	  ����������	 ��ȡ�ļ���������
		������
		�� �� ֵ��
		�㷨��
		ע��������ⲿ�߼� ���ƺܶ���ʱ,���ⲿ�߼��� ��ʹ��һ���߼�����,
			����ʹ�� CFileRecordStreamCtrl���������ļ���,������п���ʱ�ǳ���
		��    �ߣ�liwenlang
		ʱ    �䣺[2011:2:25   14:09]
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
	����������   ��ȡ �û�������ʼFieldIndex �� ��ֹFieldIndex
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:11:23]   9:15
	*********************************************************************/ 
	BOOL GetUserDataField_Range(int & iBegin,int & iEnd);

	/******************************************************************** 
	��������������ɱ�ṹ���� �������
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:26]   10:23
	*********************************************************************/ 
	virtual void RelCalFieldIndex();
	
	/******************************************************************** 
	������������Field ��ȡƫ����
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:26]   14:37
	*********************************************************************/ 
	virtual int GetFieldOffset(NDataField * pField) const;

	
	///////////////////////////////////////////////////////////////////////////
	///���ݿ����ຯ����ʼ

	/******************************************************************** 
	������������ȡ�ļ��е�һ�м�¼
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:26]   13:20
	*********************************************************************/ 
	BOOL GetFileRowRecord(int row, NRecordID &pBuffer,eBufferFlag BufferFlag = Primary) const;

	/******************************************************************** 
	����������ͨ�������� ��ȡ��������
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:11:4]   10:59
	*********************************************************************/ 
	BOOL GetFileRowRecord(NRecordID pIndexTableRowInfo, NRecordID &pBuffer) const;

	/******************************************************************** 
	������������ȡһ����¼
	������
	�� �� ֵ��0������ʧ�� 1�������� 2������
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:26]   13:22
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

	//ʹ��FieldIndex���ٲ���
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
		
	virtual long GetBlob(const CString & Column, int row ,void *& ppBolb,eBufferFlag BufferFlag = Primary, BOOL originalvalue = FALSE) const;
	virtual BOOL SetBlob(const CString & Column, int row ,void * ppBolb,long iSize);

	virtual BOOL OnRowFilter(int row);
	
	virtual BOOL OnSortCompareField(NRecordID lowRecordID, NRecordID highRecordID,NDataField * pField,int & iResult);

	virtual void  SetRowModifiedInternal(void * buff,BOOL bModified);

	///���ݿ����ຯ������
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	///���ļ��������ļ� ������ʼ

	/******************************************************************** 
	�����������򿪵���ʱ�ļ���
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:25]   20:35
	*********************************************************************/ 
	BOOL OpenLargeFile(CString strFilename);
	
	/******************************************************************** 
	�������������浽��ʱ�ļ���
	������
	�� �� ֵ��
	�㷨��
	ע������������޸����ݱ��浽 ��ʱ�ļ���
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:25]   20:33
	*********************************************************************/ 
	BOOL SaveMemory2DataFile();
	
	/**
	* @brief  ���ų���DataSet ���ݣ�����������Ƭ 
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
	�������������浽�����ļ���
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:25]   20:33
	*********************************************************************/ 
//	BOOL Save2File();
	
	CString GetScrapDataSetXML();

	///���ļ��������ļ� ��������
	//////////////////////////////////////////////////////////////////////////

	virtual void ResetFilter(BOOL bResetExp = TRUE);

protected:

	BOOL UpdataFileDataSet_Field(short iOpeningDataCode,int iRow,LPVOID pFormBuffer,CPM_StorageDataSet * pTemFileDataSet,NDataField * pDataField);

	/******************************************************************** 
	��������������FileDataSet
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2011:1:4]   10:48
	*********************************************************************/ 
	BOOL UpdataFileDataSet(CPM_StorageDataSet * pTemFileFieldDataSet,short iOpeningDataCode);

	virtual BOOL BuildItemXML(CString tableName,int row,NRecordID recordID,CString &sbuff,CString &sXMLBuf,CString &sXMLBefore);

	//////////////////////////////////////////////////////////////////////////
	///��Ƭ����ƺ��� ��ʼ

	/******************************************************************** 
	����������������ƬTable
	������iSortMode��1 �ļ�λ������ 2 ��Ƭ��С����
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:27]   11:31
	*********************************************************************/ 
	void ReSortScrapTable(int iSortMode);

	/******************************************************************** 
	�������������ļ���Ƭ ��Ϣ д�� ��Ƭ����
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:27]   11:29
	*********************************************************************/ 
	void AddScrapFileInfo(int iFilePos,int iLength,BOOL bNeedReSort = FALSE);

	/******************************************************************** 
	������������ȡ����ʵ� �ļ���Ƭ λ�� �� ��С
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:27]   15:34
	*********************************************************************/ 
	BOOL GetScrapFileInfo(NDataModelDataSet * pScrapDataSet,int iCheckLength,int & iFilePos,int & iLength,BOOL bNeedReSort = FALSE);

	///��Ƭ����ƺ��� ����
	//////////////////////////////////////////////////////////////////////////
	void FreeFileRecordBuffer(void *pBuffer);

	LPVOID AddFileRecord2Mem(int iRow,eBufferFlag BufferFlag = Primary);
	
	/******************************************************************** 
	����������ע������������ ������ڴ棬���ܺ����������
	������
	�� �� ֵ��
	�㷨��
	ע�����
	��    �ߣ�liwenlang
	ʱ    �䣺[2010:10:26]   9:32
	*********************************************************************/ 
	void * AlloFileRecordBuffer();

	CPM_StorageDataSet * m_pFileScrapDataSet;//�ļ���Ƭ��
	
//	NDataModelDataSetBuffer * m_pFileBuffers;
	NDataFields m_Filefields;
	CString m_strFileTableName;//�ļ������֣���Ҫ�����������ݽ��ж�λ
	CString m_strLargeFilePath;//�ļ�����λ��

	int m_FileRecordSize;
	int m_FileRecordBufferSize;

// 	IStorage* m_piFileRootStr;//���ڱ����ļ�������ӿڣ����ͷ�ʱҪRelease
// 	IStream* m_pFileRecordStream;//���ͷ�ʱҪRelease
	CStreamFileCtrl * m_pStreamFileCtrl;

//	BOOL m_IsInCacheMode;//�����ģʽ�´��ļ���ȡ�����Ϣ�����������ڴ��С�
};

#endif // !defined(AFX_PM_LARGEFILEDATASET_H__033D161F_9E07_4AFC_AA09_0AB04A57B76F__INCLUDED_)
