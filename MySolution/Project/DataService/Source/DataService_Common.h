

#pragma once


 namespace pmdataservice
{

	/// ���ֵĶ���
	enum
	{
		eNum_0 = 0 
		, eNum_1 = 1 
		, eNum_2 = 2 
		, eNum_3 = 3 
		, eNum_4 = 4
		, eNum_5 = 5 
		, eNum_6 = 6 
		, eNum_7 = 7
		, eNum_8 = 8 
		, eNum_9 = 9  
		, eNum_10 = 10  
		, eNum_11 = 11 
		, eNum_12 = 12 
		, eNum_13 = 13 
		, eNum_14 = 14 
		, eNum_15 = 15 
		, eNum_16 = 16 
		, eNum_17 = 17 
		, eNum_18 = 18 
		, eNum_19 = 19 
		, eNum_20 = 20  
		, eNum_21 = 21 
		, eNum_22 = 22 
		, eNum_23 = 23 
		, eNum_24 = 24 
		, eNum_25 = 25 
		, eNum_26 = 26 
		, eNum_27 = 27 
		, eNum_28 = 28 
		, eNum_29 = 29  
		, eNum_30 = 30  
		, eNum_31 = 31 
		, eNum_32 = 32 
		, eNum_33 = 33 
		, eNum_34 = 34 
		, eNum_35 = 35 
		, eNum_36 = 36 
		, eNum_37 = 37 
		, eNum_38 = 38 
		, eNum_39 = 39  
		, eNum_40 = 40  
		, eNum_41 = 41 
		, eNum_42 = 42 
		, eNum_43 = 43 
		, eNum_44 = 44 
		, eNum_45 = 45 
		, eNum_46 = 46 
		, eNum_47 = 47 
		, eNum_48 = 48 
		, eNum_49 = 49  
		, eNum_50 = 50  
		, eNum_51 = 51 
		, eNum_52 = 52 
		, eNum_53 = 53 
		, eNum_54 = 54 
		, eNum_55 = 55 
		, eNum_56 = 56 
		, eNum_57 = 57 
		, eNum_58 = 58 
		, eNum_59 = 59  
		, eNum_60 = 60  
		, eNum_61 = 61 
		, eNum_62 = 62 
		, eNum_63 = 63 
		, eNum_64 = 64 
		, eNum_65 = 65 
		, eNum_66 = 66 
		, eNum_67 = 67 
		, eNum_68 = 68 
		, eNum_69 = 69  
		, eNum_70 = 70  
		, eNum_71 = 71 
		, eNum_72 = 72 
		, eNum_73 = 73 
		, eNum_74 = 74 
		, eNum_75 = 75 
		, eNum_76 = 76 
		, eNum_77 = 77 
		, eNum_78 = 78 
		, eNum_79 = 79  
		, eNum_99 = 99 
		, eNum_100 = 100 
	} ; 

	/// һЩ�����Ķ���
	enum
	{
		eStr_Not_Find = -1  /// �����ַ����� û���ҵ�ʱ���� -1
		, eIdx_Init  = -1	/// Idx�ĳ�ʼֵ
		, eId_Init	= -1	/// id �ĳ�ʼ
		, eComType_Err = -1 /// ����� comtype 
		, eComId_Err = -1 
		, eFloorId_Err = -1
		, eTableIndex_Init = -1
		, eSpecFloorID = -1000
	} ;

	/// sql ����index 
	enum
	{
		eSql_Idx_0  = 0 ,
		eSql_Idx_1  = 1 ,
		eSql_Idx_2  = 2 ,
		eSql_Idx_3  = 3 ,
		eSql_Idx_4  = 4 ,
		eSql_Idx_5  = 5 ,
		eSql_Idx_6  = 6 ,
		eSql_Idx_7  = 7 ,
		eSql_Idx_8  = 8 ,
		eSql_Idx_9  = 9 ,
		eSql_Idx_10 = 10 ,
		eSql_Idx_11 = 11 ,
		eSql_Idx_12 = 12 ,
		eSql_Idx_13 = 13,
		eSql_Idx_14 = 14,
		eSql_Idx_15 = 15,
	} ;

