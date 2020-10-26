// SQLDataSet.cpp: implementation of the CSQLDataSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SQLDataSet.h"
#include "PMSQLBaseCtrl.h"
#include "CPub.h"
#include "NDataModelDataSet_ShareData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE( CSQLDataSet, NDataModelDataSet )

CSQLDataSet::CSQLDataSet()
{
	m_pBaseCtrl = NULL;
}

CSQLDataSet::CSQLDataSet(CPMSQLBaseCtrl * pBaseCtrl)
{
	m_pBaseCtrl = pBaseCtrl;
}

CSQLDataSet::~CSQLDataSet()
{
	m_pBaseCtrl = NULL;
}

BOOL CSQLDataSet::GetCreateIndexSQL(CStringArray & strFieldNameArr,CString & strIndexName)
{//默认不建索引
	return FALSE;
}

void CSQLDataSet::SetSqlBaseCtrl(CPMSQLBaseCtrl * pBaseCtrl)
{
	m_pBaseCtrl = pBaseCtrl;
}

void CSQLDataSet::StringFormat( PmTSQLString & str , const TCHAR *_format, ... )
{
	str.clear() ; 

	TCHAR * pBuffer = NULL ; 

	/// 步长
	int iBaseSize = 1024 ;

	/// buffer的初始大小
	int iBufferSize = 0 ; 

	/// 串化后真实的字符数
	int iRealSize = -1 ;

	/// 
	while ( iRealSize > iBufferSize 
		|| iRealSize < 0 )
	{
		iBufferSize += iBaseSize ; 

		delete [](TCHAR *)pBuffer ;

		pBuffer = new TCHAR[iBufferSize] ;

		/// memset 第三个参数为字节数
		memset( pBuffer , 0 , iBufferSize * sizeof(TCHAR)) ;

		va_list ap;  
		va_start(ap, _format);  

		try  
		{  
			iRealSize = _vsntprintf(pBuffer, iBufferSize, _format, ap);  
		} 
		catch (...)  
		{  
			/// 出错了，需要写日志
		}  

		va_end(ap);  
	}

	str.append(pBuffer);  

	delete [](TCHAR *)pBuffer ; 
}

void CSQLDataSet::DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff)
{
	if (row < 0 || row > pBuff->GetRowCount()) 
	{
		return;
	}
	if (row == 0) 
	{
		row = pBuff->GetRowCount();
	}
	void * buff = pBuff->GetBuffPtr(row - 1);
	pBuff->GetBufferPtr()->RemoveAt(row - 1);
	
	m_DeleteBuffers->AddBuff(buff);
	
	if (m_IsModified == FALSE) 
	{
		m_IsModified = TRUE;
	}
	
    //跟踪共享对象
	if (m_pShardDataList != NULL) 
	{
		for(int i = 0 ; i < m_pShardDataList->GetSize(); i++)
		{
			((NDataModelDataSet_ShareData*)m_pShardDataList->GetAt(i))->OnParentDelete(buff);
		}
	}
}

