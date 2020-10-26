// NDataModelDataSetBuffer.cpp: implementation of the NDataModelDataSetBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NDataModelDataSetBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NDataModelDataSetBuffer::NDataModelDataSetBuffer()
{
	m_CurrentRow = 0;
	m_owner = NULL;
}

NDataModelDataSetBuffer::~NDataModelDataSetBuffer()
{
	ResetBuffers();
}


int NDataModelDataSetBuffer::GetRowCount()
{
	return (int)m_Buffers.GetSize();

}

BOOL NDataModelDataSetBuffer::SetRow(int currentrow)
{   
	if (currentrow <=0 || currentrow >GetRowCount()) 
		return FALSE;

	m_CurrentRow = currentrow;
	return TRUE;
}


BOOL NDataModelDataSetBuffer::SetSort(NDataFields sortfield)
{
  return FALSE;
}

//  提供对low开始 到high结束的排序
BOOL NDataModelDataSetBuffer::QSort(int low, int high)
{
	if (m_Sortdfields.GetSize()<=0 ) return FALSE;

    if (high == -1)
        high = GetRowCount() - 1;

    int lo = low;
    int hi = high;
    
    if (hi <= lo)
        return FALSE;

    void** pData;
	pData = m_Buffers.GetData();
	if (pData == NULL) return FALSE;

	int mid = (lo + hi)/2;
	
    while (lo <= hi)
    {
            while (lo < high  && OnSortCompare( lo,  mid) < 0)
                ++lo;

			while (hi > low && OnSortCompare(hi,  mid) > 0)
				--hi;
			
        if (lo <= hi)
        {
			//对中间位置进行记录
			if (lo == mid || hi == mid)
			{
				if (lo == mid)
					mid = hi;
				else
					mid = lo;
			}
            //开始进行置换位置
			void * temp;
			temp = pData[lo-1] ;
			pData[lo-1] = pData[hi-1];
			pData[hi-1] = temp;
			
            ++lo;
            --hi;
         }
    }

    if (low < hi)
        QSort( low, hi);
    
    if (lo < high)
        QSort(lo, high);
    
    return TRUE;
}


int NDataModelDataSetBuffer::OnSortCompare(int low, int high)
{
	int iResult = 0;
	NDataField * field;
	for(int Col = 0 ;Col < m_Sortdfields.GetSize() ;Col++)
	{
			//进行比较
		    field =(NDataField *) m_Sortdfields.GetAt(Col);
		
		/********************************************************************
		liwenlang
		时间:2010:10:29   11:08
		备注:让 DataSet判断是否要 用默认实现
		*********************************************************************/
		NRecordID lowRecordID = m_Buffers.GetAt(low - 1);
		NRecordID highRecordID = m_Buffers.GetAt(high - 1);

//		if(!m_owner->OnSortCompareField(low,high,field,iResult))
//			iResult = field->Compare(m_owner->GetFieldDataPT(field,m_Buffers.GetAt(low - 1)),m_owner->GetFieldDataPT(field,m_Buffers.GetAt(high - 1)));
		if(!m_owner->OnSortCompareField(lowRecordID,highRecordID,field,iResult))
			iResult = field->Compare(m_owner->GetFieldDataPT(field,lowRecordID),m_owner->GetFieldDataPT(field,highRecordID));
			
			if(iResult == 0 ) continue;
		    //取得排序方式
			if(!m_SortColAscending.GetAt(Col))//绛序
			{
				iResult = -iResult;
			}
			return iResult;
	}
	return iResult;
}

inline BOOL NDataModelDataSetBuffer::GetFieldData(NDataField *Field,int row,  void *buffer)
{
	/*int iFieldOffset;
	iFieldOffset = m_owner->GetFieldOffset(Field);
    char * ptr;
	ptr = (char *)m_Buffers.GetAt(row -1);
	if (ptr == NULL) return FALSE;
	ptr += iFieldOffset;*/
	const void * ptr = m_owner->GetFieldDataPT(Field, m_Buffers.GetAt(row - 1));
	if(NULL == ptr)
		return FALSE;

	CopyMemory(buffer,ptr,Field->GetSize());
	return TRUE;
}

