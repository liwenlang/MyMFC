// PM_LargeFileDataSet.cpp: implementation of the CPM_LargeFileDataSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PM_LargeFileDataSet.h"
#include "CDataSet_StringBuilder.h"
#include "CPub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define COM_RESULT_OK(hr) if (!SUCCEEDED(hr)){bSaveResult = FALSE; goto Com_Error_Exit;}
#define FUN_RESULT_OK(hr) if (!(hr)){bSaveResult = FALSE; goto Com_Error_Exit;}
#define RELEASE_INTERFACE(X) if (X) { (X)->Release(); (X) = NULL; }

const CString CPM_LargeFileDataSet::CLargeFieldInfo::FileRowPos = _T("filerowpos");//文件偏移位置
const CString CPM_LargeFileDataSet::CLargeFieldInfo::MemRowPos = _T("memrowpos");//内存偏移位置
const CString CPM_LargeFileDataSet::CLargeFieldInfo::RowLength = _T("rowlength");//记录长度
const CString CPM_LargeFileDataSet::CLargeFieldInfo::RowMode = _T("rowmode");//行模式 int 保存状态0：文件位置，1：FilePos中的内容是内存地址

const CString CPM_LargeFileDataSet::CLargeFieldInfo::ScrapFilePos = _T("scrapfilepos");//碎片文件偏移位置
const CString CPM_LargeFileDataSet::CLargeFieldInfo::ScrapLength = _T("scraplength");//碎片大小	

//////////////////////////////////////////////////////////////////////////
///CPM_LargeFileDataSet::CStreamFileCtrl
CPM_LargeFileDataSet::CStreamFileCtrl::CStreamFileCtrl()
{
	m_pLargeFileDataSet = NULL;
	m_strFilePath = _T("");
	m_piFileRootStr = NULL;
	m_iFileRootCount = 0;//文件夹 引用次数

	m_pIndexRecordStream = NULL;//索引表
	m_iIndexRecordCount = 0;//索引表 引用次数

	m_pFileRecordStream = NULL;//文件主表
	m_iFileRecordCount = 0;//文件表 引用次数

	m_pScrapRecordStream = NULL;//文件碎片表
	m_iScrapRecordCount = 0;//碎片表 引用次数
}
		
CPM_LargeFileDataSet::CStreamFileCtrl::~CStreamFileCtrl()
{
	DestroyAllStream();
}

void CPM_LargeFileDataSet::CStreamFileCtrl::SetStreamFilePath(CString strFilePath,CPM_LargeFileDataSet * pLargeFileDataSet)
{
	m_strFilePath = strFilePath;
	m_pLargeFileDataSet = pLargeFileDataSet;
}

void CPM_LargeFileDataSet::CStreamFileCtrl::DestroyAllStream()
{
	if(m_pIndexRecordStream)
	{
		m_pIndexRecordStream->CloseStream();

		delete m_pIndexRecordStream;
		m_pIndexRecordStream = NULL;
	}

	if(m_pFileRecordStream)
	{
		m_pFileRecordStream->CloseStream();

		delete m_pFileRecordStream;
		m_pFileRecordStream = NULL;
	}

	if(m_pScrapRecordStream)
	{
		m_pScrapRecordStream->CloseStream();

		delete m_pScrapRecordStream;
		m_pScrapRecordStream = NULL;
	}

	if(m_piFileRootStr)
	{
		m_piFileRootStr->CloseStorage();

		delete m_piFileRootStr;
		m_piFileRootStr = NULL;
	}
// 	RELEASE_INTERFACE(m_pIndexRecordStream);
// 	RELEASE_INTERFACE(m_pFileRecordStream);
// 	RELEASE_INTERFACE(m_pScrapRecordStream);
// 	RELEASE_INTERFACE(m_piFileRootStr);

	m_iIndexRecordCount = 0;
	m_iFileRecordCount = 0;
	m_iScrapRecordCount = 0;
	m_iFileRootCount = 0;
}

BOOL CPM_LargeFileDataSet::CStreamFileCtrl::GetIndexRecordStream(CPMFileStreamCtrl *& pRecordStream)
{
	CPMFileStorageCtrl * pRootStorage = NULL;
	if(!GetRootStorage(pRootStorage))
	{
		return FALSE;
	}

	if(NULL == m_pIndexRecordStream)
	{
		CString strTableName = m_pLargeFileDataSet->GetTableName();
		m_pIndexRecordStream = new CPMFileStreamCtrl;	
		HRESULT hr = m_pIndexRecordStream->TryOpenStream(pRootStorage,strTableName); 
		if(!SUCCEEDED(hr))
		{
			m_pIndexRecordStream = NULL;
			CString strMsg = _T("CPM_LargeFileDataSet::CStreamFileCtrl::GetIndexRecordStream 构造IndexRecordStream出错!");
			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
			return FALSE;
		}
	}
	m_iIndexRecordCount++;
	pRecordStream = m_pIndexRecordStream;
	return TRUE;
}

BOOL CPM_LargeFileDataSet::CStreamFileCtrl::ReleaseIndexRecordSteam()
{
	if(m_iIndexRecordCount > 0)
	{
		m_iIndexRecordCount--;
		if(0 >= m_iIndexRecordCount)
		{
//			RELEASE_INTERFACE(m_pIndexRecordStream);
			m_pIndexRecordStream->CloseStream();
			delete m_pIndexRecordStream;
			m_pIndexRecordStream = NULL;
		}
	}

	return ReleaseRootStorage();
}

BOOL CPM_LargeFileDataSet::CStreamFileCtrl::GetFileRecordStream(CPMFileStreamCtrl *& pRecordStream)
{
	CPMFileStorageCtrl * pRootStorage = NULL;
	if(!GetRootStorage(pRootStorage))
	{
		return FALSE;
	}

	if(NULL == m_pFileRecordStream)
	{
		CString strTableName = m_pLargeFileDataSet->GetFileTableName();
		m_pFileRecordStream = new CPMFileStreamCtrl;	
		HRESULT hr = m_pFileRecordStream->TryOpenStream(pRootStorage,strTableName);
		if(!SUCCEEDED(hr))
		{
			delete m_pFileRecordStream;
			m_pFileRecordStream = NULL;
			CString strMsg = _T("CPM_LargeFileDataSet::CStreamFileCtrl::GetFileRecordStream 构造m_pFileRecordStream出错!");
			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
			return FALSE;
		}
	}
	m_iFileRecordCount++;
	pRecordStream = m_pFileRecordStream;
	return TRUE;
}
		
BOOL CPM_LargeFileDataSet::CStreamFileCtrl::ReleaseFileRecordSteam()
{
	if(m_iFileRecordCount > 0)
	{
		m_iFileRecordCount--;
		if(0 >= m_iFileRecordCount)
		{
			m_pFileRecordStream->CloseStream();
			delete m_pFileRecordStream;
			m_pFileRecordStream = NULL;
//			RELEASE_INTERFACE(m_pFileRecordStream);
		}
	}

	return ReleaseRootStorage();
}

BOOL CPM_LargeFileDataSet::CStreamFileCtrl::GetScrapRecordStream(CPMFileStreamCtrl *& pRecordStream)
{

	CPMFileStorageCtrl * pRootStorage = NULL;
	if(!GetRootStorage(pRootStorage))
	{
		return FALSE;
	}

	if(NULL == m_pScrapRecordStream)
	{
		CString strTableName = m_pLargeFileDataSet->GetTableName() + _T("Scrap");
		m_pScrapRecordStream = new CPMFileStreamCtrl;
		HRESULT hr = m_pScrapRecordStream->TryOpenStream(pRootStorage,strTableName);
		if(!SUCCEEDED(hr))
		{
			delete m_pScrapRecordStream;
			m_pScrapRecordStream = NULL;
			CString strMsg = _T("CPM_LargeFileDataSet::CStreamFileCtrl::GetScrapRecordStream 构造m_pScrapRecordStream出错!");
			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
			return FALSE;
		}
	}
	m_iScrapRecordCount++;
	pRecordStream = m_pScrapRecordStream;
	return TRUE;
}
		
BOOL CPM_LargeFileDataSet::CStreamFileCtrl::ReleaseScrapRecordSteam()
{
	if(m_iScrapRecordCount > 0)
	{
		m_iScrapRecordCount--;
		if(0 >= m_iScrapRecordCount)
		{
//			RELEASE_INTERFACE(m_pScrapRecordStream);
			m_pScrapRecordStream->CloseStream();
			delete m_pScrapRecordStream;
			m_pScrapRecordStream = NULL;
		}
	}

	return ReleaseRootStorage();
}

BOOL CPM_LargeFileDataSet::CStreamFileCtrl::GetRootStorage(CPMFileStorageCtrl *& pRootStorage)
{
	HRESULT hr;
	if(NULL == m_piFileRootStr)
	{
		// 创建复合文档
		////////////////////////////////////////////
		/// liwenlang [2011:9:23   14:08]
		/// 说明:
		/// 以前使用 立即修改 保存的方式 STGM_DIRECT，现修改为STGM_TRANSACTED 可带事务回滚机制
		m_piFileRootStr = new CPMFileStorageCtrl;	
		hr = m_piFileRootStr->TryOpenStorage(m_strFilePath);
		if(!SUCCEEDED(hr))
		{
			delete m_piFileRootStr;
			m_piFileRootStr = NULL;

			CString strMsg = _T("CPM_LargeFileDataSet::CStreamFileCtrl::GetScrapRecordStream 构造Root出错!");
			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
			return FALSE;
		}
	}

	pRootStorage = m_piFileRootStr;
	m_iFileRootCount++;

	return TRUE;
}