bool CSQLDataSet::DoDeleteActive(CUIntArray & arrPKCol)
{
//	UIBase::CTiming timing1(_T("CSQLDataSet::DoDeleteActive"),TRUE);

	PmTSQLString statement; 
	CPMSQLTableCtrl * pTableCtrl = NULL;
	BOOL bHasFirstDel = TRUE;
	for(int row = 1; row <= DeletedCount() ; row++)
	{
		if(GetRowNew(row,Delete))
			continue;	//不处理被删除的新行
		NRecordID pTemRowRecordID = GetRecordID(row ,Delete);
		
		if(TRUE == bHasFirstDel)
		{
			//产生 需要的 删除sql
			statement.erase();
			for(int col = 0; col < arrPKCol.GetSize(); col++)
			{
				int iTemCol=arrPKCol.GetAt(col);
				if(!statement.empty())
					statement += _T(" and ");

				///==================================================
				/// liwenlang [2013-1-8   17:30]
				/// 此处删除有问题 
				/// 应该使用 列的序号 
				/// statement += GetFieldName(col);
				statement += GetFieldName( iTemCol );
				
				statement += _T("=?");
			}
			PmTSQLString sqlString;
			StringFormat(sqlString,_T("delete from %s where %s"), GetTableName(), statement.c_str());
			
			if(!m_pBaseCtrl->GetSQLTableCtrl(sqlString.c_str(),pTableCtrl))
			{
				CString strTemMsg;
				strTemMsg.Format(_T("DoSqlUpdate():%s"),sqlString.c_str());
				
				CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
				continue;
			}
			else
				bHasFirstDel = FALSE;
		}
		else
			pTableCtrl->ResetSql();//将sql语句 再执行一次
		
		for(int i = 0;i < arrPKCol.GetSize();i++)
		{
			NDataField * pTemField = m_fields.GetField(arrPKCol.GetAt(i));
			if(NULL == pTemField)
				continue;

			FieldType temFieldType = pTemField->GetDataType();
			if(ftInteger == temFieldType)
			{
				int iTemValue = GetAsInteger(pTemField,pTemRowRecordID);
				pTableCtrl->BindAsInterger(i + 1,iTemValue);
			}
			else if(ftFloat == temFieldType)
			{
				double dTemValue = GetAsFloat(pTemField,pTemRowRecordID);
				pTableCtrl->BindAsDouble(i + 1,dTemValue);
			}
			else if(ftBlob == temFieldType && pTemField->IsUpdateAsBlobFiled())
			{
				LPVOID pbuffer = NULL;
				long lbufferSize = GetBlob(pTemField,pTemRowRecordID,pbuffer);
				pTableCtrl->BindAsBlob(i + 1,pbuffer,lbufferSize);
			}
			else
			{
				CString strTemValue = GetAsString(pTemField,pTemRowRecordID);
				pTableCtrl->BindAsString(i + 1,strTemValue);
			}
		}	
		pTableCtrl->Update();
	}

	//执行完 删除之后，buffer 需要清空
	m_DeleteBuffers->ResetBuffers();

	if(NULL != pTableCtrl)
	{
		delete pTableCtrl;
	}
	return true;
}

bool CSQLDataSet::DoInsertActive()
{ 
//	UIBase::CTiming timing1(_T("CSQLDataSet::DoInsertActive"),TRUE);
	
	//自动生成AutoIncrease列
	int col = 0;
	for(col = 0; col<GetFieldCount(); col++)
	{
		if(!(IsFieldPK(col) && IsFieldPKAutoIncrease(col)))
			continue;
		
		CString s = GetMaxFieldValue(col);
		int i = _ttoi(s);
		//逐行加1
		for(int row = 0; row<GetRowCount(); row++)
		{
			if(!GetRowNew(row + 1))
				continue;	//非插入行
			s.Format(_T("%d"), ++i);
			//字符型需要左边填“0”
			if(GetFieldDataType(col)==ftString)
			{
				while(s.GetLength() < GetFieldMaxLen(col))
				{
					s = _T("0") + s;
				}
			}
			SetAsString(row, col, s);
		}
	}
	
	BOOL bHasFirstInsert = TRUE;
	CPMSQLTableCtrl * pTableCtrl = NULL;
	//逐行生成
	for(int row = 0; row < GetRowCount(); row++)
	{
		if(!GetRowNew(row + 1))
			continue;	//非插入行

		NRecordID pTemRowRecordID = GetRecordID(row + 1,Primary);
		
		if(TRUE == bHasFirstInsert)
		{
			PmTSQLString sInsertCol;
			PmTSQLString statement;
			for(col=0; col < GetFieldCount(); col++)
			{
				if(!IsFieldUpdateAble(col))
					continue;
				
				if(!sInsertCol.empty())
				{
					sInsertCol.append(_T(","));
					statement.append(_T(","));
				}
				sInsertCol.append(GetFieldName(col));
				statement.append(_T("?"));
			}
			PmTSQLString sInsertSql;
			StringFormat(sInsertSql,_T("insert into %s (%s) values (%s)"),\
				GetTableName(), sInsertCol.c_str(), statement.c_str());
			
			if(!m_pBaseCtrl->GetSQLTableCtrl(sInsertSql.c_str(),pTableCtrl))
			{
				CString strTemMsg;
				strTemMsg.Format(_T("DoSqlUpdate():%s"),sInsertSql.c_str());
				
				CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);
				continue;
			}
			else
				bHasFirstInsert = FALSE;
		}
		else
			pTableCtrl->ResetSql();//将sql语句 再执行一次
		
		int iTemFieldCount = m_fields.GetCount();
		for(int i = 0;i < iTemFieldCount;i++)
		{
			NDataField * pTemField = m_fields.GetField(i);
			FieldType temFieldType = pTemField->GetDataType();
			if(ftInteger == temFieldType)
			{
				int iTemValue = GetAsInteger(pTemField,pTemRowRecordID);
				pTableCtrl->BindAsInterger(i + 1,iTemValue);
				if (i == 0)
				{
					pTableCtrl->SetCurParmID(iTemValue);
				}

			}
			else if(ftFloat == temFieldType)
			{
				double dTemValue = GetAsFloat(pTemField,pTemRowRecordID);
				pTableCtrl->BindAsDouble(i + 1,dTemValue);
			}
			else if(ftBlob == temFieldType && pTemField->IsUpdateAsBlobFiled())
			{
				LPVOID pbuffer = NULL;
				long lbufferSize = GetBlob(pTemField,pTemRowRecordID,pbuffer);
				pTableCtrl->BindAsBlob(i + 1,pbuffer,lbufferSize);
			}
			else
			{
				CString strTemValue = GetAsString(pTemField,pTemRowRecordID);
				pTableCtrl->BindAsString(i + 1,strTemValue);
			}
		}	
		pTableCtrl->Update();
	}
	if(NULL != pTableCtrl)
	{
		delete pTableCtrl;
	}
	return true;
}

