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

		//先累计时间
		m_EndTime = GetTickCount();

		//得到毫秒数
		DWORD result = m_EndTime - m_BeginTime;

		CRecord::AddRecord(m_strBelong, m_strKey, result, m_bAddUp,m_bIsTatolTimeRecord);
#endif 
	}
	
	m_bHasCount = TRUE;
	return TRUE;
}

DWORD CRecord::s_nTotalCount = 0;
CRecord::Str_TableMap CRecord::s_AllTable;
// 功能描述：插入一条记录
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
				if (bAddUp != true)//不参与累计
				{
					//取最大值
					if(aFunInfo.nTotalTime < Time)
					{
						aFunInfo.nTotalTime = Time;
					}
				}
				else//累计
				{
					aFunInfo.nCount += 1;
					aFunInfo.nTotalTime += Time;
				}

				//同一个函数名字前后要求参与统计或者不参与统计不同!
				if (aFunInfo.bAddUp != bAddUp)
				{
					CString strPromp;
					strPromp += _T("被测函数:");
					strPromp += strFunKey;
					strPromp += _T("是否参与统计前后要求不一致!");
					//OutPutMessage(strPromp);
				}
			}
			else//插入一条新的函数记录
			{
				FunInfo_T funInfo(1,Time, bAddUp,bIsTatolTimeRecord);
				aTable.insert(std::make_pair(strFunKey, funInfo) );
			}
		}
		else//插入新表
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

//统计一张表中次数,总时间, 平均时间
void CRecord::CountTable(Str_FunInfoMap &aTable, DWORD &nTotalCs, DWORD &nTotalTime, DWORD &nPjsj)
{
	nTotalCs = nTotalTime = nPjsj = 0;

	//是否存在表示全局时间的记录
	bool bHasTotalRecord = false;
	FunInfo_T aTotalTimeFunInfo;

	for (Str_FunInfoMap::iterator funIter=aTable.begin(); funIter != aTable.end(); ++funIter)
	{
		FunInfo_T &aFunInfo = funIter->second;
		//如果是全局时间,那么不给予累计
		if (aFunInfo.bIsTatolTimeRecord == true)
		{
			bHasTotalRecord = true;
			aTotalTimeFunInfo = aFunInfo;
			continue;
		}

		nTotalCs += aFunInfo.nCount;
		nTotalTime += aFunInfo.nTotalTime;
	}

	if (nTotalCs != 0 )//求平均时间,使用函数的累计时间除以累计次数
	{
		nPjsj = nTotalTime / nTotalCs;
	}

	if (bHasTotalRecord == true)
	{
		//如果存在代表全局时间的记录,输出该记录为总时间
		nTotalTime = aTotalTimeFunInfo.nTotalTime;
	}
}

//统计多张表中次数,总时间, 平均时间
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

	if (nTotalCs != 0)//求平均时间
	{
		nPjsj = nTotalTime / nTotalCs;
	}
}



//清除所有记录
void CRecord::ClearAll()
{
	s_AllTable.clear();
	//s_nTotalCount = 0;
}

//清除指定表的所有记录
void CRecord::ClearTable(const CString &strTableKey)
{
	Str_TableMap::iterator tableIter = s_AllTable.find(strTableKey);
	if (tableIter != s_AllTable.end() )
	{
// 		Table_T &table = tableIter->second;
// 		DWORD nCount = 0;//计算这个表中有几次的测试
// 		for(Table_T::iterator funIter = table.begin(); funIter != table.end(); ++funIter)
// 		{
// 			FunInfo_T &funInfo = funIter->second;
// 			nCount += funInfo.nCount;
// 		}
//		s_nTotalCount -= nCount;//减去这个表中的测试次数
		s_AllTable.erase(tableIter);
	}
}