BOOL CPM_LargeFileDataSet::CStreamFileCtrl::SaveRootStream()//保存根节点 流，只有他的保存时有效的
{
	if(NULL == m_piFileRootStr)
		return FALSE;

// 	////////////////////////////////////////////
// 	/// liwenlang [2011:9:23   14:12]
// 	/// 说明:
// 	/// 在修改的 情况下 才要进行 保存数据操作
// 	HRESULT hr = m_piFileRootStr->Commit(STGC_DEFAULT);
// 	if (!SUCCEEDED(hr))
// 	{
// 		CString strMsg;
// 		strMsg.Format(_T("CPM_LargeFileDataSet::CStreamFileCtrl,文件%s 同步Stream失败!"),m_strFilePath);	
// 		return FALSE;
// 	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::CStreamFileCtrl::ReleaseRootStorage()
{
	if(m_iFileRootCount > 0)
	{
		m_iFileRootCount--;
		if(0 >= m_iFileRootCount)
		{
//			RELEASE_INTERFACE(m_piFileRootStr);
			m_piFileRootStr->CloseStorage();
			delete m_piFileRootStr;
			m_piFileRootStr = NULL;
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
///CPM_LargeFileDataSet::CFileRecordStreamCtrl
CPM_LargeFileDataSet::CFileRecordStreamCtrl::CFileRecordStreamCtrl(CPM_LargeFileDataSet::CStreamFileCtrl * pStreamFileCtrl)
{
	m_pStreamFileCtrl = pStreamFileCtrl;
//	m_pFileRecordStream = m_pStreamFileCtrl->GetFileRecordStream();
	m_pStreamFileCtrl->GetFileRecordStream(m_pFileRecordStream);
}
		
CPM_LargeFileDataSet::CFileRecordStreamCtrl::~CFileRecordStreamCtrl()
{
	m_pStreamFileCtrl->ReleaseFileRecordSteam();

	m_pStreamFileCtrl = NULL;
	m_pFileRecordStream = NULL;
}

CPMFileStreamCtrl * CPM_LargeFileDataSet::CFileRecordStreamCtrl::GetFileRecordStream()
{
	return m_pFileRecordStream;
}

//////////////////////////////////////////////////////////////////////////
///CPM_LargeFileDataSet
IMPLEMENT_DYNCREATE( CPM_LargeFileDataSet,CPM_StorageDataSet )

CPM_LargeFileDataSet::CPM_LargeFileDataSet()
{
	m_FileRecordSize = 0;
	m_FileRecordBufferSize = 0;

	m_pStreamFileCtrl = new CStreamFileCtrl;
//	m_pFileBuffers = new NDataModelDataSetBuffer;
//	m_pFileBuffers->SetOwner(this);

//	m_IsInCacheMode = FALSE;
	
// 	m_piFileRootStr = NULL;
// 	m_pFileRecordStream = NULL;

	/********************************************************************
	liwenlang
		时间:2010:10:26   9:35
		备注:给索引表添加默认Field
		FilePos(int)  RowLength(int) RowMode(int 保存状态0：文件位置，1：FilePos中的内容是内存地址，2：)
	*********************************************************************/
	AddField(CLargeFieldInfo::FileRowPos,ftInteger,0);
	AddField(CLargeFieldInfo::MemRowPos,ftInteger,0);
	AddField(CLargeFieldInfo::RowLength,ftInteger,0);
	AddField(CLargeFieldInfo::RowMode,ftInteger,0);

	m_pFileScrapDataSet = new CPM_StorageDataSet;
	m_pFileScrapDataSet->SetVersionID(1);
	m_pFileScrapDataSet->AddField(CLargeFieldInfo::ScrapFilePos,ftInteger,0);
	m_pFileScrapDataSet->AddField(CLargeFieldInfo::ScrapLength,ftInteger,0);

	m_LargeFieldInfo.FilePos_Index = -1;
	m_LargeFieldInfo.MemRowPos_Index = -1;
	m_LargeFieldInfo.RowLength_Index = -1;
	m_LargeFieldInfo.RowMode_Index = -1;

	m_LargeFieldInfo.ScrapFilePos_Index = -1;
	m_LargeFieldInfo.ScrapLength_Index = -1;
}

CPM_LargeFileDataSet::~CPM_LargeFileDataSet()
{
	Reset();

	if(NULL != m_pStreamFileCtrl)
	{
		delete m_pStreamFileCtrl;
		m_pStreamFileCtrl = NULL;
	}
	if(NULL != m_pFileScrapDataSet)
	{
		delete m_pFileScrapDataSet;
		m_pFileScrapDataSet = NULL;
	}
}

void CPM_LargeFileDataSet::Reset()
{
	/********************************************************************
	liwenlang
		时间:2010:10:28   14:01
		备注:检查缓存是否要释放
	*********************************************************************/
	const int filterBufferCount = CopyFilterToPrimaryBuffer();

	int iIndexRowCount = this->GetRowCount();
	for(int i = 1;i <= iIndexRowCount;i++)
	{
		NRecordID pTemRowBuffer = GetRecordID(i);
		int iRowMode = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.RowMode_Index,pTemRowBuffer);
		if(InMemory & iRowMode || InMemory_Mdf & iRowMode)
		{
			NRecordID pDataRowBuffer = (NRecordID)CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.MemRowPos_Index,pTemRowBuffer);
			FreeFileRecordBuffer(pDataRowBuffer);
		}
	}
	RemoveCopyedFilterToPrimaryBuffer(filterBufferCount);

	m_PrimaryBuffers->ResetBuffers();m_DeleteBuffers->ResetBuffers();
	m_FilterBuffers->ResetBuffers();


// 	RELEASE_INTERFACE(m_pFileRecordStream);
// 	RELEASE_INTERFACE(m_piFileRootStr);
	m_pStreamFileCtrl->DestroyAllStream();

	if(NULL != m_pFileScrapDataSet)
	{
		m_pFileScrapDataSet->Reset();
	}
}

BOOL CPM_LargeFileDataSet::AddFileField(CString sFieldName, FieldType fieldtype, int size)
{
	BOOL bresult;
	bresult = m_Filefields.CreateField(this,sFieldName, fieldtype ,size);
	if (bresult) 
	{
		InteralOpen();
	}
	return bresult;
}

void CPM_LargeFileDataSet::InteralOpen()
{
	CPM_StorageDataSet::InteralOpen();
	m_FileRecordSize = m_Filefields.GetRecordSize();
	m_FileRecordBufferSize = m_FileRecordSize + sizeof(RecInfo);
}

BOOL CPM_LargeFileDataSet::OpenLargeFile(CString strFilename)
{
	if(_T("") == m_TableName)
	{
//		AfxMessageBox(_T("没有设定索引表名字"));
		CString strMsg = _T("CPM_LargeFileDataSet::OpenLargeFile错误	没有设定索引表名字");
		CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
		return FALSE;
	}
	if(_T("") == m_strFileTableName)
	{
//		AfxMessageBox(_T("没有设定内容表名字"));
		CString strMsg = _T("CPM_LargeFileDataSet::OpenLargeFile错误	没有设定内容表名字");
		CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
		return FALSE;
	}

	Reset();

	m_pStreamFileCtrl->SetStreamFilePath(strFilename,this);

	BOOL bOpenResult = TRUE;
	_bstr_t sKeyName;
	///////////////////////////////////
	///liwenlang 
	///打开复合文档
	///[31/5/2010]
	//////////////////////////////////
// 	IStorage* piRootStr = NULL;
// 	sKeyName = strFilename;
// 	int iVersion = 0;
// 	
// 	// 创建复合文档
// 	HRESULT hr = StgOpenStorage(sKeyName,NULL, STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, NULL, NULL, &piRootStr);
// 	if(!SUCCEEDED(hr))
// 		hr = StgCreateDocfile(sKeyName, STGM_DIRECT|STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, NULL, &piRootStr);
// 	SysFreeString(sKeyName);
// 	m_piFileRootStr = piRootStr;
// 	m_strLargeFilePath = strFilename;

	///////////////////////////////////
	///liwenlang 
	///打开索引表
	///[31/5/2010]
	//////////////////////////////////
// 	sKeyName = m_TableName;
// 	IStream* pTableStream = NULL;
// 	hr = piRootStr->OpenStream(sKeyName, NULL, STGM_READWRITE|STGM_SHARE_EXCLUSIVE, 0, &pTableStream); 
// 	if (!SUCCEEDED(hr))
// 		hr = piRootStr->CreateStream(sKeyName, STGM_DIRECT|STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, 0, 0, &pTableStream);
// 	SysFreeString(sKeyName);
	CPMFileStreamCtrl * pTableStream = NULL;
	if(!m_pStreamFileCtrl->GetIndexRecordStream(pTableStream))
	{
		return FALSE;
	}

	/********************************************************************
	liwenlang
		时间:2010:10:26   9:14
		备注:获取所有信息大小
	*********************************************************************/
 	LONG lIndexDataSetSize = 0;
	HRESULT hr = pTableStream->ReadStream(&lIndexDataSetSize,sizeof(LONG));
	if(lIndexDataSetSize <= 0)
	{
		return TRUE;
	}
	short iCurVersion = 0;
	short iOpeningDataCode = 0;//0:A2W(Acsii转Unicode)  1:W2A(Unicode转Acsii)
	CPM_StorageDataSet * pTemFileFieldDataSet = NULL;

	if(0 < lIndexDataSetSize)
	{		
		/********************************************************************
		liwenlang
		时间:2010:10:26   9:14
		备注:获取索引表信息
		*********************************************************************/
		LPVOID pBuffer = new BYTE[lIndexDataSetSize];
		hr = pTableStream->ReadStream(pBuffer,lIndexDataSetSize);
		if(!SUCCEEDED(hr))
		{
	//		AfxMessageBox(_T("CPM_LargeFileDataSet索引文件出现严重错误!"));
			CString strMsg = _T("CPM_LargeFileDataSet::OpenLargeFile错误	CPM_LargeFileDataSet索引文件出现严重错误");
			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
			return FALSE;
		}
		if(!Buffer2DataSet(pBuffer,lIndexDataSetSize,TRUE))
		{
			return FALSE;
		}
		
		/********************************************************************
		liwenlang
		时间:2010:10:26   9:15
		备注:获取主版本号，子版本号。
		*********************************************************************/
		short iCurStructVersion = 0;
		CopyMemory(&iCurStructVersion,pBuffer,sizeof(short));
		CopyMemory(&iCurVersion,(BYTE*)pBuffer + sizeof(short),sizeof(short));
		ULONG lFieldInfoSize = 0;
		CopyMemory(&lFieldInfoSize,(BYTE*)pBuffer + sizeof(short) + sizeof(short),sizeof(LONG));
		short iCodeMode = 0;//编码方式
		CopyMemory(&iCodeMode,(BYTE*)pBuffer + sizeof(short) + sizeof(short) + sizeof(LONG) + lFieldInfoSize,sizeof(short));
		delete []pBuffer;
		
		SetOpeningDataSetDataCode(iCodeMode,iOpeningDataCode);
		/********************************************************************
		liwenlang
			时间:2011:1:4   10:20
			备注:判断版本，判断编码方式
		*********************************************************************/
		if(iOpeningDataCode > -1 || iCurVersion != m_iVersion)
		{
			LONG iFileFieldBufferSize = 0;
			hr = pTableStream->ReadStream(&iFileFieldBufferSize,sizeof(LONG));

			pBuffer = new BYTE[iFileFieldBufferSize + sizeof(LONG)];
			memset(pBuffer,0,iFileFieldBufferSize + sizeof(LONG));
			hr = pTableStream->ReadStream((BYTE*)pBuffer + sizeof(LONG),iFileFieldBufferSize);
			if(!SUCCEEDED(hr))
			{
//				AfxMessageBox(_T("CPM_LargeFileDataSet索引文件 FileField出现严重错误!"));
				CString strMsg = _T("CPM_LargeFileDataSet::OpenLargeFile错误	CPM_LargeFileDataSet索引文件 FileField出现严重错误");
				CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
				return FALSE;
			}
			CopyMemory(pBuffer,&iFileFieldBufferSize,sizeof(LONG));

			BuildTemDataSet(iOpeningDataCode,pBuffer,pTemFileFieldDataSet);
			delete [] pBuffer;
		}

	}

//	RELEASE_INTERFACE(pTableStream);
	m_pStreamFileCtrl->ReleaseIndexRecordSteam();

	/********************************************************************
	liwenlang
		时间:2010:10:27   11:06
		备注:打开碎片表
	*********************************************************************/
	CPMFileStreamCtrl * pScrapTableStream = NULL;
	if(m_pStreamFileCtrl->GetScrapRecordStream(pScrapTableStream))
	{
		/********************************************************************
		liwenlang
		时间:2010:10:26   9:14
		备注:获取碎片表信息大小
		*********************************************************************/
		LONG lScrapDataSetSize = 0;
		hr = pScrapTableStream->ReadStream(&lScrapDataSetSize,sizeof(LONG));
		if(!SUCCEEDED(hr))
		{
			CString strMsg = _T("CPM_LargeFileDataSet::OpenLargeFile错误	CPM_LargeFileDataSet碎片表信息读取失败!");
			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
			return FALSE;
		}

		/********************************************************************
		liwenlang
		时间:2010:10:26   9:14
		备注:获取碎片表信息
		*********************************************************************/
		if(lScrapDataSetSize > 0)
		{
			LPVOID pScrapBuffer = new BYTE[lScrapDataSetSize];
			hr = pScrapTableStream->ReadStream(pScrapBuffer,lScrapDataSetSize);
			if(!m_pFileScrapDataSet->Buffer2DataSet(pScrapBuffer,lScrapDataSetSize,TRUE))
			{
				return FALSE;
			}

			delete []pScrapBuffer;
		}
//		RELEASE_INTERFACE(pScrapTableStream);
		m_pStreamFileCtrl->ReleaseScrapRecordSteam();
	}

	/********************************************************************
	liwenlang
		时间:2010:10:26   9:23
		备注:打开内容主表
	*********************************************************************/
// 	sKeyName = m_strFileTableName;
// 	hr = piRootStr->OpenStream(sKeyName, NULL, STGM_READWRITE|STGM_SHARE_EXCLUSIVE, 0, &pTableStream); 
// 	if (!SUCCEEDED(hr))
// 		hr = piRootStr->CreateStream(sKeyName, STGM_DIRECT|STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, 0, 0, &pTableStream);
// 	SysFreeString(sKeyName);
// 	m_pFileRecordStream = pTableStream;

	/********************************************************************
	liwenlang
		时间:2011:1:4   10:44
		备注:文件内容升级
	*********************************************************************/
	BOOL bUpdataMode = TRUE;
	if(NULL != pTemFileFieldDataSet)
	{
		bUpdataMode = UpdataFileDataSet(pTemFileFieldDataSet,iOpeningDataCode);
	}
	delete pTemFileFieldDataSet;
	return bUpdataMode;
}

BOOL CPM_LargeFileDataSet::UpdataFileDataSet(CPM_StorageDataSet * pTemFileFieldDataSet,short iOpeningDataCode)
{
	CFileRecordStreamCtrl fileRecordCtrl(m_pStreamFileCtrl);
	CPMFileStreamCtrl * pFileRecordStream = fileRecordCtrl.GetFileRecordStream();
	/********************************************************************
	liwenlang
		时间:2011:1:4   10:47
		备注:将内容导入到 零时DataSet中
	*********************************************************************/
	NDataFields * pTemDataFields = pTemFileFieldDataSet->GetFieldsObj();
	const CPtrArray * pTemBlobFieldArr = pTemDataFields->GetBlobFieldArr();
	int iTemDataSetRowBufferSize = pTemFileFieldDataSet->GetRowBufferSize();

//	int iTemDataFieldInfoSize = pTemDataFields->GetFieldInfoSize();
	////////////////////////////////////////////
	/// liwenlang [2013:1:17   9:09]
	/// 说明:
	/// 在x64下 指针大小是8个 字节，不是 4个，所以这里将长度统一为4个
	int iTemDataFieldInfoSize = sizeof(BYTE) + sizeof(long);

	/********************************************************************
	liwenlang
		时间:2011:1:4   13:24
		备注:将TCHAR的size置回 按照DataSet的状态控制
	*********************************************************************/
	int i = 1;
	//for (i = 0;i < pTemDataFields->GetCount();i++)
	//{
	//	NDataField * pTemField = pTemDataFields->GetField(i);
	//	if(ftString == pTemField->GetDataType())
	//	{//0:A2W(Acsii转Unicode)  1:W2A(Unicode转Acsii)
	//		int iTemSize = pTemField->GetSize();
	//		if (0 == iOpeningDataCode)
	//			iTemSize /= sizeof(TCHAR);
	//		else if(1 == iOpeningDataCode)
	//			iTemSize *= sizeof(TCHAR);
	//		pTemField->SetSize(iTemSize);
	//	}
	//}
	pTemDataFields->RefreshFieldOffset();

	for (i = 1;i <= GetRowCount();i++)
	{
		int FilePos = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.FilePos_Index,i);
		int iLength = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.RowLength_Index,i);

		if(iLength <= 0)
			continue;

		LPVOID pTemRowBuffer = new BYTE[iLength];//在原编码版本中 的大小
		LPVOID pTemCurCodeRowBuffer = new BYTE[iTemDataSetRowBufferSize];
		memset(pTemCurCodeRowBuffer,0,iTemDataSetRowBufferSize);

		LARGE_INTEGER iStreamPos;
		iStreamPos.QuadPart = FilePos;
		pFileRecordStream->SeekStream(iStreamPos,STREAM_SEEK_SET,NULL);
 
		HRESULT hr = pFileRecordStream->ReadStream(pTemRowBuffer,iLength);
		if(!SUCCEEDED(hr))
		{
			delete [](BYTE*)pTemCurCodeRowBuffer;
			CString strMsg;
			strMsg.Format(_T("CPM_LargeFileDataSet::UpdataFileDataSet 读取RecordStream发生错误!读取数据与读到数据不一致！"));
			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
//			AfxMessageBox(strMsg);
			return FALSE;
		}

		LPVOID pCurCellRowBuffer = pTemRowBuffer;//注意这个变量里面的 是 原编码版本的 流位置
		int j = 0;
		for (j = 0;j < pTemDataFields->GetCount();j++)
		{
			NDataField * pTemDataField = pTemDataFields->GetField(j);
			int iFormFieldOffset = pTemFileFieldDataSet->GetFieldOffset(pTemDataField);

			switch(pTemDataField->GetDataType())
			{
			case ftInteger:
				{
					int iValue = 0;
					CopyMemory(&iValue,(BYTE*)pCurCellRowBuffer,sizeof(int));	
					CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset,&iValue,sizeof(int));

					pCurCellRowBuffer = (BYTE*)pCurCellRowBuffer + pTemDataField->GetSize() + iTemDataFieldInfoSize;
				}
				break;
			case ftFloat:
				{
					double dValue = 0.0;
					CopyMemory(&dValue,(BYTE*)pCurCellRowBuffer,sizeof(double));
					CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset,&dValue,sizeof(double));

					pCurCellRowBuffer = (BYTE*)pCurCellRowBuffer + pTemDataField->GetSize() + iTemDataFieldInfoSize;
				}
				break;
			case ftString:
				{
					switch(iOpeningDataCode)
					{//0:A2W(Acsii转Unicode)  1:W2A(Unicode转Acsii)
					case 0://
						{
							char * pValue = (char*)pCurCellRowBuffer;
							WCHAR * pWValue = CPub::PM_A2W(pValue,(int)strlen(pValue));

							CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset,pWValue,sizeof(WCHAR) * wcslen(pWValue));
							delete [] pWValue;

							pCurCellRowBuffer = (BYTE*)pCurCellRowBuffer + pTemDataField->GetSize() / sizeof(WCHAR) + iTemDataFieldInfoSize;
						}
						break;
					case 1:
						{
							WCHAR * pWValue = (WCHAR*)pCurCellRowBuffer;
							CHAR * pValue = CPub::PM_W2A(pWValue,(int)wcslen(pWValue));

							CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset,pValue,sizeof(CHAR) * strlen(pValue));
							delete [] pValue;

							pCurCellRowBuffer = (BYTE*)pCurCellRowBuffer + pTemDataField->GetSize() * sizeof(WCHAR) + iTemDataFieldInfoSize;
						}
						break;
					default:
						{
							TCHAR * pTValue = (TCHAR*)pCurCellRowBuffer;

							CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset,pTValue,sizeof(TCHAR) * _tcslen(pTValue));

							pCurCellRowBuffer = (BYTE*)pCurCellRowBuffer + pTemDataField->GetSize() + iTemDataFieldInfoSize;
							break;
						}
					}
				}
				break;
			case ftBlob:
				{
					LPVOID pFileBlob = NULL;
					long iBlobSize = 0;	
					//CopyMemory(&iBlobSize,(char*)pCurCellRowBuffer + sizeof(void*),sizeof(long));
					//CopyMemory(&pFileBlob,(char*)pCurCellRowBuffer,sizeof(void*));
					////////////////////////////////////////////
					/// liwenlang [2013:1:17   9:09]
					/// 说明:
					/// 在x64下 指针大小是8个 字节，不是 4个，所以这里将长度统一为4个
					CopyMemory(&iBlobSize,(char*)pCurCellRowBuffer + sizeof(long),sizeof(long));
					CopyMemory(&pFileBlob,(char*)pCurCellRowBuffer,sizeof(long));

					if(iBlobSize < 0 || iBlobSize >= MaxFieldBufferSize)
					{
						delete [](BYTE*)pTemCurCodeRowBuffer;
						CString strMsg;
						strMsg.Format(_T("CPM_LargeFileDataSet::UpdataFileDataSet iBlobSize %d 大小过大!"),iBlobSize);
						//	AfxMessageBox(strMsg);
						CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
						return FALSE;
					}
					LPVOID pBlobInfo = NULL;
					iStreamPos.QuadPart = (int)pFileBlob;
					pFileRecordStream->SeekStream(iStreamPos,STREAM_SEEK_SET,NULL);

					pBlobInfo = new BYTE[iBlobSize];
					hr = pFileRecordStream->ReadStream(pBlobInfo,iBlobSize);
					if(!SUCCEEDED(hr))
					{
						delete [](BYTE*)pTemCurCodeRowBuffer;
						CString strMsg;
						strMsg.Format(_T("CPM_LargeFileDataSet::UpdataFileDataSet pFileRecordStream->ReadStream	发生错误!读取数据与读到数据不一致!"));
					//	AfxMessageBox(strMsg);
						CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
						return FALSE;
					}

					if(!pTemDataField->IsUpdateAsBlobFiled())
					{
						switch(iOpeningDataCode)
						{//0:A2W(Acsii转Unicode)  1:W2A(Unicode转Acsii)
						case 0://
							{
								char * pTemValue = new char[iBlobSize + 1];
								memset(pTemValue,0,iBlobSize + 1);
								memcpy(pTemValue,pBlobInfo,iBlobSize);
								WCHAR * pTemWValue = CPub::PM_A2W(pTemValue,iBlobSize);
								delete [] pTemValue;

								////////////////////////////////////////////
								/// liwenlang [2013:1:17   10:30]
								/// 说明:
								/// 这里是 给主内存Buffer 置值，所以还是使用LPVOID
								int iWValueSize = (int)wcslen(pTemWValue) * sizeof(WCHAR);
								CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset,&pTemWValue,sizeof(LPVOID));
								CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset + sizeof(void*),&iWValueSize,sizeof(long));
							} 
							break;
						case 1:
							{
								WCHAR * pTemWValue = new WCHAR[iBlobSize];
								memset(pTemWValue,0,iBlobSize * sizeof(WCHAR));
								memcpy(pTemWValue,pBlobInfo,iBlobSize);

								CHAR * pTemAValue = CPub::PM_W2A(pTemWValue,iBlobSize);
								delete []pTemWValue;

								////////////////////////////////////////////
								/// liwenlang [2013:1:17   10:30]
								/// 说明:
								/// 这里是 给主内存Buffer 置值，所以还是使用LPVOID
								int iAValueSize = (int)strlen(pTemAValue);
								CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset,&pTemAValue,sizeof(LPVOID));								
								CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset + sizeof(void*),&iAValueSize,sizeof(long));
							}
							break;
						default:
							{
								////////////////////////////////////////////
								/// liwenlang [2013:1:17   10:30]
								/// 说明:
								/// 这里是 给主内存Buffer 置值，所以还是使用LPVOID
								CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset,&pBlobInfo,sizeof(LPVOID));								
								CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset + sizeof(void*),&iBlobSize,sizeof(long));
								pBlobInfo = NULL;
							}
							break;
						}
					}
					else
					{
						////////////////////////////////////////////
						/// liwenlang [2013:1:17   10:30]
						/// 说明:
						/// 这里是 给主内存Buffer 置值，所以还是使用LPVOID
						CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset,&pBlobInfo,sizeof(LPVOID));								
						CopyMemory((BYTE*)pTemCurCodeRowBuffer + iFormFieldOffset + sizeof(void*),&iBlobSize,sizeof(long));
						pBlobInfo = NULL;
					}

					if(NULL != pBlobInfo)
						delete [](BYTE*)pBlobInfo;

					//pCurCellRowBuffer = (BYTE*)pCurCellRowBuffer + pTemDataField->GetSize() + iTemDataFieldInfoSize;
					////////////////////////////////////////////
					/// liwenlang [2013:1:17   10:30]
					/// 说明:
					/// 这里是 BlobField的 size，是LPVOID + long 所有修改。
					pCurCellRowBuffer = (BYTE*)pCurCellRowBuffer + sizeof(LONG) + sizeof(LONG) + iTemDataFieldInfoSize;
				}
				break;
			default:
				break;
			}
		}

		LPVOID pBuffer = new BYTE[m_FileRecordBufferSize];
		memset(pBuffer,0,m_FileRecordBufferSize);
		int iTemValue = (int)pBuffer;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.MemRowPos_Index,i,iTemValue);
		iTemValue = InMemory;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,i,iTemValue);

		SetRowNewInternal(GetRecordID(i),TRUE);

		/********************************************************************
		liwenlang
			时间:2011:1:4   11:19
			备注:进行数据交换
		*********************************************************************/
		for (j = 0;j < pTemDataFields->GetCount();j++)
		{
			NDataField * pTemField = pTemDataFields->GetField(j);
			UpdataFileDataSet_Field(iOpeningDataCode,i,pTemCurCodeRowBuffer,pTemFileFieldDataSet,pTemField);
		}
		
		delete [](BYTE*)pTemRowBuffer;
		pTemFileFieldDataSet->FreeRecordBuffer(pTemCurCodeRowBuffer);
	}

	/********************************************************************
	liwenlang
		时间:2011:1:4   14:17
		备注:将碎片合并
	*********************************************************************/
	int iScorpBegin = 0;
	int iScorpEnd = 0;
	int iScorpEndRow = m_pFileScrapDataSet->GetRowCount();
	if(iScorpEndRow > 0)
	{
		int iEndRowFilePos = m_pFileScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,m_pFileScrapDataSet->GetRowCount());
		int iEndRowFileLength = m_pFileScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapLength_Index,m_pFileScrapDataSet->GetRowCount());
		iScorpEnd = iEndRowFilePos + iEndRowFileLength;

		m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,1,iScorpBegin);
		m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,1,iScorpEnd);

		for (int j = 2;j <= m_pFileScrapDataSet->GetRowCount();)
		{
			m_pFileScrapDataSet->DeleteRow(j);
		}
	}

