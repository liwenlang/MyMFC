// Timing.cpp: implementation of the CTiming class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Timing.h"

CTiming::CTiming(const CString &strFunName, bool bAddUp, const CString &strBelong/* = QUAN_JU_TIMING*/,
				 bool bIsTatolTimeRecord/* = false*/)
:m_strKey(strFunName), m_bAddUp(bAddUp),m_strBelong(strBelong),m_bIsTatolTimeRecord(bIsTatolTimeRecord)
{
	m_bHasCount = FALSE;

#ifdef INSERT_TIME_CODE
	m_BeginTime = GetTickCount();
#endif 
	
}

CTiming::~CTiming()
{
	CountTime();
}

BOOL CTiming::CountTime()
{
	if (m_bHasCount == FALSE)
	{
#ifdef INSERT_TIME_CODE

		//���ۼ�ʱ��
		m_EndTime = GetTickCount();

		//�õ�������
		DWORD result = m_EndTime - m_BeginTime;

		CRecord::AddRecord(m_strBelong, m_strKey, result, m_bAddUp,m_bIsTatolTimeRecord);
#endif 
	}
	
	m_bHasCount = TRUE;
	return TRUE;
}

DWORD CRecord::s_nTotalCount = 0;
CRecord::Str_TableMap CRecord::s_AllTable;
// ��������������һ����¼
BOOL CRecord::AddRecord(const CString &strTableKey, const CString &strFunKey, DWORD Time, bool bAddUp, bool bIsTatolTimeRecord)
{
	try
	{

		BOOL function_return = FALSE;
		//parameter		
		if (strTableKey.IsEmpty() == TRUE
			||strFunKey.IsEmpty() == TRUE)
		{
			return FALSE;
		}
		//++s_nTotalCount;
		//end parameter
		Str_TableMap::iterator tableIter = s_AllTable.find(strTableKey);
		if (tableIter != s_AllTable.end() )
		{
			Str_FunInfoMap &aTable = tableIter->second;
			Str_FunInfoMap::iterator funIter = aTable.find(strFunKey);
			if (funIter != aTable.end() )
			{
				FunInfo_T &aFunInfo = funIter->second;
				if (bAddUp != true)//�������ۼ�
				{
					//ȡ���ֵ
					if(aFunInfo.nTotalTime < Time)
					{
						aFunInfo.nTotalTime = Time;
					}
				}
				else//�ۼ�
				{
					aFunInfo.nCount += 1;
					aFunInfo.nTotalTime += Time;
				}

				//ͬһ����������ǰ��Ҫ�����ͳ�ƻ��߲�����ͳ�Ʋ�ͬ!
				if (aFunInfo.bAddUp != bAddUp)
				{
					CString strPromp;
					strPromp += _T("���⺯��:");
					strPromp += strFunKey;
					strPromp += _T("�Ƿ����ͳ��ǰ��Ҫ��һ��!");
					//OutPutMessage(strPromp);
				}
			}
			else//����һ���µĺ�����¼
			{
				FunInfo_T funInfo(1,Time, bAddUp,bIsTatolTimeRecord);
				aTable.insert(std::make_pair(strFunKey, funInfo) );
			}
		}
		else//�����±�
		{
			Str_FunInfoMap aTable;
			FunInfo_T funInfo(1,Time, bAddUp,bIsTatolTimeRecord);
			aTable.insert(std::make_pair(strFunKey, funInfo) );
			s_AllTable.insert(std::make_pair(strTableKey, aTable) );

			function_return = TRUE;
		}

		return function_return;
	}
	catch(...)
	{
		AfxMessageBox(_T("\n FILE:Timing 2020/7/22 9:00"));

		return FALSE;
	}
}

//ͳ��һ�ű��д���,��ʱ��, ƽ��ʱ��
void CRecord::CountTable(Str_FunInfoMap &aTable, DWORD &nTotalCs, DWORD &nTotalTime, DWORD &nPjsj)
{
	nTotalCs = nTotalTime = nPjsj = 0;

	//�Ƿ���ڱ�ʾȫ��ʱ��ļ�¼
	bool bHasTotalRecord = false;
	FunInfo_T aTotalTimeFunInfo;

	for (Str_FunInfoMap::iterator funIter=aTable.begin(); funIter != aTable.end(); ++funIter)
	{
		FunInfo_T &aFunInfo = funIter->second;
		//�����ȫ��ʱ��,��ô�������ۼ�
		if (aFunInfo.bIsTatolTimeRecord == true)
		{
			bHasTotalRecord = true;
			aTotalTimeFunInfo = aFunInfo;
			continue;
		}

		nTotalCs += aFunInfo.nCount;
		nTotalTime += aFunInfo.nTotalTime;
	}

	if (nTotalCs != 0 )//��ƽ��ʱ��,ʹ�ú������ۼ�ʱ������ۼƴ���
	{
		nPjsj = nTotalTime / nTotalCs;
	}

	if (bHasTotalRecord == true)
	{
		//������ڴ���ȫ��ʱ��ļ�¼,����ü�¼Ϊ��ʱ��
		nTotalTime = aTotalTimeFunInfo.nTotalTime;
	}
}

