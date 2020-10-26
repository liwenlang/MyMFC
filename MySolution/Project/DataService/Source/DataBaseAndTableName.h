
#pragma once

#define FloatZero 0.0001
#define OwnVectorTableID	10000
#define OwnVectorID			10000000
#define SpecFloorIDNotSave	-1000
#define AllFloorID			-1000
#define TemFloor			-1000
#define TEMPLAYER			_T("tempLayerName")
#define AMLAYER				_T("AmLayer")
#define LName				_T("PM_3DENT")
#define GraphicCheck_BZLayerName _T("PM_ͼֽ��ע")

#define SpecComID			101
#define Q0					101

#define OwnGanJinJTTH		99999		//�Զ���ֽ��ͼͼ��	
#define OwnGanJinJTHeight	70			//�Զ���ֽ��ͼ�߶�

#define ProjectBakZipFileName	_T("project.zip")
#define ProjectBakFileName	_T("project.bak")

#define ProjectFileName		_T("project.pmdb")
#define Cache3dDataFileName		_T("Cache3dData.pmdb")//��ά��Ϣ


class DataServiceAPI CPmDataBase
{
public:
	//���ݿ��ļ��� ö��
	enum  DataBaseName
	{
		Project , Project_Mdb ,SysData , DEBase , QDBase , YZBTj /*Ԥ�ư�*/ , BZCTTj/*��׼��̨ͼ��*/,\
		PublicGJ,Extern , GjProject,Extern_PublicGJ  /*�ⲿ�������ļ�*/,GlobelDB_Mdb,SysData_ComParm,SGAQ_SBDB,UserDataBase,

		BimData /*BIM���ݲ�*/,ReportDB /*��������ʵ����ݿ�*/ ,pmsgDb,CloudGJBase,TemplateBase,NewReportDb,Cache3dDataDb,
		TowerBaseDb /*�����������ݿ�*/, PmObject/*�����༭��*/,GroupGJBase,GroupGJBase_OtherDb,BimData_TEMP,
		PMBlockLibrary,KnowledgeRes,AniGjObject3DCacheDB/*�����������ݻ������ݿ�*/,MuBan_Template/*ģ���������ݿ�*/,
	};
protected:
private:
};

class DataServiceAPI CPmTable
{
public:
	enum  TableName
	{
		//project
		Axis , ComFolder ,ComParmsSet ,ComParmsSet11G, ComParmsSet_Gj , ComParmsSet_Total ,ComParmsSet_Total11G, ComponentResult ,\
			Components , Components_Gj , DrawIndex ,EditOtherItem  ,Floor, FloorBGMode , \
			FloorDJXS ,FloorDJMGInfo,FloorGJXXInfo ,GJJMInfo ,SwlGJJMInfo, GraphicsResult, LayerInfo ,LiangBanZs , \
			ProjectCode , ProjectInfo , QD_Result, QDXMAtt ,  Result , SwlResult,  Room , SwlRoom, SpecificGravity ,\
			VectorGraph , VectorGraphicsGraph , VectorGraph_Gj ,Entity_QTGJ,\
			CustomSteelComponents , CustomSteelResult , GraphMap ,\
			
			//SysData
			CheckReg , SysComParmsSet , SysComParmsSet_Gj, SysComParmsSet_Total, ComParmsSet_GDTZ , \
			Component_QDDE , SysComponents , CSFLib , CSFLib_Gj , CSFVectorDiagram , CSFVectorDiagram_Gj , \
			FaceHelpInfo ,FaceHelpInfo_GJ , GJFormula ,GJFormula_GJ, GJSetup , GJSetupVectorDiagram , GJZSType , SysGraphMap ,\
			GridStyle , InitMg , Macros ,Steeldiagram ,SteeldiagramLib , SteeldiagramParms , system , SysVectorGraph,\
			SysVectorGraph_Gj , SysVectorGraphicsGraph , VectorIndex,OwnGanjinVector, HjdFaceInfo ,RegInfo ,SpecialVariable ,\
			
			//DEBase 
			chapter , ProjectCode_DEBase ,Ration ,\
			
			//QDBase 
			Bill , QDDefaultQdValue,ProjectCode_QDBase , QDXMAtt_QDBase , RuleIndex, \
			
			//BZCTTj
			Content ,JiangSu ,ZheJiang ,\
			
			//YZBTj
			Attribute_MidSouth , Attribute_RecentUsed ,Attribute_UserDefined , Zjbh_MidSouth,

			//publicGJ
			SysOwnGanjinVector
	};
};

