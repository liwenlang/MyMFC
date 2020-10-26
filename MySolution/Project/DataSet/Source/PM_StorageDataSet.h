// PM_StorageDataSet.h: interface for the CPM_StorageDataSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PM_STORAGEDATASET_H__D02863CF_6B7A_4F9A_8C5F_53848D7C4799__INCLUDED_)
#define AFX_PM_STORAGEDATASET_H__D02863CF_6B7A_4F9A_8C5F_53848D7C4799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NDataModelDataSet.h"
//#include "NDbDataSet.h"

/**
* @brief �����ĵ� DataSetʵ��
*
*        �ṩ��DataSet���� ���浽�����ĵ��еĹ��ܣ����б���Updata��
Access���ݿ��������Ϊ������ԡ�

  V1:
  �����ĵ����ݽṹ��
  Root
  DataSet����
		DataVersion:�汾
		DataFieldInfo:�ֶ�����:�ֶ�����:�ֶ�Size;
		RowsInfo://����Ϣ
		Row(1)
		.
		.
		.
		Row(n)
		
		  Row���ݽṹ��
		  �ܴ�С+NormalFieldInfo + Blob�ֶ�1��Size + Blob�ֶ�1��Value+ ...+ Blob�ֶ�n��Size + Blob�ֶ�n��Value
		  
			V2:
			�����ĵ����ݽṹ��
			Root
			DataSet����:DataSet��������
			
			 DataSet���ݣ��������ݴ�С+DataVersion��2Byte��+�ֶ���Ϣ��С+�ֶ���Ϣ+���뷽ʽ��2Byte��+����+���д�С+��������---
			  
			//////////////////////////////////////////////////////////////////////////
			V3:
			�����ĵ����ݽṹ��
			Root
			DataSet����:DataSet��������
				
			DataSet���ݣ��������ݴ�С+StructVersion(2Byte) + DataVersion��2Byte��+�ֶ���Ϣ��С+�ֶ���Ϣ+���뷽ʽ��2Byte��+����+���д�С+��������---
				  
		ע�⣺
			StructVersion��Ϊ�ṹ�汾�������������ݽṹ�����磺�ֶ���Ϣ��Сռ�õĿռ� ���ַ�����Ԥ���ռ����
					
		 //////////////////////////////////////////////////////////////////////////
					  
		���뷽ʽ��2Byte��2:Acsii  1:Unicode
						
	* @author liwenlang
	* @version v2.0
	* @date  [30/1/2012]
	* @note  
*/

/**
 * @brief �����ĵ� ����ָ���װ��
 *
 *        
 * @author liwenlang
 * @version v1.0
 * @date  [30/1/2012]
 * @note  
 */
class DataSetAPI CPMFileStorageCtrl
{
public:
	CPMFileStorageCtrl();
	~CPMFileStorageCtrl();

	/**
	* @brief ���Դ� һ�������ĵ��ļ�  
	*
	*          
	* @note  : 
	* @param :const CString & strFilePath:�����ĵ� ·�� 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	HRESULT TryOpenStorage(const CString & strFilePath);

	/**
	* @brief  ���� �����ĵ�����ָ�� 
	*
	*          
	* @note  : ����ӿ�ֻ��CPMFileStreamCtrl ʹ��
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	inline IStorage * GetStorage();

	/**
	* @brief ��ȡ�ļ� ·��  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	inline CString GetFilePath();

	/**
	* @brief   
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	HRESULT CloseStorage();

protected:
	
	IStorage * m_pStorage;//���� ���ڵ�ָ��
	CString m_strFilePath;//�ļ�·��
};

class DataSetAPI CPMFileStreamCtrl
{
public:
	CPMFileStreamCtrl();
	~CPMFileStreamCtrl();

	/**
	* @brief  ���Դ� һ���� 
	*
	*          
	* @note  : ��������� ����ʱ �ͻᴴ��һ����
	* @param : 
	*			CPMFileStorageCtrl * pStorageCtrl:�����ĵ�����ָ��
	*			const CString & strStreamName�����Դ򿪵� ������
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	HRESULT TryOpenStream(CPMFileStorageCtrl * pStorageCtrl,const CString & strStreamName);

	/**
	* @brief  ��ȡ���е� ���� 
	*
	*          
	* @note  : 
	* @param : LPVOID pBuffer�����ص� Buffer������Ҫ���ٺ�lBufferSize��С�Ŀռ�
	*			ULONG lBufferSize��pBuffer���ڴ��С��
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	HRESULT ReadStream(LPVOID pBuffer,ULONG lBufferSize);

	/**
	* @brief  ������д�� ����
	*
	*          
	* @note  : ��д�뺯���� ������ ��֤���ƣ����ڲ����Ƿ�д��ɹ���
	* @param : LPVOID pBuffer������� Buffer
	*			ULONG lBufferSize��pBuffer���ڴ��С��
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	HRESULT WriteStream(LPVOID pBuffer,ULONG lBufferSize);

	/**
	* @brief ���� ��ָ��   
	*
	*          
	* @note  : �˺��� ������ʹ��
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	IStream * GetStream();
	
	/**
	* @brief  �ر���
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	HRESULT CloseStream();

	/**
	* @brief  ������ �α�λ�� 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [31/1/2012]
	*/
	HRESULT SeekStream(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER * plibNewPosition);

protected:
	
