// PM_StorageDataSet.cpp: implementation of the CPM_StorageDataSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "TestMainFrame.h"
#include "PM_StorageDataSet.h"
#include "CPub.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const short CPM_StorageDataSet::m_iStructDataVersion = 3;

#define COM_RESULT_OK(hr) if (!SUCCEEDED(hr)){bSaveResult = FALSE; goto Com_Error_Exit;}
#define FUN_RESULT_OK(hr) if (!(hr)){bSaveResult = FALSE; goto Com_Error_Exit;}
#define RELEASE_INTERFACE(X) if (X) { (X)->Release(); (X) = NULL; }

//////////////////////////////////////////////////////////////////////////
//CPMFileStorageCtrl
CPMFileStorageCtrl::CPMFileStorageCtrl()
{
	m_pStorage = NULL;
}

CPMFileStorageCtrl::~CPMFileStorageCtrl()
{
	if(m_pStorage)
	{
		CloseStorage();

		CString strTemMsg;
		strTemMsg.Format(_T("CPMFileStorageCtrl::~CPMFileStorageCtrl 没有预先关闭文件!%s"),m_strFilePath);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
	}
}

HRESULT CPMFileStorageCtrl::TryOpenStorage(const CString & strFilePath)
{
	_bstr_t sKeyName;
	///////////////////////////////////
	///liwenlang 
	///打开复合文档
	///[31/5/2010]
	//////////////////////////////////
	sKeyName = strFilePath;

	// 创建复合文档
	HRESULT hr = StgOpenStorage(sKeyName,NULL, STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, NULL, NULL, &m_pStorage);
	if(!SUCCEEDED(hr))
	{
		hr = StgCreateDocfile(sKeyName, STGM_DIRECT|STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, NULL, &m_pStorage);
	}
	SysFreeString(sKeyName);
	if(!SUCCEEDED(hr))
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMFileStorageCtrl::TryOpenStorage打开文件失败!%s %0x"),strFilePath,hr);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return hr;
	}

	m_strFilePath = strFilePath;
	return hr;
}

IStorage * CPMFileStorageCtrl::GetStorage()
{
	return m_pStorage;
}

CString CPMFileStorageCtrl::GetFilePath()
{
	return m_strFilePath;
}

HRESULT CPMFileStorageCtrl::CloseStorage()
{
	if(NULL == m_pStorage)
		return S_OK;

	HRESULT hr = m_pStorage->Release();
	m_pStorage = NULL; 
	if(!SUCCEEDED(hr))
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMFileStorageCtrl::CloseStorage关闭文件失败!%s %0x"),m_strFilePath,hr);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return hr;
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////////
//CPMFileStreamCtrl

CPMFileStreamCtrl::CPMFileStreamCtrl()
{
	m_pCurStream = NULL;
	m_pStorageCtrl = NULL;
}

CPMFileStreamCtrl::~CPMFileStreamCtrl()
{
	if(m_pCurStream)
	{
		CloseStream();

		CString strTemMsg;
		strTemMsg.Format(_T("CPMFileStreamCtrl::~CPMFileStreamCtrl 没有预先关闭文件!%s"),m_pStorageCtrl->GetFilePath());
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
	}
}

HRESULT CPMFileStreamCtrl::TryOpenStream(CPMFileStorageCtrl * pStorageCtrl,const CString & strStreamName)
{
	if(NULL == pStorageCtrl)
		return STG_E_FILENOTFOUND;
	IStorage * pTemStorage = pStorageCtrl->GetStorage();
	if(NULL == pTemStorage)
		return STG_E_FILENOTFOUND;
	
	m_pStorageCtrl = pStorageCtrl;

	_bstr_t sKeyName;
	///////////////////////////////////
	///liwenlang 
	///打开复合文档
	///[31/5/2010]
	//////////////////////////////////
	sKeyName = strStreamName;

	////////////////////////////////////////////
	/// liwenlang [2012:2:1   14:41]
	/// 说明:
	/// 保证使用时 是open方式打开，使用Create 中途中断 表损坏概率很高
	HRESULT hr = pTemStorage->OpenStream(sKeyName,NULL, STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,  0, &m_pCurStream);
	if(!SUCCEEDED(hr))
	{
		hr = pTemStorage->CreateStream(sKeyName, STGM_DIRECT|STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, 0, 0, &m_pCurStream);
		if(SUCCEEDED(hr))
		{
			m_pCurStream->Release();
			hr = pTemStorage->OpenStream(sKeyName,NULL, STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE,  0, &m_pCurStream);
		}
	}
	SysFreeString(sKeyName);
	return hr;
}

HRESULT CPMFileStreamCtrl::ReadStream(LPVOID pBuffer,ULONG lBufferSize)
{
	if(NULL == m_pCurStream)
		return STG_E_INVALIDPOINTER;

	ULONG lReadBufferSize = 0;
	m_pCurStream->Read(pBuffer,lBufferSize,&lReadBufferSize);

	if(lReadBufferSize != lBufferSize)
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMFileStreamCtrl::ReadStream 读取文件失败!%s"),m_pStorageCtrl->GetFilePath());
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);

		return STG_E_ACCESSDENIED;
	}

	return S_OK;
}

HRESULT CPMFileStreamCtrl::WriteStream(LPVOID pBuffer,ULONG lBufferSize)
{
	if(NULL == m_pCurStream)
		return STG_E_INVALIDPOINTER;
	if(lBufferSize <= 0)
		return S_OK;

	////////////////////////////////////////////
	/// liwenlang [2012:1:30   16:31]
	/// 说明:
	/// 获取 当前位置 
	LARGE_INTEGER preMove;
	preMove.QuadPart = 0;
	ULARGE_INTEGER curStreamPos;
	HRESULT hr = m_pCurStream->Seek(preMove,STREAM_SEEK_CUR,&curStreamPos);
	if(!SUCCEEDED(hr))
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMFileStreamCtrl::WriteStream 获取游标位置失败!%s %0x"),m_pStorageCtrl->GetFilePath(),hr);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);

		return STG_E_INVALIDPOINTER;
	}

	//尝试读取当前位置的 信息，如果有并 相同的话，不用写入流
	LPVOID pReadBuffer = new BYTE[lBufferSize];
	memset(pReadBuffer,0,lBufferSize);
	hr = ReadStream(pReadBuffer,lBufferSize);
	if(SUCCEEDED(hr))
	{
// 		if(0 == memcmp(pReadBuffer,pBuffer,lBufferSize))
// 		{
// 			delete [](BYTE*)pReadBuffer;
// 			return ERROR_SUCCESS;
// 		}
	}
	delete [](BYTE*)pReadBuffer;
	
	////////////////////////////////////////////
	/// liwenlang [2012:1:30   16:32]
	/// 说明:
	/// 读取写入数据 进行验证 写入是否成功
	preMove.QuadPart = curStreamPos.QuadPart;
	hr = m_pCurStream->Seek(preMove,STREAM_SEEK_SET,&curStreamPos);
	if(!SUCCEEDED(hr))
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMFileStreamCtrl::WriteStream 回退游标位置失败!%s %0x"),m_pStorageCtrl->GetFilePath(),hr);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);

		return STG_E_INVALIDPOINTER;
	}

	////////////////////////////////////////////
	/// liwenlang [2012:1:30   16:31]
	/// 说明:
	/// 写入内容
	ULONG lWriteBufferSize = 0;
	hr = m_pCurStream->Write(pBuffer,lBufferSize,&lWriteBufferSize);
	if(!SUCCEEDED(hr))
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMFileStreamCtrl::WriteStream 写入流失败!%s %0x"),m_pStorageCtrl->GetFilePath(),hr);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		 
		return STG_E_WRITEFAULT;
	}

	//写入长度是否 一致
	if(lWriteBufferSize != lBufferSize)
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMFileStreamCtrl::WriteStream 写入流失败 长度不一致!%s %0x"),m_pStorageCtrl->GetFilePath(),hr);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);

		return STG_E_ACCESSDENIED;
	}

	return hr;
}

IStream * CPMFileStreamCtrl::GetStream()
{
	return m_pCurStream;
}

HRESULT CPMFileStreamCtrl::SeekStream(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER * plibNewPosition)
{
	if(NULL == m_pCurStream)
		return STG_E_INVALIDPOINTER;

	return m_pCurStream->Seek(dlibMove,dwOrigin,plibNewPosition);
}	

HRESULT CPMFileStreamCtrl::CloseStream()
{
	if(NULL == m_pCurStream)
		return S_OK;

	HRESULT hr = m_pCurStream->Commit(STGC_DEFAULT);
	if(!SUCCEEDED(hr))
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMFileStreamCtrl::CloseStorage关闭文件 Commit 失败!%s %0x"),m_pStorageCtrl->GetFilePath(),hr);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
	}
	
	hr = m_pCurStream->Release();
	m_pCurStream = NULL;

	if(!SUCCEEDED(hr))
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPMFileStreamCtrl::CloseStorage关闭文件失败!%s %0x"),m_pStorageCtrl->GetFilePath(),hr);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return hr;
	}
	return hr;
}

IMPLEMENT_DYNCREATE( CPM_StorageDataSet, NDataModelDataSet )


CPM_StorageDataSet::CPM_StorageDataSet()
{
	m_iVersion = -1;
	m_iOpeningDataSetDataCode = -1;
}

CPM_StorageDataSet::~CPM_StorageDataSet()
{

}

