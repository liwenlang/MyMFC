 // NDbDataSet.cpp: implementation of the NDbDataSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NDbDataSet.h"
#include "NDataModelDataSet_ShareData.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE( NDbDataSet, NDataModelDataSet )

NDbDataSet::NDbDataSet()
{
   m_pDataService = NULL;
   m_isRetrieved = FALSE;
}

NDbDataSet::~NDbDataSet()
{

}

int NDbDataSet::Retrieve()
{
// 	CString sBuff;
// 	if (m_pDataService == NULL || m_sql.GetLength() <= 0) 
// 	{
// 		return - 1;
// 	}
// 	if(m_pDataService->Retrieve(sBuff,m_sql))
// 	{
// 		LoadFromCString(sBuff);
// 		m_isRetrieved = TRUE;
// 		return GetRowCount();
// 	}
// 	else
// 	{
// 		return FALSE;
// 	}
// 	m_isRetrieved = TRUE;
// 	return 0;
    if (m_pDataService == NULL || m_sql.GetLength() <= 0) 
 	{
 		return - 1;
 	}
	SetState(dmstRetrieve);
	
	if(m_pDataService->FillDataSet(&m_xDataFill,m_sql) == TRUE)
	{
		SetState(dmstNormal);
		return this->GetRowCount();
	}
	else
	{
		SetState(dmstNormal);
		return -1;
	}
}

void NDbDataSet::SetComDB(CDataServiceAbsolute* pComDB)
{
  m_pDataService = pComDB;
}

void NDbDataSet::SetSQL(CString sSQL)
{
   m_sql = sSQL;
}

BOOL NDbDataSet::ParseSQL()
{
   if (m_fields.GetCount() <= 0 || m_TableName == "") 
   {
	   return FALSE;
   }
   CString ssql;
   ssql = _T("Select ");
   CString sFieldName;
   NDataField *pField;
   for(int i = 0 ; i < m_fields.GetCount() ; i ++)
   {
	   pField = m_fields.GetField(i);
	   if (pField != NULL&&pField->IsParseSQLAble()) 
	   {
		   sFieldName = pField->GetFieldName();
		   ssql += ((i != 0 ? _T(" ,"):_T(" ")) + sFieldName);
	   }
   }
   ssql += (_T(" from ") + m_TableName);
   //开始将所有的sql语句变为小写
   ssql.MakeLower();
   m_sql = ssql;
   return TRUE;
}

CString NDbDataSet::GetSQL()
{
   return  m_sql;
}

void NDbDataSet::SetWhere(CString sWhere,BOOL bReplaceWhere)
{
	sWhere.TrimLeft();
	if (sWhere.IsEmpty()) 
	{
		return;
	}
	sWhere.MakeLower();
	
	m_sql.MakeLower();
	if (m_sql.Find(_T("where")) != -1) 
	{
	   if (sWhere.Find(_T("where")) == 0) 
	   {
		   sWhere.Replace(_T("where"),_T(""));
	   }

		if (bReplaceWhere) 
		{
          //开始替换原来sql中的where语句
		   m_sql = m_sql.Mid(0,m_sql.Find(_T("where")));
		   m_sql+=_T(" where ") + sWhere;
		}
		else
		{
			m_sql+=_T(" and ")+sWhere;
		}
       
	}
	else
	{
	   if (sWhere.Find(_T("where")) == 0) 
	   {
		   m_sql +=( _T(" ")+sWhere);
	   }
	   else
		 m_sql +=(_T(" where ") + sWhere);
	 }
	//TRACE(m_sql);
}

int NDbDataSet::Update()
{
	
   if (m_pDataService == NULL) 
   {
	   return -1;
   }

   	CStringArray stringarray;
	CString str;

 //  m_pDataService->BeginTrans();
   GetUpdateSql(stringarray);
	for(int i = 0; i < stringarray.GetSize(); i++)
	{
		str = stringarray.GetAt(i);
		m_pDataService->ExecuteNonQuery(str);
	}
	//开始执行更新大字段的操作，前面阶段一般都进行了插入操作，因此更新大字段不会出现问题
    UpdateBlobField();
//	BOOL result = m_pDataService->Commit();
//	return result==TRUE?1:-1;
	return TRUE;
}