//	ULARGE_INTEGER newSize;
 //	newSize.QuadPart = iScorpEnd;
//	pFileRecordStream->SetSize(newSize);
	return TRUE;
}

BOOL CPM_LargeFileDataSet::UpdataFileDataSet_Field(short iOpeningDataCode,int iRow,LPVOID pFormBuffer,CPM_StorageDataSet * pTemFileDataSet,NDataField * pDataField)
{
	int iFormFieldOffset = pTemFileDataSet->GetFieldOffset(pDataField);
	CString strFieldName = pDataField->GetFieldName();

	switch(pDataField->GetDataType())
	{
	case ftInteger:
		{
			int iValue = 0;
			CopyMemory(&iValue,(BYTE*)pFormBuffer + iFormFieldOffset,sizeof(int));	
			this->SetAsInteger(pDataField->GetFieldName(),iRow,iValue);
		}
		break;
	case ftFloat:
		{
			double dValue = 0.0;
			CopyMemory(&dValue,(BYTE*)pFormBuffer + iFormFieldOffset,sizeof(double));
			this->SetAsFloat(pDataField->GetFieldName(),iRow,dValue);
		}
		break;
	case ftString:
		{
			TCHAR * pTValue = (TCHAR*)((BYTE*)pFormBuffer + iFormFieldOffset);
			CString strTemValue(pTValue);
			this->SetAsString(pDataField->GetFieldName(),iRow,strTemValue);
		}
		break;
	case ftBlob:
		{
			LPVOID pBlob = NULL;
			long iBlobSize = 0;	
			CopyMemory(&iBlobSize,(char*)pFormBuffer + iFormFieldOffset + sizeof(void*),sizeof(long));
			CopyMemory(&pBlob,(char*)pFormBuffer + iFormFieldOffset,sizeof(void*));
			
			this->SetBlob(pDataField->GetFieldName(),iRow,pBlob,iBlobSize);
		}
		break;
	default:
	    break;
	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::SaveMemory2DataFile()
{
	//	UIBase::CTiming timing(_T("CPM_LargeFileDataSet::SaveMemory2DataFile"),TRUE);
	{
		HRESULT hr;
		CFileRecordStreamCtrl fileRecordCtrl(m_pStreamFileCtrl);
		CPMFileStreamCtrl * pFileRecordStream = fileRecordCtrl.GetFileRecordStream();

		////////////////////////////////////////////
		/// liwenlang [2011:9:26   14:27]
		/// 说明:
		/// 目的在于多加一个引用计数 便于保存
		CPMFileStorageCtrl * pRootFileStorage = NULL;
		m_pStreamFileCtrl->GetRootStorage(pRootFileStorage);
		
		//防止filter中的数据没有被更新2008.5.8
		const int filterBufferCount = CopyFilterToPrimaryBuffer();
		
		const CPtrArray * pFileBlobFieldArr = m_Filefields.GetBlobFieldArr();
		int iFileBlobFieldCount = (int)pFileBlobFieldArr->GetSize();
		LPVOID * pFileBlobInfoArr = new LPVOID[iFileBlobFieldCount];
		LONG * pFileBlobSizeArr = new LONG[iFileBlobFieldCount];
		
		NDataModelDataSet * pScarpDataSet_Share = m_pFileScrapDataSet->GetShardData(TRUE,FALSE);
		pScarpDataSet_Share->ResetSortField();
		pScarpDataSet_Share->AddSortField(m_LargeFieldInfo.ScrapLength_Index,TRUE);
		pScarpDataSet_Share->Sort();
		
		int iIndexRowCount = GetRowCount();
		for(int i = 1;i <= iIndexRowCount;i++)
		{
			NRecordID pIndexRecordID = GetRecordID(i);
			/********************************************************************
			liwenlang
			时间:2010:10:27   14:23
			备注:遍历获取要保存的记录
			*********************************************************************/
			int iTemRowMode = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.RowMode_Index,pIndexRecordID);
			if(!(InMemory_Mdf & iTemRowMode))
			{
				if(InMemory & iTemRowMode)
				{
					int iTemValue = InFile;
					CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,pIndexRecordID,iTemValue);
					
					NRecordID pDataBuffer = (NRecordID)CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.MemRowPos_Index,pIndexRecordID);
					FreeFileRecordBuffer(pDataBuffer);
					
					iTemValue = 0;
					CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.MemRowPos_Index,pIndexRecordID,iTemValue);
					
				}
				continue;
			}
			
			NRecordID pDataBuffer = (NRecordID)CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.MemRowPos_Index,pIndexRecordID);
			int iTemFilePos = -1;
			int iTemBufferLength = -1;
			
			BOOL bIsNewRow = GetRowNewInternal(pIndexRecordID);
			SetRowNewInternal(pIndexRecordID,FALSE);
			if(FALSE == bIsNewRow)
			{
				iTemFilePos = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.FilePos_Index,pIndexRecordID);
				iTemBufferLength = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.RowLength_Index,pIndexRecordID);
			}
			
			/********************************************************************
			liwenlang
			时间:2010:10:27   14:24
			备注:获取记录长度
			*********************************************************************/
			int iRowFullBufferLength = m_FileRecordBufferSize;
			int j = 0;
			for (j = 0;j < iFileBlobFieldCount;j++)
			{
				NDataField * pTemDataField = (NDataField*)pFileBlobFieldArr->GetAt(j);
				LPVOID pCurBlobPtInfo = GetFieldDataPT(pTemDataField,pDataBuffer);
				
				LPVOID pCurBlob = NULL;
				LONG lcurBlobSize = pTemDataField->GetBlob(pCurBlobPtInfo,pCurBlob);
				
				pFileBlobInfoArr[j] = pCurBlob;
				pFileBlobSizeArr[j] = lcurBlobSize;
				
				iRowFullBufferLength += lcurBlobSize;
			}
			
			/********************************************************************
			liwenlang
			时间:2010:10:27   14:24
			备注:放不下 查找一个可以存放的碎片位置，没有在最后添加，并将原先空间置为碎片
			*********************************************************************/
			
			int iNewFilePos = -1;
			
			if(FALSE == bIsNewRow)
			{
				if(iTemBufferLength >= iRowFullBufferLength)
				{
					iNewFilePos = iTemFilePos;
				}
				else
				{
					int iTemLength = 0;
					if(!GetScrapFileInfo(pScarpDataSet_Share,iRowFullBufferLength,iNewFilePos,iTemLength))
					{
						LARGE_INTEGER preMove;
						preMove.QuadPart = 0;
						ULARGE_INTEGER curStreamPos;
						pFileRecordStream->SeekStream(preMove,STREAM_SEEK_END,&curStreamPos);
						
						iNewFilePos = (int)curStreamPos.QuadPart;
					}
					
					AddScrapFileInfo(iTemFilePos,iTemBufferLength);
				}
			}
			else
			{//查找新的 存放位置
				int iTemLength = 0;
				if(!GetScrapFileInfo(pScarpDataSet_Share,iRowFullBufferLength,iNewFilePos,iTemLength))
				{
					LARGE_INTEGER preMove;
					preMove.QuadPart = 0;
					ULARGE_INTEGER curStreamPos;
					pFileRecordStream->SeekStream(preMove,STREAM_SEEK_END,&curStreamPos);
					
					iNewFilePos = (int)curStreamPos.QuadPart;
				}
			}
			
			ULONG lWriteSize = 0;
			if(iFileBlobFieldCount > 0)
			{//先写入Blob字段
				int iCurBlobPos = iNewFilePos + m_FileRecordBufferSize;
				LARGE_INTEGER preMove;
				preMove.QuadPart = iCurBlobPos;
				pFileRecordStream->SeekStream(preMove,STREAM_SEEK_SET,NULL);
				for(j = 0;j < iFileBlobFieldCount;j++)
				{
					LONG lBlobSize = pFileBlobSizeArr[j];
					hr = pFileRecordStream->WriteStream(pFileBlobInfoArr[j],lBlobSize);
					if(!SUCCEEDED(hr))
					{
						delete []pFileBlobSizeArr;

						CString strMsg;
						strMsg.Format(_T("CPM_LargeFileDataSet::SaveMemory2DataFile 写入Blob失败!"));
						CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
						return FALSE;
					}
					
					NRecordID pCurTemBlob = GetFieldDataPT((NDataField*)pFileBlobFieldArr->GetAt(j),pDataBuffer);
					//				CPM_StorageDataSet::SetBlob((NDataField*)pFileBlobFieldArr->GetAt(j),pDataBuffer,(LPVOID)iCurBlobPos,lBlobSize);
					if(pCurTemBlob)
					{
						CopyMemory(pCurTemBlob,&iCurBlobPos,sizeof(void*));
						CopyMemory((char*)pCurTemBlob+sizeof(void*),&lBlobSize,sizeof(long));
					}
					iCurBlobPos += lBlobSize;
				}
			}
			
			LARGE_INTEGER preMove;
			preMove.QuadPart = iNewFilePos;
			pFileRecordStream->SeekStream(preMove,STREAM_SEEK_SET,NULL);
			hr = pFileRecordStream->WriteStream(pDataBuffer,m_FileRecordBufferSize);
			if(!SUCCEEDED(hr))
			{
				delete []pFileBlobSizeArr;
				delete []pFileBlobInfoArr;

				CString strMsg;
				strMsg.Format(_T("CPM_LargeFileDataSet::SaveMemory2DataFile 写入Blob失败!"));
				CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
				return FALSE;
			}
			
			if(iFileBlobFieldCount > 0)
			{//将Blob指针写回，为维护内存方便
				for(j = 0;j < iFileBlobFieldCount;j++)
				{
					LONG lBlobSize = pFileBlobSizeArr[j];
					
					NRecordID pCurTemBlob = GetFieldDataPT((NDataField*)pFileBlobFieldArr->GetAt(j),pDataBuffer);
					if(pCurTemBlob)
						CopyMemory(pCurTemBlob,&pFileBlobInfoArr[j],sizeof(void*));
				}
			}
			FreeFileRecordBuffer(pDataBuffer);
			
			/********************************************************************
			liwenlang
			时间:2010:10:27   16:03
			备注:修改索引表
			*********************************************************************/
			CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.FilePos_Index,pIndexRecordID,iNewFilePos);
			CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowLength_Index,pIndexRecordID,iRowFullBufferLength);
			int iTemValue = InFile;
			CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,pIndexRecordID,iTemValue);
			iTemValue = 0;
			CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.MemRowPos_Index,pIndexRecordID,iTemValue);	
		}
	
		delete pScarpDataSet_Share;
		delete []pFileBlobSizeArr;
		delete []pFileBlobInfoArr;
		RemoveCopyedFilterToPrimaryBuffer(filterBufferCount);
		
		/********************************************************************
		liwenlang
		时间:2010:10:28   11:15
		备注:保存索引表
		*********************************************************************/
		if(TRUE == m_IsModified)
		{
			LPVOID pIndexBuffer = NULL;
			ULONG lIndexBufferSize = 0;
			if(!DataSet2Buffer(pIndexBuffer,lIndexBufferSize))
				return FALSE;
			
			LPVOID pFileFieldInfo = NULL;
			int iFileFieldSize = 0;
			BuildFieldInfo(&m_Filefields,pFileFieldInfo,iFileFieldSize);
			
			//		_bstr_t sKeyName = m_TableName;
			
			CPMFileStreamCtrl* pTableStream = NULL;
			if(m_pStreamFileCtrl->GetIndexRecordStream(pTableStream))
			{
				pTableStream->WriteStream(&lIndexBufferSize,sizeof(ULONG));
				pTableStream->WriteStream(pIndexBuffer,lIndexBufferSize);
				//			pTableStream->Write(&iFileFieldSize,sizeof(ULONG),&lWriteSize);
				pTableStream->WriteStream(pFileFieldInfo,iFileFieldSize);
				
				//			RELEASE_INTERFACE(pTableStream);
				m_pStreamFileCtrl->ReleaseIndexRecordSteam();
			}
			else
			{
				AfxMessageBox(_T("保存索引表失败!"));
				return FALSE;
			}
			delete [](BYTE*)pIndexBuffer;
			delete [](BYTE*)pFileFieldInfo;
			
			/********************************************************************
			liwenlang
			时间:2010:10:28   11:16
			备注:保存碎片记录表
			*********************************************************************/
			
			LPVOID pSrapeBuffer = NULL;
			ULONG lSrapeBufferSize = 0;
			m_pFileScrapDataSet->DataSet2Buffer(pSrapeBuffer,lSrapeBufferSize);
			
			pTableStream = NULL;
			if(m_pStreamFileCtrl->GetScrapRecordStream(pTableStream))
			{
				pTableStream->WriteStream(&lSrapeBufferSize,sizeof(ULONG));
				pTableStream->WriteStream(pSrapeBuffer,lSrapeBufferSize);
				
				//			RELEASE_INTERFACE(pTableStream);
				m_pStreamFileCtrl->ReleaseScrapRecordSteam();
			}
			delete [](BYTE*)pSrapeBuffer;
		}
	}
	
	m_pStreamFileCtrl->SaveRootStream();//最后保存修改。
	m_pStreamFileCtrl->DestroyAllStream();
	return TRUE;
}