	CPMFileStorageCtrl * m_pStorageCtrl;//���� ���ڵ�ָ��
	IStream * m_pCurStream;//��ǰ��������
};

/**
* @brief DataSet�ĸ����ĵ� �ӿڷ�װ  
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class DataSetAPI CPM_StorageDataSet : public NDataModelDataSet
{
	DECLARE_DYNCREATE(CPM_StorageDataSet) 
public:

	const static short m_iStructDataVersion;//�ṹ�汾

	/**
	* @brief  DataSet���浽�����ĵ����� 
	*
	*          
	* @note  : IStorage* piFileRootStr Ҫִ��ͬ�����ݹ��ܣ����� �����޸� ���ᱣ��
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [17/10/2011]
	*/
	BOOL DataSet2Stream(CPMFileStorageCtrl & storageFileCtrl);

	/**
	* @brief   
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [17/10/2011]
	*/
	BOOL Stream2DataSet(CPMFileStorageCtrl & fileStorageCtrl,BOOL bAppend = FALSE);

	/******************************************************************** 
	 ����������
	
	
	         _   _
	        (q\_/p)
	    .-.  |. .|
	       \ =\,/=
	        )/ _ \  |\
	       (/\):(/\  )\
	        \_   _/ |Oo\
	        `""^""` `"""`
	
	 ������
	 �� �� ֵ��
	 �㷨��
	 ע�����
	 ��    �ߣ�liwenlang
	 ʱ    �䣺[31/5/2010]
	*********************************************************************/ 
	virtual BOOL GetVersionID(short & iVersion);
	void SetVersionID(short iVersion);

	CPM_StorageDataSet();
	virtual ~CPM_StorageDataSet();

	/******************************************************************** 
	 ������������ָ�������ĵ� �������ݶ���DataSet
	         _   _
	        (q\_/p)
	    .-.  |. .|
	       \ =\,/=
	        )/ _ \  |\
	       (/\):(/\  )\
	        \_   _/ |Oo\
	        `""^""` `"""`
	
	 ������bAppend��TRUE����� �����ԭ������  FALSE����� ���ԭ������
	 �� �� ֵ��
	 �㷨��
	 ע�����
	 ��    �ߣ�liwenlang
	 ʱ    �䣺[31/5/2010]
	*********************************************************************/ 
	BOOL OpenMDocument(CString strFileName,BOOL bAppend);

	/******************************************************************** 
	 ��������������DataSet �� ָ���ĸ����ĵ���
		
	         _   _
	        (q\_/p)
	    .-.  |. .|
	       \ =\,/=
	        )/ _ \  |\
	       (/\):(/\  )\
	        \_   _/ |Oo\
	        `""^""` `"""`
	
	 ������
	 �� �� ֵ��
	 �㷨��
	 ע�����
	 ��    �ߣ�liwenlang
	 ʱ    �䣺[31/5/2010]
	*********************************************************************/ 
	BOOL SaveMDocument(CString strFileName);

	/******************************************************************** 
	 ������������DataSet���浽Buffer
	 ������
	 �� �� ֵ��
	 �㷨��
	 ע�����
	 ��    �ߣ�liwenlang
	 ʱ    �䣺[19/7/2010]
	*********************************************************************/ 
	BOOL DataSet2Buffer(LPVOID & pBuffer,ULONG & lBufferSize);

	/******************************************************************** 
	 ������������Buffer��ȡ��DataSet��
	 ������
	 �� �� ֵ��
	 �㷨��
	 ע�����
	 ��    �ߣ�liwenlang
	 ʱ    �䣺[19/7/2010]
	*********************************************************************/ 
	BOOL Buffer2DataSet(LPVOID pBuffer,ULONG lBufferSize,BOOL bAppend = FALSE);