void NDbDataSet::UpdateBlobField()
{
	//这里的UpdateBlob只有当设置了SetUpdateAsBlobFiled的字段进行大字段更新操作
   CString TableName = GetTableName();

	CString s, statement, where, insertCol,fieldName;
	int row, col, i;
	CStringArray arrPKFieldName;
	CUIntArray arrPKCol;

	//获得PK数组
	for(col=0; col < GetFieldCount(); col++) {
		if(!IsFieldPK(col)) continue;
		//if(TableName.CompareNoCase(GetColTableName(col))) continue;
		arrPKCol.Add(col);
		arrPKFieldName.Add(GetFieldName(col));
	}
	
	//防止filter中的数据没有被更新2008.5.8
	const int filterBufferCount = CopyFilterToPrimaryBuffer();

	for(row=0; row<GetRowCount(); row++) 
	{
		if(GetRowNew(row + 1)) continue;				//new的行不作Update处理
		if(!GetRowModified(row + 1)) continue;			//内容未修改

		//生成where子句
		where.Empty();
		for(i=0; i < arrPKCol.GetSize(); i++) {
			col=arrPKCol.GetAt(i);
			if(i) where+= _T(" and ");
			where += arrPKFieldName.GetAt(i);

			s=GetAsString(col,row + 1,Primary,TRUE);//CDataTableStatic::GetOriginData(row, col);
			if(s.IsEmpty())
				where+=_T("=null");
			else {
				if(GetFieldDataType(col)==ftInteger || GetFieldDataType(col)==ftFloat)
					where+=_T("=") +s;
				else
					where+=_T("='") +Value2SQLValue(s)+ _T("'");
			}
		}
        //进行每个字段的循环
		statement.Empty();
		for(col=0; col < GetFieldCount(); col++) 
		{
			if(!IsFieldUpdateAble(col)) continue;		//列不可存盘
			if(!GetItemModified(row + 1, col)) continue;	//列内容未改
			if (IsUpdateAsBlobFiled(col) != TRUE) continue;         //如果不是大字段则不进行生成2008.1.10
			//if(TableName.CompareNoCase(GetColTableName(col))) continue;		//非本表

			s=GetAsString(col,row + 1);//CDataTableStatic::GetData(row, col);
			if(!statement.IsEmpty()) statement+=_T(",");
			statement+=GetFieldName(col);
			if(s.IsEmpty())
				statement+=_T("=null");
			else {
				if(GetFieldDataType(col)==ftInteger || GetFieldDataType(col)==ftFloat)
					statement+=_T("=") +s;
				else
					statement+=_T("='") +Value2SQLValue(s)+ _T("'");
			}
			fieldName = GetFieldName(col);
			statement.Format(_T("update %s set %s=? where %s"), TableName, fieldName, where);
			void * ppBolb;
			int size =  GetBlob(fieldName, row + 1 ,ppBolb);
			if (size >0 )
			{
				m_pDataService->UpdateBlob(statement,(LPSTR)ppBolb,size);
			}
			
			//执行updateBlob语句

		}
		
	}
	ResetUpdate();
	//防止filter中的数据没有被更新2008.5.8恢复过滤的数组2008.5.8
	RemoveCopyedFilterToPrimaryBuffer(filterBufferCount);
}
int NDbDataSet::RetrieveByXml()
{
	CString sBuff;
	if (m_pDataService == NULL || m_sql.GetLength() <= 0) 
	{
		return - 1;
	}
	if(m_pDataService->RetrieveXml(sBuff,m_sql))
	{
		BSTR  bsbuff = sBuff.AllocSysString();

		LoadFromXML(bsbuff);
	   if (bsbuff != NULL) {
		   SysFreeString(bsbuff);
	   }
	   m_isRetrieved = TRUE;

		return GetRowCount();
	}
	else
	{
		return -1;
	}

// 	//开始进行跟踪数据
//         CFile aa;
//    		aa.Open("this.txt",CFile::modeCreate|CFile::shareExclusive);
//    		aa.Close();
//            aa.Open("this.txt",CFile::modeWrite|CFile::shareExclusive);
//    		aa.Write(sBuff,sBuff.GetLength());
//    		aa.Close();    


	m_isRetrieved = TRUE;
	return 0;
}