//ͳ�ƶ��ű��д���,��ʱ��, ƽ��ʱ��
void CRecord::CountTable(Str_TableMap &TableArr, DWORD &nTotalCs, DWORD &nTotalTime, DWORD &nPjsj)
{
	nTotalCs = nTotalTime = nPjsj = 0;

	for (Str_TableMap::iterator tableIter = TableArr.begin(); tableIter != TableArr.end(); ++tableIter)
	{
		Str_FunInfoMap &aTable = tableIter->second;
		DWORD aCs = 0;
		DWORD aTime = 0;
		DWORD aPj = 0;
		CRecord::CountTable(aTable, aCs, aTime, aPj);
		nTotalCs += aCs;
		nTotalTime += aTime;
	}

	if (nTotalCs != 0)//��ƽ��ʱ��
	{
		nPjsj = nTotalTime / nTotalCs;
	}
}



//������м�¼
void CRecord::ClearAll()
{
	s_AllTable.clear();
	//s_nTotalCount = 0;
}

//���ָ��������м�¼
void CRecord::ClearTable(const CString &strTableKey)
{
	Str_TableMap::iterator tableIter = s_AllTable.find(strTableKey);
	if (tableIter != s_AllTable.end() )
	{
// 		Table_T &table = tableIter->second;
// 		DWORD nCount = 0;//������������м��εĲ���
// 		for(Table_T::iterator funIter = table.begin(); funIter != table.end(); ++funIter)
// 		{
// 			FunInfo_T &funInfo = funIter->second;
// 			nCount += funInfo.nCount;
// 		}
//		s_nTotalCount -= nCount;//��ȥ������еĲ��Դ���
		s_AllTable.erase(tableIter);
	}
}

//�õ��ܱ�ͷ
BOOL CRecord::GetHeadInfo(CString &strZbt)
{
	try
	{
		//BOOL function_return=FALSE;
		//parameter		
		
		//end parameter
		//��ӡʱ��
		//CTime time = CTime::GetCurrentTime();
		//CString strNowTime = time.Format(_T("��������<������>:%Y-%m-%d,ʱ��<ʱ����>:%H:%M:%S"));

		CTime t = CTime::GetCurrentTime();
		int nYear=t.GetYear();
		int nMonth=t.GetMonth();
		int nDay = t.GetDay();
		int nHour =t.GetHour();
		int nMin=t.GetMinute();
		int nSec=t.GetSecond();
		CString strNowTime;
		strNowTime.Format(_T("��������<������>:%04d��%02d��%02d�� %02d:%02d:%02d"),nYear,nMonth,nDay,nHour,nMin,nSec);
	
		strZbt = _T("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \r\n");
		strZbt += strNowTime;
		return TRUE;
	}
	catch(...)
	{
		AfxMessageBox(_T("\n FILE:Timing 2020/7/22 9:00"));

		return FALSE;
	}
}



COutPutMessage::COutPutMessage()
{
	::CreateDirectory(_T("C:\\Temp\\"),NULL);
	m_strFilePath = _T("C:\\Temp\\timing.log"); 
};