BOOL CPM_StorageDataSet::BuildSimRowBuffer(int row, LPVOID &pValue, long &iSize,eBufferFlag buffer)
{
	iSize = sizeof(long);

	NRecordID record = NULL,TargetRecord = NULL;
	if (row <= 0)
	{
		CString strTemMsg = _T("CPM_StorageDataSet::BuildSimRowBuffer,错误指定的行 小于或等于 0!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	switch (buffer)
	{
	case Primary:
		if (row > m_PrimaryBuffers->GetRowCount())
		{
			CString strTemMsg = _T("CPM_StorageDataSet::BuildSimRowBuffer,错误指定的行 大于总行数!");
			CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
			return FALSE;
		}
		TargetRecord = m_PrimaryBuffers->GetBuffPtr(row - 1);
		break;
	case Delete:
		if (row > m_DeleteBuffers->GetRowCount())
		{
			CString strTemMsg = _T("CPM_StorageDataSet::BuildSimRowBuffer,错误指定的行 大于总行数!");
			CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
			return FALSE;
		}
		TargetRecord = m_DeleteBuffers->GetBuffPtr(row - 1);
		break;
	case Filter:
		if (row > m_FilterBuffers->GetRowCount())
		{
			CString strTemMsg = _T("CPM_StorageDataSet::BuildSimRowBuffer,错误指定的行 大于总行数!");
			CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
			return FALSE;
		}
		TargetRecord = m_FilterBuffers->GetBuffPtr(row - 1);
		break;
	}

	int iFieldCount = m_fields.GetCount();
	LPVOID * pFieldValueArr = new LPVOID[iFieldCount];
	long * pFieldSizeArr = new long[iFieldCount];
	int i = 0;
	for (i = 0;i < iFieldCount;i++)
	{
		NDataField * pTemField = m_fields.GetField(i);

		LPVOID pTemFieldValue = NULL;
 		int iTemFieldSize = 0;

		if(ftBlob == pTemField->GetDataType())
		{
 			iTemFieldSize = pTemField->GetBlob(GetFieldDataPT(pTemField,TargetRecord,FALSE),pTemFieldValue);
			if(NULL == pTemFieldValue || iTemFieldSize < 0)
			{
				pTemFieldValue = NULL;
				iTemFieldSize = 0;
			}
			pFieldValueArr[i] = pTemFieldValue;
			pFieldSizeArr[i] = iTemFieldSize;//加上TCHAR为将Blob字段中的字符串后保存时，有结束标志

			iTemFieldSize += sizeof(long) + sizeof(WCHAR);
		}
		else if(ftString == pTemField->GetDataType())
		{
			pTemFieldValue = GetFieldDataPT(pTemField,TargetRecord,FALSE);
	//		int iTemsfef = strlen((char*)pTemFieldValue) + sizeof(TCHAR);
			iTemFieldSize = pTemField->GetSize() + sizeof(TCHAR);

			pFieldValueArr[i] = pTemFieldValue;
			pFieldSizeArr[i] = iTemFieldSize;

			iTemFieldSize += sizeof(long);
		}
		else
		{
			pTemFieldValue = GetFieldDataPT(pTemField,TargetRecord,FALSE);
			iTemFieldSize = pTemField->GetSize();

			pFieldValueArr[i] = pTemFieldValue;
			pFieldSizeArr[i] = iTemFieldSize;
		}

		iSize += iTemFieldSize;
	}
	
	pValue = new BYTE [iSize];
	if (pValue == NULL)
	{
		delete []pFieldValueArr;
		delete []pFieldSizeArr;

		CString strTemMsg = _T("CPM_StorageDataSet::BuildSimRowBuffer,生成行 内存申请失败!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	ZeroMemory(pValue,iSize);
	CopyMemory(pValue,&iSize,sizeof(long));

	LPVOID pFieldValue = (BYTE*)pValue + sizeof(long);
	///////////////////////////////////
	///liwenlang 
	///写入 Field内容
	///[28/5/2010]
	//////////////////////////////////
	LPVOID pCurBlobValue = pFieldValue;
	for (i = 0;i < iFieldCount;i++)
	{
		NDataField * pTemField = m_fields.GetField(i);
		if(ftBlob == pTemField->GetDataType())
		{
			CopyMemory(pCurBlobValue,&pFieldSizeArr[i],sizeof(long));
			CopyMemory((BYTE*)pCurBlobValue + sizeof(long),pFieldValueArr[i],pFieldSizeArr[i]);//减去TCHAR为了不会拷贝多余信息。
			pCurBlobValue = (BYTE*)pCurBlobValue + pFieldSizeArr[i] + sizeof(long) + sizeof(WCHAR);
		}
		else if(ftString == pTemField->GetDataType())
		{
			////////////////////////////////////////////
			/// liwenlang [2011:7:28   21:27]
			/// 说明:
			/// 由于之前版本在复制的时候会把标注位，如果是 Unicode 会复制一个随机值到 文件中。所以 Field长度要减TCHAR长度
			int iTemFieldSize = pFieldSizeArr[i];
			CopyMemory(pCurBlobValue,&iTemFieldSize,sizeof(long));
			iTemFieldSize -= sizeof(TCHAR);
			CopyMemory((BYTE*)pCurBlobValue + sizeof(long),pFieldValueArr[i],iTemFieldSize);
			pCurBlobValue = (BYTE*)pCurBlobValue + pFieldSizeArr[i] + sizeof(long);
		}
		else
		{
			CopyMemory((BYTE*)pCurBlobValue,pFieldValueArr[i],pFieldSizeArr[i]);
			pCurBlobValue = (BYTE*)pCurBlobValue + pFieldSizeArr[i];
		}
	}

	delete []pFieldValueArr;
	delete []pFieldSizeArr;
	return TRUE;
}
//
//BOOL CPM_StorageDataSet::OpenMDocument(CString strFileName,BOOL bAppend)
//{
////	UIBase::CTiming timing(_T("CPM_StorageDataSet::OpenMDocument"),TRUE);
//
//	if(_T("") == m_TableName)
//	{
//		CString strTemMsg = _T("CPM_StorageDataSet::OpenMDocument打开失败,表名为空!");
//		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
//		return FALSE;
//	}
//
//	BOOL bOpenResult = TRUE;
//	_bstr_t sKeyName;
//	///////////////////////////////////
//	///liwenlang 
//	///打开复合文档
//	///[31/5/2010]
//	//////////////////////////////////
//	IStorage* piRootStr = NULL;
//	sKeyName = strFileName;
//	int iVersion = 0;
//	
//	// 创建复合文档
//	HRESULT hr = StgOpenStorage(sKeyName,NULL, STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, NULL, NULL, &piRootStr);
//	SysFreeString(sKeyName);
//	if(!SUCCEEDED(hr))
//	{
//		CString strTemMsg = _T("CPM_StorageDataSet::OpenMDocument打开失败,表名为空!");
//		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
//		return FALSE;
//	}
//
//	///////////////////////////////////
//	///liwenlang 
//	///打开指定的Table
//	///[31/5/2010]
//	//////////////////////////////////
//	sKeyName = m_TableName;
//	IStream* pTableStream = NULL;
//	hr = piRootStr->OpenStream(sKeyName, NULL, STGM_READWRITE|STGM_SHARE_EXCLUSIVE, 0, &pTableStream); 
//	SysFreeString(sKeyName);
//	if (!SUCCEEDED(hr))
//	{
//		RELEASE_INTERFACE(piRootStr);
//		return TRUE;
//	}
//
// 	LONG lDataSetAllSize = 0;
// 	unsigned long lReadSize = 0;
// 	hr = pTableStream->Read(&lDataSetAllSize,sizeof(LONG),&lReadSize);
//	if (!SUCCEEDED(hr))
//	{
//		RELEASE_INTERFACE(pTableStream);
//		RELEASE_INTERFACE(piRootStr);
//
//		CString strTemMsg = _T("CPM_StorageDataSet::OpenMDocument打开失败,读取流长度失败!");
//		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
//		return FALSE;
//	}
//
//	BYTE * pDataSetAllInfo = new BYTE[lDataSetAllSize];
//	ZeroMemory(pDataSetAllInfo,lDataSetAllSize);
//	hr = pTableStream->Read(pDataSetAllInfo,lDataSetAllSize,&lReadSize);
//	RELEASE_INTERFACE(pTableStream);
//	RELEASE_INTERFACE(piRootStr);
//	if (!SUCCEEDED(hr) || lDataSetAllSize != lReadSize)
//	{
//		CString strTemMsg = _T("CPM_StorageDataSet::OpenMDocument打开失败,读取流信息失败!");
//		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
//		return FALSE;
//	}
//
//	bOpenResult = Buffer2DataSet(pDataSetAllInfo,lDataSetAllSize,FALSE);
//	delete [](BYTE*)pDataSetAllInfo;
//	return bOpenResult;
//}

BOOL CPM_StorageDataSet::OpenMDocument(CString strFileName,BOOL bAppend)
{
	//	UIBase::CTiming timing(_T("CPM_StorageDataSet::OpenMDocument"),TRUE);

	if(_T("") == m_TableName)
	{
		CString strTemMsg = _T("CPM_StorageDataSet::OpenMDocument打开失败,表名为空!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}

	BOOL bOpenResult = TRUE;
	///////////////////////////////////
	///liwenlang 
	///打开复合文档
	///[31/5/2010]
	//////////////////////////////////
	int iVersion = 0;

	// 创建复合文档
	CPMFileStorageCtrl fileStorageCtrl;
	HRESULT hr = fileStorageCtrl.TryOpenStorage(strFileName);
	if(!SUCCEEDED(hr))
	{
		CString strTemMsg = _T("CPM_StorageDataSet::OpenMDocument打开失败,表名为空!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	bOpenResult = Stream2DataSet(fileStorageCtrl,FALSE);
	
	fileStorageCtrl.CloseStorage();
	return bOpenResult;
}
//
//BOOL CPM_StorageDataSet::SaveMDocument(CString strFileName)
//{
////	UIBase::CTiming timing(_T("CPM_StorageDataSet::SaveMDocument"),TRUE);
//	if(FALSE == m_IsModified)
//		return TRUE;
//
//	BOOL bSaveResult = TRUE;
//
//	if(_T("") == m_TableName)
//	{
//		CString strTemMsg = _T("CPM_StorageDataSet::SaveMDocument保存失败,表名为空!");
//		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
//		return FALSE;
//	}
//	_bstr_t sKeyName;
//	///////////////////////////////////
//	///liwenlang 
//	///打开复合文档
//	///[31/5/2010]
//	//////////////////////////////////
//	IStorage* piRootStr = NULL;
//	IStream* pDataSetStream = NULL;
//
//	LPVOID pDataSetAllInfo = NULL;
//	ULONG lDataSetAllByteSize = 0;
//
//	if(!DataSet2Buffer(pDataSetAllInfo,lDataSetAllByteSize))
//	{
//		CString strTemMsg = _T("CPM_StorageDataSet::SaveMDocument保存失败,DataSet2Buffer失败!");
//		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
//		return FALSE;
//	}
//
//	ULONG iDataSetAllByteWriteSize = 0;
//
//	_bstr_t sFileName = strFileName;
//	_bstr_t sTableName = m_TableName;
//	
//	{
////		UIBase::CTiming timing(_T("CPM_StorageDataSet::SaveMDocument::WriteInfo"),TRUE);
//		// 创建复合文档
//		HRESULT hr = StgOpenStorage(sFileName,NULL, STGM_DIRECT|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, NULL, NULL, &piRootStr);
//		if(!SUCCEEDED(hr))
//			hr = StgCreateDocfile(sFileName, STGM_DIRECT|STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, NULL, &piRootStr);
//		SysFreeString(sFileName);
//		COM_RESULT_OK(hr)
//			
//		hr = piRootStr->CreateStream(sTableName, STGM_DIRECT|STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, 0, 0, &pDataSetStream);
//		SysFreeString(sTableName);
//		if (!SUCCEEDED(hr))
//		{
//			CString strMsg;
//			strMsg.Format(_T("CPM_StorageDataSet::SaveMDocument,工程%s保存表%s失败!"),strFileName,m_TableName);
//			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
//			RELEASE_INTERFACE(piRootStr);
//			return FALSE;
//		}
//		
//		hr = pDataSetStream->Write(&lDataSetAllByteSize,sizeof(LONG),&iDataSetAllByteWriteSize);
//		hr = pDataSetStream->Write(pDataSetAllInfo,lDataSetAllByteSize,&iDataSetAllByteWriteSize);
//		RELEASE_INTERFACE(pDataSetStream);
//		RELEASE_INTERFACE(piRootStr);
//		if (!SUCCEEDED(hr))
//		{
//			CString strMsg;
//			strMsg.Format(_T("CPM_StorageDataSet::SaveMDocument,工程%s保存表%s写入数据失败!"),strFileName,m_TableName);
//			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
//			return FALSE;
//		}
//		
//		ResetUpdate();
//	}
//Com_Error_Exit:
//
//
//	if(NULL != pDataSetAllInfo)
//	{
//		delete []((BYTE*)pDataSetAllInfo);
//	}
//	return bSaveResult;
//}

BOOL CPM_StorageDataSet::SaveMDocument(CString strFileName)
{
	//	UIBase::CTiming timing(_T("CPM_StorageDataSet::SaveMDocument"),TRUE);
	if(FALSE == m_IsModified)
		return TRUE;

	BOOL bSaveResult = TRUE;

	if(_T("") == m_TableName)
	{
		CString strTemMsg = _T("CPM_StorageDataSet::SaveMDocument保存失败,表名为空!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	///////////////////////////////////
	///liwenlang 
	///打开复合文档
	///[31/5/2010]
	//////////////////////////////////
	ULONG iDataSetAllByteWriteSize = 0;

	{
		//		UIBase::CTiming timing(_T("CPM_StorageDataSet::SaveMDocument::WriteInfo"),TRUE);
		// 创建复合文档
		CPMFileStorageCtrl storageFileCtrl;
		HRESULT hr = storageFileCtrl.TryOpenStorage(strFileName);
		if (!SUCCEEDED(hr))
		{
			CString strMsg;
			strMsg.Format(_T("CPM_StorageDataSet::SaveMDocument,工程%s保存表%s失败!"),strFileName,m_TableName);
			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
			storageFileCtrl.CloseStorage();
			return FALSE;
		}

		bSaveResult = DataSet2Stream(storageFileCtrl);

		hr = storageFileCtrl.CloseStorage();
		if (!SUCCEEDED(hr))
		{
			CString strMsg;
			strMsg.Format(_T("CPM_StorageDataSet::SaveMDocument,工程%s保存表%s失败!同步失败"),strFileName,m_TableName);
			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
			storageFileCtrl.CloseStorage();
			return FALSE;
		}
		ResetUpdate();
	}
	return bSaveResult;
}

BOOL CPM_StorageDataSet::GetVersionID(short & iVersion)
{
	if(-1 == m_iVersion)
	{
		CString strMsg = _T("DataSet:") + this->GetTableName() + _T(" 没有设置版本!");
		CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
		return FALSE;
	}
	iVersion = m_iVersion;
	return TRUE;
}

void CPM_StorageDataSet::SetVersionID(short iVersion)
{
	m_iVersion = iVersion;
}

BOOL CPM_StorageDataSet::BuildFieldInfo(NDataFields * pNDataFields,LPVOID & pFieldInfo,int & iFieldSize)
{
	///////////////////////////////////
	///liwenlang 
	///生成Field信息
	///[19/6/2010]
	//////////////////////////////////
	CString strFieldInfo;
	for (int i = 0;i < pNDataFields->GetCount();i++)
	{
		NDataField * pTemField = pNDataFields->GetField(i);
		
		FieldType temFieldType = pTemField->GetDataType();
		int iTemFieldSize = pTemField->GetSize();
		if(ftString == temFieldType)
		{
			iTemFieldSize -= sizeof(_TCHAR);//去除结束符号 大小
			iTemFieldSize /= sizeof(_TCHAR);//对于UNICODE 要转化为字符个数，不是内存空间大小
		}

		CString strTemFieldName;
		if(0 == i)
			strTemFieldName.Format(_T("%s:%d:%d"),pTemField->GetFieldName(),temFieldType,iTemFieldSize);
		else
			strTemFieldName.Format(_T(";%s:%d:%d"),pTemField->GetFieldName(),temFieldType,iTemFieldSize);
		strFieldInfo += strTemFieldName;
	}

	////////////////////////////////////////////
	/// liwenlang [2011:10:8   10:33]
	/// 说明:
	/// 表头信息后 加入一个空格符号
	int iFieldInfoSize = (strFieldInfo.GetLength() + 1) * sizeof(TCHAR);
	iFieldSize = iFieldInfoSize + sizeof(long);
	pFieldInfo = new BYTE[iFieldSize];
	ZeroMemory(pFieldInfo,iFieldSize);
	CopyMemory(pFieldInfo,&iFieldInfoSize,sizeof(long));
	CopyMemory((BYTE*)pFieldInfo + sizeof(long),strFieldInfo.GetBuffer(0),iFieldInfoSize);
	return TRUE;
}

BOOL CPM_StorageDataSet::ReadRowsFStorage_DVersion(short iStructVer,LONG lDataSetBufferSize,LPVOID pFieldInfo,LPVOID pRowsInfo)
{
	///////////////////////////////////
	///liwenlang 
	///读取DataFieldInfo
	///[19/6/2010]
	//////////////////////////////////
	CPM_StorageDataSet * pNewDataSet = NULL;
	if(!BuildTemDataSet(m_iOpeningDataSetDataCode,pFieldInfo,pNewDataSet))
		return FALSE;
	if(!ReadRowsFStorage_SVersion(iStructVer,pNewDataSet,lDataSetBufferSize,pRowsInfo))
		return FALSE;

	int i = 1;
	for (i = 1;i <= pNewDataSet->GetRowCount();i++)
	{
		LPVOID pRowBuffer = this->AlloRecordBuffer();
		
		//开始重新设置标志2003.6.18
		NDataField *field = NULL;
		void *ptr;
		for(int j = 0 ; j < this->m_fields.GetCount(); j++)
		{
			field = this->m_fields.GetField(j);
			if (field != NULL) 
			{
				ptr = this->GetFieldDataPT(field,pRowBuffer);
				field->FreeOrigValueBuff(ptr,FALSE);
				field->SetItemModified((char*)ptr,FALSE);
			}
		}
		
		this->SetRowNewInternal(pRowBuffer,FALSE);
		this->SetRowModifiedInternal(pRowBuffer,FALSE);
		this->m_PrimaryBuffers->InsertBuff(pRowBuffer,-1);
	}

	for (i = 1;i <= pNewDataSet->GetRowCount();i++)
	{
		int iNewRow = i;
		for (int j = 0;j < pNewDataSet->m_fields.GetCount();j++)
		{
			NDataField * pTemField = pNewDataSet->m_fields.GetField(j);
			if(NULL == pTemField)
				continue;

			CString strTemFieldName=  pTemField->GetFieldName();
			FieldType fieldType = pTemField->GetDataType();

			if(ftInteger == fieldType)
			{
				int iValue = pNewDataSet->GetAsInteger(j,i);
				this->SetAsInteger(strTemFieldName,iNewRow,iValue);
			}
			else if(ftFloat == fieldType)
			{
				double dValue = pNewDataSet->GetAsFloat(j,i);
				this->SetAsFloat(strTemFieldName,iNewRow,dValue);
			}
			else if(ftBlob == fieldType)
			{
				LPVOID pTemBlob = NULL;
				LONG lBlobSize = pNewDataSet->GetBlob(j,i,pTemBlob);
				this->SetBlob(strTemFieldName,iNewRow,pTemBlob,lBlobSize);
			}
			else
			{
				CString strValue = pNewDataSet->GetAsString(j,i);
				this->SetAsString(strTemFieldName,iNewRow,strValue);
			}
		}
	}
	delete pNewDataSet;
	pNewDataSet = NULL;
	return TRUE;
}

BOOL CPM_StorageDataSet::ReadRowsFStorage_SVersion(short iStructVer,CPM_StorageDataSet * pDataSet,LONG lDataSetBufferSize,LPVOID pRowsInfo)
{
	int iRowCount = 0;
	CopyMemory(&iRowCount,pRowsInfo,sizeof(LONG));

	LPVOID pCurRowAllInfo = (BYTE*)pRowsInfo + sizeof(LONG);

	for (int i = 0;i < iRowCount;i++)
	{
		LONG lCurRowSize = 0;
		CopyMemory(&lCurRowSize,pCurRowAllInfo,sizeof(LONG));
		if(lDataSetBufferSize <= lCurRowSize || lCurRowSize <= 0)
		{
			CString strMsg;
			strMsg.Format(_T("CPM_StorageDataSet::ReadRowsFStorage_SVersion	Table:%s 在读取%d|%d行出现 问题 行Size:%d DataSet大小:%d"),pDataSet->GetTableName(),i,iRowCount,lCurRowSize,lDataSetBufferSize);
			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
			return FALSE;
		}

		LPVOID pCurRowInfo = (BYTE*)pCurRowAllInfo + sizeof(LONG);
		if(!ReadRowFStream_SVersion(iStructVer,pDataSet,pCurRowInfo,lCurRowSize))
			return FALSE;

		pCurRowAllInfo = (BYTE*)pCurRowAllInfo + lCurRowSize;
	}
	return TRUE;
}

BOOL CPM_StorageDataSet::ReadRowFStream_SVersion(short iStructVer,CPM_StorageDataSet * pDataSet,LPVOID pCurRowInfo,LONG lRowBufferSize)
{
	switch(iStructVer)
	{
	case 1:
	case 2:	
		return ReadRowFStream_SVersion_V1(iStructVer,pDataSet,pCurRowInfo);
		break;
	}
	///////////////////////////////////
	///liwenlang 
	///复制 普通 字段类型
	///[1/6/2010]
	//////////////////////////////////
	LPVOID pRowBuffer = pDataSet->AlloRecordBuffer();
	int iFieldCount = pDataSet->m_fields.GetCount();
	LPVOID pStreamBuffer = (BYTE*)pCurRowInfo;
	int i = 0;
	for (i = 0;i < iFieldCount;i++)
	{
		NDataField * pField = pDataSet->m_fields.GetField(i);
		if(ftBlob == pField->GetDataType())
		{
			void *pRowbufferOffset = pDataSet->GetFieldDataPT(pField,pRowBuffer,FALSE);
			ZeroMemory(pRowbufferOffset,sizeof(void*));
			ZeroMemory((char*)pRowbufferOffset+sizeof(void*),sizeof(long));

			long lSize;
			CopyMemory(&lSize,pStreamBuffer,sizeof(long));
			if(lRowBufferSize < lSize || lSize < 0)
			{
				CDataSetErrorInfo::Instance()->PrintDataSetInfo(pDataSet,pRowBuffer,pField);
				CString strTemMsg;
				strTemMsg.Format(_T("CPM_StorageDataSet::ReadRowFStream_SVersion	BlobFieldSize:%d大于RowBufferSize:%d"),lSize,lRowBufferSize);
				CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
				return FALSE;
			}

			LPVOID pCurBlob = (BYTE*)pStreamBuffer + sizeof(long);

			/********************************************************************
			liwenlang
				时间:2010:9:29   16:04
				备注:根据编码方式转换编码 lSize等于0 不用处理。
			*********************************************************************/
			if(FALSE == pField->IsUpdateAsBlobFiled() && lSize > 0)
			{//只处理 字符串的 Blob
				switch(m_iOpeningDataSetDataCode)
				{
				case 0://A2W
					{
						LONG lTemWSize = (lSize << 1);

						PWCHAR pTemWInfo = new WCHAR[lSize + 1];
						memset(pTemWInfo,0,lTemWSize+1);

						ATLA2WHELPER(pTemWInfo, (PCHAR)pCurBlob, lSize + 1, CP_ACP);

						pField->SetBlob(pRowbufferOffset,pTemWInfo,((int)wcslen(pTemWInfo) << 1),FALSE);

						delete []pTemWInfo;
					}
					break;
				case 1://W2A
					{
						PWCHAR pTemWInfo = new WCHAR[lSize + 1];
						memset(pTemWInfo,0,(lSize + 1) * sizeof(WCHAR));
						CopyMemory(pTemWInfo,pCurBlob,lSize);

						long lASize = lSize + 2;
						PCHAR pTemAInfo = new CHAR[lASize];
						memset(pTemAInfo,0,lASize);
 
						ATLW2AHELPER(pTemAInfo, (WCHAR*)pTemWInfo, lASize, CP_ACP);

						pField->SetBlob(pRowbufferOffset,pTemAInfo,(int)strlen(pTemAInfo),FALSE);

						delete []pTemWInfo;
						delete []pTemAInfo;
					}
					break;
				default:
					pField->SetBlob(pRowbufferOffset,pCurBlob,lSize,FALSE);
					break;
				}
			}
			else
				pField->SetBlob(pRowbufferOffset,pCurBlob,lSize,FALSE);


			///////////////////////////////////
			///liwenlang 
			///将指针 移动到 下个位置
			///[1/6/2010]
			//////////////////////////////////
			pStreamBuffer = (BYTE*)pStreamBuffer + sizeof(long) + sizeof(WCHAR) + lSize;
		}
		else if(ftString == pField->GetDataType())
		{
			void *pRowbufferOffset = pDataSet->GetFieldDataPT(pField,pRowBuffer,FALSE);
			long lSize;
			CopyMemory(&lSize,pStreamBuffer,sizeof(long));

			if(lRowBufferSize < lSize)
			{
				CDataSetErrorInfo::Instance()->PrintDataSetInfo(pDataSet,pRowBuffer,pField);
				CString strTemMsg;
				strTemMsg.Format(_T("CPM_StorageDataSet::ReadRowFStream_SVersion	StringFieldSize:%d大于RowBufferSize:%d"),lSize,lRowBufferSize);
				CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
				return FALSE;
			}

			LPVOID pTemFieldValue = (BYTE*)pStreamBuffer + sizeof(long);

			/********************************************************************
			liwenlang
				时间:2010:9:29   16:04
				备注:根据编码方式转换编码
			*********************************************************************/
			switch(m_iOpeningDataSetDataCode)
			{
			case 0://A2W
				{
					LONG lTemWSize = (lSize << 1);
					PWCHAR pTemWInfo = new WCHAR[lSize];
					memset(pTemWInfo,0,lTemWSize);

					ATLA2WHELPER(pTemWInfo, (PCHAR)pTemFieldValue, lSize, CP_ACP);

					CopyMemory(pRowbufferOffset,pTemWInfo,(wcslen(pTemWInfo) << 1));

					delete []pTemWInfo;
				}
				break;
			case 1://W2A
				{
					PCHAR pTemAInfo = new CHAR[lSize];
					memset(pTemAInfo,0,lSize);

					ATLW2AHELPER(pTemAInfo, (WCHAR*)pTemFieldValue, lSize, CP_ACP);

					CopyMemory(pRowbufferOffset,pTemAInfo,strlen(pTemAInfo));

					delete []pTemAInfo;
				}
			    break;
			default:
			CopyMemory(pRowbufferOffset,pTemFieldValue,lSize);
			    break;
			}

			///////////////////////////////////
			///liwenlang 
			///将指针 移动到 下个位置
			///[1/6/2010]
			//////////////////////////////////
			pStreamBuffer = (BYTE*)pStreamBuffer + sizeof(long) + lSize;
		}
		else
		{
			int iTemFieldSize = pField->GetSize();
			void *pRowbufferOffset = pDataSet->GetFieldDataPT(pField,pRowBuffer,FALSE);
			ZeroMemory(pRowbufferOffset,iTemFieldSize + m_fields.GetFieldInfoSize());

			CopyMemory(pRowbufferOffset,pStreamBuffer,iTemFieldSize);

			pStreamBuffer = (BYTE*)pStreamBuffer + iTemFieldSize;
		}
	}
	
	//开始重新设置标志2003.6.18
	NDataField *field = NULL;
	void *ptr;
	for(i = 0 ; i < pDataSet->m_fields.GetCount(); i++)
	{
		field = pDataSet->m_fields.GetField(i);
		if (field != NULL) 
		{
			ptr = pDataSet->GetFieldDataPT(field,pRowBuffer);
			field->FreeOrigValueBuff(ptr,FALSE);
			field->SetItemModified((char*)ptr,FALSE);
		}
	}
	
	pDataSet->SetRowNewInternal(pRowBuffer,FALSE);
    pDataSet->SetRowModifiedInternal(pRowBuffer,FALSE);
	pDataSet->m_PrimaryBuffers->InsertBuff(pRowBuffer,-1);
	return TRUE;
}

void CPM_StorageDataSet::SplitCString(CString source, CStringArray& dest, TCHAR division)
{
	dest.RemoveAll();
	int finddivision = source.Find(division);
	if (finddivision == -1) //没有找到
	{
		dest.Add(source);
		return;
	}
	int oldFind = 0;
	while (finddivision >= 0)
	{
		dest.Add(source.Mid(oldFind,finddivision - oldFind));
		oldFind = finddivision + 1;
		finddivision = source.Find(division,oldFind);
	}
	//处理最后一种情况
	if (source.GetLength() >0 && oldFind>=0 && source.GetLength() > (oldFind))
	{
		dest.Add(source.Mid(oldFind,source.GetLength() - oldFind));
	}
}

BOOL CPM_StorageDataSet::BuildTemDataSet(int DataCode,LPVOID pFieldInfo, CPM_StorageDataSet *&pNewDataSet)
{
	if(NULL == pFieldInfo)
	{
		CString strTemMsg = _T("CPM_StorageDataSet::BuildTemDataSet,参数pFieldInfo没有初始化失败!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}

	LONG iFieldSize = 0;
	CopyMemory(&iFieldSize,pFieldInfo,sizeof(LONG));
	
	CString sFieldInfo;
	if (iFieldSize >0)
	{
		LPTSTR buffer = NULL;

		BYTE * pFieldBufferP = (BYTE*)pFieldInfo + sizeof(LONG);

		switch(DataCode)
		{
		case 0://A2W
			{
				buffer = sFieldInfo.GetBufferSetLength(iFieldSize);

				LONG lTemWSize = (iFieldSize << 1);
				
				PCHAR pTemAInfo = new CHAR[iFieldSize + 1];
				memset(pTemAInfo,0,iFieldSize + 1);
				memcpy(pTemAInfo,pFieldBufferP,iFieldSize);

				PWCHAR pTemWInfo = new WCHAR[iFieldSize + 1];
				memset(pTemWInfo,0,lTemWSize+1);
				
				ATLA2WHELPER(pTemWInfo, pTemAInfo, iFieldSize + 1, CP_ACP);
				
				lTemWSize = (long)wcslen(pTemWInfo) * sizeof(TCHAR);

// 				if(lTemWSize != iFieldSize)
// 				{
// 					CString strTemMsg;
// 					strTemMsg.Format(_T("CPM_StorageDataSet::BuildTemDataSet,A2W Field长度不一致%d %d!"),\
// 						lTemWSize,iFieldSize);
// 					CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
// 					return FALSE;
// 				}
				memcpy(buffer,pTemWInfo,lTemWSize);
				
				delete [] pTemAInfo;
				delete []pTemWInfo;
			}
			break;
		case 1://W2A
			{	
				LPSTR pAstr = CPub::PM_W2A((WCHAR*)pFieldBufferP, iFieldSize);

				int iNewASize = (int)strlen(pAstr);

				buffer = sFieldInfo.GetBufferSetLength(iNewASize);

				//if(iNewASize != (iFieldSize / sizeof(WCHAR)))
				//{
				//	CString strTemMsg;
				//	strTemMsg.Format(_T("CPM_StorageDataSet::BuildTemDataSet,W2A Field长度不一致%d %d!"),\
				//		iNewASize,iFieldSize / sizeof(WCHAR));
				//	CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
				//	return FALSE;
				//}
				memcpy(buffer,pAstr,iNewASize);
				
				delete []pAstr;
			}
			break;
		default:
			buffer = sFieldInfo.GetBufferSetLength(iFieldSize / sizeof(TCHAR));
			memcpy(buffer,pFieldBufferP,iFieldSize);
			break;
		}
	}

	CStringArray strBigFieldInfoArr;
	SplitCString(sFieldInfo,strBigFieldInfoArr,_T(';'));
	pNewDataSet = new CPM_StorageDataSet;
	BOOL bHasError = FALSE;
	int i = 0;
	for (i = 0;i < strBigFieldInfoArr.GetSize();i++)
	{
		CStringArray strTemFieldArr;
		SplitCString(strBigFieldInfoArr.GetAt(i),strTemFieldArr,_T(':'));
		if(strTemFieldArr.GetSize() != 3)
		{
			bHasError = TRUE;
			break;
		}
		CString strFieldName = strTemFieldArr.GetAt(0);
		int iFieldType = _ttoi(strTemFieldArr.GetAt(1));
		int iFieldSizeTemp = _ttoi(strTemFieldArr.GetAt(2));

		pNewDataSet->AddField(strFieldName,(FieldType)iFieldType,iFieldSizeTemp);
	}

	////////////////////////////////////////////
	/// liwenlang [2012:5:25   8:59]
	/// 说明:
	/// 在生成TemDataSet时，需要考虑 Blob字段在 二进制时的 特殊情况。
	NDataFields * pTemFields = this->GetFieldsObj();
	for (i = 0;i < pTemFields->GetCount();i++)
	{
		NDataField * pTemField = pTemFields->GetField(i);
		if(pTemField->IsUpdateAsBlobFiled())
		{
			pNewDataSet->SetUpdateAsBlobFiled(pTemField->GetFieldName(),TRUE);
		}
	}
	return TRUE;
}

BOOL CPM_StorageDataSet::DataSet2Stream(CPMFileStorageCtrl & storageFileCtrl)
{
	CPMFileStreamCtrl fileStreamCtrl;
	HRESULT hr = fileStreamCtrl.TryOpenStream(&storageFileCtrl,m_TableName);
	if (!SUCCEEDED(hr))
	{
		CString strMsg;
		strMsg.Format(_T("StorgeFileCtrl::WriteTableInfo,创建表%s失败!"),m_TableName);	
		CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
		return FALSE;
	}

	short iCurVersion = 0;
	if(!GetVersionID(iCurVersion))
	{
		fileStreamCtrl.CloseStream();
		return FALSE;
	}

	// 	if(_T("") == m_TableName)
	// 		return FALSE;

	if(0 >= GetFieldCount())
	{
		fileStreamCtrl.CloseStream();
		CString strTemMsg = _T("CPM_StorageDataSet::DataSet2Buffer,表格没有初始化列信息!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}

	int i = 0;

	//防止filter中的数据没有被更新2008.5.8
	const int filterBufferCount = CopyFilterToPrimaryBuffer();
	
	int iPrimaryRowCount = GetRowCount();
	
	///////////////////////////////////
	///liwenlang 
	///生成FieldInfo
	///[6/7/2010]
	//////////////////////////////////
	LPVOID pFieldInfo = NULL;
	int iFieldInfoSize = 0;
	if(!BuildFieldInfo(&m_fields,pFieldInfo,iFieldInfoSize))
	{
		fileStreamCtrl.CloseStream();
		CString strTemMsg = _T("CPM_StorageDataSet::DataSet2Buffer,创建表格头失败!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	
	int iDataSetHeadByteSize = 0;
	int iDataSetAllByteSize = 0;
	
	iDataSetHeadByteSize += iFieldInfoSize;
	iDataSetHeadByteSize += 3 * sizeof(short) + sizeof(long);//加上StructVersion：2个字节 DataVersion：2个字节 编码方式：2个字节 DataSet行数:4个字节
	
	iDataSetAllByteSize += iDataSetHeadByteSize;

	LPVOID pDataSetHeadInfo = new BYTE[iDataSetHeadByteSize];
	ZeroMemory(pDataSetHeadInfo,iDataSetHeadByteSize);

	CopyMemory(pDataSetHeadInfo,&m_iStructDataVersion,sizeof(short));//写入结构版本
	CopyMemory((BYTE*)pDataSetHeadInfo + sizeof(short),&m_iVersion,sizeof(short));//写入版本
	CopyMemory((BYTE*)pDataSetHeadInfo + sizeof(short) * 2,pFieldInfo,iFieldInfoSize);//写入FieldInfo
	if(NULL != pFieldInfo)
	{
		delete []((BYTE*)pFieldInfo);
		pFieldInfo = NULL;
	}

	short iCodeMode;
#ifdef UNICODE
	iCodeMode = 1;
	CopyMemory((BYTE*)pDataSetHeadInfo + iFieldInfoSize + sizeof(short) * 2,&iCodeMode,sizeof(short));
#else
	iCodeMode = 2;
	CopyMemory((BYTE*)pDataSetHeadInfo + iFieldInfoSize + sizeof(short) * 2,&iCodeMode,sizeof(short));
#endif

	LPVOID pDataRowsInfo = (BYTE*)pDataSetHeadInfo + iFieldInfoSize + 3 * sizeof(short);
	CopyMemory(pDataRowsInfo,&iPrimaryRowCount,sizeof(LONG));

	ULONG iDataSetAllByteWriteSize = 0;
	hr = fileStreamCtrl.WriteStream(&iDataSetAllByteSize,sizeof(LONG));
	if (!SUCCEEDED(hr))
	{
		fileStreamCtrl.CloseStream();
		CString strTemMsg;
		strTemMsg.Format(_T("StorgeFileCtrl::WriteTableInfo,保存表%s失败!"),m_TableName);	
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}

	hr = fileStreamCtrl.WriteStream(pDataSetHeadInfo,iDataSetHeadByteSize);
	if (!SUCCEEDED(hr))
	{
		fileStreamCtrl.CloseStream();
		CString strTemMsg;
		strTemMsg.Format(_T("StorgeFileCtrl::WriteTableInfo,保存表%s失败!"),m_TableName);	
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	delete [](BYTE*)pDataSetHeadInfo;

	///////////////////////////////////
	///liwenlang 
	///生成Rows 信息
	///[6/7/2010]
	//////////////////////////////////
	for (i = 0;i < iPrimaryRowCount;i++)
	{
		LPVOID pRowInfo = NULL;
		long lRowSize = 0;
		if(!BuildSimRowBuffer(i + 1,pRowInfo,lRowSize,NDataModelDataSet::Primary))
		{
			///////////////////////////////////
			///liwenlang 
			///要记录异常
			///[31/5/2010]
			//////////////////////////////////
			continue;
		}
		hr = fileStreamCtrl.WriteStream(pRowInfo,lRowSize);
		delete [](BYTE*)pRowInfo;
		
		if (!SUCCEEDED(hr))
		{
			fileStreamCtrl.CloseStream();
			CString strTemMsg;
			strTemMsg.Format(_T("StorgeFileCtrl::WriteTableInfo,保存表%s失败!"),m_TableName);	
			CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
			return FALSE;
		}
		
		iDataSetAllByteSize += lRowSize;
	}
	////////////////////////////////////////////
	/// liwenlang [2012:5:16   11:22]
	/// 说明:
	/// 将游标移回开始位置 写入 总长度
	LARGE_INTEGER iStreamPos;
	iStreamPos.QuadPart = 0;
	fileStreamCtrl.SeekStream(iStreamPos,STREAM_SEEK_SET,NULL);
	hr = fileStreamCtrl.WriteStream(&iDataSetAllByteSize,sizeof(LONG));
	if (!SUCCEEDED(hr))
	{
		fileStreamCtrl.CloseStream();
		CString strTemMsg;
		strTemMsg.Format(_T("StorgeFileCtrl::WriteTableInfo,保存表%s失败!"),m_TableName);	
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	
	fileStreamCtrl.CloseStream();

	RemoveCopyedFilterToPrimaryBuffer(filterBufferCount);
	return TRUE;
}

BOOL CPM_StorageDataSet::DataSet2Buffer(LPVOID & pBuffer,ULONG & lBufferSize)
{
	short iCurVersion = 0;
	if(!GetVersionID(iCurVersion))
		return FALSE;

// 	if(_T("") == m_TableName)
// 		return FALSE;

	if(0 >= GetFieldCount())
	{
		CString strTemMsg = _T("CPM_StorageDataSet::DataSet2Buffer,表格没有初始化列信息!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	
	int i = 0;
	
	//防止filter中的数据没有被更新2008.5.8
	const int filterBufferCount = CopyFilterToPrimaryBuffer();
	
	LPVOID pFieldInfo = NULL;
	int iFieldInfoSize = 0;
	LPVOID * pRowsInfo = NULL;
	int * piRowsCount = NULL;

	LPVOID pDataSetAllInfo = NULL;
	int iDataSetAllByteSize = 0;

	int iPrimaryRowCount = GetRowCount();
	
	{
//		UIBase::CTiming timing(_T("CPM_StorageDataSet::SaveMDocument::BuildInfo"),TRUE);
		///////////////////////////////////
		///liwenlang 
		///生成FieldInfo
		///[6/7/2010]
		//////////////////////////////////
		if(!BuildFieldInfo(&m_fields,pFieldInfo,iFieldInfoSize))
		{
			CString strTemMsg = _T("CPM_StorageDataSet::DataSet2Buffer,创建表格头失败!");
			CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
			return FALSE;
		}
		
		iDataSetAllByteSize += iFieldInfoSize;
		iDataSetAllByteSize += 3 * sizeof(short) + sizeof(long);//加上StructVersion：2个字节 DataVersion：2个字节 编码方式：2个字节
		//DataSet行数
		
		///////////////////////////////////
		///liwenlang 
		///生成Rows 信息
		///[6/7/2010]
		//////////////////////////////////
		pRowsInfo = new LPVOID[iPrimaryRowCount];
		piRowsCount = new int[iPrimaryRowCount];
		for (i = 0;i < iPrimaryRowCount;i++)
		{
			LPVOID pRowInfo = NULL;
			long lRowSize = 0;
			if(!BuildSimRowBuffer(i + 1,pRowInfo,lRowSize,NDataModelDataSet::Primary))
			{
				///////////////////////////////////
				///liwenlang 
				///要记录异常
				///[31/5/2010]
				//////////////////////////////////
				continue;
			}
			pRowsInfo[i] = pRowInfo;
			piRowsCount[i] = lRowSize;
			
			iDataSetAllByteSize += lRowSize;
		}
		
		pDataSetAllInfo = new BYTE[iDataSetAllByteSize];
		ZeroMemory(pDataSetAllInfo,iDataSetAllByteSize);
// 		int iDataSetInfoSize = iDataSetAllByteSize;
// 		CopyMemory(pDataSetAllInfo,&iDataSetInfoSize,sizeof(long));
		
		LPVOID pDataVesion = (BYTE*)pDataSetAllInfo;
		CopyMemory(pDataVesion,&m_iStructDataVersion,sizeof(short));//写入结构版本
		
		CopyMemory((BYTE*)pDataVesion + sizeof(short),&m_iVersion,sizeof(short));//写入版本
		
		CopyMemory((BYTE*)pDataVesion + sizeof(short) * 2,pFieldInfo,iFieldInfoSize);//写入FieldInfo
		
		short iCodeMode;
#ifdef UNICODE
		iCodeMode = 1;
		CopyMemory((BYTE*)pDataVesion + iFieldInfoSize + sizeof(short) * 2,&iCodeMode,sizeof(short));
#else
		iCodeMode = 2;
		CopyMemory((BYTE*)pDataVesion + iFieldInfoSize + sizeof(short) * 2,&iCodeMode,sizeof(short));
#endif
		
		LPVOID pDataRowsInfo = (BYTE*)pDataVesion + iFieldInfoSize + 3 * sizeof(short);
		CopyMemory(pDataRowsInfo,&iPrimaryRowCount,sizeof(LONG));
		
		pDataRowsInfo = (BYTE*)pDataRowsInfo + sizeof(LONG);
		
		for (i = 0;i < iPrimaryRowCount;i++)
		{
			CopyMemory(pDataRowsInfo,pRowsInfo[i],piRowsCount[i]);
			
			pDataRowsInfo = (BYTE*)pDataRowsInfo + piRowsCount[i];
		}
		
	}
	pBuffer = pDataSetAllInfo;
	lBufferSize = iDataSetAllByteSize;

	RemoveCopyedFilterToPrimaryBuffer(filterBufferCount);

	if(NULL != pFieldInfo)
	{
		delete []((BYTE*)pFieldInfo);
	}
	for (i = 0;i < iPrimaryRowCount;i++)
	{
		delete []((BYTE*)pRowsInfo[i]);
	}
	delete []pRowsInfo;
	delete []piRowsCount;
	return TRUE;
}


BOOL CPM_StorageDataSet::Stream2DataSet(CPMFileStorageCtrl & fileStorageCtrl,BOOL bAppend)
{
	if(FALSE == bAppend)
	{
		this->Reset();
	}

	BOOL bSaveResult = TRUE;
	short iCurVersion = 0; 
	if(!GetVersionID(iCurVersion))
	{
		CString strTemMsg = _T("CPM_StorageDataSet::Stream2DataSet,获取版本失败!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}

	// 	if(_T("") == m_TableName)
	// 		return FALSE;

	if(0 >= GetFieldCount())
	{
		CString strTemMsg = _T("CPM_StorageDataSet::Stream2DataSet,表格列保存失败!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}

	int i = 0;
	CPMFileStreamCtrl fileStreamCtrl;
	HRESULT hr = fileStreamCtrl.TryOpenStream(&fileStorageCtrl,m_TableName); 
	if (!SUCCEEDED(hr))
	{
		return TRUE;
	}

	ULONG iDatSetAllBufferSize = 0;//DataSet整个buffer的大小
	hr = fileStreamCtrl.ReadStream(&iDatSetAllBufferSize,sizeof(LONG));
	if (!SUCCEEDED(hr) || iDatSetAllBufferSize <= 0)
	{
		fileStreamCtrl.CloseStream();

		return TRUE;
	}

	int iHeadBufferSize = sizeof(short) + sizeof(short) + sizeof(LONG);//StructVersion(2Byte) + DataVersion（2Byte）+字段信息大小
	LPVOID pHeadBufferInfo = new BYTE[iHeadBufferSize];
	memset(pHeadBufferInfo,0,iHeadBufferSize);
	hr = fileStreamCtrl.ReadStream(pHeadBufferInfo,iHeadBufferSize);
	if (!SUCCEEDED(hr))
	{
		fileStreamCtrl.CloseStream();

		CString strTemMsg;
		strTemMsg.Format(_T("CPM_StorageDataSet::Stream2DataSet,表格：%s ，表格头信息读取失败!"),m_TableName);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}

	/********************************************************************
	liwenlang
	时间:2010:9:30   11:00
	备注:读取结构版本号
	*********************************************************************/
	short iCurStructVersion = 0;
	CopyMemory(&iCurStructVersion,pHeadBufferInfo,sizeof(short));

	BOOL bOpenDataSetStatus = FALSE;
	switch(iCurStructVersion)
	{
	case 1:
	case 2:	
		bOpenDataSetStatus = Stream2DataSet_V1(iCurStructVersion,iDatSetAllBufferSize,fileStreamCtrl);
		break;
	default:
		{
			LPVOID pTemHeadBufferInfo = (BYTE*)pHeadBufferInfo + sizeof(short);

			short iVersion = 0;
			CopyMemory(&iVersion,(BYTE*)pTemHeadBufferInfo,sizeof(short));//读取版本号

			ULONG lFieldInfoSize = 0;
			CopyMemory(&lFieldInfoSize,(BYTE*)pTemHeadBufferInfo + sizeof(short),sizeof(LONG));

			if(lFieldInfoSize >= iDatSetAllBufferSize)
			{
				CString strTemMsg;
				strTemMsg.Format(_T("CPM_StorageDataSet::Buffer2DataSet,表格长度%d大于流长度%d!"),lFieldInfoSize,iDatSetAllBufferSize);
				CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
				return FALSE;
			}

			int iHeadBuffer2Size = lFieldInfoSize + sizeof(short);
			LPVOID pHeadBuffer2Info = new BYTE[iHeadBuffer2Size];
			memset(pHeadBuffer2Info,0,iHeadBuffer2Size);
			hr = fileStreamCtrl.ReadStream(pHeadBuffer2Info,iHeadBuffer2Size);
			if (!SUCCEEDED(hr))
			{
				fileStreamCtrl.CloseStream();

				CString strTemMsg;
				strTemMsg.Format(_T("CPM_StorageDataSet::Stream2DataSet,表格：%s ，表格头2信息读取失败!"),m_TableName);
				CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
				return FALSE;
			}

			short iCodeMode = 0;//编码方式
			CopyMemory(&iCodeMode,(BYTE*)pHeadBuffer2Info + lFieldInfoSize,sizeof(short));

			SetOpeningDataSetDataCode(iCodeMode,m_iOpeningDataSetDataCode);

			if(iCurVersion != iVersion)
			{
				LPVOID pTemTableInfo = new BYTE[lFieldInfoSize + sizeof(LONG)];
				memset(pTemTableInfo,0,lFieldInfoSize + sizeof(LONG));
				memcpy(pTemTableInfo,&lFieldInfoSize,sizeof(LONG));
				memcpy((BYTE*)pTemTableInfo + sizeof(LONG),pHeadBuffer2Info,lFieldInfoSize);

				//处理不同版本问题
				bOpenDataSetStatus = ReadRowsFStorage_DVersion_Stream(iCurStructVersion,iDatSetAllBufferSize,pTemTableInfo,fileStreamCtrl);
				
				delete [](BYTE*)pTemTableInfo;
			}
			else
			{
				//版本一致的情况
				bOpenDataSetStatus = ReadRowsFStorage_SVersion_Stream(iCurStructVersion,this,iDatSetAllBufferSize,fileStreamCtrl);
			}

			delete [](BYTE*)pHeadBuffer2Info;
			pHeadBuffer2Info = NULL;
		}
	}
	delete [](BYTE*)pHeadBufferInfo;
	pHeadBufferInfo = NULL;

	fileStreamCtrl.CloseStream();

	/********************************************************************
	liwenlang
	时间:2010:9:29   15:50
	备注:将编码版本 重置
	*********************************************************************/
	m_iOpeningDataSetDataCode = -1;
	return bOpenDataSetStatus;
}

BOOL CPM_StorageDataSet::Stream2DataSet_V1(short iStructVer,ULONG lDataSetAllBufferSize,CPMFileStreamCtrl & fileStreamCtrl)
{	
	short iCurVersion = 0; 
	if(!GetVersionID(iCurVersion))
		return FALSE;

	LARGE_INTEGER preMove;
	preMove.QuadPart = 0;
	ULARGE_INTEGER curStreamPos;
	fileStreamCtrl.SeekStream(preMove,STREAM_SEEK_SET,&curStreamPos);

	int iHeadBufferSize = sizeof(short) + sizeof(LONG);//版本号（2BYTE） + 表格头长度（4BYTE）
	LPVOID pHeadBufferInfo = new BYTE[iHeadBufferSize];
	memset(pHeadBufferInfo,0,iHeadBufferSize);
	HRESULT hr = fileStreamCtrl.ReadStream(pHeadBufferInfo,iHeadBufferSize);
	if (!SUCCEEDED(hr))
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPM_StorageDataSet::Stream2DataSet_V1,表格：%s ，表格头信息读取失败!"),m_TableName);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}

	short iVersion = 0;
	CopyMemory(&iVersion,(BYTE*)pHeadBufferInfo,sizeof(short));//读取版本号

	ULONG lFieldInfoSize = 0;
	CopyMemory(&lFieldInfoSize,(BYTE*)pHeadBufferInfo + sizeof(short),sizeof(LONG));

	if(lFieldInfoSize >= lDataSetAllBufferSize)
	{
		delete [](BYTE*)pHeadBufferInfo;
		pHeadBufferInfo = NULL;

		CString strTemMsg;
		strTemMsg.Format(_T("Table:%s,DatSet展开失败,FieldSize：%d大于DataSet总长:%d"),GetTableName(),lFieldInfoSize,lDataSetAllBufferSize);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg,CDataSetErrorInfo::DataSetError);
		return FALSE;
	}

	int iHeadBuffer2Size = lFieldInfoSize + sizeof(short);//
	LPVOID pHeadBuffer2Info = new BYTE[iHeadBuffer2Size];
	memset(pHeadBuffer2Info,0,iHeadBuffer2Size);
	hr = fileStreamCtrl.ReadStream(pHeadBuffer2Info,iHeadBuffer2Size);
	if (!SUCCEEDED(hr))
	{
		delete [](BYTE*)pHeadBufferInfo;
		pHeadBufferInfo = NULL;
		delete [](BYTE*)pHeadBuffer2Info;
		pHeadBuffer2Info = NULL;

		CString strTemMsg;
		strTemMsg.Format(_T("CPM_StorageDataSet::Stream2DataSet_V1,表格：%s ，表格头2信息读取失败!"),m_TableName);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}

	short iCodeMode = 0;//编码方式
	CopyMemory(&iCodeMode,(BYTE*)pHeadBuffer2Info + lFieldInfoSize,sizeof(short));

	SetOpeningDataSetDataCode(iCodeMode,m_iOpeningDataSetDataCode);

	BOOL bDataSetOpenStatus = FALSE;
	if(iCurVersion != iVersion)
	{
		LPVOID pTemTableInfo = new BYTE[lFieldInfoSize + sizeof(LONG)];
		memset(pTemTableInfo,0,lFieldInfoSize + sizeof(LONG));
		memcpy(pTemTableInfo,&lFieldInfoSize,sizeof(LONG));
		memcpy((BYTE*)pTemTableInfo + sizeof(LONG),pHeadBuffer2Info,lFieldInfoSize);

		//处理不同版本问题
		bDataSetOpenStatus = ReadRowsFStorage_DVersion_Stream(iStructVer,lDataSetAllBufferSize,pTemTableInfo,fileStreamCtrl);

		delete [](BYTE*)pTemTableInfo;
	}
	else
	{
		//版本一致的情况
		bDataSetOpenStatus = ReadRowsFStorage_SVersion_Stream(iStructVer,this,lDataSetAllBufferSize,fileStreamCtrl);
	}
	delete [](BYTE*)pHeadBufferInfo;
	pHeadBufferInfo = NULL;
	delete [](BYTE*)pHeadBuffer2Info;
	pHeadBuffer2Info = NULL;

	return bDataSetOpenStatus;
}

BOOL CPM_StorageDataSet::ReadRowsFStorage_SVersion_Stream(short iStructVer,CPM_StorageDataSet * pDataSet,LONG lDataSetBufferSize,CPMFileStreamCtrl & fileStreamCtrl)
{
	int iRowCount = 0;
	HRESULT hr = fileStreamCtrl.ReadStream(&iRowCount,sizeof(LONG));
	if(!SUCCEEDED(hr))
	{
		CString strTemMsg;
		strTemMsg.Format(_T("CPM_StorageDataSet::ReadRowsFStorage_SVersion_Stream,表格：%s ，读取行数失败!"),m_TableName);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}

	for (int i = 0;i < iRowCount;i++)
	{
		LONG lCurRowSize = 0;
		hr = fileStreamCtrl.ReadStream(&lCurRowSize,sizeof(LONG));
		if(!SUCCEEDED(hr))
		{
			CString strTemMsg;
			strTemMsg.Format(_T("CPM_StorageDataSet::ReadRowsFStorage_SVersion_Stream,表格：%s ，读取单行 大小失败（%d/%d）!"),m_TableName,i,iRowCount);
			CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
			return FALSE;
		}

		if(lDataSetBufferSize <= lCurRowSize || lCurRowSize <= 0)
		{
			CString strMsg;
			strMsg.Format(_T("CPM_StorageDataSet::ReadRowsFStorage_SVersion	Table:%s 在读取%d|%d行出现 问题 行Size:%d DataSet大小:%d"),pDataSet->GetTableName(),i,iRowCount,lCurRowSize,lDataSetBufferSize);
			CDataSetErrorInfo::Instance()->PrintInfo(strMsg);
			return FALSE;
		}

		lCurRowSize -= sizeof(LONG);//当前行 大小，包括了 描述大小本身的 4个字节。
		LPVOID pCurRowInfo = new BYTE[lCurRowSize];
		memset(pCurRowInfo,0,lCurRowSize);
		hr = fileStreamCtrl.ReadStream(pCurRowInfo,lCurRowSize);
		if(!SUCCEEDED(hr))
		{
			CString strTemMsg;
			strTemMsg.Format(_T("CPM_StorageDataSet::ReadRowsFStorage_SVersion_Stream,表格：%s ，读取单行内容失败!"),m_TableName);
			CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
			return FALSE;
		}

		BOOL bTemRunMode = ReadRowFStream_SVersion(iStructVer,pDataSet,pCurRowInfo,lCurRowSize);
		delete [](BYTE*)pCurRowInfo;
		if(FALSE == bTemRunMode)
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CPM_StorageDataSet::ReadRowsFStorage_DVersion_Stream(short iStructVer,LONG lDataSetBufferSize,LPVOID pFieldInfo,CPMFileStreamCtrl & fileStreamCtrl)
{
	///////////////////////////////////
	///liwenlang 
	///读取DataFieldInfo
	///[19/6/2010]
	//////////////////////////////////
	CPM_StorageDataSet * pNewDataSet = NULL;
	if(!BuildTemDataSet(m_iOpeningDataSetDataCode,pFieldInfo,pNewDataSet))
		return FALSE;
	if(!ReadRowsFStorage_SVersion_Stream(iStructVer,pNewDataSet,lDataSetBufferSize,fileStreamCtrl))
		return FALSE;

	int i = 1;
	for (i = 1;i <= pNewDataSet->GetRowCount();i++)
	{
		LPVOID pRowBuffer = this->AlloRecordBuffer();

		//开始重新设置标志2003.6.18
		NDataField *field = NULL;
		void *ptr;
		for(int j = 0 ; j < this->m_fields.GetCount(); j++)
		{
			field = this->m_fields.GetField(j);
			if (field != NULL) 
			{
				ptr = this->GetFieldDataPT(field,pRowBuffer);
				field->FreeOrigValueBuff(ptr,FALSE);
				field->SetItemModified((char*)ptr,FALSE);
			}
		}

		this->SetRowNewInternal(pRowBuffer,FALSE);
		this->SetRowModifiedInternal(pRowBuffer,FALSE);
		this->m_PrimaryBuffers->InsertBuff(pRowBuffer,-1);
	}

	for (i = 1;i <= pNewDataSet->GetRowCount();i++)
	{
		int iNewRow = i;
		for (int j = 0;j < pNewDataSet->m_fields.GetCount();j++)
		{
			NDataField * pTemField = pNewDataSet->m_fields.GetField(j);
			if(NULL == pTemField)
				continue;

			CString strTemFieldName=  pTemField->GetFieldName();
			FieldType fieldType = pTemField->GetDataType();

			if(ftInteger == fieldType)
			{
				int iValue = pNewDataSet->GetAsInteger(j,i);
				this->SetAsInteger(strTemFieldName,iNewRow,iValue);
			}
			else if(ftFloat == fieldType)
			{
				double dValue = pNewDataSet->GetAsFloat(j,i);
				this->SetAsFloat(strTemFieldName,iNewRow,dValue);
			}
			else if(ftBlob == fieldType)
			{
				LPVOID pTemBlob = NULL;
				LONG lBlobSize = pNewDataSet->GetBlob(j,i,pTemBlob);
				this->SetBlob(strTemFieldName,iNewRow,pTemBlob,lBlobSize);
			}
			else
			{
				CString strValue = pNewDataSet->GetAsString(j,i);
				this->SetAsString(strTemFieldName,iNewRow,strValue);
			}
		}
	}
	delete pNewDataSet;
	pNewDataSet = NULL;
	return TRUE;
}

BOOL CPM_StorageDataSet::Buffer2DataSet(LPVOID pBuffer,ULONG lBufferSize,BOOL bAppend)
{
	if(FALSE == bAppend)
	{
		this->Reset();
	}
	if(NULL == pBuffer)
		return TRUE;
	
	BOOL bSaveResult = TRUE;
	short iCurVersion = 0; 
	if(!GetVersionID(iCurVersion))
	{
		CString strTemMsg = _T("CPM_StorageDataSet::Buffer2DataSet,获取版本失败!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}

// 	if(_T("") == m_TableName)
// 		return FALSE;

	if(0 >= GetFieldCount())
	{
		CString strTemMsg = _T("CPM_StorageDataSet::Buffer2DataSet,表格列保存失败!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}
	
	int i = 0;
// 	CArray<NDataField*,NDataField*&> pkFieldArr;
// 	for (i = 0;i < m_fields.GetCount();i++)
// 	{
// 		NDataField * pNDataField = m_fields.GetField(i);
// 		if(pNDataField->IsPK())
// 			pkFieldArr.Add(pNDataField);
// 	}

	LPVOID pDataSetAllInfo = pBuffer;
	ULONG lDataSetAllSize = lBufferSize;
	if (NULL == pDataSetAllInfo)
	{
		CString strTemMsg = _T("CPM_StorageDataSet::Buffer2DataSet,参数pBuffer没有初始化失败!");
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
		return FALSE;
	}

	/********************************************************************
	liwenlang
		时间:2010:9:30   11:00
		备注:读取结构版本号
	*********************************************************************/
	short iCurStructVersion = 0;
	CopyMemory(&iCurStructVersion,pDataSetAllInfo,sizeof(short));
	
	BOOL bOpenDataSetStatus = FALSE;
	switch(iCurStructVersion)
	{
	case 1:
	case 2:	
		bOpenDataSetStatus = Buffer2DataSet_V1(iCurStructVersion,pDataSetAllInfo,lDataSetAllSize);
		break;
	default:
		{
			pDataSetAllInfo = (BYTE*)pDataSetAllInfo + sizeof(short);

			short iVersion = 0;
			CopyMemory(&iVersion,(BYTE*)pDataSetAllInfo,sizeof(short));//读取版本号

			LPVOID pFieldInfo = (BYTE*)pDataSetAllInfo + sizeof(short);
			ULONG lFieldInfoSize = 0;
			CopyMemory(&lFieldInfoSize,pFieldInfo,sizeof(LONG));

			if(lFieldInfoSize >= lBufferSize)
			{
				CString strTemMsg;
				strTemMsg.Format(_T("CPM_StorageDataSet::Buffer2DataSet,表格长度%d大于流长度%d!"),lFieldInfoSize,lBufferSize);
				CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
				return FALSE;
			}

			short iCodeMode = 0;//编码方式
			CopyMemory(&iCodeMode,(BYTE*)pFieldInfo + lFieldInfoSize + sizeof(LONG),sizeof(short));

			SetOpeningDataSetDataCode(iCodeMode,m_iOpeningDataSetDataCode);

			LPVOID pRowsInfo = (BYTE*)pFieldInfo + lFieldInfoSize + sizeof(LONG) + sizeof(short);
// 			if(lFieldInfoSize >= lDataSetAllSize)
// 			{//指针访问可能越界
// 				return FALSE;
// 			}
			if(iCurVersion != iVersion)
			{
				//处理不同版本问题
				bOpenDataSetStatus = ReadRowsFStorage_DVersion(iCurStructVersion,lDataSetAllSize,pFieldInfo,pRowsInfo);
			}
			else
			{
				//版本一致的情况
				bOpenDataSetStatus = ReadRowsFStorage_SVersion(iCurStructVersion,this,lDataSetAllSize,pRowsInfo);
			}
		}
	}

	/********************************************************************
	liwenlang
		时间:2010:9:29   15:50
		备注:将编码版本 重置
	*********************************************************************/
	m_iOpeningDataSetDataCode = -1;
	return bOpenDataSetStatus;
}

BOOL CPM_StorageDataSet::Buffer2DataSet_V1(short iStructVer,LPVOID pDataSetAllInfo,ULONG lDataSetAllSize)
{
	short iCurVersion = 0; 
	if(!GetVersionID(iCurVersion))
		return FALSE;

	short iVersion = 0;
	CopyMemory(&iVersion,(BYTE*)pDataSetAllInfo,sizeof(short));//读取版本号

	LPVOID pFieldInfo = (BYTE*)pDataSetAllInfo + sizeof(short);
	ULONG lFieldInfoSize = 0;
	CopyMemory(&lFieldInfoSize,pFieldInfo,sizeof(LONG));
	
	if(lFieldInfoSize >= lDataSetAllSize)
	{
		CString strTemMsg;
		strTemMsg.Format(_T("Table:%s,DatSet展开失败,FieldSize：%d大于DataSet总长:%d"),GetTableName(),lFieldInfoSize,lDataSetAllSize);
		CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg,CDataSetErrorInfo::DataSetError);
		return FALSE;
	}

	short iCodeMode = 0;//编码方式
	CopyMemory(&iCodeMode,(BYTE*)pFieldInfo + lFieldInfoSize + sizeof(LONG),sizeof(short));
	
	SetOpeningDataSetDataCode(iCodeMode,m_iOpeningDataSetDataCode);

	LPVOID pRowsInfo = (BYTE*)pFieldInfo + lFieldInfoSize + sizeof(LONG) + sizeof(short);
// 	if(lFieldInfoSize >= lDataSetAllSize)
// 	{//指针访问可能越界
// 		return FALSE;
// 	}

	BOOL bDataSetOpenStatus = FALSE;
 	if(iCurVersion != iVersion)
 	{
 		//处理不同版本问题
 		bDataSetOpenStatus = ReadRowsFStorage_DVersion(iStructVer,lDataSetAllSize,pFieldInfo,pRowsInfo);
 	}
 	else
 	{
 		//版本一致的情况
 		bDataSetOpenStatus = ReadRowsFStorage_SVersion(iStructVer,this,lDataSetAllSize,pRowsInfo);
 	}
	return bDataSetOpenStatus;
}

BOOL CPM_StorageDataSet::ReadRowFStream_SVersion_V1(short iStructVer,CPM_StorageDataSet * pDataSet,LPVOID pCurRowInfo)
{
	///////////////////////////////////
	///liwenlang 
	///复制 普通 字段类型
	///[1/6/2010]
	//////////////////////////////////
	LPVOID pRowBuffer = pDataSet->AlloRecordBuffer();
	int iFieldCount = pDataSet->m_fields.GetCount();
	LPVOID pStreamBuffer = (BYTE*)pCurRowInfo;
	int i = 0;
	for (i = 0;i < iFieldCount;i++)
	{
		NDataField * pField = pDataSet->m_fields.GetField(i);
		if(ftBlob == pField->GetDataType())
		{
			void *pRowbufferOffset = pDataSet->GetFieldDataPT(pField,pRowBuffer,FALSE);
			ZeroMemory(pRowbufferOffset,sizeof(void*));
			ZeroMemory((char*)pRowbufferOffset+sizeof(void*),sizeof(long));

			long lSize;
			CopyMemory(&lSize,pStreamBuffer,sizeof(long));

			LPVOID pCurBlob = (BYTE*)pStreamBuffer + sizeof(long);

			/********************************************************************
			liwenlang
				时间:2010:9:29   16:04
				备注:根据编码方式转换编码
			*********************************************************************/
			if(FALSE == pField->IsUpdateAsBlobFiled())
			{//只处理 字符串的 Blob
				switch(m_iOpeningDataSetDataCode)
				{
				case 0://A2W
					{
						LONG lTemWSize = ((lSize + 1) << 1);
						PCHAR pTemAInfo = new CHAR[lSize + 1];
						memset(pTemAInfo,0,lSize + 1);

						CopyMemory(pTemAInfo,pCurBlob,lSize);

						PWCHAR pTemWInfo = new WCHAR[lSize + 1];
						memset(pTemWInfo,0,lTemWSize);

						ATLA2WHELPER(pTemWInfo, (PCHAR)pTemAInfo, lSize + 1, CP_ACP);

						pField->SetBlob(pRowbufferOffset,pTemWInfo,((long)wcslen(pTemWInfo) << 1),FALSE);

						delete []pTemWInfo;
						delete [] pTemAInfo;
					}
					break;
				case 1://W2A
					{
						PWCHAR pTemWInfo = new WCHAR[(lSize << 1) + 1];
						memset(pTemWInfo,0,lSize + 2);

						CopyMemory(pTemWInfo,pCurBlob,lSize);

						PCHAR pTemAInfo = new CHAR[lSize + 1];
						memset(pTemAInfo,0,lSize + 1);

						ATLW2AHELPER(pTemAInfo, (WCHAR*)pTemWInfo, lSize, CP_ACP);

						pField->SetBlob(pRowbufferOffset,pTemAInfo,(long)strlen(pTemAInfo),FALSE);

						delete []pTemAInfo;
						delete []pTemWInfo;
					}
					break;
				default:
					pField->SetBlob(pRowbufferOffset,pCurBlob,lSize,FALSE);
					break;
				}
			}
			else
				pField->SetBlob(pRowbufferOffset,pCurBlob,lSize,FALSE);


			///////////////////////////////////
			///liwenlang 
			///将指针 移动到 下个位置
			///[1/6/2010]
			//////////////////////////////////
			pStreamBuffer = (BYTE*)pStreamBuffer + sizeof(long) + lSize;
		}
		else if(ftString == pField->GetDataType())
		{
			void *pRowbufferOffset = pDataSet->GetFieldDataPT(pField,pRowBuffer,FALSE);
			long lSize;
			CopyMemory(&lSize,pStreamBuffer,sizeof(long));

			LPVOID pTemFieldValue = (BYTE*)pStreamBuffer + sizeof(long);

			/********************************************************************
			liwenlang
				时间:2010:9:29   16:04
				备注:根据编码方式转换编码
			*********************************************************************/
			switch(m_iOpeningDataSetDataCode)
			{
			case 0://A2W
				{
					LONG lTemWSize = (lSize << 1);
					PWCHAR pTemWInfo = new WCHAR[lSize];
					memset(pTemWInfo,0,lTemWSize);

					ATLA2WHELPER(pTemWInfo, (PCHAR)pTemFieldValue, lSize, CP_ACP);

					CopyMemory(pRowbufferOffset,pTemWInfo,(wcslen(pTemWInfo) << 1));

					delete []pTemWInfo;
				}
				break;
			case 1://W2A
				{
					PCHAR pTemAInfo = new CHAR[lSize];
					memset(pTemAInfo,0,lSize);

					ATLW2AHELPER(pTemAInfo, (WCHAR*)pTemFieldValue, lSize, CP_ACP);

					CopyMemory(pRowbufferOffset,pTemAInfo,strlen(pTemAInfo));

					delete []pTemAInfo;
				}
			    break;
			default:
				CopyMemory(pRowbufferOffset,pTemFieldValue,lSize);
			    break;
			}

			///////////////////////////////////
			///liwenlang 
			///将指针 移动到 下个位置
			///[1/6/2010]
			//////////////////////////////////
			pStreamBuffer = (BYTE*)pStreamBuffer + sizeof(long) + lSize;
		}
		else
		{
			int iTemFieldSize = pField->GetSize();
			void *pRowbufferOffset = pDataSet->GetFieldDataPT(pField,pRowBuffer,FALSE);
			ZeroMemory(pRowbufferOffset,iTemFieldSize + m_fields.GetFieldInfoSize());

			CopyMemory(pRowbufferOffset,pStreamBuffer,iTemFieldSize);

			pStreamBuffer = (BYTE*)pStreamBuffer + iTemFieldSize;
		}
	}
	
	//开始重新设置标志2003.6.18
	NDataField *field = NULL;
	void *ptr;
	for(i = 0 ; i < pDataSet->m_fields.GetCount(); i++)
	{
		field = pDataSet->m_fields.GetField(i);
		if (field != NULL) 
		{
			ptr = GetFieldDataPT(field,pRowBuffer);
			field->FreeOrigValueBuff(ptr,FALSE);
			field->SetItemModified((char*)ptr,FALSE);
		}
	}
	
	pDataSet->SetRowNewInternal(pRowBuffer,FALSE);
    pDataSet->SetRowModifiedInternal(pRowBuffer,FALSE);
	pDataSet->m_PrimaryBuffers->InsertBuff(pRowBuffer,-1);
	return TRUE;
}

void CPM_StorageDataSet::DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff)
{
	NDataModelDataSet::DeleteRowInternal(row,pBuff);
}

void CPM_StorageDataSet::SetOpeningDataSetDataCode(int iCodeMode,short & iOpeningDataCode)
{
#ifdef UNICODE
	switch(iCodeMode)
	{
	case 1://DataSet为Unicode
		iOpeningDataCode = -1;
	    break;
	case 2://DataSet为Acsii
		iOpeningDataCode = 0;
		break;	
	default:
		iOpeningDataCode = -1;
	    break;
	}
#else
	switch(iCodeMode)
	{
	case 1://DataSet为Unicode
		iOpeningDataCode = 1;
	    break;
	case 2://DataSet为Acsii
		iOpeningDataCode = -1;
		break;	
	default:
		iOpeningDataCode = -1;
	    break;
	}
#endif
}
