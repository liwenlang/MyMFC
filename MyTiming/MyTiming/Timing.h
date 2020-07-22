// Timing.h: interface for the CTiming class.

/**************************************************************** 
 �� �� ��   : Timing.h
 �ļ�˵��   : ʱ��ⶨ��
 ��    ��   : liwenlang
 ��������   : 2020��7��22��
 ����޸�   : 
 ��������   :	����ʹ�ú� ,��release���Զ�ȥ��ʱ����Դ���,
				  �Ѿ���������Ͳ鿴���������: timing ,���������÷�����������
				  �������������ѡ�����:
				  1:�Ƿ���ձ���Ľ��?ֱ����������ѡ��
				  2:�Ƿ�����Ѿ�����Ľ��?�������ļ�c:\TimingSet.ini������
				  ���ֱ���ڴ�����Ƕ��CTiming ����,��Ѹö�����������ں�����ڴ�.
				  һ��Ĭ��ʹ��ֻ��Ҫʹ�� DEFAULT_TIMING ��
 �޸���ʷ   : 
 1.��  ��   : 2020/07/22  12:38:29
   ��  ��   : liwenlang
   �޸����� : �����ļ�
***************************************************************/


#if !defined(AFX_TIMING_H__1B5079C5_8A63_4E1B_B164_115388BAC23B__INCLUDED_)
#define AFX_TIMING_H__1B5079C5_8A63_4E1B_B164_115388BAC23B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

//ȫ��Ĭ�ϲ���
#ifdef  _DEBUG
#define DEFAULT_TIMING(strFunName) \
CTiming OneFunTimeing(strFunName);
#else
#define DEFAULT_TIMING(strFunName) 
#endif

#ifdef  _DEBUG
#define _TIMING_1(strFunName,bAddUp) \
CTiming OneFunTimeing_1(strFunName, bAddUp);
#else
#define _TIMING_1(strFunName,bAddUp) 
#endif


#ifdef  _DEBUG
#define _TIMING_2(strFunName,bAddUp,strBelong) \
CTiming OneFunTimeing_2(strFunName,bAddUp,strBelong);
#else
#define _TIMING_2(strFunName,bAddUp,strBelong) 
#endif

//����ĳһ��������ʱ�亯��
#ifdef  _DEBUG
	#define _TIMING_TOTAL(strFunName,bAddUp,strBelong) \
		CTiming OneFunTimeing_3(strFunName,bAddUp,strBelong,true);
#else
	#define _TIMING_TOTAL(strFunName,bAddUp,strBelong) 
#endif

//�����ֶβ��Ժ�
#ifdef _DEBUG 
#define	_BEGIN_SEG_TIMING(nSegTag,strFunName,bAddUp,strBelong) \
			CTiming *pNewObj##nSegTag = NULL;\
					{\
					static CString str1 = #nSegTag;\
					static CString str2 = str1 + strFunName;\
					pNewObj##nSegTag = new CTiming(str2,bAddUp,strBelong);\
					}\


