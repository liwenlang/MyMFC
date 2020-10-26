

#include "StdAfx.h"

#include "Pdbt_ExcelOpr.h "


COleVariant
covTrue((short)TRUE),
covFalse((short)FALSE),
covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);


CExcelOpr::CExcelOpr() : m_lFileFormat(0)
{
	
}

CExcelOpr::~CExcelOpr()
{
	
}



// CExcelOpr & CExcelOpr::Instance() 
// {
// 	static CExcelOpr excelOpr ; 
// 	return excelOpr ; 
// }


void	CExcelOpr::StartExcel()
{
	if(!m_app.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("Couldn't start Excel and get Application object."));
		exit(1);   
	}

	//********************************************************************************************
	// 避免调用Excel VBA的时候弹出 “由于另一个程序正在运行...”
	/*AfxOleGetMessageFilter()-> EnableBusyDialog(FALSE);
	AfxOleGetMessageFilter()-> SetBusyReply(SERVERCALL_RETRYLATER);
	AfxOleGetMessageFilter()-> EnableNotRespondingDialog(TRUE);
	AfxOleGetMessageFilter()-> SetMessagePendingDelay   (-1);*/
	//********************************************************************************************

// 	m_app.SetVisible(TRUE);
// 	m_app.SetUserControl(TRUE);
}

void	CExcelOpr::ReleaseExcel()
{
	m_range.ReleaseDispatch();    
	m_sheet.ReleaseDispatch();    
	m_sheets.ReleaseDispatch();    
	m_book.ReleaseDispatch();    
	m_book.ReleaseDispatch();

	m_app.ReleaseDispatch();  
}

/// 启动excel 
void	CExcelOpr::OpenBook( const PMTSTRING & strFileName ) 
{
	
	m_books.AttachDispatch(m_app.GetWorkbooks(),true);    
	LPDISPATCH lpDis = NULL;   
	lpDis = m_books.Add( _variant_t(strFileName.c_str())); // 如何判断文件是否打开？   
	if (lpDis)   
	{   
		m_book.AttachDispatch(lpDis);    
		//得到Worksheets    
		m_sheets.AttachDispatch( m_book.GetWorksheets(),true);    

		m_lFileFormat = m_book.GetFileFormat() ;
	}   

}

void	CExcelOpr::CloseBook() 
{
	m_book.Close( covFalse , covOptional , covOptional ) ;
	m_books.Close() ; 
}


void    CExcelOpr::LoadSheet( const int iIndex)
{
	LPDISPATCH lpDis = NULL;   
	m_range.ReleaseDispatch();   
	m_sheet.ReleaseDispatch();   
	lpDis = m_sheets.GetItem(_variant_t((long)iIndex));   
	if (lpDis)   
	{   
		m_sheet.AttachDispatch(lpDis,true);   
		m_range.AttachDispatch(m_sheet.GetCells(), true);   
	}   
}

void    CExcelOpr::LoadXLGSheet( const int iIndex )
{
	LPDISPATCH lpDis = NULL; 
	m_range.ReleaseDispatch();   
	m_sheet.ReleaseDispatch();  
	lpDis = m_sheets.GetItem(_variant_t((long)iIndex));   
	if (lpDis)   
	{   
		m_sheet.AttachDispatch(lpDis,true);   
		m_range.AttachDispatch(m_sheet.GetCells(), true);   
	}   
}

void    CExcelOpr::LoadSheet( const CString & strSheetName )
{
	int iSheetCount = GetSheetCount() ; 

	int iLoadSheet = -1 ; 

	int i = 1 ; 
	for ( ; i <= iSheetCount ; ++i )
	{
		CString strSheetNameTemp = GetSheetName(i) ; 

		if ( strSheetNameTemp.Compare( strSheetName ) == 0 )
		{
			iLoadSheet = i ; 
			break; 
		}
	}

	if ( -1 == iLoadSheet )
	{
		_Worksheet wsa ;
		wsa.AttachDispatch(m_sheets.GetItem(COleVariant(m_sheets.GetCount())));       
		m_sheets.Add(vtMissing,_variant_t(wsa),vtMissing,vtMissing);
		iLoadSheet = iSheetCount + 1 ;
	}

	LoadSheet( iLoadSheet ) ;

	m_sheet.SetName( strSheetName ) ;

	
}

