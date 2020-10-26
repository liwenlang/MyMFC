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
* @brief 复合文档 DataSet实现
*
*        提供将DataSet对象 保存到复合文档中的功能，其中保留Updata到
Access数据库的能力，为方便调试。

  V1:
  复合文档数据结构：
  Root
  DataSet名字
		DataVersion:版本
		DataFieldInfo:字段名字:字段类型:字段Size;
		RowsInfo://行信息
		Row(1)
		.
		.
		.
		Row(n)
		
		  Row数据结构：
		  总大小+NormalFieldInfo + Blob字段1：Size + Blob字段1：Value+ ...+ Blob字段n：Size + Blob字段n：Value
		  
			V2:
			复合文档数据结构：
			Root
			DataSet名字:DataSet所有内容
			
			 DataSet内容：所有内容大小+DataVersion（2Byte）+字段信息大小+字段信息+编码方式（2Byte）+行数+单行大小+单行内容---
			  
			//////////////////////////////////////////////////////////////////////////
			V3:
			复合文档数据结构：
			Root
			DataSet名字:DataSet所有内容
				
			DataSet内容：所有内容大小+StructVersion(2Byte) + DataVersion（2Byte）+字段信息大小+字段信息+编码方式（2Byte）+行数+单行大小+单行内容---
				  
		注意：
			StructVersion：为结构版本，用于升级数据结构，比如：字段信息大小占用的空间 ，字符串后预留空间个数
					
		 //////////////////////////////////////////////////////////////////////////
					  
		编码方式（2Byte）2:Acsii  1:Unicode
						
	* @author liwenlang
	* @version v2.0
	* @date  [30/1/2012]
	* @note  
*/

/**
 * @brief 复合文档 控制指针包装类
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
	* @brief 尝试打开 一个复合文档文件  
	*
	*          
	* @note  : 
	* @param :const CString & strFilePath:复合文档 路径 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	HRESULT TryOpenStorage(const CString & strFilePath);

	/**
	* @brief  返回 复合文档控制指针 
	*
	*          
	* @note  : 这个接口只给CPMFileStreamCtrl 使用
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	inline IStorage * GetStorage();

	/**
	* @brief 获取文件 路径  
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
	
	IStorage * m_pStorage;//流的 父节点指针
	CString m_strFilePath;//文件路径
};

class DataSetAPI CPMFileStreamCtrl
{
public:
	CPMFileStreamCtrl();
	~CPMFileStreamCtrl();

	/**
	* @brief  尝试打开 一个流 
	*
	*          
	* @note  : 当这个流不 存在时 就会创建一个流
	* @param : 
	*			CPMFileStorageCtrl * pStorageCtrl:复合文档控制指针
	*			const CString & strStreamName：尝试打开的 流名称
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	HRESULT TryOpenStream(CPMFileStorageCtrl * pStorageCtrl,const CString & strStreamName);

	/**
	* @brief  读取流中的 数据 
	*
	*          
	* @note  : 
	* @param : LPVOID pBuffer：返回的 Buffer，但是要开辟好lBufferSize大小的空间
	*			ULONG lBufferSize：pBuffer的内存大小。
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	HRESULT ReadStream(LPVOID pBuffer,ULONG lBufferSize);

	/**
	* @brief  将数据写入 流中
	*
	*          
	* @note  : 在写入函数中 加入了 验证机制，用于测试是否写入成功。
	* @param : LPVOID pBuffer：传入的 Buffer
	*			ULONG lBufferSize：pBuffer的内存大小。
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	HRESULT WriteStream(LPVOID pBuffer,ULONG lBufferSize);

	/**
	* @brief 返回 流指针   
	*
	*          
	* @note  : 此函数 不建议使用
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [30/1/2012]
	*/
	IStream * GetStream();
	
	/**
	* @brief  关闭流
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
	* @brief  设置流 游标位置 
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
	
	CPMFileStorageCtrl * m_pStorageCtrl;//流的 父节点指针
	IStream * m_pCurStream;//当前操作的流
};

/**
* @brief DataSet的复合文档 接口封装  
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

	const static short m_iStructDataVersion;//结构版本

	/**
	* @brief  DataSet保存到复合文档流中 
	*
	*          
	* @note  : IStorage* piFileRootStr 要执行同步数据功能，否则 数据修改 不会保存
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
	 功能描述：
	
	
	         _   _
	        (q\_/p)
	    .-.  |. .|
	       \ =\,/=
	        )/ _ \  |\
	       (/\):(/\  )\
	        \_   _/ |Oo\
	        `""^""` `"""`
	
	 参数：
	 返 回 值：
	 算法：
	 注意事项：
	 作    者：liwenlang
	 时    间：[31/5/2010]
	*********************************************************************/ 
	virtual BOOL GetVersionID(short & iVersion);
	void SetVersionID(short iVersion);

	CPM_StorageDataSet();
	virtual ~CPM_StorageDataSet();

	/******************************************************************** 
	 功能描述：打开指定复合文档 并将数据读入DataSet
	         _   _
	        (q\_/p)
	    .-.  |. .|
	       \ =\,/=
	        )/ _ \  |\
	       (/\):(/\  )\
	        \_   _/ |Oo\
	        `""^""` `"""`
	
	 参数：bAppend：TRUE，添加 不清除原先内容  FALSE，添加 清除原先内容
	 返 回 值：
	 算法：
	 注意事项：
	 作    者：liwenlang
	 时    间：[31/5/2010]
	*********************************************************************/ 
	BOOL OpenMDocument(CString strFileName,BOOL bAppend);

	/******************************************************************** 
	 功能描述：保存DataSet 到 指定的复合文档中
		
	         _   _
	        (q\_/p)
	    .-.  |. .|
	       \ =\,/=
	        )/ _ \  |\
	       (/\):(/\  )\
	        \_   _/ |Oo\
	        `""^""` `"""`
	
	 参数：
	 返 回 值：
	 算法：
	 注意事项：
	 作    者：liwenlang
	 时    间：[31/5/2010]
	*********************************************************************/ 
	BOOL SaveMDocument(CString strFileName);

	/******************************************************************** 
	 功能描述：将DataSet保存到Buffer
	 参数：
	 返 回 值：
	 算法：
	 注意事项：
	 作    者：liwenlang
	 时    间：[19/7/2010]
	*********************************************************************/ 
	BOOL DataSet2Buffer(LPVOID & pBuffer,ULONG & lBufferSize);

	/******************************************************************** 
	 功能描述：将Buffer读取到DataSet中
	 参数：
	 返 回 值：
	 算法：
	 注意事项：
	 作    者：liwenlang
	 时    间：[19/7/2010]
	*********************************************************************/ 
	BOOL Buffer2DataSet(LPVOID pBuffer,ULONG lBufferSize,BOOL bAppend = FALSE);

