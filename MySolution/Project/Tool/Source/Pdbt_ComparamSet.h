

#pragma once 

/**
 * @brief 参数操作的基类,定义需要支持的借口
 *
 *        
 * @author liwenlang
 * @version v1.0
 * @date  [2012-7-30]
 * @note  
 */


class CComParamSetBase
{
public:
	CComParamSetBase() ;
	virtual ~CComParamSetBase() ; 

public:

	/// 导入到数据库
	virtual BOOL	Export2Db( const CString & strExcelPath  ) 
	{
		return TRUE;
	}

	/// 导入到常量文件
	virtual void	Export2ConstFile( const CString & strExcelPath , const CString & strConstFilePath ) 
	{

	}

protected:
	virtual void	InitData() 
	{

	}

	/// 对字符中包含的回车换行做一个特殊处理,替换成\r\n字符
	virtual void	 Replace( CString & str) ; 


protected:

	struct CComParamsSetData 
	{
		static int m_iFieldCol ;		/// 列名开始列
		static int m_iFieldRow ;		/// 列名所在的行

		static		 int	 ParmID_Index;
		static		 int	 parentid_Index;
		static		 int	 gangjin_mode_Index;
		static		 int	 UpdateMode_Index;
		static		 int	 comtype_Index;
		static		 int	 SubComType_Index;
		static		 int	 parmtype_Index;
		static		 int	 parmname_Index;
		static		 int	 parmdisplayname_Index;
		static		 int	 parmvalue_Index;
		static		 int	 InputMode_Index;
		static		 int	 DataType_Index;
		static		 int	 ListData_Index;
		static		 int	 ListData03_Index;
		static		 int	 ListData11_Index;
		static		 int	 processType_Index;
		static		 int	 exp_Index;
		static		 int	 ValidateExpr_Index;
		static		 int	 ValidateExpr03_Index;
		static		 int	 ValidateExpr11_Index;
		static		 int	 CheckReg_Index;
		static		 int	 CheckReg03_Index;
		static		 int	 CheckReg11_Index;
		static		 int	 HitInfo_Index;
		static		 int	 HitInfo03_Index;
		static		 int	 HitInfo11_Index;
		static		 int	 InvalidMsg_Index;
		static		 int	 ReadOnly_Index;
		static		 int	 Hide_Index;
		static		 int	 SortID_Index;
		static		 int	 TreeNode_Index;
	};

};

