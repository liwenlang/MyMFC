// NDataModelBase.cpp: implementation of the NDataModelBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NDataModelBase.h"
#include "NDataField.h"
#include "NDataFields.h"
#include "NDataModelDataSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
///CDataSetErrorInfo
CDataSetErrorInfo * CDataSetErrorInfo::m_pInstance = NULL;

CDataSetErrorInfo::CDataSetErrorInfo()
{
	m_strErrorFilePath = _T("C:\\Temp\\");
}

CDataSetErrorInfo::~CDataSetErrorInfo()
{

}

CString CDataSetErrorInfo::GetErrorFilePath()
{
	return m_strErrorFilePath;
}

void CDataSetErrorInfo::SetErrorFilePath(CString strFilePath)
{
	m_strErrorFilePath = strFilePath;
	if(m_strErrorFilePath.GetLength() > 1)
	{
		if(_T("\\") != m_strErrorFilePath.Right(1) )
		{
			m_strErrorFilePath += _T("\\");
		}
	}
}

BOOL CDataSetErrorInfo::PrintInfo(CString strErrorMsg,DWORD error)
{
	CFile cFile;
	CFileException ex;
	CString strFileName = GetErrorFilePath() + _T("error.log");
	OFSTRUCT sFile;

#ifdef UNICODE
	USES_CONVERSION;
	LPCSTR pTmp = NULL;
	pTmp = W2A(strFileName.GetBuffer(0) );
#else
	LPCSTR pTmp = NULL;	
	pTmp = strFileName.GetBuffer(0);
#endif

	if(::OpenFile(/*strFileName.GetBuffer(0)*/pTmp,&sFile,OF_EXIST) == HFILE_ERROR)
	{
		if(!cFile.Open(strFileName, CFile::modeCreate, &ex))
		{
			TCHAR szError[255];
			ex.GetErrorMessage(szError, 255);
			return FALSE;
		}
		cFile.Close();
	}

	if(!cFile.Open(strFileName, CFile::modeWrite, &ex)) {
		TCHAR szError[255];
		ex.GetErrorMessage(szError, 255);
		return FALSE;
	}	
	DWORD dfileLength = (DWORD)cFile.SeekToEnd();

	CTime time=CTime::GetCurrentTime();
	int Year = time.GetYear();
	int Mouth = time.GetMonth();
	int Day = time.GetDay();
	int hour=time.GetHour();
	int min=time.GetMinute();
	int sec=time.GetSecond();

	CString strMsg;
	strMsg.Format(_T("%d.%d.%d %d:%d:%d...%s,%u \r\n"),Year,Mouth,Day,hour,min,sec,strErrorMsg,error);

	LPCSTR pMsgInfo = NULL;
	pMsgInfo = T2A(strMsg.GetBuffer(0) );

	cFile.Write(pMsgInfo,(UINT)strlen(pMsgInfo));
	cFile.Close();
	return TRUE;	
}

BOOL CDataSetErrorInfo::PrintDataSetInfo(NDataModelDataSet * pDataSet,int iRow,NDataField * pField)
{
	NDataFields * pDataFields = pDataSet->GetFieldsObj();

	CString strTemTableName = pDataSet->GetTableName();
	CString strErrorInfo = _T("Table:") + strTemTableName + _T("	");
	for (int i = 0;i < pDataFields->GetCount();i++)
	{
		CString strTemErr;
		NDataField * pTemField = pDataFields->GetField(i);
		CString strFieldName = pTemField->GetFieldName();
		if(pTemField == pField)
		{
			strTemErr.Format(_T("在Field：%s出错！"),strFieldName);
			break;
		}
		else
		{
			CString strFieldValue = pDataSet->GetAsString(strFieldName,iRow);
			strTemErr.Format(_T("在Field：%sFieldValue:%s;"),strFieldName,strFieldValue);
		}
		strErrorInfo += strTemErr;
	}
	PrintInfo(strErrorInfo,CDataSetErrorInfo::DataSetError);
	return TRUE;
}

BOOL CDataSetErrorInfo::PrintDataSetInfo(NDataModelDataSet * pDataSet,NRecordID pRowBuffer,NDataField * pField)
{
	NDataFields * pDataFields = pDataSet->GetFieldsObj();

	CString strTemTableName = pDataSet->GetTableName();
	CString strErrorInfo = _T("Table:") + strTemTableName + _T("	");
	for (int i = 0;i < pDataFields->GetCount();i++)
	{
		CString strTemErr;
		NDataField * pTemField = pDataFields->GetField(i);
		CString strFieldName = pTemField->GetFieldName();
		if(pTemField == pField)
		{
			strTemErr.Format(_T("在Field：%s出错！"),strFieldName);
			break;
		}
		else
		{
			CString strFieldValue /*= pTemField->GetAsString(pRowBuffer)*/;
			strTemErr.Format(_T("在Field：%sFieldValue:%s;"),strFieldName,strFieldValue);
		}
		strErrorInfo += strTemErr;
	}
	PrintInfo(strErrorInfo,CDataSetErrorInfo::DataSetError);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//NDataModelBase
NDataModelBase::NDataModelBase()
{

}

NDataModelBase::~NDataModelBase()
{

}

void NDataModelBase::AttachObserver( NObserver *ob )
{
	m_ObserverList.AddTail(ob);
}
	
void NDataModelBase::DetachObserver( NObserver *ob )
{
    POSITION pos = m_ObserverList.Find(ob);
	if (pos != NULL)
	    m_ObserverList.RemoveAt(pos);
}

void NDataModelBase::NotifyObserver(LPARAM lHint, CObject*pHint)
{
	POSITION pos;
	NObserver *ob;
   for( pos = m_ObserverList.GetHeadPosition(); pos != NULL; )
   {
      ob = (NObserver *)m_ObserverList.GetNext( pos );
	  ob->UpdateObserver(this, 0, pHint);
   }

}