bool CSQLDataSet::DoMdfActive(CUIntArray & arrPKCol)
{
//	UIBase::CTiming timing1(_T("CSQLDataSet::DoMdfActive"),TRUE);
	
	//生成update的set ??=??, ??=??部分
	CPMSQLTableCtrl * pTableCtrl = NULL;
	BOOL bHasFirstInsert = TRUE;
	CUIntArray iFieldIDArr;//用于暂存 对应 绑定列的 列位置

	for(int row = 0; row < GetRowCount(); row++)
	{
		if(GetRowNew(row + 1))
			continue;					//new的行不作Update处理
		if(!GetRowModified(row + 1))
			continue;			//内容未修改

		NRecordID pTemRowRecordID = GetRecordID(row + 1,Primary);

		if(TRUE == bHasFirstInsert)
		{
			PmTSQLString statement; 	
			for(int col=0; col < GetFieldCount(); col++)
			{
				if(!IsFieldUpdateAble(col))
					continue;		//列不可存盘
								
				if(!statement.empty())
					statement += _T(",");
				statement += GetFieldName(col);
				statement += _T("=?");

				iFieldIDArr.Add(col);
			}
			
			//生成where子句
			PmTSQLString strWhere;
			for(int i = 0 ; i < arrPKCol.GetSize(); i++)
			{
				int iTemCol = arrPKCol.GetAt(i);
				if(i)
					strWhere += _T(" and ");
				strWhere += GetFieldName(iTemCol);
				strWhere+= _T("=?");

				iFieldIDArr.Add(iTemCol);
			}
			
			//加入到数组
			PmTSQLString updateSql;
			StringFormat(updateSql,_T("update %s set %s where %s"),\
				GetTableName(), statement.c_str(), strWhere.c_str());

			if(!m_pBaseCtrl->GetSQLTableCtrl(updateSql.c_str(),pTableCtrl))
			{
				CString strTemMsg;
				strTemMsg.Format(_T("DoSqlUpdate():%s"),updateSql.c_str() );
				CDataSetErrorInfo::Instance()->PrintInfo(strTemMsg);

				continue;
			}
			else
				bHasFirstInsert = FALSE;
		}
		else
			pTableCtrl->ResetSql();
		
		for(int i = 0;i < iFieldIDArr.GetSize();i++)
		{
			NDataField * pTemField = m_fields.GetField(iFieldIDArr.GetAt(i));
			if(NULL == pTemField)
				continue;

			FieldType temFieldType = pTemField->GetDataType();
			if(ftInteger == temFieldType)
			{
				int iTemValue = GetAsInteger(pTemField,pTemRowRecordID);
				pTableCtrl->BindAsInterger(i + 1,iTemValue);
			}
			else if(ftFloat == temFieldType)
			{
				double dTemValue = GetAsFloat(pTemField,pTemRowRecordID);
				pTableCtrl->BindAsDouble(i + 1,dTemValue);
			}
			else if(ftBlob == temFieldType && pTemField->IsUpdateAsBlobFiled())
			{
				LPVOID pbuffer = NULL;
				long lbufferSize = GetBlob(pTemField,pTemRowRecordID,pbuffer);
				pTableCtrl->BindAsBlob(i + 1,pbuffer,lbufferSize);
			}
			else
			{
				CString strTemValue = GetAsString(pTemField,pTemRowRecordID);
				pTableCtrl->BindAsString(i + 1,strTemValue);
			}
		}	
		pTableCtrl->Update();
	}
	if(NULL != pTableCtrl)
	{
		delete pTableCtrl;
	}
	return true;
}