//BOOL CPM_LargeFileDataSet::Save2File()
//{
//	SaveMemory2DataFile();
//
//	return FALSE;
//}

void * CPM_LargeFileDataSet::AlloFileRecordBuffer()
{
	if (m_FileRecordBufferSize == 0) 
		return NULL;
	void * buff = new BYTE [m_FileRecordBufferSize];
	if (buff == NULL)
	{
		MessageBox(NULL,_T("内存申请失败!"),_T("警告"),MB_OK);
	}
	ZeroMemory(buff,m_FileRecordBufferSize);
	return buff;
}

int CPM_LargeFileDataSet::InsertRow(int iberforrow)
{
	int iNewRow = CPM_StorageDataSet::InsertRow(iberforrow);
	
	AddFileRecord2Mem(iNewRow);
	return iNewRow;
}

LPVOID CPM_LargeFileDataSet::AddFileRecord2Mem(int iRow,eBufferFlag BufferFlag)
{
	/********************************************************************
	liwenlang
		时间:2010:10:26   9:28
		备注:给主表添加数据
	*********************************************************************/
	int iFileberforrow = iRow - 1;//转化为index
	void * newFilebuff = AlloFileRecordBuffer();
//	m_pFileBuffers->InsertBuff(newFilebuff,iFileberforrow);

	/********************************************************************
	liwenlang
		时间:2010:10:26   9:59
		备注:在索引表中修改
	*********************************************************************/
	NRecordID pIndexRecord = GetRecordID(iRow, BufferFlag);

	int iTemValue = (int)newFilebuff;
	CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.MemRowPos_Index,pIndexRecord,iTemValue);
	iTemValue = InMemory_Mdf;
	CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,pIndexRecord,iTemValue);
	return newFilebuff;
}

