
#include "stdafx.h"

BOOL CToolkit::TraceDataSet_HTML(NDataModelDataSet* pDataSet, CString strPrintFileName)
{
	if (pDataSet == NULL)
		return FALSE;

	//CLarge_StringBuilder sbBody(LargeStringBuilder_NodeSize * 10);
	PmTString sbBody;
	CString strHTMLBody = _T("");
	sbBody.append(_T("<head><title></title></head><body><table border = '1'>"));

	int iFieldCount = pDataSet->GetFieldCount();

	sbBody.append(_T("<tr>"));
	int i = 0;
	for (i = 0; i < iFieldCount; i++)
	{
		sbBody.append(_T("<td align = 'center'>"));
		CString strFieldName = pDataSet->GetFieldName(i);
		sbBody.append(strFieldName);
		sbBody.append(_T("</td>"));
	}
	sbBody.append(_T("</tr>"));
	for (i = 1; i <= pDataSet->GetRowCount(); i++)
	{
		sbBody.append(_T("<tr>"));
		for (int j = 0; j < iFieldCount; j++)
		{
			CString strFieldValue;
			if (pDataSet->GetFieldDataType(j) == ftBlob || pDataSet->GetFieldDataType(j) == ftString)
				strFieldValue = pDataSet->GetAsString(j, i);
			else if (pDataSet->GetFieldDataType(j) == ftFloat)
			{
				strFieldValue.Format(_T("%.3f"), pDataSet->GetAsFloat(j, i));

			}
			else
			{
				int iFieldValue = pDataSet->GetAsInteger(j, i);
				strFieldValue.Format(_T("%d"), iFieldValue);
			}
			sbBody.append(_T("<td>"));
			sbBody.append(strFieldValue);
			sbBody.append(_T("</td>"));
		}
		sbBody.append(_T("</tr>"));
	}
	sbBody.append(_T("</table></body>"));
	strHTMLBody = sbBody.c_str();

	SaveFileContent(strPrintFileName, strHTMLBody);
	return TRUE;
}

BOOL CToolkit::SaveFileContent(const CString& fileName, const CString& content)
{
	//保存脚本文件内容
	CFile fScript;
	//memset(tmp,0,iLen+1);
	if (!fScript.Open(fileName, CFile::modeCreate | CFile::modeWrite))
		return FALSE;

	LPSTR pAInfo = PM_T2A(((CString)content).GetBuffer(0));
	fScript.Write(pAInfo, (UINT)strlen(pAInfo));
	delete[] pAInfo;
	PMUnregMemory(pAInfo);

	fScript.Close();
	return TRUE;
}

LPSTR CToolkit::PM_T2A(LPCTSTR pTInfo)
{
	LPSTR pAInfo = NULL;
#ifdef UNICODE
	pAInfo = PM_W2A(pTInfo);
#else
	int iLength = _tcslen(pTInfo);
	int iALength = (iLength + sizeof(CHAR));
	pAInfo = new CHAR[iALength];
	ZeroMemory(pAInfo, iALength);
	CopyMemory(pAInfo, pTInfo, iALength - sizeof(CHAR));

#endif
	return pAInfo;
}

LPSTR CToolkit::PM_W2A(LPCWSTR pWInfo)
{
	int iLength = lstrlenW(pWInfo);
	int iALength = (iLength + 1) * sizeof(WCHAR);
	LPSTR pAInfo = new CHAR[iALength];
	ZeroMemory(pAInfo, iALength);
	ATLW2AHELPER(pAInfo, pWInfo, iALength, CP_ACP);
	return pAInfo;
}
