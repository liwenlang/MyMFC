#pragma once

#include "Pdbt_ComparamSet.h"

class CPdbt_DrawIndexTable  : public CComParamSetBase
{
public:
	CPdbt_DrawIndexTable(void);
	~CPdbt_DrawIndexTable(void);


public:

	/// 导入到数据库
	virtual BOOL	Export2Db( const CString & strExcelPath )  ;

protected:
	virtual void	InitData() ;

protected:
	struct CConstData
	{
		static int Id_Index  ;
		static int FloorId_Index ; 
		static int ComType_Index ; 
		static int ComName_Index ; 
		static int ShowIndex_Index ; 
	};
};
