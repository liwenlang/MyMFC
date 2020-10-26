#pragma once

class DataServiceAPI CToolkit
{
public:
	BOOL TraceDataSet_HTML(NDataModelDataSet* pDataSet, CString strPrintFileName);

	BOOL SaveFileContent(const CString& fileName, const CString& content);

	LPSTR PM_T2A(LPCTSTR pTInfo);

	LPSTR PM_W2A(LPCWSTR pWInfo);

};