#define  _END_SEG_TIMING(nSegTag) \
			if (pNewObj##nSegTag != NULL)\
			{\
				delete pNewObj##nSegTag ;\
				pNewObj##nSegTag = NULL;\
			}
#else

#define  _BEGIN_SEG_TIMING(SegTag,strFunName,bAddUp,strBelong)
#define  _END_SEG_TIMING(SegTag)

#endif



//�Ƿ�����ʱ����ԵĿ���,���ε��ö���͹ر��˲��� 
#define INSERT_TIME_CODE
//#ifdef INSERT_TIME_CODE
//	#define TIME_CODE(code_seg){code_seg}
//#else 
//	#define TIME_CODE(code_seg){}
//#endif

const CString QUAN_JU_TIMING = _T("ȫ��ͳ��");//ȫ��ͳ��

// ����������ʱ��ⶨ,����ͳ�Ƶ�����������ʱ��
class CTiming  
{
public:
	//****************************************!
	/*
	*@brief		:  ʹ�ú�������,��Ӧ��ӡ����
	*@author 	:  liwenlang
	*@date  	:  2020��7��22�� 13:01:53
	*@descrip   :  strFunNameʹ�ú�����,���ڱ�ʶ��ʲô��ʱ,
					bAddUp��ʶ�ڶ���ظ�����ʱ�Ƿ���ͳ�ƻ���ֻͳ��һ��,	Ĭ���ǲ��ö��ͳ��
					strBelong��ʶ������˭����,Ĭ��ȡȫ��,Ŀ�����ڿ�������Լ���ͳ�ƽ������Ӱ�����
					bIsTatolTimeRecord ��ʾ�Ƿ��Ǹñ��ȫ��ʱ��.�������ʱ��,��ô����ʹ�ø��������ۼӵõ���ʱ��
					һ����Ӧ�����ֻ����һ�������͵ļ�¼,�������
	*@param[out]:  
	*@param[in] :  
	*@return    :  
	****************************************/
	CTiming(const CString &strFunName, bool bAddUp = true, const CString &strBelong = QUAN_JU_TIMING, 
		bool bIsTatolTimeRecord = false);
	virtual ~CTiming();
	
	//****************************************!
	/*
	*@brief		:  �Ƿ�����ۼ�ͳ��
	*@author 	:  liwenlang
	*@date  	:  2020��7��22�� 13:02:55
	*@descrip   :  �ۼ���Ŀ����������ô���,��ʱ��
					�������Ϊ������Ļ�,��ֻͳ��һ�ε��û��ѵ�ʱ��
	*@param[out]:  
	*@param[in] :  
	*@return    :  
	****************************************/
	void SetAddUp(bool bAddUp){m_bAddUp = bAddUp;};

	//****************************************!
	/*
	*@brief		:  ���ý������
	*@author 	:  liwenlang
	*@date  	:  2020��7��22�� 13:03:21
	*@descrip   :  
	*@param[out]:  
	*@param[in] :  
	*@return    :  
	****************************************/
	void SetBelong(const CString &strBlong){m_strBelong = strBlong;};

	//****************************************!
	/*
	*@brief		:  ��ǰ����ʱ��,��������ʱ��������
	*@author 	:  liwenlang
	*@date  	:  2020��7��22�� 13:03:34
	*@descrip   :  
	*@param[out]:  
	*@param[in] :  
	*@return    :  
	****************************************/
	BOOL CountTime();

private:
	BOOL		m_bHasCount;			//�Ƿ��Ѿ���ʱ����
	bool		m_bAddUp;					//�Ƿ���Ҫͳ��
	bool		m_bIsTatolTimeRecord;	//�Ƿ��Ǹñ��ȫ����ʱ��

	CString		m_strBelong;			//������Χ,��������˭������,�����ֲ���ģ��
	CString		m_strKey;				//������
	DWORD		m_BeginTime;			//��ʼʱ��
	DWORD		m_EndTime;
	
};

//������Լ�¼
class  CRecord
{
	struct FunInfo_T //������Ϣ
	{
		DWORD nCount;					//���ô���
		DWORD nTotalTime;
		bool bAddUp;					//�Ƿ���ͳ��
		bool bIsTatolTimeRecord;		//�Ƿ���ȫ��ʱ��ļ�¼

		FunInfo_T(DWORD nC = 0, DWORD nT = 0, bool bA = true, bool bIsTR = false):nCount(nC),nTotalTime(nT),bAddUp(bA),bIsTatolTimeRecord(bIsTR){};
	};
	//����ͳ�ƽ��
	typedef std::map<CString, FunInfo_T> Str_FunInfoMap;//����һ����λ�Ĳ��Ա�����,��������ӳ��
	typedef std::map<CString, Str_FunInfoMap> Str_TableMap;//���еĲ�������

public:

	//������м�¼
	static void ClearAll();

	//���ָ��������м�¼
	static void ClearTable(const CString &strTableKey);

	//�õ��ܱ�ͷ
	static BOOL GetHeadInfo(CString &strZbt);

protected:

	friend class CTiming;
	friend class COutPutMessage;

	//****************************************!
	/*
	*@brief		:  ����һ����¼
	*@author 	:  liwenlang
	*@date  	:  2020��7��22�� 13:06:49
	*@descrip   :  ��Ĳ������ֻ����CTimingʹ��
	*@param[out]:  
	*@param[in] :  
	*@return    :  
	****************************************/
	static BOOL AddRecord(const CString &strTableKey, const CString &strFunKey, DWORD Time, bool bAddUp, bool bIsTatolTimeRecord);
	
	CRecord(){};
	~CRecord(){};

	//ͳ��һ�ű��д���,��ʱ��, ƽ��ʱ��
	static void CountTable(Str_FunInfoMap &aTable, DWORD &nTotalCs, DWORD &nTotalTime, DWORD &nPjsj);

	//ͳ�ƶ��ű��д���,��ʱ��, ƽ��ʱ��
	static void CountTable(Str_TableMap &aTable, DWORD &nTotalCs, DWORD &nTotalTime, DWORD &nPjsj);

private:
	static Str_TableMap s_AllTable;
	static DWORD s_nTotalCount;			//�ܵĵ��ô���
};


//�����Ϣ,���������ض���,�����Ҫ���¶������Ŀ��,�����ظ���,��ʵ�������麯��
//��ʱ��������֧��!

//ʵ��Ϊ������ļ�
class  COutPutMessage
{
public:
	COutPutMessage();
	virtual ~COutPutMessage(){};

	//�����Ϣ
	BOOL PrintRecord();

protected:
	//���Ŀ���������
	virtual BOOL Clear();
	
	//�����ļ�����
	BOOL AppendFileContent(const CString & strFilePath,const CString & strAppendInfo);

	//����һ��
	virtual BOOL AddOneLine(const CString &strOneLineContext);

	//��ʾ����
	virtual void Show();

private:
	CString m_strFilePath;
};

//ʱ�������ص�ѡ��������
class  CTimingSet
{
public:
	static CTimingSet *Instance();
	static void Release();

	//****************************************!
	/*
	*@brief		:  ��Ϣ����Ƿ���׷��ģʽ,�������,��ô�ϴε������Ŀ������ݻᱻ���.
	*@author 	:  liwenlang
	*@date  	:  2020��7��22�� 13:08:52
	*@descrip   :  
	*@param[out]:  
	*@param[in] :  
	*@return    :  
	****************************************/
	BOOL IsSuperadditionMode();

	//����׷��ģʽ
	BOOL SetSuperadditionMode(BOOL bSuperaddition);

protected:
	CTimingSet(){m_strTimingIniFilePath = _T("c:\\TimingSet.ini"); };
	~CTimingSet(){};

	//���ú�ȡ��ֵ
	BOOL SetKeyAndValue(const CString &strKey, int nValue);
	BOOL GetKeyAndValue(const CString &strKey, int &nValue);

	static const CString s_strBoolDefault;		//����ֵĬ��
	static const CString s_strAppName;		
	static const CString s_strAdditionMode;//׷��ģʽ
	static const int s_iBufSize;

private:
	TCHAR	m_chBuf[100];
	CString m_strTimingIniFilePath;
	static CTimingSet *s_pTimingSet;
};

#endif // !defined(AFX_TIMING_H__1B5079C5_8A63_4E1B_B164_115388BAC23B__INCLUDED_)