BOOL NDbDataSet::GetRetrieved()
{
  return m_isRetrieved;
}
void NDbDataSet::SetFieldParseSQLAble(int col,BOOL b )
{
   NDataField *field = GetField(col);
   if (field != NULL) 
   {
	    field->SetParseSQLAble(b);
   }
}

void NDbDataSet::DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff)
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

// 
// int NDbDataSet::FillData()
// {
// 	if (m_pIUnknownFreeThreadedMarshaler == NULL)
// 	{
// 		InitCom();
// 	}
//    	CString sBuff;
// 	if (m_pDataService == NULL || m_sql.GetLength() <= 0) 
// 	{
// 		return - 1;
// 	}
// 	if(m_pDataService->FillDataSet(static_cast<IDataSetFill*>(this),m_sql))
// 	{
// 		return GetRowCount();
// 	}
// 	else
// 	{
// 		return -1;
// 	}
// }
// 
// HRESULT __stdcall NDbDataSet::QueryInterface(const IID& iid, void** ppv)
// { 	
// 	if (iid == IID_IUnknown)
// 	{
// 		//trace("Return pointer to IUnknown.") ;
// 		*ppv = static_cast<IDataSetFill*>(this) ;
// 	} 
// 	else if (iid == IID_DATASETFILL)
// 	{
// 		//trace("Return pointer to IX.") ;
// 		*ppv = static_cast<IDataSetFill*>(this) ;
// 	}
// // 	else if (iid == IID_IMarshal)
// // 	{
// // 		HRESULT  rt= m_pIUnknownFreeThreadedMarshaler->QueryInterface(iid,
// // 		                                                        ppv);
// // 		return  rt;
// // 	}
// 	else
// 	{  	   
// 		//trace("Interface not supported.") ;
// 		*ppv = NULL ;
// 		return E_NOINTERFACE ;
// 	}
// 	reinterpret_cast<IUnknown*>(*ppv)->AddRef() ; 
// 	return S_OK ;
// }
// 
// ULONG __stdcall NDbDataSet::AddRef()
// {
// 	return InterlockedIncrement(&m_cRef) ;
// }
// 
// ULONG __stdcall NDbDataSet::Release() 
// {
// 	if (InterlockedDecrement(&m_cRef) == 0)
// 	{
// 		//delete this ;
// 		return 0 ;
// 	}
// 	return m_cRef ;
// 
// }
// 
// BOOL __stdcall NDbDataSet::FillItem(int columnIndex,int rowindex,VARIANT& val) 
// {
// 	int vt=val.vt;
// 	CString ColDat;
//    if((vt!=VT_NULL) && (vt!=VT_EMPTY)) 
//    {
// 		if(vt==VT_DATE)			//Date
// 		{
// 			CPub::VariantDate2CString(ColDat, val);
// 			SetAsString(columnIndex,rowindex+1,ColDat);
// 		}
// 		else if(vt & VT_ARRAY)	//Binary
// 		{
// 			    LPSTR pBufer;
// 				SAFEARRAY *psa=NULL;
// 				LPSTR p;
// 				psa=val.parray;
// 				long size=psa->rgsabound[0].cElements;
// 				SafeArrayAccessData(psa, (void HUGEP**)&p);
// 				
// 				pBufer=(LPSTR)malloc(size);
// 				memcpy(pBufer, p, size);
// 				
// 				SafeArrayUnaccessData(psa);
// 
// 			    SetBlob(columnIndex,rowindex+1,pBufer,size);
// 				free(pBufer);
// 			;
// 		}
// 		else {
// 			
// 				ColDat=(LPTSTR)(_bstr_t)val;
// 				SetAsString(columnIndex,rowindex+1,ColDat);
// 		}
//    }
//    return TRUE;
// }
//  //这里返回行的索引
//  int __stdcall NDbDataSet:: AddRow()
//  {
// 	 return InsertRow(0) - 1;
//  }