	/// �е��±�
	enum
	{
		eCol_0 = 0 ,
		eCol_1 = 1 ,
		eCol_2 = 2 ,
		eCol_3 = 3 ,
		eCol_4 = 4 ,
		eCol_5 = 5 ,
		eCol_6 = 6 ,
		eCol_7 = 7 ,
		eCol_8 = 8 ,
		eCol_9 = 9 ,
	} ;

	/// 
	enum
	{
		eResultType_Zero = 0 ,	/// ʵ������������resulttype��Ϊ0
		eResultType_One = 1 ,	/// �Զ���������ĸֽ���
		eResultType_Two	= 2 ,	/// ���������Ƶ� �ֽ��� 
		eResultType_Lock = 10 ,	/// ���������Ľ�������ִ���10��Ϊ�����Ľ�� �� ��ԭ����ֵ�Ļ�����׷��10 
	} ; 

	/// ����\1;���溸��\2;˫�溸��\3;����ѹ����\4;׶��������\5;ֱ��������\6;����Ժ�\7;�׹��伷ѹ\8;
	enum
	{
		eDjxs_Bz = 1 ,
		eDjxs_Dmbz = 2 ,
		eDjxs_Smbz = 3 , 
		eDjxs_Dzylh = 4 , 
		eDjxs_Zuilwlj = 5 ,
		eDjxs_ZhiLwlj = 6 ,
		eDjxs_Sgdj = 7 ,
		eDjxs_Tgljy = 8
	} ; 

	/// 
	const PmTString g_strInsertInto = _T(" insert into ") ;
	const PMTSTRING	g_strDelete		= _T(" delete ");
	const PMTSTRING	g_strSelect		= _T(" select ") ;
	const PMTSTRING	g_strSet		= _T(" set ");
	const PMTSTRING g_strUpdate		= _T(" update ");
	const PMTSTRING	g_strFrom		= _T(" from ") ;
	const PMTSTRING g_strWhere		= _T(" where ") ;
	const PMTSTRING g_strAnd		= _T(" and ") ;
	const PMTSTRING	g_strOr			= _T(" or ") ;
	const PMTSTRING g_strOrderBy	= _T(" order by ") ;
	const PMTSTRING g_strDesc		= _T(" desc ") ;		/// ����
	const PMTSTRING g_strAsc		= _T(" asc ") ;			/// ���� 
	const PMTSTRING g_strGroupBy	= _T(" group by ") ;
	const PMTSTRING g_strIn			= _T(" in ") ;
	const PMTSTRING	g_strDot		= _T(".") ;				/// ��
	const PMTSTRING	g_strComma		= _T(" , ") ;			/// ����
	const PMTSTRING	g_strSingleQuotes = _T("'") ;			/// ������
	const PMTSTRING	g_strEqu		= _T(" = ") ;
	const PMTSTRING	g_strLike		= _T(" like ") ;		/// 
	const PMTSTRING	g_strInerJoin	= _T(" inner join ") ;	/// ��֮������ӷ���
	const PMTSTRING	g_strOn			= _T(" on ") ;			/// 
	const PMTSTRING g_strDou		= _T("\"") ;			/// ˫����
	const PMTSTRING	g_strZs			= _T("%d") ;
	const PMTSTRING g_strStr		= _T("%s") ;
	const PMTSTRING g_strPercent	= _T("%") ;
	const PMTSTRING g_strDollar		= _T("$") ;
	const PMTSTRING g_strSpace		= _T(" ") ;
	const PMTSTRING	g_strSem		= _T(";")	;			/// �ֺ� semicolon
	const PMTSTRING	g_strMax		= _T(" max") ;
	const PMTSTRING g_strMin		= _T(" min") ;
	const PMTSTRING	g_strLeftKh		= _T("(") ;				/// (
	const PMTSTRING g_strRightKh	= _T(")") ;				/// ) 
	const PMTSTRING g_strQue		= _T("?") ;				/// ? 
	const PMTSTRING	g_strZero		= _T(" 0 " ) ;			/// 0 
	const PMTSTRING	g_strOne		= _T("1") ; 
	const PMTSTRING g_strTwo		= _T("2") ;
	const PMTSTRING g_strMinusOne	= _T("-1") ;			/// -1
	const PMTSTRING g_strMinusTwo	= _T("-2") ;			/// -2
	const PMTSTRING	g_strSx			= _T("|") ;				/// |
	const PMTSTRING g_strDiv		= _T("/") ;
	const PMTSTRING g_strFormatComType = _T("100*100") ;