//得到总表头
BOOL CRecord::GetHeadInfo(CString &strZbt)
{
	try
	{
		//BOOL function_return=FALSE;
		//parameter		
		
		//end parameter
		//打印时间
		//CTime time = CTime::GetCurrentTime();
		//CString strNowTime = time.Format(_T("测试日期<年月日>:%Y-%m-%d,时间<时分秒>:%H:%M:%S"));

		CTime t = CTime::GetCurrentTime();
		int nYear=t.GetYear();
		int nMonth=t.GetMonth();
		int nDay = t.GetDay();
		int nHour =t.GetHour();
		int nMin=t.GetMinute();
		int nSec=t.GetSecond();
		CString strNowTime;
		strNowTime.Format(_T("测试日期<年月日>:%04d年%02d月%02d日 %02d:%02d:%02d"),nYear,nMonth,nDay,nHour,nMin,nSec);
	
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


//输出信息
BOOL COutPutMessage::PrintRecord()
{
	try
	{
		BOOL function_return=FALSE;
		//parameter		
		if (CTimingSet::Instance()->IsSuperadditionMode() == FALSE)
		{
			//非追加模式,清除目标区域
			Clear();
		}
		//end parameter
		//分表输出
		//总表头:时间,日期
		CString strZbt;
		if (CRecord::GetHeadInfo(strZbt) == TRUE)
		{
			AddOneLine(strZbt);
		}

		//取得总的:次数,时间,平均时间
		DWORD nAllTotalCs = 0;//总表总次数
		DWORD nAllTotalTime = 0;//总表总时间
		DWORD nAllPjsj = 0;//总表平局时间
		CRecord::CountTable(CRecord::s_AllTable, nAllTotalCs, nAllTotalTime, nAllPjsj);

		for (CRecord::Str_TableMap::iterator tableIter = CRecord::s_AllTable.begin(); tableIter!=CRecord::s_AllTable.end();
		++tableIter)
		{
			//分表头:表名
			CString strTableName = tableIter->first;
			CString strBt = _T("<<分表>>:");
			strBt += strTableName;
			AddOneLine(strBt);

			CRecord::Str_FunInfoMap &aTable = tableIter->second;
			//先统计一张表:
			DWORD nOneTotalCs = 0;//分表总时间
			DWORD nOneTotalTime = 0;//分表总时间
			DWORD nOnePjsj = 0;//分表平局时间
			CRecord::CountTable(aTable, nOneTotalCs, nOneTotalTime, nOnePjsj);
			const CString str = _T("%") ; 
			for (CRecord::Str_FunInfoMap::iterator funIter = aTable.begin(); funIter != aTable.end(); ++funIter)
			{
				//打印函数名称
				CString strFunName = funIter->first;
				CRecord::FunInfo_T &aFunInfo = funIter->second;
				CString strFun ;
				if (aFunInfo.bIsTatolTimeRecord == false)
				{
					strFun =_T("@函数@:");
				}
				else
				{
					strFun = _T("@当前表时间统计总函数@:");
				}
				strFun += strFunName;
				AddOneLine(strFun);

				CString strInfo;
				if (aFunInfo.bAddUp == true)// 累计
				{
					if (aFunInfo.nCount == 0)
					{
// 						OutPutMessage(_T("除0错误!"));
// 						THROW_ERROR;
						return FALSE;
					}
					DWORD dEve = aFunInfo.nTotalTime / aFunInfo.nCount;

					double dFbBfb = 0.0;//占分表时间百分比
					if (nOneTotalTime != 0)
					{
						dFbBfb = (double) aFunInfo.nTotalTime  / nOneTotalTime;
						dFbBfb *= 100.0;
					}
					double dZbBfb = 0.0;//占总表时间百分比
					if (nAllTotalTime != 0)
					{
						dZbBfb = (double)aFunInfo.nTotalTime / nAllTotalTime;
						dZbBfb *= 100.0;
					}

				//如果是统计项,输出次数,运行时间
							
				strInfo.Format(_T("统计:累计次数:<%d>次, 累计时间:<%d>毫秒, 平均时间:<%d>毫秒, 分表百分比:<%.1f%s>,总表百分比:<%.1f%s>"), 
						aFunInfo.nCount, aFunInfo.nTotalTime, dEve, dFbBfb, str , dZbBfb , str );

				}
				else
				{
					//如果是非统计项,输出最大运行时间
					strInfo.Format(_T("单次运行:最长时间<%d>毫秒"), aFunInfo.nTotalTime);
				}
				
				AddOneLine(strInfo);
			}

			// >>分表汇总<<: 次数: 时间: 平均时间: 总表百分比:
			double dFbHz = 0.0;//分表的汇总百分比
			if (nAllTotalTime != 0)
			{
				dFbHz = (double)nOneTotalTime / nAllTotalTime;
				dFbHz *= 100.0;
			}
			CString strFbHz;
			strFbHz.Format(_T(">>分表汇总<<:次数:<%d>次,  时间:<%d>毫秒, 平均时间:<%d>毫秒, 总表百分比:<%.1f%s>"),
				nOneTotalCs, nOneTotalTime, nOnePjsj, dFbHz ,str );
			AddOneLine(strFbHz);

			CString strFG = _T("==========================================================");
				//输出分表分割符号
			AddOneLine(strFG);

		}

		//>>总表汇总<<:次数: 时间:  平均时间: 
		CString strHz;
		strHz.Format(_T(">>总表汇总<<:次数:<%d>次,时间:<%d>毫秒, 平均时间:<%d>毫秒"),
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

//清除目标输出区域
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

	//保证全部转化为char，存入
	USES_CONVERSION;
	LPSTR pBuff = T2A(((CString)strAppendInfo).GetBuffer(0));//这里主要针对unicode
	UINT  len = (UINT)strlen(pBuff); 	
	
	fFile.SeekToEnd();
	fFile.Write(pBuff, len);
	fFile.Close();
	
	//delete []pBuff;
	return TRUE;
}

//加入一行
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
const CString CTimingSet::s_strAdditionMode = _T("SuperadditionMode");//追加模式
const CString CTimingSet::s_strBoolDefault = _T("0");//布尔值默认
const int CTimingSet::s_iBufSize = 100;

// 功能描述：信息输出是否是追加模式,如果不是,那么上次的输出到目标的内容会被清除.
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

//设置追加模式
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

//设置和取得值
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