int CExcelOpr::GetRowCount() 
{
	Range range;   
	Range usedRange;   
	usedRange.AttachDispatch(m_sheet.GetUsedRange(), true);   
	range.AttachDispatch(usedRange.GetRows(), true);   
	int count = range.GetCount();   
	usedRange.ReleaseDispatch();   
	range.ReleaseDispatch();   
	return count;   
}

int CExcelOpr::GetColCount()
{
	Range range;   
	Range usedRange;   
	usedRange.AttachDispatch(m_sheet.GetUsedRange(), true);   
	range.AttachDispatch(usedRange.GetColumns(), true);   
	int count = range.GetCount();   
	usedRange.ReleaseDispatch();   
	range.ReleaseDispatch();   
	return count;   
}

void	CExcelOpr::SetCell( const int iCol , const int iRow , const PMTSTRING & strValue )
{
	/// 
	Range range;   
	range.AttachDispatch( m_range.GetItem (COleVariant((long)iRow),COleVariant((long)iCol)).pdispVal, true);   

	range.SetNumberFormat( COleVariant(_T("@") ) ) ;
	/// 
	range.SetValue2( COleVariant( strValue.c_str() ) ) ; 

	/// 
	range.ReleaseDispatch();   

}

/// 
CString CExcelOpr::GetCell(  const int iCol , const int iRow  )
{
	Range range;   
	range.AttachDispatch( m_range.GetItem (COleVariant((long)iRow),COleVariant((long)iCol)).pdispVal, true);   
	COleVariant vResult =range.GetValue2();   
	CString str;   

	switch( vResult.vt )
	{
	case VT_BSTR:
		{
			str = vResult.bstrVal;   
		}
		break ;
	case VT_INT:
		{
			str.Format(_T("%d"), (int)vResult.pintVal);	
		}
		break ; 
	case VT_R8:
		{
			str.Format(_T("%f"), vResult.dblVal);
	}
		break ;
	case VT_DATE:
		{
			SYSTEMTIME st;   
			VariantTimeToSystemTime(vResult.date, &st);   
		}
		break ; 

	case VT_EMPTY:
		{
			str = _T("") ; 
		}
		break ;

	default:
		break ;
	}

	range.ReleaseDispatch();   
	return str;   
}

int		CExcelOpr::GetSheetCount()
{
	 return m_sheets.GetCount();   
}


CString CExcelOpr::GetSheetName( const int iIndex)   
{   
	_Worksheet sheet;   
	sheet.AttachDispatch(m_sheets.GetItem(_variant_t((long)iIndex)),true);   
	CString name = sheet.GetName();   
	sheet.ReleaseDispatch();   
	return name;   
}  


/// 关闭excel 
void	CExcelOpr::CloseExcel() 
{
	m_app.Quit() ; 
}

/// 获取指定表格的内容
PMTSTRING	CExcelOpr::GetText( const PMTSTRING & strRange ) 
{
	Range range;   
	range = m_sheet.GetRange(COleVariant( strRange.c_str() ),COleVariant( strRange.c_str() ));

	VARIANT strRangeValue = range.GetText() ; 

	range.ReleaseDispatch();   

	CString strValue = V_BSTR(&strRangeValue) ;

	return (LPCTSTR)strValue ; 
}

void	CExcelOpr::SaveBook( const PMTSTRING & strFileName )
{
 	m_book.SaveAs( 
 		COleVariant( strFileName.c_str() ) , 
		COleVariant( m_lFileFormat ) ,
 		covOptional, 
 		covOptional, 
 		covOptional, 
 		covOptional, 
 		1,
 		covOptional,
 		covOptional,
 		covOptional,
 		covOptional,
 		covOptional
 		) ; 

//	m_book.Save() ; 
}