inline void NDataModelDataSetBuffer::SetFieldData(NDataField *field, int row ,void *buffer)
{
	int iFieldOffset;
	iFieldOffset = m_owner->GetFieldOffset(field);
    char * ptr;
	ptr = (char *)m_Buffers.GetAt(row -1);
	ASSERT(ptr>0);
	ptr += iFieldOffset;
	CopyMemory(ptr,buffer,field->GetSize());

}

//取得当前字段的内存指针
// inline  void * NDataModelDataSetBuffer::GetFieldDataPT(NDataField *field,int row, BOOL originalvalue )
// {
//    return GetFieldDataPT(field, m_Buffers.GetAt(row -1),originalvalue);
// 	/*ptr =(char *) m_Buffers.GetAt(row -1);
// 	ASSERT(ptr != NULL);
// 	ptr += iFieldOffset;
// 	return ptr;*/
// }
// inline  void * NDataModelDataSetBuffer::GetFieldDataPT(NDataField *field,void *buff, BOOL originalvalue )
// {
// 	/*ASSERT(buff != NULL);
// 	int iFieldOffset;
// 	iFieldOffset = m_owner->GetFieldOffset(field);
//     char * ptr = (char *)buff;
// 	//是否是原值进行判断
// 	if (originalvalue) 
// 	{
// 		//取得原值的指针
// 		ptr+= iFieldOffset + field->GetSize() + 1;//1BYTE 的状态位置;
// 		ptr = (char*)(*ptr);
// 		if (ptr == NULL)
// 		{
// 			ptr = (char *)buff;
// 			ptr += iFieldOffset;
// 		}
// 	}
// 	else
// 	{
// 	   ptr += iFieldOffset;
// 	}
// 	return ptr;*/
// 	return m_owner->GetFieldDataPT(field,buff,originalvalue);
// }

/*inline  void * NDataModelDataSetBuffer::AlloOrigValueBuff(NDataField *field,void *buff, BOOL originalvalue,BOOL AlloOrigBuff)
{
}*/

//CString NDataModelDataSetBuffer::GetAsString(NDataField *field,int row, BOOL originalvalue)
//{
	/*BOOL result;
	char * buffer;
	buffer = InitFieldMem(field);
    result = GetFieldData(field ,row ,buffer);
	if (result == FALSE) 
		return CString("");*/
//	CString sresult;
//	void * buffer = GetFieldDataPT(field ,row );
//	sresult = field->GetAsString(buffer);
//	delete []buffer;
//	return sresult;
//}

//double  NDataModelDataSetBuffer::GetAsFloat(NDataField *field,int row, BOOL originalvalue)
//{
	/*BOOL result;
	char * buffer;
	buffer = InitFieldMem(field);
    result = GetFieldData(field ,row ,buffer);
	if (result == FALSE) 
	{
      delete []buffer;
	  return 0;
	}*/
//	double dresult;
//	const void * buffer = GetFieldDataPT(field ,row );
//	dresult = field->GetAsFloat(buffer);
	//delete []buffer;
//	return dresult;
//}

//int NDataModelDataSetBuffer::GetAsInteger(NDataField *field,int row, BOOL originalvalue)
//{
	/*BOOL result;
	char * buffer;
	buffer = InitFieldMem(field);
    result = GetFieldData(field ,row ,buffer);
	if (result == FALSE) 
	{
      delete []buffer;
	  return 0;
	}*/
//	int dresult;
//	const void * buffer = GetFieldDataPT(field ,row );
//	dresult = field->GetAsInteger(buffer);
	//delete []buffer;
//	return dresult;
//}

//VARIANT NDataModelDataSetBuffer::GetAsVariant(NDataField *field,int row, BOOL originalvalue)
//{
	/*BOOL result;
	char * buffer;
	VARIANT dresult;
	buffer = InitFieldMem(field);
    result = GetFieldData(field ,row ,buffer);
	if (result == FALSE) 
	{
      delete []buffer;
	  return dresult;
	}
	*/
//	VARIANT dresult;
//	const void * buffer = GetFieldDataPT(field ,row );
//	dresult = field->GetAsVariant(buffer);
	//delete []buffer;
//	return dresult;

//}

//COleDateTime  NDataModelDataSetBuffer::GetAsDateTime(NDataField *field,int row, BOOL originalvalue)
//{
	//BOOL result;
//	const void * buffer;
//	COleDateTime dresult;
	/*buffer = InitFieldMem(field);
    result = GetFieldData(field ,row ,buffer);
	if (result == FALSE) 
	{
      delete []buffer;
	  return dresult;
	}*/
