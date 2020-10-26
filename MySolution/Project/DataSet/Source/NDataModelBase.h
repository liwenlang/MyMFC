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

//���ڴ洢��������ݹ���

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
	* @brief  ��ȡ����Ԫ�ظ��� 
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
	* @brief �ж��Ƿ��ǿն���  
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
	* @brief  ���ö��е� �ڴ��С�� 
	*			������չ���еĴ�С�����������ֵ��ɣ��ڴ淴������
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
	* @brief ��ն���Ԫ��  
	*
	*          
	* @note  :û�н� �����ڴ� ���� 
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
	* @brief  ��ȡ����Ԫ�� 
	*
	*          
	* @note  : ��̬�������޸� Ԫ��
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
	* @brief  ��ȡ����Ԫ��  
	*
	*          
	* @note  : �����޸� Ԫ�أ���ͬ����������
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
	* @brief �޸Ķ���Ԫ��  
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
	* @brief  ������ ���кϲ� 
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
	* @brief  ����Ԫ�� 
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
���������� ������־ ��������࣬���ڸ���ɿ���Ϊ DataSet�޸��ṩ���ݵĹ���
������
�� �� ֵ��
�㷨��
ע�����
��    �ߣ�liwenlang
ʱ    �䣺[2011:5:20   9:52]
*****************************************************************/
class NDataField;
class NDataModelDataSet;

/**
* @brief DataSet�Ļ��࣬��������Ҫ�Ľ��  
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
												strTemErrInfo.Format(_T("DataSet����!%s"),str);\
												CDataSetErrorInfo::Instance()->PrintInfo(strTemErrInfo);\
											}
#else
#	define PMDataSetShowErr(str)
#endif

//DataSet�Ļ��࣬��������Ҫ�Ľ��
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
