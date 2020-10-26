      // NGridColumn.cpp: implementation of the NGridColumn class.
//     
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NDataField.h"
#include "NDataModelDataSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NDataField::NDataField(void *pOwner)
{
	m_fieldNo = 0;
	m_dataSize = 0;
	m_pOwner = pOwner;
	m_DataType = ftInteger;
	m_pFilterValue = NULL;
	m_iFilerValueSize = 0;

	m_Status=0x80 | 0x0010 |0x0100;	//Visible、EditAble、SortAble, CanNull,ParseSQLAble
	m_pFuncompare = NULL;
	m_isUpdateAsBlobFiled = FALSE;

	m_iSpecStatus = 0;
}

NDataField::~NDataField()
{
	//FreeFilterValue();
  if (m_pFilterValue != NULL) {
	  delete []m_pFilterValue;
	  m_pFilterValue = NULL;
  }
}

void NDataField::OnChanged()
{

}

// void NDataField::DoDrawCell(CDC *pDC,CRect *rect,BOOL Selected, BOOL Focused,BOOL 
// 							 NewitemRow,BOOL RightEdge,CBrush *br,CString *text,COLORREF color,
// 							 CFont *font,Alignment alig,BOOL ADone)
// {
// 
// }

/*inline int NDataField::GetSize()
{
	return m_dataSize;
}*/

/*int NDataField::Compare(void *pData1, void *pData2)
{
	return 

}*/

/*BOOL NDataField::GetAsBoolean()
{

}

COleDateTime NDataField::GetAsDateTime()
{

}

Double NDataField::GetAsFloat()
{

}

long NDataField::GetAsInteger()
{

}

CString NDataField::GetAsString()
{

}
*/

/*BOOL NDataField::GetData(int row, void *buffer)
{
	//return m_pModelDataSet->m_PrimaryBuffers.GetFieldData(this,row,buffer);
	return FALSE;
}*/

BOOL NDataField::SetData(void *pbuffer,const void * Sourcebuffer,int size)
{
	if(((NDataModelDataSet *)(m_pOwner))->m_State != dmstRetrieve)
	{
		if(!GetItemModified((char*)pbuffer))
		{
			if(memcmp(pbuffer,Sourcebuffer,size) != 0)//值于原值不一样
			{
				AlloOrigValueBuff(pbuffer);
				SetItemModified((char*)pbuffer,TRUE);
				//开始设置行改变的属性
				((NDataModelDataSet *)(m_pOwner))->SetRowModifiedInternal((char*)((NDataModelDataSet *)(m_pOwner))->GetRowBuffPtr(this,(char*)pbuffer),TRUE);
				//((NDataModelDataSet *)(m_pOwner))->get
			}
			else
			{
				return FALSE;
			}
		}
	}
	CopyMemory(pbuffer,Sourcebuffer,size);
	return TRUE;
}
void NDataField::FreeBolbData(void *pRowbufferOffset)
{
	if (m_DataType == ftBlob) 
	{
		void *ptr;
   		//long iSize;
		//CopyMemory(&iSize,(char*)(pRowbufferOffset)+sizeof(void*),sizeof(long));
		CopyMemory(&ptr,pRowbufferOffset,sizeof(void*));
		if (ptr != NULL) {
			delete [](char*)ptr;
		}
		//将指针清空
		memset(pRowbufferOffset,0,GetSize());
	}
}