void CPM_LargeFileDataSet::RelCalFieldIndex()
{
	m_LargeFieldInfo.FilePos_Index = m_fields.GetFieldIndex(CLargeFieldInfo::FileRowPos);
	m_LargeFieldInfo.MemRowPos_Index = m_fields.GetFieldIndex(CLargeFieldInfo::MemRowPos);
	m_LargeFieldInfo.RowLength_Index = m_fields.GetFieldIndex(CLargeFieldInfo::RowLength);
	m_LargeFieldInfo.RowMode_Index = m_fields.GetFieldIndex(CLargeFieldInfo::RowMode);

	m_LargeFieldInfo.ScrapFilePos_Index = m_pFileScrapDataSet->GetFieldIndex(CLargeFieldInfo::ScrapFilePos);
	m_LargeFieldInfo.ScrapLength_Index = m_pFileScrapDataSet->GetFieldIndex(CLargeFieldInfo::ScrapLength);

	/********************************************************************
	liwenlang
		时间:2010:10:28   9:57
		备注:重置Field标志
	*********************************************************************/
	int i = 0;
	for (i = 0;i < m_fields.GetCount();i++)
	{
		m_fields.GetField(i)->SetSpecStatus(IndexField);
	}
	for (i = 0;i < m_Filefields.GetCount();i++)
	{
		m_Filefields.GetField(i)->SetSpecStatus(DataField);
	}
}

// void CPM_LargeFileDataSet::EndCacheMode()
// {
// 	m_IsInCacheMode = FALSE;
// 
// 	/********************************************************************
// 	liwenlang
// 		时间:2010:10:26   13:16
// 		备注:将内存数据进行保存
// 	*********************************************************************/
// }

BOOL CPM_LargeFileDataSet::SetAsString(int Column, int row ,const CString& val)
{
	//if(Column <= m_LargeFieldInfo.RowMode_Index)
	//	return FALSE;

	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,row,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	CString strTemValue = pField->GetAsString(pCellBuffer);
	if(strTemValue.GetLength() == val.GetLength() && strTemValue == val)
		return TRUE;

	pField->SetAsString(val,pCellBuffer);
	if(DataField == iRecordMode)
	{
		int iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,row,iTemValue);
	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::SetAsFloat(int Column, int row ,const double& val)
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,row,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	double dTemValue = pField->GetAsFloat(pCellBuffer);
	if(true == CPub::CompareDouble(dTemValue,val))
		return TRUE;

	pField->SetAsFloat(val,pCellBuffer);
	if(DataField == iRecordMode)
	{
		int iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,row,iTemValue);
	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::SetAsInteger(int Column, int row ,const int& val)
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,row,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	int iTemValue = pField->GetAsInteger(pCellBuffer);
	if(iTemValue == val)
		return TRUE;

	pField->SetAsInteger(val,pCellBuffer);
	if(DataField == iRecordMode)
	{
		/*int*/ iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,row,iTemValue);
	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::SetAsVariant(int Column, int row ,const VARIANT& val)
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,row,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	pField->SetAsVariant(val,pCellBuffer);
	if(DataField == iRecordMode)
	{
		int iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,row,iTemValue);
	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::SetAsDateTime(int Column, int row ,const COleDateTime& val)
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,row,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	pField->SetAsDateTime(val,pCellBuffer);
	if(DataField == iRecordMode)
	{
		int iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,row,iTemValue);
	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::SetAsBool(int Column, int row ,const BOOL & val)
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,row,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	BOOL bTemValue = pField->GetAsBoolean(pCellBuffer);
	if(bTemValue == val)
		return TRUE;

	pField->SetAsBoolean(val,pCellBuffer);
	if(DataField == iRecordMode)
	{
		int iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,row,iTemValue);
	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::SetBlob(int Column, int row ,void * ppBolb,long iSize)
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,row,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	pField->SetBlob(pCellBuffer,ppBolb,iSize);
	if(DataField == iRecordMode)
	{
		int iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,row,iTemValue);
	}
	return TRUE;
}

CString CPM_LargeFileDataSet::GetAsString(int Column,int row, eBufferFlag BufferFlag, BOOL originalvalue) const
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,row,pBuffer);
	if(NullField == iRecordMode)
		return _T("");

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	return pField->GetAsString(pCellBuffer);
}

double CPM_LargeFileDataSet::GetAsFloat(int Column,int row, eBufferFlag BufferFlag, BOOL originalvalue) const
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,row,pBuffer);
	if(NullField == iRecordMode)
		return 0.0;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	return pField->GetAsFloat(pCellBuffer);
}

int CPM_LargeFileDataSet::GetAsInteger(int Column,int row, eBufferFlag BufferFlag, BOOL originalvalue) const
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,row,pBuffer);
	if(NullField == iRecordMode)
		return 0;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	return pField->GetAsInteger(pCellBuffer);
}

VARIANT CPM_LargeFileDataSet::GetAsVariant(int Column,int row, eBufferFlag BufferFlag, BOOL originalvalue) const
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,row,pBuffer);
	if(NullField != iRecordMode)
	{
		VARIANT var;
		var.bVal = 0;
		return var;
	}

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	return pField->GetAsVariant(pCellBuffer);
}

COleDateTime CPM_LargeFileDataSet::GetAsDateTime(int Col,int row, eBufferFlag BufferFlag, BOOL originalvalue) const
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Col,row,pBuffer);
	if(NullField == iRecordMode)
	{
		COleDateTime oleVar;
		return oleVar;
	}

	NDataField * pField = GetField(Col);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	return pField->GetAsDateTime(pCellBuffer);
}

BOOL CPM_LargeFileDataSet::GetAsBool(int Col,int row, eBufferFlag BufferFlag, BOOL originalvalue) const
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Col,row,pBuffer);
	if(NullField == iRecordMode)
		return 0;

	NDataField * pField = GetField(Col);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	return pField->GetAsBoolean(pCellBuffer);
}

long CPM_LargeFileDataSet::GetBlob(int iColumn, int row ,void *& ppBolb,eBufferFlag BufferFlag, BOOL originalvalue) const
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(iColumn,row,pBuffer);
	if(NullField == iRecordMode)
		return 0;

	NDataField * pField = GetField(iColumn);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	return pField->GetBlob(pCellBuffer,ppBolb);
}

CString CPM_LargeFileDataSet::GetAsString(const CString & Column,int row, eBufferFlag BufferFlag, BOOL originalvalue) const
{
	return GetAsString(GetFieldIndex(Column),row,BufferFlag,originalvalue);
}

double CPM_LargeFileDataSet::GetAsFloat(const CString & Column,int row, eBufferFlag BufferFlag, BOOL originalvalue) const
{
	return GetAsFloat(GetFieldIndex(Column),row,BufferFlag,originalvalue);
}

int CPM_LargeFileDataSet::GetAsInteger(const CString & Column,int row, eBufferFlag BufferFlag, BOOL originalvalue) const
{
	return GetAsInteger(GetFieldIndex(Column),row,BufferFlag,originalvalue);
}

VARIANT CPM_LargeFileDataSet::GetAsVariant(const CString & Column,int row, eBufferFlag BufferFlag, BOOL originalvalue) const
{
	return GetAsVariant(GetFieldIndex(Column),row,BufferFlag,originalvalue);

}

COleDateTime CPM_LargeFileDataSet::GetAsDateTime(const CString & Column,int row, eBufferFlag BufferFlag, BOOL originalvalue) const
{
	return GetAsDateTime(GetFieldIndex(Column),row,BufferFlag,originalvalue);
}

BOOL CPM_LargeFileDataSet::GetAsBool(const CString & Column,int row, eBufferFlag BufferFlag, BOOL originalvalue) const
{
	return GetAsBool(GetFieldIndex(Column),row,BufferFlag,originalvalue);
}

BOOL CPM_LargeFileDataSet::SetAsString(const CString & Column, int row, const CString& val)
{
	return SetAsString(GetFieldIndex(Column),row,val);
}

BOOL CPM_LargeFileDataSet::SetAsFloat(const CString & Column, int row, const double& val)
{
	return SetAsFloat(GetFieldIndex(Column),row,val);
}

BOOL CPM_LargeFileDataSet::SetAsInteger(const CString & Column, int row,const int& val)
{
	return SetAsInteger(GetFieldIndex(Column),row,val);
}

BOOL CPM_LargeFileDataSet::SetAsVariant(const CString & Column, int row,const VARIANT& val)
{
	return SetAsVariant(GetFieldIndex(Column),row,val);
}

BOOL CPM_LargeFileDataSet::SetAsDateTime(const CString & Column, int row,const COleDateTime& val)
{
	return SetAsDateTime(GetFieldIndex(Column),row,val);
}

BOOL CPM_LargeFileDataSet::SetAsBool(const CString & Column, int row,const BOOL & val)
{
	return SetAsBool(GetFieldIndex(Column),row,val);
}
		
long CPM_LargeFileDataSet::GetBlob(const CString & Column, int row ,void *& ppBolb,eBufferFlag BufferFlag, BOOL originalvalue) const
{
	return GetBlob(GetFieldIndex(Column),row,ppBolb,BufferFlag,originalvalue);
}

BOOL CPM_LargeFileDataSet::SetBlob(const CString & Column, int row ,void * ppBolb,long iSize)
{
	return SetBlob(GetFieldIndex(Column),row,ppBolb,iSize);
}

