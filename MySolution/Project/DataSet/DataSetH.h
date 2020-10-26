//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the DataSet Library
// Copyright (C) 1998-2000 
// All rights reserved.
//
// 
// Add By zhangym 2003.6.12
// 
//*******************************************************************************
// dataset.h

#ifndef __DATASETH_H
#define __DATASETH_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//

/// 
/// 本模块需要依赖的头文件
/// 
#include <map>
#include <vector>


#ifdef DataSet_Moudle
	#define DataSetAPI __declspec(dllexport)
#else
	#define DataSetAPI __declspec(dllimport)
	#pragma comment(lib, "DataSet.lib")
#endif


/// 本模块导出的头文件
/// 
#include "CommonDefine.h"

#include "Source\NDataField.h"
#include "Source\NDataFields.h"
#include "Source\NDataModelDataSet.h"
#include "Source\NDataModelDataSetBuffer.h"
#include "Source\NDateTimeField.h"
#include "Source\NFloatField.h"
#include "Source\NIntegerField.h"
#include "Source\NNumericField.h"
#include "Source\NStringField.h"
#include "Source\NDataModelDataSet_Clone.h"
#include "Source\NDataModelDataSet_ShareData.h"
#include "Source\PM_StorageDataSet.h"
#include "Source\PM_SharebufferDataSet.h"
#include "Source\PM_LargeFileDataSet.h"
//#include "Source\PM_LinkDataSet.h"
#include "Source\PMSQLBaseCtrl.h"
#include "Source\SQLDataSet.h"
#include "Source\PM_TranStringDataSet.h"

#include "Source\NDbDataSet.h"

/// SQLiteCtrl
#include <dataset/SQLiteCtrl\sqlite3.h>
#include <dataset/SQLiteCtrl\SqliteDBBase.h>


#endif // __DATASETH_H
