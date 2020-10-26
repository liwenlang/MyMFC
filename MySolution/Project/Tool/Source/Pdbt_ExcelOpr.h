


#pragma  once 

// 操作的封装类

 
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// 
/// 该类不能使用单键，使用单键的时候，存在资源释放的问题。

class CExcelOpr
{

public:
//private:
	CExcelOpr() ; 

public:
	virtual ~CExcelOpr();
//	static CExcelOpr & Instance() ; 


public:
	/// 启动excel 
	void	StartExcel() ;

	/// 释放资源
	void	ReleaseExcel() ; 

	/// 启动excel 
	void	OpenBook( const PMTSTRING & strFileName ) ;

	/// 关闭
	void	CloseBook() ; 

	/// 
	void    LoadSheet( const int iIndex);  

	void    LoadXLGSheet( const int iIndex );

	/// 
	void    LoadSheet( const CString & strSheetName ) ;

	/// 
	int GetRowCount() ;   

	/// 
	int GetColCount();   

	/// 
	CString GetCell(  const int iCol , const int iRow  );   
	
	/// 
	int		GetSheetCount() ;

	/// 
	CString GetSheetName( const int iIndex ) ;


	/// 关闭excel 
	void	CloseExcel() ; 

	/// 获取指定表格的内容
	PMTSTRING	GetText( const PMTSTRING & strRange ) ; 

public:

	void	SetCell( const int iCol , const int iRow , const PMTSTRING & strValue ) ; 

	void	SaveBook(  const PMTSTRING & strFileName  ) ; 


private:

	/// excel app 操作
	_Application m_app;

	/// 
	Workbooks m_books;
	
	/// 
	_Workbook m_book;

	/// sheet 页
	Worksheets m_sheets;

	/// 单个sheet 页
	_Worksheet m_sheet;

	/// 处理范围
	Range m_range;

	long m_lFileFormat ; 
};

//#define Excel_Opr CExcelOpr::Instance()
