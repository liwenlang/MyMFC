

#include "StdAfx.h"
#include "Pdbt_ComparamSet.h"

using namespace pmdataservice;

int CComParamSetBase::CComParamsSetData::m_iFieldCol = -1 ;		/// 列名开始列
int CComParamSetBase::CComParamsSetData::m_iFieldRow = -1;		/// 列名所在的行

int	 CComParamSetBase::CComParamsSetData::ParmID_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::parentid_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::gangjin_mode_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::UpdateMode_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::comtype_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::SubComType_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::parmtype_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::parmname_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::parmdisplayname_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::parmvalue_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::InputMode_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::DataType_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::ListData_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::ListData03_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::ListData11_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::processType_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::exp_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::ValidateExpr_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::ValidateExpr03_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::ValidateExpr11_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::CheckReg_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::CheckReg03_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::CheckReg11_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::HitInfo_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::HitInfo03_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::HitInfo11_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::InvalidMsg_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::ReadOnly_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::Hide_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::SortID_Index = -1 ;
int	 CComParamSetBase::CComParamsSetData::TreeNode_Index = -1 ;


CComParamSetBase::CComParamSetBase() 
{

}


CComParamSetBase::~CComParamSetBase() 
{

}

void	 CComParamSetBase::Replace( CString & str ) 
{
	pmdataservice::DealHchh( str ) ; 
}

