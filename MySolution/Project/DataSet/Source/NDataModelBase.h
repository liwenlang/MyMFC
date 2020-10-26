// NDataModelBase.h: interface for the NDataModelBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NDATAMODELBASE_H__774F7715_940D_403D_9ECA_7E7C61364F96__INCLUDED_)
#define AFX_NDATAMODELBASE_H__774F7715_940D_403D_9ECA_7E7C61364F96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include  "PMSharePtr.h"
#include "NObserver.h"

//用于存储对象的数据管理

template<typename TNode>
class DataSetAPI CPMArray  
{
public:

	typedef std::vector<TNode>					TNodeList ; 
	typedef typename TNodeList::iterator		TNodeListIter ; 
	typedef typename TNodeList::const_iterator	TNodeListConstIter ; 

	CPMArray(){};
	virtual ~CPMArray(){}

	/**
	* @brief  获取队列元素个数 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [23/5/2012]
	*/
	inline INT_PTR GetSize() const
	{
		return m_PMArray.size();
	}

	/**
	* @brief 判断是否是空队列  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [23/5/2012]
	*/
	inline BOOL IsEmpty() const
	{
		if(0 >= GetSize())
			return TRUE;
		return FALSE;
	}

	/**
	* @brief  设置队列的 内存大小。 
	*			用于扩展队列的大小，避免多次添加值造成，内存反复开辟
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [23/5/2012]
	*/
	inline void SetMemorySize(INT_PTR nNewSize)
	{
		m_PMArray.reserve(nNewSize);
	}

	/**
	* @brief 清空队列元素  
	*
	*          
	* @note  :没有将 队列内存 回收 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [23/5/2012]
	*/
	inline void RemoveAll()
	{
		m_PMArray.erase( m_PMArray.begin(), m_PMArray.end());
	}

	/**
	* @brief  获取队列元素 
	*
	*          
	* @note  : 静态不可以修改 元素
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [23/5/2012]
	*/
	inline const TNode & GetAt(UINT nIndex) const
	{
		if(nIndex < 0 || nIndex >= m_PMArray.size())
			AfxThrowInvalidArgException();
		return m_PMArray[nIndex];
	}

	/**
	* @brief  获取队列元素  
	*
	*          
	* @note  : 可以修改 元素，并同步到队列中
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [23/5/2012]
	*/
	TNode & operator[](UINT nIndex)
	{
		if(nIndex < 0 || nIndex >= m_PMArray.size())
			AfxThrowInvalidArgException();
		return m_PMArray[nIndex];
	}

	/**
	* @brief 修改队列元素  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [23/5/2012]
	*/
	inline BOOL SetAt(UINT nIndex, const TNode & newElement)
	{
		if(nIndex < 0 || nIndex >= m_PMArray.size())
			return FALSE;
		m_PMArray[nIndex] = newElement;
		return TRUE;
	}

	/**
	* @brief   
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [23/5/2012]
	*/
	inline void Add(const TNode & newElement)
	{
		m_PMArray.push_back(newElement);
		//		m_PMArray.insert(m_PMArray.end(),newElement);
		//		return m_PMArray.size() - 1;
	}

	/**
	* @brief  将队列 进行合并 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [23/5/2012]
	*/
	inline void Append(const CPMArray & src)
	{
		m_PMArray.insert(m_PMArray.end(),src.m_PMArray.begin(),src.m_PMArray.end());
	}
	//	void Copy(const CDWordArray& src);

	/**
	* @brief  插入元素 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [23/5/2012]
	*/
	inline BOOL InsertAt(UINT nIndex, const TNode & newElement)
	{
		//if(0 > nIndex)
		//	return FALSE;

		TNodeListIter temIter = m_PMArray.begin();
		//if(nIndex >= m_PMArray.size())
		//	temIter = m_PMArray.end();
		//else
		temIter += nIndex;
		m_PMArray.insert(temIter,newElement);
		return TRUE;
	}

	inline BOOL RemoveAt(UINT nIndex, INT_PTR nCount = 1)
	{
		if(0 > nIndex || nIndex >= m_PMArray.size())
			return FALSE;
		if(nCount <= 0)
			return FALSE;

		TNodeListIter temBeginIter = m_PMArray.begin() + nIndex;
		TNodeListIter temEndIter = temBeginIter + nCount;

		m_PMArray.erase(temBeginIter,temEndIter);
		return TRUE;
	}

	inline TNode * GetData()
	{
		return &(*m_PMArray.begin());
	}
	//	void InsertAt(INT_PTR nStartIndex, CDWordArray* pNewArray);

protected:

	TNodeList	m_PMArray;
};

typedef CPMArray<LPVOID>	CPMPtrArray;
typedef void* NRecordID;

/****************************************************************
功能描述： 错误日志 输出管理类，后期改造成可以为 DataSet修复提供依据的工具
参数：
返 回 值：
算法：
注意事项：
作    者：liwenlang
时    间：[2011:5:20   9:52]
*****************************************************************/
class NDataField;
class NDataModelDataSet;

/**
* @brief DataSet的基类，定义了主要的借口  
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [15/1/2014]
*/
class DataSetAPI CDataSetErrorInfo
{
public:

	enum PMErrorStatus
	{
		DataSetError = 10000
	};
	static CDataSetErrorInfo * Instance()
	{
		if(NULL == m_pInstance)
		{
			m_pInstance = new CDataSetErrorInfo();
		}
		return m_pInstance;
	}

	static void Release()
	{
		if(NULL != m_pInstance)
		{
			delete m_pInstance;
		}
		m_pInstance = NULL;
	}

	void SetErrorFilePath(CString strFilePath);
	CString GetErrorFilePath();

	BOOL PrintInfo(CString strErrorMsg,DWORD error = CDataSetErrorInfo::DataSetError);

	BOOL PrintDataSetInfo(NDataModelDataSet * pDataSet,int iRow,NDataField * pField);

	BOOL PrintDataSetInfo(NDataModelDataSet * pDataSet,NRecordID pRowBuffer,NDataField * pField);

protected:

	CDataSetErrorInfo();
	virtual ~CDataSetErrorInfo();

	CString m_strErrorFilePath;
	static CDataSetErrorInfo * m_pInstance;
};

#ifdef _TESTMEMORY
#	define PMDataSetShowErr(str)			{\
												CString strTemErrInfo;\
												strTemErrInfo.Format(_T("DataSet出错!%s"),str);\
												CDataSetErrorInfo::Instance()->PrintInfo(strTemErrInfo);\
											}
#else
#	define PMDataSetShowErr(str)
#endif

//DataSet的基类，定义了主要的借口
class DataSetAPI NDataModelBase  :public CObject ,public CNotifyPtr
{
public:
	NDataModelBase();
	virtual ~NDataModelBase();
	
public:

	virtual LONG GetNDataMemorySize(){return 0;}

	// Attach observer wnd.
	virtual void AttachObserver( NObserver *ob );
	
	// Detach observer wnd.
	virtual void DetachObserver( NObserver *ob );

	// Notify observer wnd.
	virtual void NotifyObserver(LPARAM lHint, CObject*pHint = NULL);

protected:
		// The list pointer of wnd.
	CPtrList			m_ObserverList;
};

#endif // !defined(AFX_NDATAMODELBASE_H__774F7715_940D_403D_9ECA_7E7C61364F96__INCLUDED_)
