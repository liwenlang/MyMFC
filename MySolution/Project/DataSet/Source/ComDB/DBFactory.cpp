// DBFactory.cpp: implementation of the CDBFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CComDB *CDBFactory::m_pComDB = NULL;
//CDataServiceHTTP *CDBFactory::m_pHTTPDataService = NULL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBFactory::CDBFactory()
{
   
}

CDBFactory::~CDBFactory()
{
  
}

void CDBFactory::Release()
{
	if (m_pComDB != NULL) 
	{
		delete m_pComDB;
		m_pComDB = NULL;
	}
// 	if (m_pHTTPDataService != NULL) 
// 	{
// 		delete m_pHTTPDataService;
// 		m_pHTTPDataService = NULL;
// 	}
}
CDataServiceAbsolute* CDBFactory::GetSingle(DATASERVICETYPE type)
{
	if (type == COMDB) 
	{
	   if (m_pComDB == NULL) 
	   {
		   m_pComDB = new CComDB;
	   }
	   return m_pComDB;
	}
// 	if (type == HTTPDATASERVICE) 
// 	{
// 	   if (m_pHTTPDataService == NULL) 
// 	   {
// 		   m_pHTTPDataService = new CDataServiceHTTP;
// 	   }
// 	   return m_pHTTPDataService;
// 	}
	return NULL;
}
CDataServiceAbsolute* CDBFactory::GetSingle()
{
// 	if (m_pHTTPDataService != NULL) 
// 	{
// 		return m_pHTTPDataService;
// 	}
	if (m_pComDB != NULL) 
	{
		return m_pComDB;
	}
	return NULL;
}
DATASERVICETYPE  CDBFactory::GetDataServiceType()
	{
// 			if (m_pHTTPDataService != NULL) 
// 			{
// 				return HTTPDATASERVICE;
// 			}
			if (m_pComDB != NULL) 
			{
				return COMDB;
			}
			return NoneDB;
	}
