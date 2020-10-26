

#pragma once 


#include "Source/Pdbt_Db_Connect.h"

/**
 * @brief 该模块的公用 常量和函数定义
 *
 *        
 * @author liwenlang
 * @version v1.0
 * @date  [2012-7-30]
 * @note  
 */

namespace pmdbtool
{
	const CString	g_strComParamSet_Total = _T("施工安全参数(全国)") ;
	const CString g_strComParamSet_Global = _T("绘制顺序") ;

	enum TableIndex
	{
		eComParamSet_Total = 1 ,
		eDrawIndex = 2 ,
		eDefaultSheet = 1 , /// 默认操作第一页
	};

	enum
	{
		eMaxColIndex = 20 , /// 最大列
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