	const PmTString g_strReturn		= _T("\r\n") ; ///	�س�����

	const CString	g_strEmpty		= _T("") ;				/// "" 



//	const PMTSTRING	g_strComParamsSet = _T("ComParamsSet.ini") ;
	const PMTSTRING g_strConstFileName = _T("constdata.ini") ;

	const PMTSTRING g_strListData = _T("ListData") ;
	const PMTSTRING g_strListData03	= _T("ListData03") ;
	const PMTSTRING g_strListData11 = _T("ListData11") ;

	const PMTSTRING g_strValidateExpr = _T("ValidateExpr") ;
	const PMTSTRING g_strValidateExpr03 = _T("ValidateExpr03") ;
	const PMTSTRING g_strValidateExpr11 = _T("ValidateExpr11") ;

	const PMTSTRING g_strHitInfo = _T("HitInfo") ;
	const PMTSTRING g_strHitInfo03 = _T("HitInfo03") ;
	const PMTSTRING g_strHitInfo11 = _T("HitInfo11") ;

	const PMTSTRING g_strCheckReg = _T("CheckReg") ;
	const PMTSTRING g_strCheckReg03 = _T("CheckReg03") ;
	const PMTSTRING g_strCheckReg11 = _T("CheckReg11") ;

	/// 
	const PMTSTRING g_strLcxzListData = _T("lcxzListData") ;
	const PMTSTRING g_strTQDListData  = _T("TQDListData ") ;
	const PMTSTRING g_strZDJListData  = _T("ZDJListData") ;
	const PMTSTRING g_strSJDJListData = _T("SJDJListData") ;
	const PMTSTRING g_strKZDJListData  = _T("KZDJListData") ;
	const PMTSTRING g_strGJLXListData = _T("GJLXListData") ;
	const PMTSTRING g_strBflListData = _T("bflListData") ;
	const PMTSTRING g_strJDFFListData = _T("JDFFListData") ;
	const PMTSTRING g_strbhcListData = _T("bhcListData") ;
	const PMTSTRING g_strCGListData = _T("CGListData") ;
	const PMTSTRING	g_strDjxsListData = _T("djxsListData") ;


	/// ����س����� , �س����д���� \r\n 
	inline void	DealHchh( CString & strValue ) 
	{
		strValue.Replace( _T("\n") , _T("\\r\\n") ) ;
		strValue.Replace( _T("\r\n") , _T("\\r\\n") ) ; 
	}

	/// �� \r\n ����ɻس�����
	inline void	DealHchh_Reverse( CString & strValue ) 
	{
		strValue.Replace(  _T("\\r\\n") , _T("\r\n")  ) ; 
	}

	/// 
	/// ��ComType�ĺ���λ��Ϊ0
	/// ��	301 --> 300
	///		1101 --> 1100
	/// 
	inline int FormatComType( const int iComType ) 
	{
		return iComType / eNum_100 * eNum_100 ;
	}
	/// 
	/// �� ary ������ת��Ϊ 1,2,3,4 �ĸ�ʽ 
	/// 
	inline PMTSTRING GetStr( const CUIntArray & ary ) 
	{
		PMTSTRING strReturn ; 

		const int iSize = (int)ary.GetSize() ; 
		
		int i = 0 ; 
		for ( ; i < iSize ; ++i )
		{
			if(i > 0)
			{
				strReturn.append( _T(",") ) ; 
			}
			int value = ary.GetAt(i) ; 
			CString strValue;
			strValue.Format(_T("%d"), value);
			strReturn.append(strValue) ;
		}

		return strReturn ; 
	}

