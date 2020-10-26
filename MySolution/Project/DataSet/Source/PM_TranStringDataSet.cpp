#include "stdafx.h"
#include "PM_TranStringDataSet.h"
#include "CDataSet_StringBuilder.h"

#define SplitRowKey			_T(";;")
#define SplitColKey			_T("&&")

IMPLEMENT_DYNCREATE( CPM_TranStringDataSet, CPM_StorageDataSet )


void SplitCString2(const CString& source, CStringArray& dest, const CString& strDivision)
{
	int iDivLength = strDivision.GetLength();
	dest.RemoveAll();
	int finddivision = source.Find(strDivision);
	if (finddivision == -1) //没有找到
	{
		dest.Add(source);
		return;
	}
	int oldFind = 0;
	while (finddivision >= 0)
	{
		dest.Add(source.Mid(oldFind, finddivision - oldFind));
		oldFind = finddivision + iDivLength;
		finddivision = source.Find(strDivision, oldFind);
	}
	//处理最后一种情况
	if (source.GetLength() > 0 && oldFind >= 0 && source.GetLength() > (oldFind))
	{
		dest.Add(source.Mid(oldFind, source.GetLength() - oldFind));
	}
}


CPM_TranStringDataSet::CPM_TranStringDataSet(void)
{
}

CPM_TranStringDataSet::~CPM_TranStringDataSet(void)
{
}

BOOL CPM_TranStringDataSet::LoadDataSetInfo(const CString & strInfo)
{//列名1&&列名2&&列名3;;值1&&值2&&值3;;
	CStringArray strTemSplitInfoArr;
	SplitCString2(strInfo,strTemSplitInfoArr,SplitRowKey);
	if(strTemSplitInfoArr.GetSize() <= 1)
		return TRUE;

	CString strSplitColKey = SplitColKey;
	//拆分头信息
	CStringArray strHeadInfoArr;
	SplitCString2(strTemSplitInfoArr.GetAt(0),strHeadInfoArr,strSplitColKey);

	//不可能出现 没有列头信息
	if(strHeadInfoArr.GetSize() <= 0)
		return FALSE;

	//拆分内部内容
	for (int i = 1;i < strTemSplitInfoArr.GetSize();i++)
	{//拆分行信息
		const CString & strTemRowInfo = strTemSplitInfoArr.GetAt(i);
		CStringArray strTemRowValueArr;
		SplitCString2(strTemRowInfo,strTemRowValueArr,strSplitColKey);

		ASSERT(strTemRowValueArr.GetSize() != strHeadInfoArr.GetSize());//判断决不可能的情况

		int iNewRow = this->InsertRow(0);

		for (int j = 0;j < strTemRowValueArr.GetSize();j++)
		{
			SetAsString(strHeadInfoArr.GetAt(j),iNewRow,strTemRowValueArr.GetAt(j));
		}
	}
	return TRUE;
}

BOOL CPM_TranStringDataSet::SaveDataSetInfo(CString & strInfo)
{//列名1&&列名2&&列名3;;值1&&值2&&值3;;
	//没有内容 不用保存
	if(GetRowCount() <= 0)
		return TRUE;

	CDataSet_StringBuilder sDataSetInfo;

	//合成列头
	int i = 0;
	int iFieldCount = GetFieldCount();
	for(i = 0;i < iFieldCount;i++)
	{
		if(i > 0)
			sDataSetInfo.Append(SplitColKey);
		sDataSetInfo.Append(GetFieldName(i));
	}
	sDataSetInfo.Append(SplitRowKey);

	for(i = 1;i <= GetRowCount();i++)
	{//合成行信息
		for (int j = 0;j < iFieldCount;j++)
		{//合成列信息
			if(j > 0)
				sDataSetInfo.Append(SplitColKey);
			sDataSetInfo.Append(GetAsString(j,i));
		}
		sDataSetInfo.Append(SplitRowKey);
	}
	sDataSetInfo.GetString(strInfo);
	return TRUE;
}

void CPM_TranStringDataSet::SetUpdateAsBlobFiled(int col,BOOL b)
{
	AfxMessageBox(_T("CPM_TranStringDataSet是不能设置二进制保存方式"));
}

void CPM_TranStringDataSet::SetUpdateAsBlobFiled(CString colName,BOOL b)
{
	AfxMessageBox(_T("CPM_TranStringDataSet是不能设置二进制保存方式"));
}