//    buffer = GetFieldDataPT(field,row);
//	dresult = field->GetAsDateTime(buffer);
	//delete []buffer;
//	return dresult;
//}

//BOOL    NDataModelDataSetBuffer::GetAsBool(NDataField *field,int row, BOOL originalvalue)
//{
	/*BOOL result;
	char * buffer;
	buffer = InitFieldMem(field);
    result = GetFieldData(field ,row ,buffer);
	if (result == FALSE) 
	{
      delete []buffer;
	  return FALSE;
	}*/
//	const void * buffer = GetFieldDataPT(field ,row );
//	BOOL dresult;
//	dresult = field->GetAsBoolean(buffer);
	//delete []buffer;
//	return dresult;
//}

//BOOL NDataModelDataSetBuffer::SetAsString(NDataField *field, int row, CString &val)
//{
	/*char * buffer;
	buffer = InitFieldMem(field);
	if (field->SetAsString(val,buffer) == FALSE ) 
	{   delete [](char*)buffer;
		return FALSE;
	}
	

	SetFieldData(field,row,buffer);
	delete [](char*)buffer;
	return TRUE;*/
//	void * buffer = GetFieldDataPT(field ,row );
//	return field->SetAsString(val,buffer);
//}


//BOOL NDataModelDataSetBuffer::SetAsFloat(NDataField *field, int row ,double& val)
//{
	/*char * buffer;
	buffer = InitFieldMem(field);
	if (field->SetAsFloat(val,buffer) == FALSE ) 
	{   delete []buffer;
		return FALSE;
	}

	SetFieldData(field,row,buffer);
	delete []buffer;
	return TRUE;*/
//	void * buffer = GetFieldDataPT(field ,row );
//	return field->SetAsFloat(val,buffer);
//}

//BOOL NDataModelDataSetBuffer::SetAsInteger(NDataField *field, int row ,int & val)
//{
	/*char * buffer;
	buffer = InitFieldMem(field);
	if (field->SetAsInteger(val,buffer) == FALSE ) 
	{   delete []buffer;
		return FALSE;
	}

	SetFieldData(field,row,buffer);
	delete []buffer;
	return TRUE;*/
//	void * buffer = GetFieldDataPT(field ,row );
//	return field->SetAsInteger(val,buffer);
//}

//BOOL NDataModelDataSetBuffer::SetAsVariant(NDataField *field, int row ,VARIANT & val)
//{
/*	char * buffer;
	buffer = InitFieldMem(field);
	if (field->SetAsVariant(val,buffer) == FALSE ) 
	{   delete []buffer;
		return FALSE;
	}

	SetFieldData(field,row,buffer);
	delete []buffer;
	return TRUE;*/
//	void * buffer = GetFieldDataPT(field ,row );
//	return field->SetAsVariant(val,buffer);
//}

//BOOL NDataModelDataSetBuffer::SetAsDateTime(NDataField *field, int row ,COleDateTime& val)
//{
	/*char * buffer;
	buffer = InitFieldMem(field);
	if (field->SetAsDateTime(val,buffer) == FALSE ) 
	{   delete []buffer;
		return FALSE;
	}

	SetFieldData(field,row,buffer);
	delete []buffer;
	return TRUE;*/
//	void * buffer = GetFieldDataPT(field ,row );
//	return field->SetAsDateTime(val,buffer);	
//}

//BOOL NDataModelDataSetBuffer::SetAsBool(NDataField *field, int row ,BOOL & val)
//{
/*	char * buffer;
	buffer = InitFieldMem(field);
	if (field->SetAsBoolean(val,buffer) == FALSE ) 
	{   delete []buffer;
		return FALSE;
	}

	SetFieldData(field,row,buffer);
	delete []buffer;
	return TRUE;*/
//	void * buffer = GetFieldDataPT(field ,row );
//	return field->SetAsBoolean(val,buffer);
//}

char * NDataModelDataSetBuffer::InitFieldMem(NDataField *field)
{
	char * buffer;
	buffer = new char[field->GetSize()];
	return buffer;
}

void NDataModelDataSetBuffer::AddBuff(void *buff)
{
	m_Buffers.Add(buff);
}

/*void NDataModelDataSetBuffer::RemoveBuff(int index)
{
   m_Buffers.RemoveAt(index);
}*/