BOOL NDataField::SetBolbData(void *pRowbufferOffset, void *pBlob, long iSize,BOOL alloOrigValueBuff)
{
	if(((NDataModelDataSet *)(m_pOwner))->m_State != dmstRetrieve)
	{
		if(!GetItemModified((char*)pRowbufferOffset))
		{
			//取得原数据指针
			void *ptr;
			long orgSize;
            CopyMemory(&orgSize,(char*)(pRowbufferOffset)+sizeof(void*),sizeof(long));
			CopyMemory(&ptr,pRowbufferOffset,sizeof(void*));
			if(orgSize != iSize || memcmp(ptr,pBlob,iSize) != 0)//值于原值不一样
			{
				if(alloOrigValueBuff == TRUE)
				   AlloOrigValueBuff(pRowbufferOffset);
				SetItemModified((char*)pRowbufferOffset,TRUE);
				//开始设置行改变的属性
				((NDataModelDataSet *)(m_pOwner))->SetRowModifiedInternal((char*)((NDataModelDataSet *)(m_pOwner))->GetRowBuffPtr(this,(char*)pRowbufferOffset),TRUE);
				//((NDataModelDataSet *)(m_pOwner))->get
			}
			else
			{
				delete []((char*)pBlob);
				return FALSE;
			}
		}
        else
		{
			FreeBolbData(pRowbufferOffset);
		}
		
	}
	else
		FreeBolbData(pRowbufferOffset);

	CopyMemory(pRowbufferOffset,&pBlob,sizeof(void*));
	CopyMemory((char*)pRowbufferOffset+sizeof(void*),&iSize,sizeof(long));
	return TRUE;
}
inline void NDataField::AlloOrigValueBuff(void *buff)
{
	/********************************************************************
	liwenlang
		时间:2010:10:26   15:05
		备注:去除 DataSet修改历史记录功能
	*********************************************************************/
// 	char *itemInfo = GetItemInfo((char *)buff);
// 	itemInfo++;//增加状态位
// 	char * ptr;
// 	CopyMemory(&ptr,itemInfo,sizeof(char*));
// 	if ((ptr) == NULL) //尚未分配内存
// 	{
// 		ptr = new char[GetSize()];
// 		//(*((long*)itemInfo)) = (long)(newptr);
// 		CopyMemory(itemInfo,&ptr,sizeof(char*));
// 		//test
// 		/*char *test = (char*)(long)(newptr);
// 		 test =(char*) *itemInfo;
// 		 CopyMemory(&test,itemInfo,sizeof(char*));*/
// 	}
// 	CopyMemory((void*)ptr,buff,GetSize());
	if(ftBlob == this->GetDataType())
		FreeBolbData(buff);
}

void NDataField::FreeOrigValueBuff(void *buff,BOOL DeleteBuf/*,BOOL deleteBlobData*/)
{
	char *itemInfo = GetItemInfo((char *)buff);
	itemInfo++;//增加状态位
	char * ptr;
	CopyMemory(&ptr,itemInfo,sizeof(char*));
	if (((ptr) != NULL) && (DeleteBuf)) //尚未分配内存
	{
		//增加大字段支持修改
		if (m_DataType == ftBlob) 
		{
			char *tempptr = NULL;
			CopyMemory((void*)&tempptr,ptr,sizeof(void*));
			if (tempptr != NULL /*&& deleteBlobData == TRUE*/) 
			{
				delete []tempptr;
			}
		}
		delete []ptr;
	}
    //清除指针
	ptr = NULL;
	CopyMemory(itemInfo,&ptr,sizeof(char*));

}

void NDataField::ClearModified(void *buff)
{
	if(!GetItemModified((char*)buff)) return;

	FreeOrigValueBuff(buff);
	SetItemModified((char*)buff,FALSE);
}

char* NDataField::GetItemInfo(char *pbuffer)
{
    //char *pbuff =(char *) pbuffer;
	pbuffer += GetSize();//1BYTE 的状态位置;
	return pbuffer;
}

void NDataField::SetItemModified(char *pbuffer, BOOL bModified)
{ 
   char *iteminfo = GetItemInfo(pbuffer);
   BYTE status = 0;
   if (iteminfo != NULL) 
   {
	   	 if(bModified) 
			 status |= 0x01; 
		 else
		 {
			 //开始将所有的原值空间释放
			 status &= ~(0x01);
		 }
		 //(*iteminfo) = status;
		 CopyMemory(iteminfo,&status,sizeof(BYTE));
   }
}


BOOL NDataField::GetItemModified(char *pbuffer)
{
   BYTE *iteminfo = (BYTE *)GetItemInfo(pbuffer); 
   if (iteminfo != NULL) 
   {
	   BYTE byte = *iteminfo;
	   return ((byte & 0x01) ? TRUE:FALSE); 
   }
   return FALSE;
}

void NDataField::SetDataType(FieldType type)
{
	m_DataType = type;
}

FieldType NDataField::GetDataType( ) const
{
	return m_DataType;
}


void NDataField::GetText(CString &cstr ,void *buffer)
{
	cstr = GetAsString(buffer);
}


CString NDataField::GetAsString(const void *buffer)
{
	CString str("");
	return str;
}
int NDataField::GetAsInteger(const void *buffer)
{
	return 0;
}
double NDataField::GetAsFloat(const void *buffer)
{
	return 0;
}
BOOL NDataField::GetAsBoolean(const  void *buffer)
{
	return FALSE;
}
COleDateTime NDataField::GetAsDateTime(const void *buffer)
{
	COleDateTime dt;
	return dt;
}

