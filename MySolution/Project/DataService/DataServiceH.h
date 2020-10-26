
#pragma once

#ifdef DataService_Module
	#define DataServiceAPI __declspec(dllexport)
#else
	#define DataServiceAPI __declspec(dllimport)
	#pragma comment(lib, "DataService.lib")
#endif

#include "Source/DataBaseAndTableName.h"
#include "Source/ConnectManager.h"
#include "Source/Toolkit.h"
#include "Source/DataService_Common.h"

#include "Table/AccessTable.h"
#include "Table/AccessDrawIndexTable.h"