

#pragma once 


#include "Source/Pdbt_Db_Connect.h"

/**
 * @brief ��ģ��Ĺ��� �����ͺ�������
 *
 *        
 * @author liwenlang
 * @version v1.0
 * @date  [2012-7-30]
 * @note  
 */

namespace pmdbtool
{
	const CString	g_strComParamSet_Total = _T("ʩ����ȫ����(ȫ��)") ;
	const CString g_strComParamSet_Global = _T("����˳��") ;

	enum TableIndex
	{
		eComParamSet_Total = 1 ,
		eDrawIndex = 2 ,
		eDefaultSheet = 1 , /// Ĭ�ϲ�����һҳ
	};

	enum
	{
		eMaxColIndex = 20 , /// �����
	};


	/// 
	enum
	{
		/// 
		
	} ; 

	inline int	GetTableIndex( const CString & strTable ) 
	{
		if ( strTable.Compare(g_strComParamSet_Total) == 0  )
		{
			return eComParamSet_Total ;
		}
		else if ( strTable.Compare(g_strComParamSet_Global) == 0 )
		{
			return eDrawIndex;
		}
	}

	class CTestClock
	{
	public:
		CTestClock( const CString & strHintInfo = _T("") ) : m_strHintInfo( strHintInfo )
		{
			m_dwBegin = GetTickCount() ; 
		}

		~CTestClock()
		{
			DWORD dwEnd = GetTickCount() ; 

			DWORD dwSpan = dwEnd - m_dwBegin ; 

			CString strMsg;
			strMsg.Format(_T("%d"), (int)dwSpan);
			CDataSetErrorInfo::Instance()->PrintInfo( m_strHintInfo + strMsg , 0 );
		}

	private:
		DWORD m_dwBegin ; 
		CString	m_strHintInfo ; 
	};


} ; 

#define Db_Opr CDbConnect::Instance()