VARIANT NDataField::GetAsVariant(const void * buffer)
{
	VARIANT var;
	var.vt = VT_EMPTY;
	return var;
}

BOOL NDataField::SetAsString(const CString & val,void *buffer)
{
	return FALSE;
}
BOOL NDataField::SetAsInteger(const long & val,void *buffer)
{
	return FALSE;
}
BOOL NDataField::SetAsFloat(const double & val,void *buffer)
{
	return FALSE;
}
BOOL NDataField::SetAsBoolean(const BOOL & val,void *buffer)
{
	return FALSE;
}
BOOL NDataField::SetAsDateTime(const COleDateTime & val,void *buffer)
{
	return FALSE;
}
BOOL NDataField::SetAsVariant(const VARIANT & val,void *buffer)
{
	return FALSE;
}

int NDataField::Compare(const void * pData1,const void * pData2)
{
	//return 0;
	//进行内存比较
	return memcmp(pData1,pData2,GetSize());
}


void NDataField::SetSize(int size)
{
	m_dataSize = size;
}

void NDataField::SetFieldName(CString name)
{
   m_FieldName = name;
}

CString NDataField::GetFieldName( )
{
   return m_FieldName;
}
//返回FALSE不符合过滤条件，最终进行过滤。 ＝ TRUE复合过滤条件
BOOL NDataField::FilterCompare(const void * pData1)
{
	if (m_pFilterValue == NULL || m_pFuncompare == NULL) {
		return TRUE;
	}
	return (this->*m_pFuncompare)(pData1,m_pFilterValue);
	return TRUE;
}
 BOOL  NDataField::StringTranToThisType(CString sValue,void *& pValue,int &iSize)
{
	return FALSE;
}
BOOL NDataField::SetFilter(LPTSTR sOperator ,LPTSTR sValue)
{
	//先将数据转换为本数据类型
	int isize;
	FreeFilterValue();
	if(StringTranToThisType(sValue,m_pFilterValue,isize)== FALSE)
	{
		return FALSE;
	}
	
	//NDataField::SetFilter(sOperator,sValue);
	CString sop = sOperator;
	if (sop == _T("<")) {
		m_pFuncompare = (BOOL (NDataField::*) ( const void *  ,const void * ))(&NDataField::IsLess);
	}
	else if (sop == _T(">")) {
		m_pFuncompare = (BOOL (NDataField::*) ( const void *  ,const void * ))(&NDataField::IsGreater);
	}
	else if (sop == _T("=")) {
		m_pFuncompare = (BOOL (NDataField::*) ( const void *  ,const void * ))(&NDataField::IsEqual);
	}
	else {
		return FALSE;
	}
	
//	m_pFilterValue = new BYTE [GetSize()];
//	CopyMemory((char *)m_pFilterValue,sValue,GetSize());
	m_FilterOperator = sOperator;
	return TRUE;
	//return FALSE;
}

BOOL NDataField::FreeFilterValue()
{
	return FALSE;
}

BOOL NDataField::IsEqual(const void * pData1,const void * pData2)
{
	return memcmp(pData2,pData1,m_dataSize) == 0;
}

/*BOOL NDataField::FilterCompareLike(const void * pData1,const void * pData2)
{
	return strcspn((char *)pData1,(char *)pData2) >= 0;
}*/

BOOL NDataField::IsGreater(const void * pData1,const void * pData2)
{
	//BOOL test = strcmp((char *)pData1,(char *)pData2) > 0;
// 	CString strData1 = (char *)pData1;
// 	CString strData2 = (char *)pData2;
	return memcmp(pData1,pData2,m_dataSize) > 0;
}

BOOL NDataField::IsLess(const void * pData1,const void * pData2)
{
// 	int iData1 = *((int *)pData1);
// 	int iData2 = *((int *)pData2);
	return memcmp(pData1,pData2,m_dataSize) < 0;
}

long NDataField::GetBlob(const void *pRowbufferOffset,void *&pBlob)
{
	pBlob = NULL;
   return 0;
}

BOOL NDataField::SetBlob(void *pRowbufferOffset,void *pBlob,long iBlockSize,BOOL alloOrigValueBuff)
{
	return FALSE;
}


