#include "stdafx.h"
#include "CPub.h"
#define NOTESTART		_T("/*")
#define NOTEEND			_T("*/")
#define FILE_DATETIME_FORMAT	_T("%Y-%m-%d %H:%M:%S")
#define FIELD_DATETIME_FORMAT	_T("%Y.%m.%d %H:%M:%S")
#define FIELD_DATE_FORMAT		_T("%Y.%m.%d")

//////////////////////////////////////////////////////////////////////
//=======================简单的串处理==============================
//////////////////////////////////////////////////////////////////////
//返回串中sep左边的部分, 未找到返回""
CString CPub::GetLeft(CString& str, LPCTSTR seperate, BOOL bTrim)
{
	CString s;
	int iTemFind = str.Find(seperate);
	if(iTemFind <= 0)
	{
		return s;
	}
	s = str.Left(iTemFind);
	if(bTrim)
	{
		s.TrimLeft();
		s.TrimRight();
	}
	return s;
}
//返回串中sep右边的部分, 未找到返回""
CString CPub::GetRight(CString& str, LPCTSTR seperate, BOOL bTrim)
{
	CString s;
	CString sep(seperate);
	int iTemFind;
	iTemFind = str.Find(sep);
	if(iTemFind < 0)
		return s;
	s = str.Right((str.GetLength() - iTemFind) - sep.GetLength());
	if(bTrim) {
		s.TrimLeft();
		s.TrimRight();
	}
	return s;
}
//不区分大小写查找串
int CPub::FindNoCase(LPCTSTR string, LPCTSTR subs)
{
	CString s1(string), s2(subs);
	s1.MakeLower();		s2.MakeLower();
	return s1.Find(s2);
}

//从str串中找出并返回 key= 后面的value部分
CString CPub::GetEqualValue(CString& str, LPCTSTR keystr)
{
	CString key(keystr), value, s;
	key.TrimLeft();	key.TrimRight();
	if( key.Right(1) == _T('='))
		key=key.Left(key.GetLength() - 1);

	s = GetLeft(str, _T("="), TRUE);
	if(!key.CompareNoCase(s))
		value=GetRight(str, _T("="));
	return value;
}

CString CPub::Fill(LPCTSTR fillStr, int num)
{
	CString s;
	if(!fillStr) return s;
	{
		for(int xx=0;xx < num;xx++)
		{
			s+=fillStr;
		}
	}
	return s;
}

//计算子串在主串的出现次数
int CPub::CountSubString(CString& s, LPCTSTR subs)
{
	int count,len=(int)_tcslen(subs);
	long off;
	for(off=count=0;;count++) {
		off=s.Find(subs, off);
		if(off==-1) break;
		off+=len;
	}
	return count;
}

//删除右侧字符
void CPub::DeleteRight(CString& str, int nCount)
{
	int i=str.GetLength();
	if(nCount<0) return;
	if(nCount>i) nCount=i;
	str=str.Left(str.GetLength() -nCount);
}

//////////////////////////////////////////////////////////////////////
//==================== 简单mem处理 ===================================
//////////////////////////////////////////////////////////////////////
//返回字节串mem1包含mem2的位置，-1:未包含
int CPub::memmem(const LPBYTE pMem1,const LPBYTE pMem2, long offset, long lenMem1, long lenMem2)
{
	long off,i;
	if((lenMem1==0) || (lenMem2==0)) return -1;
	if(lenMem1<lenMem2) return -1;
	for(off=offset; off < (lenMem1 - lenMem2) + 1; off++) {
		for(i=0; i<lenMem2; i++)
			if(pMem1[off+i] - pMem2[i]) break;
		if(i>=lenMem2) return off;
	}
	return -1;
}