BOOL NDbDataSet::CDataFill::SetValue(int colIndex,int rowIndex,CString &columnName,VARIANT &value)
{
	METHOD_PROLOGUE_(NDbDataSet, DataFill);
	//CString ColDat;
	//WORD dosDate, dosTime;
	//COleDateTime cTime;
    int col = pThis->GetColNum(columnName);
	if (col == -1)
	{
		return FALSE;
	}
	if((value.vt!=VT_NULL) && (value.vt!=VT_EMPTY)) 
	{
// 		switch(value.vt)
// 		{
// 		case VT_DATE:
// 				
// 				VariantTimeToDosDateTime(value.date, &dosDate, &dosTime);
// 				//CTime cTime(dosDate, dosTime);
// 				//COleDateTime cTime(dosDate, dosTime);
// 				//cTime.SetTime()
// 				cTime.m_status = DosDateTimeToVariantTime(dosDate, dosTime, &(cTime.m_dt)) ?COleDateTime::valid : COleDateTime::invalid;
// 			    pThis->SetAsDateTime(colIndex,rowIndex+1,cTime);
// 			break;
// 		case VT_ARRAY:
// 
// 			SetBlobValue(colIndex,rowIndex,value);
// 			break;
// // 		case :
// // 		    break;
// // 		case :
// // 		    break;
// 		default:
// 			ColDat=(LPTSTR)(_bstr_t)value;
// 			pThis->SetAsString(colIndex,rowIndex+1,ColDat);
// 		    break;
// 		}
		if(value.vt==VT_DATE)			//Date
		{
				WORD dosDate, dosTime;
				VariantTimeToDosDateTime(value.date, &dosDate, &dosTime);
				//CTime cTime(dosDate, dosTime);
				COleDateTime cTime(dosDate, dosTime);
			    pThis->SetAsDateTime(col,rowIndex+1,cTime);
		}
		else if(value.vt & VT_ARRAY)	//Binary
		{
			SetBlobValue(col,rowIndex,value);
		}
		else 
		{
			CString ColDat=(LPTSTR)(_bstr_t)value;
			pThis->SetAsString(col,rowIndex+1,ColDat);
		}
	}
	return TRUE;
}
int NDbDataSet::CDataFill::AddRow()
{
	METHOD_PROLOGUE_(NDbDataSet, DataFill);
	return pThis->InsertRow(0);
	
}

BOOL NDbDataSet::CDataFill::SetBlobValue(int colIndex,int rowIndex,VARIANT &value)
{
	METHOD_PROLOGUE_(NDbDataSet, DataFill);
			    LPSTR pBufer;
				SAFEARRAY *psa=NULL;
				LPSTR p;
	            psa=value.parray;
				long size=psa->rgsabound[0].cElements;
				SafeArrayAccessData(psa, (void HUGEP**)&p);
				
				pBufer=(LPSTR)malloc(size);
				memcpy(pBufer, p, size);
				
// 		CFile aa;
//    		aa.Open("this.txt",CFile::modeCreate|CFile::shareExclusive);
//    		aa.Close();
//            aa.Open("this.txt",CFile::modeWrite|CFile::shareExclusive);
//    		aa.Write(pBufer,size);
//    		aa.Close(); 

				SafeArrayUnaccessData(psa);

			    pThis->SetBlob(colIndex,rowIndex+1,pBufer,size);
				free(pBufer);
				//SafeArrayDestroy(psa);
				return TRUE;
}