BOOL CPM_LargeFileDataSet::GetFileRowRecord(int row, NRecordID &pBuffer,eBufferFlag BufferFlag) const
{
	NRecordID pIndexTableRowInfo = GetRecordID(row,BufferFlag);
	return GetFileRowRecord(pIndexTableRowInfo,pBuffer);
}

BOOL CPM_LargeFileDataSet::GetFileRowRecord(NRecordID pIndexTableRowInfo, NRecordID &pBuffer) const
{
	int iRowMode = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.RowMode_Index,pIndexTableRowInfo);
	if((InMemory | InMemory_Mdf) & iRowMode)
	{
		pBuffer = (LPVOID)CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.MemRowPos_Index,pIndexTableRowInfo);
	}
	else
	{
//		UIBase::CTiming timing(_T("CPM_LargeFileDataSet::GetFileRowRecord::File"),TRUE);
		CFileRecordStreamCtrl fileRecordCtrl(m_pStreamFileCtrl);
		CPMFileStreamCtrl* pFileRecordStream = fileRecordCtrl.GetFileRecordStream();

		int FilePos = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.FilePos_Index,pIndexTableRowInfo);
		int iLength = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.RowLength_Index,pIndexTableRowInfo);

		pBuffer = new BYTE[m_FileRecordBufferSize];

		LARGE_INTEGER iStreamPos;
		iStreamPos.QuadPart = FilePos;
		pFileRecordStream->SeekStream(iStreamPos,STREAM_SEEK_SET,NULL);
 
		HRESULT hr = pFileRecordStream->ReadStream(pBuffer,m_FileRecordBufferSize);
		if(!SUCCEEDED(hr))
		{
			CString strMsg;
			strMsg.Format(_T("DataSet发生错误!读取数据与读到数据不一致!"));
//			AfxMessageBox(strMsg);
			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
			return FALSE;
		}

		if(TRUE == m_Filefields.HasBlobField())
		{
			const CPtrArray * pBlobFieldArr = m_Filefields.GetBlobFieldArr();
			for (int i = 0;i < pBlobFieldArr->GetSize();i++)
			{
				NDataField * pTemField = (NDataField*)pBlobFieldArr->GetAt(i);

				LPVOID pBlobFieldBuffer = GetFieldDataPT(pTemField,pBuffer);
				LPVOID pTemBuffer = NULL;	
				long lBufferSize = 0;
				lBufferSize = pTemField->GetBlob(pBlobFieldBuffer,pTemBuffer);

				LPVOID pBlobInfo = NULL;
				iStreamPos.QuadPart = (int)pTemBuffer;
				pFileRecordStream->SeekStream(iStreamPos,STREAM_SEEK_SET,NULL);

				pBlobInfo = new BYTE[lBufferSize];
				hr = pFileRecordStream->ReadStream(pBlobInfo,lBufferSize);	
				if(!SUCCEEDED(hr))
				{
					CString strMsg;
					strMsg.Format(_T("DataSet发生错误!读取数据与读到数据不一致!"));
//					AfxMessageBox(strMsg);
					CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
					return FALSE;
				}

				/********************************************************************
				liwenlang
					时间:2010:10:28   13:58
					备注:将Blob指针直接写入 否则里面的偏移地址 维护会出错。
				*********************************************************************/
				if(pBlobFieldBuffer)
					CopyMemory(pBlobFieldBuffer,&pBlobInfo,sizeof(void*));
				//pTemField->SetBolbData(pBlobFieldBuffer,pBlobInfo,lReadSize);
			}
		}
		
		int iTemValue = (int)pBuffer;
//		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.MemRowPos_Index,pIndexTableRowInfo,iTemValue);
		{
			NDataField * pTemField = GetField(m_LargeFieldInfo.MemRowPos_Index);
			void * pTemBuffer = GetFieldDataPT(pTemField ,pIndexTableRowInfo );
			pTemField->SetAsInteger(iTemValue,pTemBuffer);
		}
//		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,pIndexTableRowInfo,iTemValue);
		{
			NDataField * pTemField = GetField(m_LargeFieldInfo.RowMode_Index);
			void * pTemBuffer = GetFieldDataPT(pTemField ,pIndexTableRowInfo );
			pTemField->SetAsInteger(InMemory,pTemBuffer);
		}
	}
	return TRUE;
}

CPM_LargeFileDataSet::FieldMode CPM_LargeFileDataSet::GetRowRecord(int iCol,int row,NRecordID &pBuffer) const
{
	if(iCol < 0)
		return NullField;

	if(iCol >= m_fields.GetCount())
	{//从文件主表中取数据
		if(!GetFileRowRecord(row,pBuffer))
			return NullField;
		return DataField;
	}
	else
	{//索引表取数据
		pBuffer = m_PrimaryBuffers->GetBuffPtr(row - 1);
		return IndexField;
	}
	return NullField;
}

CPM_LargeFileDataSet::FieldMode CPM_LargeFileDataSet::GetRowRecord(int iCol,NRecordID pIndexBuffer,NRecordID &pBuffer) const
{
	if(iCol < 0)
		return NullField;

	if(iCol >= m_fields.GetCount())
	{//从文件主表中取数据
		if(!GetFileRowRecord(pIndexBuffer,pBuffer))
			return NullField;
		return DataField;
	}
	else
	{//索引表取数据
		pBuffer = pIndexBuffer;
		return IndexField;
	}
	return NullField;
}

CPM_LargeFileDataSet::FieldMode CPM_LargeFileDataSet::GetRowRecord(NDataField * pField,NRecordID pIndexBuffer,NRecordID &pBuffer) const
{
	if(DataField == pField->GetSpecStatus())
	{//从文件主表中取数据
		if(!GetFileRowRecord(pIndexBuffer,pBuffer))
			return NullField;
		return DataField;
	}
	else
	{//索引表取数据
		pBuffer = pIndexBuffer;
		return IndexField;
	}
	return NullField;
}

int CPM_LargeFileDataSet::GetFieldIndex(const CString & sFieldName) const
{
	int iFieldIndex = m_fields.GetFieldIndex(sFieldName);
	if(-1 == iFieldIndex)
	{
		iFieldIndex = m_Filefields.GetFieldIndex(sFieldName);
		if(-1 == iFieldIndex)
			return iFieldIndex;
		iFieldIndex += m_fields.GetCount();
	}
	return iFieldIndex;
}

NDataField * CPM_LargeFileDataSet::GetField(int index) const
{
	int iFieldCount = GetFieldCount();
	NDataField * pTemField = NULL;
	if(GetFieldCount() > index)
		pTemField = m_fields.GetField(index);
	else
	{
		index -= iFieldCount;
		pTemField = m_Filefields.GetField(index);
	}
	return pTemField;
}

CString CPM_LargeFileDataSet::GetFieldName(int index) const
{
	NDataField * pTemField = GetField(index);
	return pTemField->GetFieldName();
}
// 
// void * CPM_LargeFileDataSet::GetFieldData_LargeFile(int iFieldMode,NDataField *field,void *buff, BOOL originalvalue)
// {
// 	if (buff == NULL) {
// 		return NULL;
// 	}
// 	
// 	ASSERT(buff != NULL);
// 	int iFieldOffset;
// 	iFieldOffset = GetFieldOffset_LargeFile(iFieldMode,field);
// 	char * ptr = (char *)buff;
// 	//是否需要创建原值
// 	//是否是原值进行判断
// 	if (originalvalue) 
// 	{
// 		//取得原值的指针
// 		ptr+= iFieldOffset + field->GetSize() + 1;//1BYTE 的状态位置;
// 		//ptr = (char*)(*ptr);
// 		CopyMemory(&ptr,&(*ptr),sizeof(void*));
// 		if (ptr == NULL)
// 		{
// 			ptr = (char *)buff;
// 			ptr += iFieldOffset;
// 		}
// 	}
// 	else
// 	{
// 		ptr += iFieldOffset;
// 	}
// 	return ptr;
// }
// 
// int CPM_LargeFileDataSet::GetFieldOffset_LargeFile(int iFieldMode,NDataField * pField)
// {
// 	ASSERT(AfxIsValidAddress(pField,sizeof(NDataField)));	
// 	
// 	if(1 == iFieldMode)
// 		return m_fields.GetFieldOffset(pField);
// 	else
// 		return m_Filefields.GetFieldOffset(pField);
// 	return 0;
// }

int CPM_LargeFileDataSet::GetFieldOffset(NDataField * pField) const
{
	ASSERT(AfxIsValidAddress(pField,sizeof(NDataField)));	
	
	if(IndexField == pField->GetSpecStatus())
		return m_fields.GetFieldOffset(pField);
	else
		return m_Filefields.GetFieldOffset(pField);
	return 0;
}

void CPM_LargeFileDataSet::FreeFileRecordBuffer(void *pBuffer)
{
	NDataField * pField = NULL;
	if (m_Filefields.HasBlobField() == TRUE) 
	{
		const CPtrArray * pBlobFieldArr = m_Filefields.GetBlobFieldArr();
		for(int i = 0 ; i < pBlobFieldArr->GetSize() ; i++)
		{
			pField = (NDataField *)pBlobFieldArr->GetAt(i);
			pField->FreeBolbData(GetFieldDataPT(pField,pBuffer));
		}
	}

//	FreeRecordOrigValueBuffer(pBuffer);
	delete []((BYTE*)pBuffer);
}

void CPM_LargeFileDataSet::DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff)
{
	/********************************************************************
	liwenlang
	时间:2010:10:27   11:15
	备注:删除记录 操作
	*********************************************************************/
	if (row < 0 || row > pBuff->GetRowCount()) 
	{
		return;
	}
	if (row == 0) 
	{
		row = pBuff->GetRowCount();
	}
	void * pRowBuffer = pBuff->GetBuffPtr(row - 1);

	int iRowMode = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.RowMode_Index,pRowBuffer);
	int iFilePos = 0,iBufferLength = 0;
	if((iRowMode & InMemory_Mdf) || (iRowMode & InMemory))
	{
		NRecordID pFileRecord = (NRecordID)CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.MemRowPos_Index,pRowBuffer);

		if(TRUE == GetRowNewInternal(pRowBuffer))
		{//是新行 在内存中，就不用 标记 文件碎片
			pBuff->GetBufferPtr()->RemoveAt(row - 1);

			FreeFileRecordBuffer(pFileRecord);
			FreeRecordBuffer(pRowBuffer);

			return ;
		}
		else
		{
			iFilePos = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.FilePos_Index,pRowBuffer);
			iBufferLength = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.RowLength_Index,pRowBuffer);
			FreeFileRecordBuffer(pFileRecord);
		}
	}
	else
	{
		iFilePos = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.FilePos_Index,pRowBuffer);
		iBufferLength = CPM_StorageDataSet::GetAsInteger(m_LargeFieldInfo.RowLength_Index,pRowBuffer);
	}
	FreeRecordBuffer(pRowBuffer);
	/********************************************************************
	liwenlang
		时间:2010:10:28   14:17
		备注:将记录移除
	*********************************************************************/
	pBuff->GetBufferPtr()->RemoveAt(row - 1);
	
	/********************************************************************
	liwenlang
	时间:2010:10:27   11:15
	备注:将指定的 文件位置 标记为 碎片
	*********************************************************************/
	AddScrapFileInfo(iFilePos,iBufferLength);
}