//复制一个串
//当bCompareFirst=TRUE、一致而未复制时返回FALSE
BOOL CPub::copystr(LPTSTR* ppDest, LPCTSTR pSour, BOOL bCompareFirst)
{
	if(NULL == pSour)
		return FALSE;

	UINT len=0;
	if(pSour)
		len = (int)_tcslen(pSour);

	//先比较是否相同
	if(bCompareFirst) {
		if(*ppDest) {
			if(len)
				if(!_tcscmp(*ppDest, pSour)) return FALSE;
		}
		else if(len==0) return FALSE;
	}

	//复制
	if(*ppDest) {
		free(*ppDest);
		*ppDest=NULL;
	}
	if(len) {
		*ppDest=(LPTSTR)calloc(len+1, sizeof(TCHAR));
		if(*ppDest)
			_tcscpy_s(*ppDest, strlen((char *)ppDest), pSour);
	}
	return TRUE;
}
BOOL CPub::IsDigit(TCHAR c, BOOL bIncludeDot, BOOL bIncludePunc, BOOL bIncludeNegative)
{
	if(c>=_T('0') && c<=_T('9')) return TRUE;
	if(c==_T('-') && bIncludeNegative) return TRUE;
	if(c==_T('.') && bIncludeDot) return TRUE;
	if(c==_T(',') && bIncludePunc) return TRUE;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//==================== 字符串数组 ====================================
//////////////////////////////////////////////////////////////////////
//将字符串s以sep为分隔符、转换到数组中
void CPub::CString2Array(CStringArray& Array, const CString& str, LPCTSTR sep, BOOL bTrim)
{
	CString s, buf=str, cSep(sep);
	Array.RemoveAll();
	int off, sepLen;
	if(buf.IsEmpty()) return;
	sepLen=cSep.GetLength();
	for(;;) {
		off=buf.Find(cSep,0);
		if(off == -1) break;
		s=buf.Left(off);
		if(bTrim) {
			s.TrimLeft();
			s.TrimRight();
		}
		Array.Add(s);
		buf.Delete(0, off+sepLen);
	}
	if(bTrim) {
		buf.TrimLeft();
		buf.TrimRight();
	}
	if(!buf.IsEmpty()) Array.Add(buf);
}
//将数组转换到以sep为分隔的字符串s
CString CPub::Array2CString(const CStringArray& arr, LPCTSTR sep)
{
	CString str, cSep(sep);
	int i;
	str.Empty();
	for(i=0; i<arr.GetSize(); i++) {
		if(i!=0) str+=cSep;
		str+=arr.GetAt(i);
	}
	return str;
}
//从数组中找出并返回 key= 后面的Value部分
CString CPub::ArraySearchValue(const CStringArray& arr, LPCTSTR item)
{
	int i;
	CString ret;
	for(i=0;i<arr.GetSize();i++) {
		CString strTemValue = arr.GetAt(i);
		ret=GetEqualValue(strTemValue, item);
		if(!ret.IsEmpty()) return ret;
	}
	return ret;
}
//从数组中找出string的元素序号
int CPub::ArrayCompareNoCase(const CStringArray& arr, const LPCTSTR string)
{
	int i;
	for(i=0;i<arr.GetSize();i++) {
		if(!arr.GetAt(i).CompareNoCase(string)) return i;
	}
	return -1;
}
//从数组中找出带有string的元素序号
int CPub::ArrayFindNoCase(const CStringArray& arr, const LPCTSTR string, int start)
{
	CString str(string);
	for(int i=start; i<arr.GetSize(); i++) {
		if(FindNoCase(arr.GetAt(i), str)!=-1) return i;
	}
	return -1;
}
//从数组的第n行开始找出string位于左侧的元素序号
int CPub::ArrayFindLeftNoCase(const CStringArray& arr, const LPCTSTR string, int start)
{
	CString str(string);
	for(int i=start; i<arr.GetSize(); i++) {
		if(FindNoCase(arr.GetAt(i), str)==0) return i;
	}
	return -1;
}

//===========================路径、文件、目录=========================
//返回Path1+Path2, 能自动判断处理Path1后面的\符号
CString CPub::AppendPath(const CString & Path1,const CString & Path2)
{
	CString s;
	s=Path1;
	if(!s.IsEmpty()) if(s.Right(1)!=_T('\\')) s+=_T("\\");
	s+=Path2;
	return s;
}

//////////////////////////////////////////////////////////////////////
//==========================转换======================================
//////////////////////////////////////////////////////////////////////
//将BSTR类型转换成CString类型(此函数比直接调用W2A要安全，可避免栈溢出)
BOOL CPub::BSTR2CString(CString& s, const BSTR bstr)
{
	s.Empty();
	if(bstr) {
		USES_CONVERSION;
		s=W2T(bstr);
		return TRUE;
	}
	return FALSE;
}
//VARIANT转换成CString
BOOL CPub::VariantDate2CString(CString&s, VARIANT& var)
{
	//修改By zhangym 2003.02.13
	s.Empty();
	if(var.vt!=VT_DATE) return TRUE;

	WORD dosDate, dosTime;
	VariantTimeToDosDateTime(var.date, &dosDate, &dosTime);
	//CTime cTime(dosDate, dosTime);
	COleDateTime cTime(dosDate, dosTime);
	if(dosTime)
		s=cTime.Format(FIELD_DATETIME_FORMAT);
	else
		s=cTime.Format(FIELD_DATE_FORMAT);
	return TRUE;
}
BOOL CPub::CString2VariantDate(VARIANT* pVar, CString& s)
{
	if(s.IsEmpty()) {
		pVar->vt=VT_EMPTY;
		return TRUE;
	}
	CTime cTime=CString2CTime(s);
	WORD dosDate, dosTime;
	BYTE B;
	dosDate=cTime.GetDay();
	B=cTime.GetMonth();		dosDate|=(B<<5);
	B=cTime.GetYear()-1980;	dosDate|=(B<<9);

	dosTime=cTime.GetSecond()/2;
	B=cTime.GetMinute();	dosTime|=(B<<5);
	B=cTime.GetHour();		dosTime|=(B<<11);

	pVar->vt=VT_DATE;
	DosDateTimeToVariantTime(dosDate, dosTime, &(pVar->date));
	return TRUE;
}

//整数转串(0转为空)
CString CPub::i2CStringMayNULL(int i)
{
	CString s;
	if(!i) return s;
	s.Format(_T("%d"), i);
	return s;
}

//转换时间,时间格式如"2000-11-02 09:11:01"
//时间转换成串
CString CPub::CTime2CString(const CTime& cTime)
{
	return cTime.Format(FILE_DATETIME_FORMAT);
}
//串转换成时间
CTime CPub::CString2CTime(const CString& str)
{
	CStringArray arr1, arr2;
	CString s;
	CTime ctime;
	int i[6];
	CString2Array(arr1, str, _T(" "));
	arr1.SetSize(2);

	s=arr1.GetAt(0);
	if(s.GetLength()==8) {
		arr2.Add(s.Left(4));
		arr2.Add(s.Mid(4, 2));
		arr2.Add(s.Right(2));
	}
	else {
		CString2Array(arr2, s, _T("-"));
		if(arr2.GetSize()!=3) CString2Array(arr2, s, _T("."));
	}
	arr2.SetSize(3);

	i[0]=_ttoi(arr2.GetAt(0));
	i[1]=_ttoi(arr2.GetAt(1));
	i[2]=_ttoi(arr2.GetAt(2));

	CString2Array(arr2, arr1.GetAt(1), _T(":"));
	arr2.SetSize(3);
	i[3]=_ttoi(arr2.GetAt(0));
	i[4]=_ttoi(arr2.GetAt(1));
	i[5]=_ttoi(arr2.GetAt(2));
	return CTime(i[0],i[1],i[2],i[3],i[4],i[5]);
}

COleDateTime CPub::CString2COleDateTime(const CString& str)
{
	CStringArray arr1, arr2;
	CString s;
	CTime ctime;
	int i[6];
	CString2Array(arr1, str, _T(" "));
	arr1.SetSize(2);

	s=arr1.GetAt(0);
	if(s.GetLength()==8) {
		arr2.Add(s.Left(4));
		arr2.Add(s.Mid(4, 2));
		arr2.Add(s.Right(2));
	}
	else {
		CString2Array(arr2, s, _T("-"));
		if(arr2.GetSize()!=3) CString2Array(arr2, s, _T("."));
	}
	arr2.SetSize(3);

	i[0]=_ttoi(arr2.GetAt(0));
	i[1]=_ttoi(arr2.GetAt(1));
	i[2]=_ttoi(arr2.GetAt(2));

	CString2Array(arr2, arr1.GetAt(1), _T(":"));
	arr2.SetSize(3);
	i[3]=_ttoi(arr2.GetAt(0));
	i[4]=_ttoi(arr2.GetAt(1));
	i[5]=_ttoi(arr2.GetAt(2));
	return COleDateTime(i[0],i[1],i[2],i[3],i[4],i[5]);
}

double CPub::GetAbs(double src)
{
	if (src > 0)
		return src;
	else
		return -src;
}

bool CPub::CompareDouble(double dValue1,double dValue2)
{
	if(CPub::GetAbs(dValue1 - dValue2) < 0.0001)
		return TRUE;
	return FALSE;
}

LPWSTR CPub::PM_A2W(LPCSTR pAInfo, const int iLength )
{
	int iWLength = iLength +1 ;
	if ( 0 == iLength )
	{
		/// 
		/// 注意:此处不能用 _tcslen 代替
		/// 
		iWLength = (int)strlen(pAInfo ) + 1 ;
	}

	LPWSTR pWInfo = new WCHAR[iWLength];
	
	ZeroMemory(pWInfo,iWLength * sizeof(WCHAR));
	
	ATLA2WHELPER(pWInfo, pAInfo,  iWLength - 1 , CP_ACP );

	return pWInfo;
}

LPSTR CPub::PM_W2A(LPCWSTR pWInfo, const int iLength )
{
	int iALength = (iLength + 1) * 2 ;

	if ( 0 == iLength )
	{
		/// 
		/// 注意:此处不能用 _tcslen 代替
		/// 
		iALength = ((int)wcslen(pWInfo ) + 1) * 2  ; /// 加1为结束符
	}

	LPSTR pAInfo = new CHAR[iALength];

	ZeroMemory(pAInfo,iALength);

	ATLW2AHELPER(pAInfo, pWInfo, iALength  , CP_ACP);

	return pAInfo;
}

LPSTR CPub::PM_T2A(LPCTSTR pTInfo)
{
	int iLength = (int)_tcslen(pTInfo);
	LPSTR pAInfo = NULL;
#ifdef UNICODE
	pAInfo = PM_W2A(pTInfo,iLength);
#else
	int iALength = (iLength + 1 ); /// 加1为结束符
	pAInfo = new CHAR[iALength];
	ZeroMemory(pAInfo,iALength);
	CopyMemory( pAInfo, pTInfo, iALength - 1 );
#endif
	return pAInfo;
}

LPTSTR CPub::PM_A2T(LPCSTR pAInfo)
{
	LPTSTR pTInfo = NULL;
#ifdef UNICODE
	pTInfo = PM_A2W(pAInfo,(int)strlen(pAInfo));
#else
	int iAInfoSize = strlen(pAInfo);
	pTInfo = new char[iAInfoSize + 1];
	ZeroMemory(pTInfo,iAInfoSize + 1);
	CopyMemory(pTInfo,pAInfo,iAInfoSize);
#endif
	return pTInfo;
}


LPWSTR CPub::PM_T2W(LPCTSTR pTInfo)
{//将可能是Unicode或Acsii字符串 转为Unicode字符串
	LPWSTR pWInfo = NULL;
#ifdef UNICODE
	int iLength = (int)_tcslen(pTInfo);
	int iWLength = (iLength + 1);	/// 加结束符
	pWInfo = new WCHAR[iWLength];	/// 开辟内存
	//PMRegMemory(pWInfo);
	ZeroMemory(pWInfo,iWLength * sizeof(WCHAR) );	/// 第二个参数是字节的长度
	CopyMemory(pWInfo,pTInfo,(iWLength - 1) * sizeof(WCHAR) ); /// 最后一个参数是字节的长度
#else
	pWInfo = PM_A2W(pTInfo);
#endif
	return pWInfo;
}

LPTSTR CPub::PM_W2T(LPCWSTR pWInfo)
{
	LPTSTR pTInfo = NULL;
#ifdef UNICODE
	int iWLength = (int)_tcslen(pWInfo);
	pTInfo = new WCHAR[iWLength + 1];
//	PMRegMemory(pTInfo);
	ZeroMemory(pTInfo,(iWLength + 1) * sizeof(WCHAR));
	CopyMemory(pTInfo,pWInfo,iWLength * sizeof(WCHAR));
#else
	pTInfo = PM_W2A(pWInfo);
#endif
	return pTInfo;
}