//�����Ϣ
BOOL COutPutMessage::PrintRecord()
{
	try
	{
		BOOL function_return=FALSE;
		//parameter		
		if (CTimingSet::Instance()->IsSuperadditionMode() == FALSE)
		{
			//��׷��ģʽ,���Ŀ������
			Clear();
		}
		//end parameter
		//�ֱ����
		//�ܱ�ͷ:ʱ��,����
		CString strZbt;
		if (CRecord::GetHeadInfo(strZbt) == TRUE)
		{
			AddOneLine(strZbt);
		}

		//ȡ���ܵ�:����,ʱ��,ƽ��ʱ��
		DWORD nAllTotalCs = 0;//�ܱ��ܴ���
		DWORD nAllTotalTime = 0;//�ܱ���ʱ��
		DWORD nAllPjsj = 0;//�ܱ�ƽ��ʱ��
		CRecord::CountTable(CRecord::s_AllTable, nAllTotalCs, nAllTotalTime, nAllPjsj);

		for (CRecord::Str_TableMap::iterator tableIter = CRecord::s_AllTable.begin(); tableIter!=CRecord::s_AllTable.end();
		++tableIter)
		{
			//�ֱ�ͷ:����
			CString strTableName = tableIter->first;
			CString strBt = _T("<<�ֱ�>>:");
			strBt += strTableName;
			AddOneLine(strBt);

			CRecord::Str_FunInfoMap &aTable = tableIter->second;
			//��ͳ��һ�ű�:
			DWORD nOneTotalCs = 0;//�ֱ���ʱ��
			DWORD nOneTotalTime = 0;//�ֱ���ʱ��
			DWORD nOnePjsj = 0;//�ֱ�ƽ��ʱ��
			CRecord::CountTable(aTable, nOneTotalCs, nOneTotalTime, nOnePjsj);
			const CString str = _T("%") ; 
			for (CRecord::Str_FunInfoMap::iterator funIter = aTable.begin(); funIter != aTable.end(); ++funIter)
			{
				//��ӡ��������
				CString strFunName = funIter->first;
				CRecord::FunInfo_T &aFunInfo = funIter->second;
				CString strFun ;
				if (aFunInfo.bIsTatolTimeRecord == false)
				{
					strFun =_T("@����@:");
				}
				else
				{
					strFun = _T("@��ǰ��ʱ��ͳ���ܺ���@:");
				}
				strFun += strFunName;
				AddOneLine(strFun);

				CString strInfo;
				if (aFunInfo.bAddUp == true)// �ۼ�
				{
					if (aFunInfo.nCount == 0)
					{
// 						OutPutMessage(_T("��0����!"));
// 						THROW_ERROR;
						return FALSE;
					}
					DWORD dEve = aFunInfo.nTotalTime / aFunInfo.nCount;

					double dFbBfb = 0.0;//ռ�ֱ�ʱ��ٷֱ�
					if (nOneTotalTime != 0)
					{
						dFbBfb = (double) aFunInfo.nTotalTime  / nOneTotalTime;
						dFbBfb *= 100.0;
					}
					double dZbBfb = 0.0;//ռ�ܱ�ʱ��ٷֱ�
					if (nAllTotalTime != 0)
					{
						dZbBfb = (double)aFunInfo.nTotalTime / nAllTotalTime;
						dZbBfb *= 100.0;
					}

				//�����ͳ����,�������,����ʱ��
							
				strInfo.Format(_T("ͳ��:�ۼƴ���:<%d>��, �ۼ�ʱ��:<%d>����, ƽ��ʱ��:<%d>����, �ֱ�ٷֱ�:<%.1f%s>,�ܱ�ٷֱ�:<%.1f%s>"), 
						aFunInfo.nCount, aFunInfo.nTotalTime, dEve, dFbBfb, str , dZbBfb , str );

				}
				else
				{
					//����Ƿ�ͳ����,����������ʱ��
					strInfo.Format(_T("��������:�ʱ��<%d>����"), aFunInfo.nTotalTime);
				}
				
				AddOneLine(strInfo);
			}

			// >>�ֱ����<<: ����: ʱ��: ƽ��ʱ��: �ܱ�ٷֱ�:
			double dFbHz = 0.0;//�ֱ�Ļ��ܰٷֱ�
			if (nAllTotalTime != 0)
			{
				dFbHz = (double)nOneTotalTime / nAllTotalTime;
				dFbHz *= 100.0;
			}
			CString strFbHz;
			strFbHz.Format(_T(">>�ֱ����<<:����:<%d>��,  ʱ��:<%d>����, ƽ��ʱ��:<%d>����, �ܱ�ٷֱ�:<%.1f%s>"),
				nOneTotalCs, nOneTotalTime, nOnePjsj, dFbHz ,str );
			AddOneLine(strFbHz);

			CString strFG = _T("==========================================================");
				//����ֱ�ָ����
			AddOneLine(strFG);

		}

		//>>�ܱ����<<:����: ʱ��:  ƽ��ʱ��: 
		CString strHz;
		strHz.Format(_T(">>�ܱ����<<:����:<%d>��,ʱ��:<%d>����, ƽ��ʱ��:<%d>����"),
			nAllTotalCs, nAllTotalTime, nAllPjsj);
		AddOneLine(strHz);

		Show();

		return TRUE;
	}
	catch(...)
	{
		AfxMessageBox(_T("\n FILE:Timing 2020/7/22 9:00"));
		return FALSE;
	}
}