protected:
	
	virtual void DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff);

	void SetOpeningDataSetDataCode(int iCodeMode,short & iOpeningDataCode);
	
	BOOL Buffer2DataSet_V1(short iStructVer,LPVOID pDataSetAllInfo,ULONG lDataSetAllSize);

	BOOL Stream2DataSet_V1(short iStructVer,ULONG lDataSetAllBufferSize,CPMFileStreamCtrl & fileStreamCtrl);

	BOOL ReadRowFStream_SVersion_V1(short iStructVer,CPM_StorageDataSet * pDataSet,LPVOID pCurRowInfo);
	
	/******************************************************************** 
	 ��������������һ������ ��Ϣָ�� ���� ��������
	         _   _
	        (q\_/p)
	    .-.  |. .|
	       \ =\,/=
	        )/ _ \  |\
	       (/\):(/\  )\
	        \_   _/ |Oo\
	        `""^""` `"""`	
	 ������
	 �� �� ֵ��
	 �㷨��
	 ע�����
	 ��    �ߣ�liwenlang
	 ʱ    �䣺[31/5/2010]
	*********************************************************************/ 
	BOOL BuildSimRowBuffer(int row, LPVOID &pValue, long &iSize,eBufferFlag buffer);

	short m_iVersion;//����DataSet�汾��������Ҫ�趨��ʼֵ��������Ƹ����ĵ�ʧ�� (Ĭ�ϣ�-1)

	short m_iOpeningDataSetDataCode;//������ʱ���棬�ڶ�ȡ��DataSet�ı��뷽ʽ��0:A2W(AcsiiתUnicode)  1:W2A(UnicodeתAcsii)

	/********************************************************************
	liwenlang
		ʱ��:2011:1:4   9:43
		��ע:DataCode:�ڶ�ȡ��DataSet�ı��뷽ʽ��0:A2W(AcsiiתUnicode)  1:W2A(UnicodeתAcsii)
	*********************************************************************/
	BOOL BuildTemDataSet(int DataCode,LPVOID pFieldInfo, CPM_StorageDataSet *& pNewDataSet);

	///////////////////////////////////
	///liwenlang 
	///д DataSet Field��Ϣ
	///[1/6/2010]
	//////////////////////////////////
	BOOL BuildFieldInfo(NDataFields * pNDataFields,LPVOID & pFieldInfo,int & iFieldSize);
	
private:

	void SplitCString(CString source, CStringArray& dest, TCHAR division);	
	
	BOOL ReadRowFStream_SVersion(short iStructVer,CPM_StorageDataSet * pDataSet,LPVOID pCurRowInfo,LONG lRowBufferSize);

	///////////////////////////////////
	///liwenlang 
	///��ȡDataSet��¼ ����ͬ�汾��
	///[1/6/2010]
	//////////////////////////////////
	BOOL ReadRowsFStorage_SVersion(short iStructVer,CPM_StorageDataSet * pDataSet,LONG lDataSetBufferSize,LPVOID pRowsInfo);

	BOOL ReadRowsFStorage_SVersion_Stream(short iStructVer,CPM_StorageDataSet * pDataSet,LONG lDataSetBufferSize,CPMFileStreamCtrl & fileStreamCtrl);

	///////////////////////////////////
	///liwenlang 
	///��ȡDataSet��¼ �ڲ�ͬ�汾��
	///[1/6/2010]
	//////////////////////////////////
	BOOL ReadRowsFStorage_DVersion(short iStructVer,LONG lDataSetBufferSize,LPVOID pFieldInfo,LPVOID pRowsInfo);

	BOOL ReadRowsFStorage_DVersion_Stream(short iStructVer,LONG lDataSetBufferSize,LPVOID pFieldInfo,CPMFileStreamCtrl & fileStreamCtrl);

};

#define PMStorgeDataSet2Buffer(pDataSet,pBuffer,ulSize)	{\
	if(pDataSet->IsKindOf(RUNTIME_CLASS(CPM_StorageDataSet)))\
	((CPM_StorageDataSet*)pDataSet)->DataSet2Buffer( pBuffer , ulSize ) ; \
															else\
															{\
															CString strMsgInfo;\
															strMsgInfo.Format(_T("CPM_StorageDataSetǿ��ת������:%s(%d)"),__FILE__,__LINE__);\
															AfxMessageBox(strMsgInfo);\
															}\
}

#define PMBuffer2StorgeDataSet(pBuffer,ulSize,pDataSet)	{\
	if(pDataSet->IsKindOf(RUNTIME_CLASS(CPM_StorageDataSet)))\
	((CPM_StorageDataSet*)pDataSet)->Buffer2DataSet( pBuffer , ulSize ) ; \
															else\
															{\
															CString strMsgInfo;\
															strMsgInfo.Format(_T("CPM_StorageDataSetǿ��ת������:%s(%d)"),__FILE__,__LINE__);\
															AfxMessageBox(strMsgInfo);\
															}\
}
#endif // !defined(AFX_PM_STORAGEDATASET_H__D02863CF_6B7A_4F9A_8C5F_53848D7C4799__INCLUDED_)