	inline CString Value2SqlValue( const CString & strValue ) 
	{
		CString strValueTemp = strValue ; 
		if ( strValue.IsEmpty() )
		{
			return strValueTemp ;
		}

		if ( strValue.Find( _T("\'")) == -1 )
		{
			return strValueTemp ; 
		}

		CString oldstr = _T("'");
		CString newstr = _T("''");

		strValueTemp.Replace( oldstr , newstr ) ; 

		return strValueTemp ; 
	}

	/// 
	inline void	GetRangeComId( const CUIntArray & aryComId , const int iStart , const int iEnd , CUIntArray & aryDesComId )
	{
		int i = iStart ; 

		for ( ; i < iEnd && i < aryComId.GetSize() ; ++i )
		{
			aryDesComId.Add( aryComId.GetAt(i) ) ;
		}
	}

	inline int FindInt_DataSet(NDataModelDataSet * pDataSet,CString strFieldName,int iValue,BOOL bNeedSort)
	{
		if(bNeedSort)
		{
			pDataSet->ResetSortField();
			pDataSet->AddSortField(strFieldName,TRUE);
			pDataSet->Sort();
		}

		LPVOID pBuff = (LPVOID)&iValue;
		int iRow = pDataSet->Bisearch(pBuff, strFieldName);
		if(iRow != -1)
		{
			iRow++;
			return iRow;
		}
		return -1;
	}

	inline int FindInt_DataSet(NDataModelDataSet * pDataSet,int iFieldIndex,int iValue,BOOL bNeedSort = TRUE )
	{
		if(bNeedSort)
		{
			pDataSet->ResetSortField();
			pDataSet->AddSortField(iFieldIndex,TRUE);
			pDataSet->Sort();
		}

		LPVOID pBuff = (LPVOID)&iValue;
		int iRow = pDataSet->Bisearch(pBuff, iFieldIndex);
		if(iRow != -1)
		{
			iRow++;
			return iRow;
		}
		return -1;
	}

	
	inline int FindString_DataSet(NDataModelDataSet * pDataSet,CString strFieldName,CString strValue,BOOL bNeedSort  = TRUE )
	{
		if(bNeedSort)
		{
			pDataSet->ResetSortField();
			pDataSet->AddSortField(strFieldName,TRUE);
			pDataSet->Sort();
		}

		LPVOID pBuff = (LPVOID)strValue.GetBuffer(0);
		int iRow = pDataSet->Bisearch(pBuff, strFieldName);
		if(iRow != -1)
		{
			iRow++;
			return iRow;
		}
		return -1;
	}

	inline int FindString_DataSet(NDataModelDataSet * pDataSet,int iFieldIndex,CString strValue,BOOL bNeedSort = TRUE )
	{
		if(bNeedSort)
		{
			pDataSet->ResetSortField();
			pDataSet->AddSortField(iFieldIndex,TRUE);
			pDataSet->Sort();
		}

		LPVOID pBuff = (LPVOID)strValue.GetBuffer(0);
		int iRow = pDataSet->Bisearch(pBuff, iFieldIndex);
		if(iRow != -1)
		{
			iRow++;
			return iRow;
		}
		return -1;
	}


	namespace PmSl_CSFVectorDiagram
	{
		enum VectorID
		{
			s_dcbzjd1 = 20111,//��������ڵ� ����1
			s_dcbzjd2 = 20112,//��������ڵ� ����2
			s_dcbzjd3 = 20113,//��������ڵ� ����3
			s_dcjzjd1 = 20114,//��������ڵ� ����1
			s_dcjzjd2 = 20115,//��������ڵ� ����2
			s_dcjzjd3 = 20116,//��������ڵ� ����3
			s_bjmwzcd1 = 20104,//�������ʱ�������۳��� ����1
			s_bjmwzcd2 = 20105//�������ʱ�������۳��� ����2
		};
	}

	namespace PmSl_VectorInfo
	{
		enum VectorID
		{
			KL_Rect = 10101,//���������
			CL_Rect = 10201,//��������
			JCL_Rect = 10401,//����������
			QL_Rect = 10501,//Ȧ������
			GL_Rect = 10601//��������
		};
	}



} 

using namespace pmdataservice ;