BOOL CPM_LargeFileDataSet::OnRowFilter(int row)
{
	int FieldCount = m_Filefields.GetCount();
	
	NRecordID pDataRecord = NULL;
	NDataField * pField = NULL;
	for(int Col = 0 ;Col<FieldCount ;Col++)
	{
		pField =m_Filefields.GetField(Col);
		if (pField != NULL && pField->HasFilterValue())
		{
			if(NULL == pDataRecord)
			{
				/********************************************************************
				liwenlang
					时间:2010:10:29   10:44
					备注:这样处理为了 在主表没有过滤操作时 可以提高效率
				*********************************************************************/
				GetFileRowRecord(row,pDataRecord);
			}

			if (pField->FilterCompare(GetFieldDataPT(pField,pDataRecord)) == FALSE) 
			{
				return FALSE;
			}
		}
		
	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::OnSortCompareField(NRecordID lowRecordID, NRecordID highRecordID,NDataField * pField,int & iResult)
{
	
	NRecordID plowRowRecordInfo = NULL;
	GetRowRecord(pField,lowRecordID,plowRowRecordInfo);
	NRecordID phighRowRecordInfo = NULL;
	GetRowRecord(pField,highRecordID,phighRowRecordInfo);
	
	iResult = pField->Compare(GetFieldDataPT(pField,plowRowRecordInfo),GetFieldDataPT(pField,phighRowRecordInfo));
	
	return TRUE;
}

void CPM_LargeFileDataSet::ReSortScrapTable(int iSortMode)
{
	m_pFileScrapDataSet->ResetSortField();
	if(1 == iSortMode)
		m_pFileScrapDataSet->AddSortField(m_LargeFieldInfo.ScrapFilePos_Index,TRUE);
	else
		m_pFileScrapDataSet->AddSortField(m_LargeFieldInfo.ScrapLength_Index,TRUE);
	m_pFileScrapDataSet->Sort();
}

void CPM_LargeFileDataSet::AddScrapFileInfo(int iFilePos,int iLength,BOOL bNeedReSort)
{//在记录中 查找可以 连接的碎片空间，没有办法连接 新建记录
	if(TRUE == bNeedReSort)
	{
		ReSortScrapTable(1);
	}
	int iScrapRowCount = m_pFileScrapDataSet->GetRowCount();
	if(iScrapRowCount <= 0)
	{
		int iNewRow = m_pFileScrapDataSet->InsertRow(0);
		m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iNewRow,iFilePos);
		m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iNewRow,iLength);
		return;
	}
	int iLeftMovePos = 1;
	int iRightMovePos = iScrapRowCount;
	int iMiddlePos = 0;
	for (;iLeftMovePos <= iRightMovePos;)
	{
		iMiddlePos = (iRightMovePos + iLeftMovePos)/2;
		
		int iTemFilePos = m_pFileScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos);
		if(iTemFilePos < iFilePos)
		{
			if(iMiddlePos == iScrapRowCount)
			{//已经到记录的结束
				int iTemBufferLength = m_pFileScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos);
				if(iFilePos == (iTemFilePos + iTemBufferLength))
				{
					int iNewBufferLength = iTemBufferLength + iLength;
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos,iNewBufferLength);
				}
				else
				{
					int iNewRow = m_pFileScrapDataSet->InsertRow(iMiddlePos + 1);
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iNewRow,iFilePos);
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iNewRow,iLength);
				}
				return;
			}
			
			int iNextFilePos = m_pFileScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos + 1);
			if(iNextFilePos < iFilePos)
			{
				iLeftMovePos = iMiddlePos + 1;
				continue;
			}
			else if(iNextFilePos == iFilePos)
			{
				AfxMessageBox(_T("出现文件碎片管理异常:删除碎片行"));
				return;
			}
			else
			{
				int iTemBufferLength = m_pFileScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos);
				int iNextBufferLength = m_pFileScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos + 1);
				if((iTemFilePos + iTemBufferLength) == iFilePos)
				{//可以连接的情况。
					//判断是否可以和后一个记录一起连接
					int iNewBufferLength = 0;
					if((iFilePos + iLength) == iNextFilePos)
					{//可以前后连接的情况
						iNewBufferLength = iTemBufferLength + iLength + iNextBufferLength;	
						m_pFileScrapDataSet->DeleteRow(iMiddlePos + 1);
					}
					else
					{
						iNewBufferLength = iTemBufferLength + iLength;
					}
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos,iNewBufferLength);
				}
				else if((iFilePos + iLength) == iNextFilePos)
				{//与后面相连
					int iNewBufferLength = iLength + iNextBufferLength;
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos + 1,iFilePos);
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos + 1,iNewBufferLength);
				}
				else
				{//新建 新的碎片记录
					int iNewRow = m_pFileScrapDataSet->InsertRow(iMiddlePos + 1);
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iNewRow,iFilePos);
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iNewRow,iLength);
				}
				return;
			}
		}
		else if(iTemFilePos == iFilePos)
		{
			AfxMessageBox(_T("出现文件碎片管理异常:删除碎片行"));
			return;
		}
		else
		{
			if(iMiddlePos == 1)
			{//已经到记录的结束
				if((iFilePos + iLength) == iTemFilePos)
				{
					int iTemBufferLength = m_pFileScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos);
					int iNewBufferLength = iLength + iTemBufferLength;
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos,iFilePos);
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos,iNewBufferLength);
				}
				else
				{
					int iNewRow = m_pFileScrapDataSet->InsertRow(iMiddlePos);
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iNewRow,iFilePos);
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iNewRow,iLength);
				}
				return;
			}
			int iPreFilePos = m_pFileScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos - 1);
			if(iPreFilePos > iFilePos)
			{
				iRightMovePos = iMiddlePos - 1;
				continue;
			}
			else if(iPreFilePos == iFilePos)
			{
				AfxMessageBox(_T("出现文件碎片管理异常:删除碎片行"));
				return;
			}
			else
			{
				int iPreBufferLength = m_pFileScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos - 1);
				int iTemBufferLength = m_pFileScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos);

				if((iPreFilePos + iPreBufferLength) == iFilePos)
				{//跟前端 相连
					int iNewBufferLength = 0;
					if((iFilePos + iLength) == iTemFilePos)
					{//跟后端 相连
						iNewBufferLength = iPreBufferLength + iLength + iTemBufferLength;
						m_pFileScrapDataSet->DeleteRow(iMiddlePos);
					}
					else
					{
						iNewBufferLength = iPreBufferLength + iLength;	
					}
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos - 1,iNewBufferLength);
				}
				else if((iFilePos + iLength) == iTemFilePos)
				{//跟后端 相连
					int iNewBufferLength = iLength + iTemBufferLength;
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos,iFilePos);
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos,iNewBufferLength);
				}
				else
				{
					int iNewRow = m_pFileScrapDataSet->InsertRow(iMiddlePos);
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iNewRow,iFilePos);
					m_pFileScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iNewRow,iLength);
				}
				return;
			}
		}
	}

	AfxMessageBox(_T("出现文件碎片管理异常:AddScrapFileInfo有没有考虑的情况!"));
	return;
}

BOOL CPM_LargeFileDataSet::GetScrapFileInfo(NDataModelDataSet * pScrapDataSet,int iCheckLength,int & iFilePos,int & iLength,BOOL bNeedReSort)
{
	if(TRUE == bNeedReSort)
		ReSortScrapTable(2);

	iFilePos = -1;
	iLength = -1;

	int iScrapRowCount = pScrapDataSet->GetRowCount();
	if(iScrapRowCount <= 0)
		return FALSE;
	int iLeftMovePos = 1;
	int iRightMovePos = iScrapRowCount;
	int iMiddlePos = 0;
	for (;iLeftMovePos <= iRightMovePos;)
	{
		iMiddlePos = (iRightMovePos + iLeftMovePos)/2;
		
		int iTemLength = pScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos);
		if(iTemLength < iCheckLength)
		{
			iLeftMovePos = iMiddlePos + 1;
			continue;
		}
		else if(iTemLength == iCheckLength)
		{
			iFilePos = pScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos);
			iLength = iTemLength;

			NRecordID pTemBuffer = pScrapDataSet->GetRecordID(iMiddlePos);
			m_pFileScrapDataSet->DeleteRow(pTemBuffer);
			return TRUE;
		}
		else
		{
			if(iMiddlePos == 1)
			{
				iFilePos = pScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos);
				iLength = iTemLength;

				int iSumLength = iTemLength - iCheckLength;
				int iSumFilePos = iFilePos + iCheckLength;
				if(iSumLength >= m_FileRecordBufferSize)
				{
					pScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos,iSumFilePos);
					pScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos,iSumLength);
					iLength = iCheckLength;
				}
				else
				{
					NRecordID pTemBuffer = pScrapDataSet->GetRecordID(iMiddlePos);
					m_pFileScrapDataSet->DeleteRow(pTemBuffer);
				}
				return TRUE;
			}

			int iPreLength = pScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos - 1);
			if(iPreLength > iCheckLength)
			{
				iRightMovePos = iMiddlePos - 1;
				continue;
			}
			else
			{
				iFilePos = pScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos);
				iLength = iTemLength;

				int iSumLength = iTemLength - iCheckLength;
				int iSumFilePos = iFilePos + iCheckLength;
				/********************************************************************
				liwenlang
					时间:2011:1:1   17:25
					备注:GetSorcap函数不把小于最小长度的碎片 抛弃，否则碎片无法维护。
				*********************************************************************/
//				if(iSumLength >= m_FileRecordBufferSize)
				if(iSumLength > 0)
				{
					pScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos,iSumFilePos);
					pScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos,iSumLength);
					iLength = iCheckLength;
				}
//				else
//				{
//					NRecordID pTemBuffer = pScrapDataSet->GetRecordID(iMiddlePos);
//					m_pFileScrapDataSet->DeleteRow(pTemBuffer);
//				}
				return TRUE;	
			}
		}
	}
	
	//iFilePos = pScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos);
	//iLength = pScrapDataSet->GetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos);
	//			
	//int iSumLength = iLength - iCheckLength;
	//int iSumFilePos = iFilePos + iCheckLength;
	//if(iSumLength >= m_FileRecordBufferSize)
	//{
	//	pScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapFilePos_Index,iMiddlePos,iSumFilePos);
	//	pScrapDataSet->SetAsInteger(m_LargeFieldInfo.ScrapLength_Index,iMiddlePos,iSumLength);
	//	iLength = iCheckLength;
	//}
	//else
	//{
	//	NRecordID pTemBuffer = pScrapDataSet->GetRecordID(iMiddlePos);
	//	m_pFileScrapDataSet->DeleteRow(pTemBuffer);
	//}
	return FALSE;
}

CString CPM_LargeFileDataSet::GetScrapDataSetXML()
{
	CString strXML;
	m_pFileScrapDataSet->SaveDataSetXMLBuf(strXML);
	return strXML;
}