void NDataModelDataSetBuffer::SetOwner(NDataModelDataSet *owner)
{
	m_owner = owner;

}

void NDataModelDataSetBuffer::ResetBuffers()
{
    void * buff;
	int iTotal = (int)m_Buffers.GetSize();
	for(int i = 0 ;i <= iTotal-1 ; i++)
	{
		buff = m_Buffers.GetAt(i);
		//delete [] buff;
		m_owner->FreeRecordBuffer(buff);
	}
	m_Buffers.RemoveAll();
}

void NDataModelDataSetBuffer::AddSortField(NDataField *sortfield,int order)
{
	int iTotal;
	iTotal = (int)m_Sortdfields.GetSize();
	for (int i=0 ;i<iTotal ;i++)
	  if(m_Sortdfields.GetAt(i) == (void*)sortfield)
		  return ;

	m_Sortdfields.Add(sortfield);
	m_SortColAscending.Add(order);
//	m_SortColCount = (int)m_Sortdfields.GetSize();
}

void NDataModelDataSetBuffer::ResetSortFields()
{
	m_Sortdfields.RemoveAll();
	m_SortColAscending.RemoveAll();
//	m_SortColCount = 0;
}

BOOL NDataModelDataSetBuffer::Sort(int low, int high)
{
	return QSort(low,high);

}

/*void NDataModelDataSetBuffer::test()
{
	CString ss;
	int i;
	NDataField *field1,*field2,*field3,*field4,*field5;
	field1 = m_owner->m_fields.GetField(1);
	field2 = m_owner->m_fields.GetField(2);
	field3 = m_owner->m_fields.GetField(3);
	field4 = m_owner->m_fields.GetField(4);
	field5 = m_owner->m_fields.GetField(5);
	TRACE(ss+"'\r\n");
	for (i=1 ;i<=10 ;i++)
	{
	  ss = GetAsString(field1,i);
	  TRACE(ss+"'");
	  ss = GetAsString(field2,i);
	  TRACE(ss+"'");
	  ss = GetAsString(field3,i);
	  TRACE(ss+"'");
	  ss = GetAsString(field4,i);
	  TRACE(ss+"'");
	  ss = GetAsString(field5,i);
	  TRACE(ss+"\r\n");
	}
}*/



int NDataModelDataSetBuffer::InsertBuff(void *buff, int beforRowIndex)
{
	if (m_Buffers.GetSize() <= beforRowIndex || beforRowIndex </*=取消等号的状态，可能需要在0前*/0 )
	{
		AddBuff(buff);
		return (int)m_Buffers.GetSize() - 1;
	}
	m_Buffers.InsertAt(beforRowIndex,buff);
   return beforRowIndex;
}

void * NDataModelDataSetBuffer::GetBuffPtr(int index) const
{
#ifdef _TESTMEMORY
	if(index < 0 || index >= m_Buffers.GetSize())
	{
		/*CString strErrInfo;
		strErrInfo.Format(_T("NDataModelDataSetBuffer::GetBuffPtr越界,%d"),index);
		PMDataSetShowErr(strErrInfo);*/
		return NULL;
	}
#endif
   return m_Buffers.GetAt(index);
}

void NDataModelDataSetBuffer::Filter()
{
	//CPtrArray FilterBuffers;
	LPVOID * pFilterBuffers = new LPVOID[m_Buffers.GetSize()];
	int * pFilterRows = new int[m_Buffers.GetSize()];
	int iFilterCount = 0;
	
    int rowcount = GetRowCount();
	int i = 0;
	for (i = 0; i< rowcount; i++)
	{
		if (OnRowFilter(i + 1) == FALSE || m_owner->OnRowFilter(i + 1) == FALSE) 
		{
			pFilterBuffers[iFilterCount] = m_Buffers[i];
			pFilterRows[iFilterCount] = i;
			iFilterCount++;
			// 				FilterBuffers.Add(m_Buffers[i]);
			// 				m_Buffers.RemoveAt(i);
			// 				rowcount --;
			// 				i--;
		}
	}

	for(i = iFilterCount - 1;i >= 0;i--)
	{
		m_Buffers.RemoveAt(pFilterRows[i]);
	}
	m_owner->FillFilterBuff(pFilterBuffers,iFilterCount);
	delete []pFilterBuffers;
	delete [] pFilterRows;
}
// 
// void NDataModelDataSetBuffer::Filter()
// {
// 	CPtrArray FilterBuffers;
// 	
//     int rowcount = GetRowCount();
// 	for (int i = 0; i< rowcount; i++)
// 	{
// 		if (OnRowFilter(i + 1) == FALSE || m_owner->OnRowFilter(i + 1) == FALSE) 
// 		{
// //			if (m_owner->OnRowFilter(i + 1) == TRUE)
// 			{
// 		
// 				FilterBuffers.Add(m_Buffers[i]);
// 				m_Buffers.RemoveAt(i);
// 				rowcount --;
// 				i--;
// 			}
// 		}
// 	}
// 	if (FilterBuffers.GetSize() > 0) {
// 		m_owner->FillFilterBuff(FilterBuffers);
// 	}
// }

