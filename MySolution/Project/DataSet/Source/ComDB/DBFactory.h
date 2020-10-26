// DBFactory.h: interface for the CDBFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBFACTORY_H__1C2E2597_A532_4BB8_A27F_288AF12DA19E__INCLUDED_)
#define AFX_DBFACTORY_H__1C2E2597_A532_4BB8_A27F_288AF12DA19E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CComDB.h"
//#include "DataServiceHTTP.h"

enum DATASERVICETYPE{ COMDB,HTTPDATASERVICE,NoneDB};

class DATASETDLLEXPORT CDBFactory
{
public:	
	//单一对象支持
	static void Release();
	static CDataServiceAbsolute* GetSingle(DATASERVICETYPE type);
    static CDataServiceAbsolute* GetSingle();
	static DATASERVICETYPE      GetDataServiceType();
	CDBFactory();
	virtual ~CDBFactory();
protected:
	static CComDB * m_pComDB;
	//static CDataServiceHTTP * m_pHTTPDataService;
	DATASERVICETYPE m_type;
};

#endif // !defined(AFX_DBFACTORY_H__1C2E2597_A532_4BB8_A27F_288AF12DA19E__INCLUDED_)
