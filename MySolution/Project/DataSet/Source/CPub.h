/////////////////////////////////////////////////////////
//公共函数库静态类

#ifndef CPUB
#define CPUB
#define PAPERNAME_SPACE		16384
#define PAPERSIZE_SPACE		4096
#define CPUB_BUFLEN			256

#ifdef EXT_CLASS
class AFX_EXT_CLASS CPub
#else
class CPub
#endif
{
public:
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	/// 此处的iLength 都为字符的长度( 不需要添加结束符)
	/// 
	/// 注意这一系列函数 PM_X2Y 返回的都是需要删除的
	/// 
	static LPWSTR PM_A2W(LPCSTR pAInfo, const int iLength = 0 );
	static LPSTR PM_W2A(LPCWSTR pWInfo,const int iLength = 0 );

	
	static LPTSTR PM_A2T(LPCSTR pAInfo);
	static LPSTR PM_T2A(LPCTSTR pTInfo);

	static LPWSTR PM_T2W(LPCTSTR pTInfo) ;
	static LPTSTR PM_W2T(LPCWSTR pWInfo) ;


	static double		GetAbs(double dValue);
	
	static bool		CompareDouble(double dValue1,double dValue2);
	
	////////////////////////////////////////////////////////
	//内存及字符串函数
	static int		memmem(const LPBYTE pMem1, const LPBYTE pMem2, long offset, long lenMem1, long lenMem2);
	static CString	GetLeft(CString& str, LPCTSTR sepChar, BOOL bTrim=FALSE);
	static CString	GetRight(CString& str, LPCTSTR sepChar, BOOL bTrim=FALSE);
	static int		FindNoCase(LPCTSTR string, LPCTSTR subs);
	static CString	GetEqualValue(CString& str, LPCTSTR item);
	static int		CountSubString(CString& s, LPCTSTR subs);
	static CString	Fill(LPCTSTR fillStr, int num);
	static CString	AppendPath(const CString & Path1,const CString & Path2);
	static BOOL		parseFilename(LPCTSTR fullname, CString& filename, CString& pathname);
	static void		DeleteRight(CString& str, int nCount);
	static BOOL		IsDigit(TCHAR c, BOOL bIncludeDot=FALSE, BOOL bIncludePunc=FALSE, BOOL bIncludeNegative=TRUE);
	static BOOL		copystr(LPTSTR* ppDest, LPCTSTR pSour, BOOL bCompareFirst=FALSE);
	
	////////////////////////////////////////////////////////
	//类型转换函数
	static BOOL		VariantDate2CString(CString&s, VARIANT& var);
	static BOOL		CString2VariantDate(VARIANT* pVar, CString&s);
	static BOOL		BSTR2CString(CString& s,const BSTR b);
	static CString	i2CStringMayNULL(int i);
	static CString	CTime2CString(const CTime& cTime);
	static CTime		CString2CTime(const CString& s);
	static COleDateTime CString2COleDateTime(const CString& str);
	static void		CString2Array(CStringArray& Array, const CString& s, LPCTSTR sep, BOOL bTrim=FALSE);
	
	////////////////////////////////////////////////////////
	//串数组函数
	static CString	Array2CString(const CStringArray& arr, LPCTSTR sep);
	static CString	ArraySearchValue(const CStringArray& arr, LPCTSTR item);
	static int		ArrayCompareNoCase(const CStringArray& arr, const LPCTSTR string);
	static int		ArrayFindNoCase(const CStringArray& arr, const LPCTSTR string, int start=0);
	static int		ArrayFindLeftNoCase(const CStringArray& arr, const LPCTSTR string, int start=0);
	static int		ReadText2Buf(CString& buf, LPCTSTR filename, LPCTSTR sectionName, int MaxTabs);
	
	////////////////////////////////////////////////////////
	//文件读写
	static BOOL		File2Mem(LPBYTE *pMem, LPCTSTR filename, long *filesize);
	static BOOL		File2CString(CString& buf, LPCTSTR filename);
	static BOOL		File2Array(CStringArray& arr, LPCTSTR filename);
	static BOOL		Mem2File(LPBYTE pMem, LPCTSTR filename, long memSize);
	static BOOL		CString2File(CString& buf, LPCTSTR filename);
	static BOOL		Array2File(CStringArray& buf, LPCTSTR filename);
	
	static BOOL		File2MemW(LPWSTR *pMem, LPCTSTR filename, long *WChars);
	static BOOL		File2CStringW(CString& buf, LPCTSTR filename);
	static BOOL		File2ArrayW(CStringArray& arr, LPCTSTR filename);
	static BOOL		Mem2FileW(LPCWSTR pMem, LPCTSTR filename, long WChars);
	static BOOL		CString2FileW(CString& buf, LPCTSTR filename);
	static BOOL		Array2FileW(CStringArray& buf, LPCTSTR filename);
	static BOOL		WriteUnicodeFileFlag(CFile& cFile);
	static BOOL		OpenNewFile(CFile& cFile, LPCTSTR filename, BOOL bWriteOnly=TRUE);
	
	
	////////////////////////////////////////////////////////
	//文件及日期函数
	static void		WriteLine(CFile& cFile, LPCTSTR buf);
	static CString	ProfileString(LPCTSTR inifile, LPCTSTR section, LPCTSTR item, LPCTSTR defaultStr);
	static void		SetProfileString(LPCTSTR inifile, LPCTSTR section, LPCTSTR item, LPCTSTR Str);
	
	//以下文件、日期函数为DllExport
	static BOOL APIENTRY IsFileReadOnly(LPCTSTR filename);
	static BOOL APIENTRY FileExists(LPCTSTR filename);
	static BOOL APIENTRY Create1File(LPCTSTR filename);
	static BOOL APIENTRY SetFileReadOnly(LPCTSTR filename, BOOL readOnly);
	static int  APIENTRY CompareFileTime(LPCTSTR tm1, LPCTSTR tm2);
	static int  APIENTRY CompareFileTime(CTime& tm1, CTime& tm2);
	static int  APIENTRY CompareFileTimeSize(LPCTSTR filename1, LPCTSTR filename2, BOOL bCompSize=TRUE, BOOL bCompTime=TRUE);
	static int  GetFileTime(LPCTSTR filename, CString& createtime, CString& modifytime);
	static int  APIENTRY FileTimeSet(LPCTSTR filename, LPCTSTR createtime);
	static int  APIENTRY FileTimeGet(LPCTSTR filename, LPTSTR createtime, LPTSTR modifytime);
	static int  APIENTRY DeleteDirFiles(LPCTSTR dir,LPCTSTR files);
	static BOOL APIENTRY CopyDirFiles(LPCTSTR dirSour, LPCTSTR dirDest, LPCTSTR spec);
	static int  APIENTRY FilesFindFirst(LPCTSTR filespec, LPTSTR filename, unsigned long *filesize, LPTSTR createtime, LPTSTR modifytime);
	static int  APIENTRY FilesFindClose(HANDLE handle);
	static int  APIENTRY FilesFindNext(HANDLE handle, LPTSTR filename, unsigned long *filesize, LPTSTR createtime, LPTSTR modifytime);
	static int  APIENTRY SetDisk(int disk);
	static int  APIENTRY GetDisk();
	static int  APIENTRY Explore_Browser(LPCTSTR notes, LPTSTR buf, long startflag,long ulflags);
	static int  APIENTRY ExploreBrowser(LPCTSTR notes, CString& buf, long startflag,long ulflags);
	static int  APIENTRY FileExecute(HWND hwnd, LPCTSTR filename);
	static BOOL APIENTRY LZCopyFile(LPCTSTR sour,LPCTSTR dest);
	static BOOL APIENTRY LZCopyFile2Dir(LPCTSTR sour,LPCTSTR dir, CString& fileDest);
	static BOOL APIENTRY DirExists(LPCTSTR dir);
	static BOOL APIENTRY DirCreate(LPCTSTR dir);
	
	////////////////////////////////////////////////////////
	//系统函数
	static int  APIENTRY GetValidPrintPaper(LPCTSTR prtname, LPCTSTR portname, LPTSTR retstr);
	static int  APIENTRY GetValidPrintResolution(LPCTSTR prtname, LPCTSTR portname, int num);
	static void APIENTRY Shift_VK(HWND hwnd, int vk);
	static void APIENTRY Control_VK(HWND hwnd, int vk);
	static void APIENTRY DispatchMsg(HWND hwnd, DWORD DispatchMsg);
	static void APIENTRY TransparentWindow(HWND hwnd, int nTran=250);
	static BOOL APIENTRY GetVersion(LPCTSTR FileName, DLLVERSIONINFO *pInfo);
	static CString APIENTRY GetVersion(LPCTSTR FileName);
	static BOOL DetectProcess(LPCTSTR Name, DWORD nType=TH32CS_SNAPPROCESS);
	
	//系统信息函数
	static CString APIENTRY GetSystemDir();
	static CString APIENTRY GetWindowDir();
	static CString APIENTRY GetCurrentDir();
	static CString APIENTRY GetTempDir();
	static CString APIENTRY GetComputerName();
	static CString APIENTRY GetUserName();
	static BOOL APIENTRY IsWinNT();
	static int  APIENTRY GetBitsPixel();
	
	//错误处理
	static CString GetLastError();
	static void Error(LPCTSTR ErrMsg);
private:
	static int  CALLBACK explore_browserCallbackProc(HWND hwnd, UINT uMsg, LPARAM, LPARAM);
};

#endif