protected:
	
	virtual void DeleteRowInternal(int row ,NDataModelDataSetBuffer *pBuff);

	void SetOpeningDataSetDataCode(int iCodeMode,short & iOpeningDataCode);
	
	BOOL Buffer2DataSet_V1(short iStructVer,LPVOID pDataSetAllInfo,ULONG lDataSetAllSize);

	BOOL Stream2DataSet_V1(short iStructVer,ULONG lDataSetAllBufferSize,CPMFileStreamCtrl & fileStreamCtrl);

	BOOL ReadRowFStream_SVersion_V1(short iStructVer,CPM_StorageDataSet * pDataSet,LPVOID pCurRowInfo);
	
	/******************************************************************** 
	 功能描述：建立一行数据 信息指针 包括 所有内容
	         _   _
	        (q\_/p)
	    .-.  |. .|
	       \ =\,/=
	        )/ _ \  |\
	       (/\):(/\  )\
	        \_   _/ |Oo\
	        `""^""` `"""`	
	 参数：
	 返 回 值：
	 算法：
	 注意事项：
	 作    者：liwenlang
	 时    间：[31/5/2010]
	*********************************************************************/ 
	BOOL BuildSimRowBuffer(int row, LPVOID &pValue, long &iSize,eBufferFlag buffer);

	short m_iVersion;//保存DataSet版本，派生类要设定初始值，否则控制复合文档失败 (默认：-1)

	short m_iOpeningDataSetDataCode;//用于暂时保存，在读取的DataSet的编码方式。0:A2W(Acsii转Unicode)  1:W2A(Unicode转Acsii)

	/********************************************************************
	liwenlang
		时间:2011:1:4   9:43
		备注:DataCode:在读取的DataSet的编码方式。0:A2W(Acsii转Unicode)  1:W2A(Unicode转Acsii)
	*********************************************************************/
	BOOL BuildTemDataSet(int DataCode,LPVOID pFieldInfo, CPM_StorageDataSet *& pNewDataSet);

	///////////////////////////////////
	///liwenlang 
	///写 DataSet Field信息
	///[1/6/2010]
	//////////////////////////////////
	BOOL BuildFieldInfo(NDataFields * pNDataFields,LPVOID & pFieldInfo,int & iFieldSize);
	
private:

	void SplitCString(CString source, CStringArray& dest, TCHAR division);	
	
	BOOL ReadRowFStream_SVersion(short iStructVer,CPM_StorageDataSet * pDataSet,LPVOID pCurRowInfo,LONG lRowBufferSize);

	///////////////////////////////////
	///liwenlang 
	///读取DataSet记录 在相同版本中
	///[1/6/2010]
	//////////////////////////////////
	BOOL ReadRowsFStorage_SVersion(short iStructVer,CPM_StorageDataSet * pDataSet,LONG lDataSetBufferSize,LPVOID pRowsInfo);

	BOOL ReadRowsFStorage_SVersion_Stream(short iStructVer,CPM_StorageDataSet * pDataSet,LONG lDataSetBufferSize,CPMFileStreamCtrl & fileStreamCtrl);

	///////////////////////////////////
	///liwenlang 
	///读取DataSet记录 在不同版本中
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
															strMsgInfo.Format(_T("CPM_StorageDataSet强制转换出错:%s(%d)"),__FILE__,__LINE__);\
															AfxMessageBox(strMsgInfo);\
															}\
}

#define PMBuffer2StorgeDataSet(pBuffer,ulSize,pDataSet)	{\
	if(pDataSet->IsKindOf(RUNTIME_CLASS(CPM_StorageDataSet)))\
	((CPM_StorageDataSet*)pDataSet)->Buffer2DataSet( pBuffer , ulSize ) ; \
															else\
															{\
															CString strMsgInfo;\
															strMsgInfo.Format(_T("CPM_StorageDataSet强制转换出错:%s(%d)"),__FILE__,__LINE__);\
															AfxMessageBox(strMsgInfo);\
															}\
}
#endif // !defined(AFX_PM_STORAGEDATASET_H__D02863CF_6B7A_4F9A_8C5F_53848D7C4799__INCLUDED_)
