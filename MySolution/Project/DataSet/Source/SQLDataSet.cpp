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
{//Ĭ�ϲ�������
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

	/// ����
	int iBaseSize = 1024 ;

	/// buffer�ĳ�ʼ��С
	int iBufferSize = 0 ; 

	/// ��������ʵ���ַ���
	int iRealSize = -1 ;

	/// 
	while ( iRealSize > iBufferSize 
		|| iRealSize < 0 )
	{
		iBufferSize += iBaseSize ; 

		delete [](TCHAR *)pBuffer ;

		pBuffer = new TCHAR[iBufferSize] ;

		/// memset ����������Ϊ�ֽ���
		memset( pBuffer , 0 , iBufferSize * sizeof(TCHAR)) ;

		va_list ap;  
		va_start(ap, _format);  

		try  
		{  
			iRealSize = _vsntprintf(pBuffer, iBufferSize, _format, ap);  
		} 
		catch (...)  
		{  
			/// �����ˣ���Ҫд��־
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
	
    //���ٹ������
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
			continue;	//������ɾ��������
		NRecordID pTemRowRecordID = GetRecordID(row ,Delete);
		
		if(TRUE == bHasFirstDel)
		{
			//���� ��Ҫ�� ɾ��sql
			statement.erase();
			for(int col = 0; col < arrPKCol.GetSize(); col++)
			{
				int iTemCol=arrPKCol.GetAt(col);
				if(!statement.empty())
					statement += _T(" and ");

				///==================================================
				/// liwenlang [2013-1-8   17:30]
				/// �˴�ɾ�������� 
				/// Ӧ��ʹ�� �е���� 
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
			pTableCtrl->ResetSql();//��sql��� ��ִ��һ��
		
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

	//ִ���� ɾ��֮��buffer ��Ҫ���
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
	
	//�Զ�����AutoIncrease��
	int col = 0;
	for(col = 0; col<GetFieldCount(); col++)
	{
		if(!(IsFieldPK(col) && IsFieldPKAutoIncrease(col)))
			continue;
		
		CString s = GetMaxFieldValue(col);
		int i = _ttoi(s);
		//���м�1
		for(int row = 0; row<GetRowCount(); row++)
		{
			if(!GetRowNew(row + 1))
				continue;	//�ǲ�����
			s.Format(_T("%d"), ++i);
			//�ַ�����Ҫ����0��
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
	//��������
	for(int row = 0; row < GetRowCount(); row++)
	{
		if(!GetRowNew(row + 1))
			continue;	//�ǲ�����

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
			pTableCtrl->ResetSql();//��sql��� ��ִ��һ��
		
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
	
	//����update��set ??=??, ??=??����
	CPMSQLTableCtrl * pTableCtrl = NULL;
	BOOL bHasFirstInsert = TRUE;
	CUIntArray iFieldIDArr;//�����ݴ� ��Ӧ ���е� ��λ��

	for(int row = 0; row < GetRowCount(); row++)
	{
		if(GetRowNew(row + 1))
			continue;					//new���в���Update����
		if(!GetRowModified(row + 1))
			continue;			//����δ�޸�

		NRecordID pTemRowRecordID = GetRecordID(row + 1,Primary);

		if(TRUE == bHasFirstInsert)
		{
			PmTSQLString statement; 	
			for(int col=0; col < GetFieldCount(); col++)
			{
				if(!IsFieldUpdateAble(col))
					continue;		//�в��ɴ���
								
				if(!statement.empty())
					statement += _T(",");
				statement += GetFieldName(col);
				statement += _T("=?");

				iFieldIDArr.Add(col);
			}
			
			//����where�Ӿ�
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
			
			//���뵽����
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
		AfxMessageBox( _T("û�а����ݿ�,����������.") ) ;
#endif
		ErrorLog( _T("û�а����ݿ�,����������.") ) ; 
		return false;
	}

	CString strTableName = GetTableName();

	//���PK����
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

	//���м���CanNull��
	int row=0;
	//���д���
	for(col=0; col<GetFieldCount(); col++)
	{
		if(!IsFieldUpdateAble(col))
			continue;		//�в��ɴ���
		if(IsFieldCanNull(col))
			continue;		//������Ϊ��
		if(IsFieldPK(col) && IsFieldPKAutoIncrease(col))
				continue;			//�Զ�������������
		for(row=0; row < GetRowCount(); row++)
		{
			if(!GetRowModified(row + 1))
				continue;			//����δ�޸ģ������
			
			if(!GetItemModified(row + 1, col))
				continue;	//����δ�޸�
			
			CString s = GetAsString(col,row + 1);
			if(!s.IsEmpty())
				continue;				//���ݷǿ�
			
			s.Format(_T("Field %s at row %d is Empty."), GetFieldName(col), row);
			
			CDataSetErrorInfo::Instance()->PrintInfo(s);
			return FALSE;
		}
	}

	//��ֹfilter�е�����û�б�����2008.5.8
	const int filterBufferCount = CopyFilterToPrimaryBuffer();

	{
//		UIBase::CTiming timing1(_T("CSQLDataSet.DoSqlUpdate.DoMarkSql"),TRUE);

		CPMSQLBaseCtrl::CSQLTransaction doSqlTransaction(m_pBaseCtrl);
		
		/////////////////////////////////////////////////////////////////
		//Step2: ����Delete��SQL���
		/////////////////////////////////////////////////////////////////
		DoDeleteActive(arrPKCol);
		
		/////////////////////////////////////////////////////////////////
		//Step3: ����Update��SQL���
		/////////////////////////////////////////////////////////////////
		DoMdfActive(arrPKCol);
		
		/////////////////////////////////////////////////////////////////
		//Step4: ����Insert���, �Զ����ɵ�������ҪLoop 5��
		/////////////////////////////////////////////////////////////////
		//����insert���Ĺ̶��ֶ�������
		DoInsertActive();
	}

	ResetUpdate(FALSE);
	
	RemoveCopyedFilterToPrimaryBuffer(filterBufferCount);
	return TRUE;
}
