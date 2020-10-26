


#pragma  once 

// �����ķ�װ��

 
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// 
/// ���಻��ʹ�õ�����ʹ�õ�����ʱ�򣬴�����Դ�ͷŵ����⡣

class CExcelOpr
{

public:
//private:
	CExcelOpr() ; 

public:
	virtual ~CExcelOpr();
//	static CExcelOpr & Instance() ; 


public:
	/// ����excel 
	void	StartExcel() ;

	/// �ͷ���Դ
	void	ReleaseExcel() ; 

	/// ����excel 
	void	OpenBook( const PMTSTRING & strFileName ) ;

	/// �ر�
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


	/// �ر�excel 
	void	CloseExcel() ; 

	/// ��ȡָ����������
	PMTSTRING	GetText( const PMTSTRING & strRange ) ; 

public:

	void	SetCell( const int iCol , const int iRow , const PMTSTRING & strValue ) ; 

	void	SaveBook(  const PMTSTRING & strFileName  ) ; 


private:

	/// excel app ����
	_Application m_app;

	/// 
	Workbooks m_books;
	
	/// 
	_Workbook m_book;

	/// sheet ҳ
	Worksheets m_sheets;

	/// ����sheet ҳ
	_Worksheet m_sheet;

	/// ����Χ
	Range m_range;

	long m_lFileFormat ; 
};

//#define Excel_Opr CExcelOpr::Instance()