//���Ŀ���������
BOOL COutPutMessage::Clear()
{
	try
	{
		BOOL function_return=TRUE;

		::DeleteFile(m_strFilePath);
		return function_return;
	}
	catch(...)
	{
		AfxMessageBox(_T("\n FILE:Timing 2020/7/22 9:00"));
		return FALSE;
	}
}

BOOL COutPutMessage::AppendFileContent(const CString & strFilePath,const CString & strAppendInfo)
{
	CFile fFile;	
	if (! fFile.Open(strFilePath,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		return FALSE;

	//��֤ȫ��ת��Ϊchar������
	USES_CONVERSION;
	LPSTR pBuff = T2A(((CString)strAppendInfo).GetBuffer(0));//������Ҫ���unicode
	UINT  len = (UINT)strlen(pBuff); 	
	
	fFile.SeekToEnd();
	fFile.Write(pBuff, len);
	fFile.Close();
	
	//delete []pBuff;
	return TRUE;
}

//����һ��
BOOL COutPutMessage::AddOneLine(const CString &strOneLineContext)
{
	try
	{
		BOOL function_return=FALSE;
		
		CString strInfo = _T("\r\n");
		strInfo = strInfo + strOneLineContext + _T("\r\n");
		function_return = AppendFileContent(m_strFilePath,strInfo);

		return function_return;
	}
	catch(...)
	{
		AfxMessageBox(_T("\n FILE:Timing 2020/7/22 9:00"));
		return FALSE;
	}
}

void COutPutMessage::Show()
{
	ShellExecute( NULL, _T("open"), m_strFilePath, NULL, NULL ,SW_RESTORE );
}

const CString CTimingSet::s_strAppName = _T("TimingSet");//
const CString CTimingSet::s_strAdditionMode = _T("SuperadditionMode");//׷��ģʽ
const CString CTimingSet::s_strBoolDefault = _T("0");//����ֵĬ��
const int CTimingSet::s_iBufSize = 100;

// ������������Ϣ����Ƿ���׷��ģʽ,�������,��ô�ϴε������Ŀ������ݻᱻ���.
BOOL CTimingSet::IsSuperadditionMode()
{
	try
	{
		BOOL function_return=FALSE;
		//parameter		
		//end parameter
		int nGet = 0;
		if (GetKeyAndValue(s_strAdditionMode, nGet) == TRUE)
		{
			if (nGet != 0)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		return function_return;
	}
	catch(...)
	{
		AfxMessageBox(_T("\n FILE:Timing 2020/7/22 9:00"));
		return FALSE;
	}
}

//����׷��ģʽ
BOOL CTimingSet::SetSuperadditionMode(BOOL bSuperaddition)
{
	try
	{
		BOOL function_return=FALSE;
		//parameter		
		
		//end parameter
		function_return = SetKeyAndValue(s_strAdditionMode, bSuperaddition);

		return function_return;
	}
	catch(...)
	{
		AfxMessageBox(_T("\n FILE:Timing 2020/7/22 9:00"));
		return FALSE;
	}
}

//���ú�ȡ��ֵ
BOOL CTimingSet::SetKeyAndValue(const CString &strKey, int nValue)
{
	try
	{
		BOOL function_return=FALSE;
		//parameter		
		//end parameter

		CString strValue;
		strValue.Format(_T("%d"),nValue);

		function_return = WritePrivateProfileString(s_strAppName, strKey, strValue, m_strTimingIniFilePath);

		return function_return;
	}
	catch(...)
	{
		AfxMessageBox(_T("\n FILE:Timing 2020/7/22 9:00"));
		return FALSE;
	}
}

BOOL CTimingSet::GetKeyAndValue(const CString &strKey, int &nValue)
{
	try
	{
		BOOL function_return=FALSE;
		//parameter		
		nValue = 0;
		//end parameter
		if (GetPrivateProfileString(s_strAppName, strKey, s_strBoolDefault, m_chBuf, s_iBufSize, m_strTimingIniFilePath) > 0)
		{
			CString strGet = m_chBuf;
			nValue = _ttoi(strGet);
			
			function_return = TRUE;
		}
		return function_return;
	}
	catch(...)
	{
		AfxMessageBox(_T("\n FILE:Timing 2020/7/22 9:00"));
		return FALSE;
	}
}

CTimingSet *CTimingSet::s_pTimingSet = NULL;
CTimingSet *CTimingSet::Instance()
{
	if (s_pTimingSet == NULL)
	{
		s_pTimingSet = new CTimingSet;
	}
	return s_pTimingSet;
}
void CTimingSet::Release()
{
	if (s_pTimingSet != NULL)
	{
		delete s_pTimingSet;
		s_pTimingSet = NULL;
	}
}