//返回FALSE 不符合过滤条件 ＝TRUE符合过滤条件进行过滤
BOOL NDataModelDataSetBuffer::OnRowFilter(int row)
{
	int FieldCount = m_owner->GetFieldCount();
	
	NDataField * field;
	for(int Col = 0 ;Col<FieldCount ;Col++)
	{
		    field =(NDataField *) m_owner->GetField(Col);
			if (field != NULL) {
				if (field->FilterCompare(m_owner->GetFieldDataPT(field,m_Buffers.GetAt(row - 1))) == FALSE) 
				{
					return FALSE;
				}
			}

	}
	return TRUE;
}

int NDataModelDataSetBuffer::MoveRow(int row, int beforRow)
{
   if ((row<0 && row > GetRowCount())) 
   {
	   return -1;   
   }

   void * ptr = GetBuffPtr(row - 1);

   if (row == beforRow) {
	   return 0;
   }
    
   	if (m_Buffers.GetSize() < beforRow || beforRow <=0 )
	{
		AddBuff(ptr);	
	}
	else
	{
		if (row < beforRow) 
		{
		 m_Buffers.InsertAt(beforRow ,ptr);
		}
		else
		 m_Buffers.InsertAt(beforRow - 1,ptr);
	}

    if ((row > beforRow) && (!(beforRow <=0)) ) 
	{
	   row++; //程序在最后remove所在的行，由于在行之前插入了移动的行，所以需要++
	}
	m_Buffers.RemoveAt(row - 1);
    return beforRow;
}

CPMPtrArray * NDataModelDataSetBuffer::GetBufferPtr()
{
   return &m_Buffers;
}

LONG NDataModelDataSetBuffer::GetBufferMemorySize()
{
	LONG lMemoryBufferSize = 0;
	LONG lBufferCount = (int)m_Buffers.GetSize();
	int iFieldCount = m_owner->GetFieldCount();

	CUIntArray iBlobFieldIndexArr;
	for(int i = 0;i < lBufferCount;i++)
	{
		NRecordID pTemRecordID = m_Buffers.GetAt(i);
		for(int j = 0;j < iFieldCount;j++)
		{
			NDataField * pTemField = m_owner->GetField(j);
			if(NULL == pTemField)
				continue;
			
			LPVOID pTemCellBuffer = m_owner->GetFieldDataPT(pTemField,pTemRecordID,FALSE);
			
			if(pTemField->GetItemModified((char*)pTemCellBuffer))
			{
				char *itemInfo = (char *)pTemCellBuffer + pTemField->GetSize();
				itemInfo++;//增加状态位
				char * ptr;
				CopyMemory(&ptr,itemInfo,sizeof(char*));
				if ((ptr) != NULL) //尚未分配内存
				{
					//增加大字段支持修改
					if (pTemField->GetDataType() == ftBlob) 
					{
						char *tempptr = NULL;
						CopyMemory((void*)&tempptr,ptr,sizeof(void*));
						if (tempptr != NULL /*&& deleteBlobData == TRUE*/) 
						{
							lMemoryBufferSize += (int)strlen(tempptr);
						}
					}
					lMemoryBufferSize += (int)strlen(ptr);
				}
			}
			
			if(ftBlob == pTemField->GetDataType())
			{
				LPVOID pTemBlob = NULL;
				lMemoryBufferSize += pTemField->GetBlob(pTemCellBuffer,pTemBlob);
			}
		}
		lMemoryBufferSize += m_owner->m_fields.GetRecordSize();
	}
	lMemoryBufferSize += sizeof(*this);
	return lMemoryBufferSize;
}
