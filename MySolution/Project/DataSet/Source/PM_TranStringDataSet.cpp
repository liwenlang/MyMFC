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
	if (finddivision == -1) //û���ҵ�
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
	//�������һ�����
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
{//����1&&����2&&����3;;ֵ1&&ֵ2&&ֵ3;;
	CStringArray strTemSplitInfoArr;
	SplitCString2(strInfo,strTemSplitInfoArr,SplitRowKey);
	if(strTemSplitInfoArr.GetSize() <= 1)
		return TRUE;

	CString strSplitColKey = SplitColKey;
	//���ͷ��Ϣ
	CStringArray strHeadInfoArr;
	SplitCString2(strTemSplitInfoArr.GetAt(0),strHeadInfoArr,strSplitColKey);

	//�����ܳ��� û����ͷ��Ϣ
	if(strHeadInfoArr.GetSize() <= 0)
		return FALSE;

	//����ڲ�����
	for (int i = 1;i < strTemSplitInfoArr.GetSize();i++)
	{//�������Ϣ
		const CString & strTemRowInfo = strTemSplitInfoArr.GetAt(i);
		CStringArray strTemRowValueArr;
		SplitCString2(strTemRowInfo,strTemRowValueArr,strSplitColKey);

		ASSERT(strTemRowValueArr.GetSize() != strHeadInfoArr.GetSize());//�жϾ������ܵ����

		int iNewRow = this->InsertRow(0);

		for (int j = 0;j < strTemRowValueArr.GetSize();j++)
		{
			SetAsString(strHeadInfoArr.GetAt(j),iNewRow,strTemRowValueArr.GetAt(j));
		}
	}
	return TRUE;
}

BOOL CPM_TranStringDataSet::SaveDataSetInfo(CString & strInfo)
{//����1&&����2&&����3;;ֵ1&&ֵ2&&ֵ3;;
	//û������ ���ñ���
	if(GetRowCount() <= 0)
		return TRUE;

	CDataSet_StringBuilder sDataSetInfo;

	//�ϳ���ͷ
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
	{//�ϳ�����Ϣ
		for (int j = 0;j < iFieldCount;j++)
		{//�ϳ�����Ϣ
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
	AfxMessageBox(_T("CPM_TranStringDataSet�ǲ������ö����Ʊ��淽ʽ"));
}

void CPM_TranStringDataSet::SetUpdateAsBlobFiled(CString colName,BOOL b)
{
	AfxMessageBox(_T("CPM_TranStringDataSet�ǲ������ö����Ʊ��淽ʽ"));
}