BOOL CPM_LargeFileDataSet::BuildItemXML(CString tableName,int row,NRecordID recordID,CString &sbuff,CString &sXMLBuf,CString &sXMLBefore)
{
//	UIBase::CTiming timing(_T("CPM_LargeFileDataSet::BuildItemXML"),TRUE);
	
	//先生成行信息
	//sXMLBuf = "<"+m_TableName+GetRowModifiedInternal(recordID)?" hasChanges="+"\"modified\"":""+
	//	GetRowNewInternal(recordID)?" IsNew="+"true":""+">";
	
	//开始生成每一个item
	BOOL rowModified = GetRowModifiedInternal((char*)recordID);
	BOOL rowNew = GetRowNewInternal(recordID);
	CString Data/*,s,Orgs*/;
	CDataSet_StringBuilder sInstance,sBefroe;
	CString FieldName;
	int datatype;
	int i = 0 ;
	for(i = 0 ; i < GetFieldCount(); i++)
	{
		Data = CPM_StorageDataSet::GetAsString(i,recordID);

		///////////////////////////////////
		///liwenlang 
		///不将空的属性 保存 在LoadXML时 在读取Before数据时 没有办法 更新空数据库
		///[25/12/2009 9:40]
		//////////////////////////////////
//		if(Data.IsEmpty()) continue;

		datatype=GetFieldType(i);
		FieldName=GetFieldName(i);

		//日期型特殊处理
		if(datatype ==(int) ftDateTime) {
			Data.Replace(_T('.'), _T('-'));
			Data.Replace(_T(' '), _T('T'));
		}
		//字符型特殊处理
		if(datatype==ftString || datatype==ftBlob) {
			Data.Replace(_T("&"), _T("&amp;"));
			Data.Replace(_T("<"), _T("&lt;"));
			Data.Replace(_T(">"), _T("&gt;"));
		}
//		s.Format(_T("<%s>%s</%s>"), FieldName, Data, FieldName);
		sInstance.Append(_T("<"));
		sInstance.Append(FieldName);
		sInstance.Append(_T(">"));
		sInstance.Append(Data);
		sInstance.Append(_T("</"));
		sInstance.Append(FieldName);
		sInstance.Append(_T(">"));
		//开始组织befor段的内容
		if (rowModified && (!rowNew))
		{
			if (GetItemModifiedInternal(recordID,i)) 
			{
				Data = CPM_StorageDataSet::GetAsString(i,recordID,TRUE);
				//日期型特殊处理
				if(datatype ==(int) ftDateTime) {
					Data.Replace(_T('.'), _T('-'));
					Data.Replace(_T(' '), _T('T'));
				}
				//字符型特殊处理
				if(datatype==ftString || datatype==ftBlob) {
					Data.Replace(_T("&"), _T("&amp;"));
					Data.Replace(_T("<"), _T("&lt;"));
					Data.Replace(_T(">"), _T("&gt;"));
				}
//				Orgs.Format(_T("<%s>%s</%s>"), FieldName, Data, FieldName);
				sBefroe.Append(_T("<"));
				sBefroe.Append(FieldName);
				sBefroe.Append(_T(">"));
				sBefroe.Append(Data);
				sBefroe.Append(_T("</"));
				sBefroe.Append(FieldName);
				sBefroe.Append(_T(">"));
			}
		}
	}

	NRecordID pDataRecordInfo = NULL;
	if(GetFileRowRecord(recordID,pDataRecordInfo))
	{
		for(i = 0 ; i < m_Filefields.GetCount(); i++)
		{
			NDataField * pTemField = m_Filefields.GetField(i);
			NRecordID pTemCellBuffer = GetFieldDataPT(pTemField,pDataRecordInfo);

			Data = pTemField->GetAsString(pTemCellBuffer);
			
			///////////////////////////////////
			///liwenlang 
			///不将空的属性 保存 在LoadXML时 在读取Before数据时 没有办法 更新空数据库
			///[25/12/2009 9:40]
			//////////////////////////////////
			//		if(Data.IsEmpty()) continue;
			
			datatype = pTemField->GetDataType();
			FieldName = pTemField->GetFieldName();
			
			//日期型特殊处理
			if(datatype ==(int) ftDateTime) {
				Data.Replace(_T('.'), _T('-'));
				Data.Replace(_T(' '), _T('T'));
			}
			//字符型特殊处理
			if(datatype==ftString || datatype==ftBlob) {
				Data.Replace(_T("&"), _T("&amp;"));
				Data.Replace(_T("<"), _T("&lt;"));
				Data.Replace(_T(">"), _T("&gt;"));
			}
//			s.Format(_T("<%s>%s</%s>"), FieldName, Data, FieldName);
			sInstance.Append(_T("<"));
			sInstance.Append(FieldName);
			sInstance.Append(_T(">"));
			sInstance.Append(Data);
			sInstance.Append(_T("</"));
			sInstance.Append(FieldName);
			sInstance.Append(_T(">"));
			
		}
	}

	CString strRow ;
	strRow.Format( _T("%d") , row ) ; 

// 	TCHAR cRows[20];
// 	_itot(row,cRows,10);
	if (sBefroe.GetLength() > 0 )//为原始数据加上封装节点 
	{
//		sXMLBefore.Format(_T("<%s rowid=\"%d\" buff=%s>%s</%s>"),tableName,row,sbuff,sBefroe.m_pMem,tableName);
		CDataSet_StringBuilder sBuildXMLBefore;
		sBuildXMLBefore.Append(_T("<"));
		sBuildXMLBefore.Append(tableName);
		sBuildXMLBefore.Append(_T(" rowid=\""));
		sBuildXMLBefore.Append( strRow );
		sBuildXMLBefore.Append(_T("\" buff="));
		sBuildXMLBefore.Append(sbuff);
		sBuildXMLBefore.Append(_T(">"));
		sBuildXMLBefore.Append(sBefroe);
		sBuildXMLBefore.Append(_T("<"));
		sBuildXMLBefore.Append(_T("tableName"));
		sBuildXMLBefore.Append(_T(">"));
		sBuildXMLBefore.GetString(sXMLBefore);
	}

    CString sRowModify,sRownew;
	sRowModify = rowModified&&(!rowNew) ? _T("hasChanges=\"modified\" "):_T("");
	sRownew = rowNew?_T(" IsNew=\"true\" "):_T("");
//	sXMLBuf.Format(_T("<%s %s%s\"%d\" buff=%s>%s</%s>"),tableName , sRowModify+sRownew,_T("rowid="),row,sbuff,sInstance.m_pMem,tableName);
	CDataSet_StringBuilder sBuildXMLBuff;
	sBuildXMLBuff.Append(_T("<"));
	sBuildXMLBuff.Append(tableName);
	sBuildXMLBuff.Append(_T(" "));
	sBuildXMLBuff.Append(sRowModify+sRownew);
	sBuildXMLBuff.Append(_T("rowid="));
	sBuildXMLBuff.Append(_T("\""));
	sBuildXMLBuff.Append( strRow );
	sBuildXMLBuff.Append(_T("\" buff="));
	sBuildXMLBuff.Append(sbuff);
	sBuildXMLBuff.Append(_T(">"));
	sBuildXMLBuff.Append(sInstance);
	sBuildXMLBuff.Append(_T("</"));
	sBuildXMLBuff.Append(tableName);
	sBuildXMLBuff.Append(_T(">"));
	sBuildXMLBuff.GetString(sXMLBuf);

	//sXMLBuf+="/"+m_TableName+">"
	return TRUE;
}

void CPM_LargeFileDataSet::SetRowModifiedInternal(void * buff,BOOL bModified)
{
	if (m_IsModified == FALSE) 
	{
		m_IsModified = TRUE;
	}
}

BOOL CPM_LargeFileDataSet::GetUserDataField_Range(int & iBegin,int & iEnd)
{
	int iFieldCount = m_fields.GetCount() + m_Filefields.GetCount();

	/********************************************************************
	liwenlang
		时间:2010:11:23   9:19
		备注:  没有用户定义数据
	*********************************************************************/
	if(iFieldCount <= 4)
		return FALSE;

	iBegin = 4;
	iEnd = iFieldCount;
	return TRUE;
}

CString CPM_LargeFileDataSet::GetAsString(int Column,NRecordID id, BOOL originalvalue) const
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,id,pBuffer);
	if(NullField == iRecordMode)
		return _T("");

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	return pField->GetAsString(pCellBuffer);
}

double CPM_LargeFileDataSet::GetAsFloat(int Column,NRecordID id, BOOL originalvalue) const
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,id,pBuffer);
	if(NullField == iRecordMode)
		return 0.0;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	return pField->GetAsFloat(pCellBuffer);
}

int CPM_LargeFileDataSet::GetAsInteger(int Column,NRecordID id, BOOL originalvalue) const
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,id,pBuffer);
	if(NullField == iRecordMode)
		return 0;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	return pField->GetAsInteger(pCellBuffer);
}

BOOL CPM_LargeFileDataSet::GetAsBool(int Column,NRecordID id, BOOL originalvalue) const
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,id,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	return pField->GetAsBoolean(pCellBuffer);
}

BOOL CPM_LargeFileDataSet::SetAsString(int Column, NRecordID id ,const CString& val)
{
	//if(Column <= m_LargeFieldInfo.RowMode_Index)
	//	return FALSE;

	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,id,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	CString strTemValue = pField->GetAsString(pCellBuffer);
	if(strTemValue.GetLength() == val.GetLength() && strTemValue == val)
		return TRUE;

	pField->SetAsString(val,pCellBuffer);
	if(DataField == iRecordMode)
	{
		int iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,id,iTemValue);
	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::SetAsFloat(int Column, NRecordID id ,const double& val)
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,id,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	double dTemValue = pField->GetAsFloat(pCellBuffer);
	if(dTemValue == val)
		return TRUE;

	pField->SetAsFloat(val,pCellBuffer);
	if(DataField == iRecordMode)
	{
		int iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,id,iTemValue);
	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::SetAsInteger(int Column, NRecordID id ,const int& val)
{
	//if(Column <= m_LargeFieldInfo.RowMode_Index)
	//	return FALSE;

	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,id,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	int iTemValue = pField->GetAsInteger(pCellBuffer);
	if(iTemValue == val)
		return TRUE;

	pField->SetAsInteger(val,pCellBuffer);
	if(DataField == iRecordMode)
	{
		/*int*/ iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,id,iTemValue);
	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::SetAsBool(int Column, NRecordID id ,const BOOL & val)
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,id,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	BOOL bTemValue = pField->GetAsBoolean(pCellBuffer);
	if(bTemValue == val)
		return TRUE;

	pField->SetAsBoolean(val,pCellBuffer);
	if(DataField == iRecordMode)
	{
		int iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,id,iTemValue);
	}
	return TRUE;
}

BOOL CPM_LargeFileDataSet::SetBlob(int Column, NRecordID id ,void * ppBolb,long iSize)
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,id,pBuffer);
	if(NullField == iRecordMode)
		return FALSE;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	pField->SetBlob(pCellBuffer,ppBolb,iSize);
	if(DataField == iRecordMode)
	{
		int iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,id,iTemValue);
	}
	return TRUE;
}

long CPM_LargeFileDataSet::GetBlob(int Column, NRecordID id ,void *& ppBolb) const
{
	NRecordID pBuffer = NULL;
	int iRecordMode = GetRowRecord(Column,id,pBuffer);
	if(NullField == iRecordMode)
		return 0;

	NDataField * pField = GetField(Column);
	NRecordID pCellBuffer = GetFieldDataPT(pField,pBuffer);

	return pField->GetBlob(pCellBuffer,ppBolb);
}

void CPM_LargeFileDataSet::GetAllField(CArray<NDataField*,NDataField*> & dataFields)
{
	CPM_StorageDataSet::GetAllField(dataFields);

	int iFieldCount = m_Filefields.GetCount();
	for (int i = 0;i < iFieldCount;i++)
	{
		dataFields.Add(m_Filefields.GetField(i));
	}
}

void CPM_LargeFileDataSet::ResetFilter(BOOL bResetExp)
{
	CPM_StorageDataSet::ResetFilter(bResetExp);
	
	if (bResetExp) 
	{
		int iFieldCount = m_Filefields.GetCount();
		for (int i = 0;i < iFieldCount;i++)
		{
			m_Filefields.GetField(i)->FreeFilterValue();
		}
	}
}

int CPM_LargeFileDataSet::RowsCopy(int startrow,int endrow,eBufferFlag copybuffer,NDataModelDataSet *TargetData,int beforerow, eBufferFlag	targetbuffer)
{
	AfxMessageBox(_T("RowsCopy函数没有实现!"));
	return 1;
}

NDataModelDataSet * CPM_LargeFileDataSet::Clone()
{
	AfxMessageBox(_T("Clone函数没有实现!"));
	return NULL;
}

void CPM_LargeFileDataSet::DoFilter()
{
	CFileRecordStreamCtrl fileRecordCtrl(m_pStreamFileCtrl);
	CPM_StorageDataSet::DoFilter();
}

void CPM_LargeFileDataSet::Sort()
{
	CFileRecordStreamCtrl fileRecordCtrl(m_pStreamFileCtrl);
	CPM_StorageDataSet::Sort();
}

BOOL CPM_LargeFileDataSet::ReSortLargeFileDataSetFile()
{
	CFileRecordStreamCtrl fileRecordCtrl(m_pStreamFileCtrl);

	int iRowCount = GetRowCount();
	for(int i = 1;i <= iRowCount;i++)
	{
		NRecordID pFileRecordID = NULL;
		this->GetFileRowRecord(i,pFileRecordID);

		int iTemValue = InMemory_Mdf;
		CPM_StorageDataSet::SetAsInteger(m_LargeFieldInfo.RowMode_Index,i,iTemValue);

		this->SetRowNew(i,TRUE);
	}

	m_pFileScrapDataSet->Reset();
	IStream * pFileStreamCtrl = fileRecordCtrl.GetFileRecordStream()->GetStream();
	if(NULL == pFileStreamCtrl)
		return FALSE;

	ULARGE_INTEGER newSize;
	newSize.QuadPart = 0;
	pFileStreamCtrl->SetSize(newSize);

	CPMFileStreamCtrl * pScrapRecordStream = NULL;
	m_pStreamFileCtrl->GetScrapRecordStream(pScrapRecordStream);
	IStream * pScrapRecordStreamCtrl = pScrapRecordStream->GetStream();
	if(NULL == pScrapRecordStreamCtrl)
		return FALSE;

	pScrapRecordStreamCtrl->SetSize(newSize);
	return TRUE;
}