bool CSQLDataSet::DoSqlUpdate()
{
	if(NULL == m_pBaseCtrl)
	{
#ifdef _TESTMEMORY
		AfxMessageBox( _T("没有绑定数据库,不能做更新.") ) ;
#endif
		ErrorLog( _T("没有绑定数据库,不能做更新.") ) ; 
		return false;
	}

	CString strTableName = GetTableName();

	//获得PK数组
	CUIntArray arrPKCol;
	CUIntArray arrBlobCol;
	int col=0 ;
	for(col=0 ; col < GetFieldCount() ; col++)
	{
		if(IsUpdateAsBlobFiled(col))
		{
			arrBlobCol.Add(col);
		}
		if(!IsFieldPK(col))
			continue;
		arrPKCol.Add(col);
//		arrPKFieldName.Add(GetFieldName(col));
	}

	if(arrPKCol.GetSize()==0)
	{
		ErrorLog(_T("Pimary Key Column not found!"));
		return false;
	}

	//逐行检查非CanNull列
	int row=0;
	//逐列处理
	for(col=0; col<GetFieldCount(); col++)
	{
		if(!IsFieldUpdateAble(col))
			continue;		//列不可存盘
		if(IsFieldCanNull(col))
			continue;		//列允许为空
		if(IsFieldPK(col) && IsFieldPKAutoIncrease(col))
				continue;			//自动增长的主键列
		for(row=0; row < GetRowCount(); row++)
		{
			if(!GetRowModified(row + 1))
				continue;			//本行未修改：不检查
			
			if(!GetItemModified(row + 1, col))
				continue;	//数据未修改
			
			CString s = GetAsString(col,row + 1);
			if(!s.IsEmpty())
				continue;				//数据非空
			
			s.Format(_T("Field %s at row %d is Empty."), GetFieldName(col), row);
			
			CDataSetErrorInfo::Instance()->PrintInfo(s);
			return FALSE;
		}
	}

	//防止filter中的数据没有被更新2008.5.8
	const int filterBufferCount = CopyFilterToPrimaryBuffer();

	{
//		UIBase::CTiming timing1(_T("CSQLDataSet.DoSqlUpdate.DoMarkSql"),TRUE);

		CPMSQLBaseCtrl::CSQLTransaction doSqlTransaction(m_pBaseCtrl);
		
		/////////////////////////////////////////////////////////////////
		//Step2: 生成Delete的SQL语句
		/////////////////////////////////////////////////////////////////
		DoDeleteActive(arrPKCol);
		
		/////////////////////////////////////////////////////////////////
		//Step3: 生成Update的SQL语句
		/////////////////////////////////////////////////////////////////
		DoMdfActive(arrPKCol);
		
		/////////////////////////////////////////////////////////////////
		//Step4: 生成Insert语句, 自动生成的主键需要Loop 5遍
		/////////////////////////////////////////////////////////////////
		//生成insert语句的固定字段名部分
		DoInsertActive();
	}

	ResetUpdate(FALSE);
	
	RemoveCopyedFilterToPrimaryBuffer(filterBufferCount);
	return TRUE;
}
