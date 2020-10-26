#pragma once

/**
* @brief 镜像指针 基类  
*
*          
* @note  : 只为让CNotifyPtr类，有可以实际操作类
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [26/12/2013]
*/
class DataSetAPI CPMMirrorPtr
{
public:
	CPMMirrorPtr();
	virtual ~CPMMirrorPtr();

	virtual void DoClearMirrorPtr() = 0;

protected:


};
typedef CArray<CPMMirrorPtr*,CPMMirrorPtr*&>	CPMMirrorPtrArr;

/**
* @brief 基类接口  
*
*          
* @note  : 镜像指针 控制接口
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [23/12/2013]
*/
class DataSetAPI CNotifyPtr
{
public:
	CNotifyPtr();
	virtual ~CNotifyPtr();

	/**
	* @brief 添加 镜像 通知指针  
	*
	*          
	* @note  : 
	* @param : CPMMirrorPtr * pTemMirrorBase:镜像实现类
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/12/2013]
	*/
	bool AddNotifyPtr(CPMMirrorPtr * pTemMirrorBase);

	/**
	* @brief 移除镜像 通知指针  
	*
	*          
	* @note  : 
	* @param :  CPMMirrorPtr * pTemMirrorBase:镜像实现类
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/12/2013]
	*/
	bool RemoveNotifyPtr(const CPMMirrorPtr * pTemMirrorBase);

	/**
	* @brief 清除 所有 镜像指针关联  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/12/2013]
	*/
	void DoClearMirrorPtr();

protected:

	CPMMirrorPtrArr m_MirrorPtrArr;
};

/**
* @brief 镜像指针 模版类  
*
*          
* @note  : 
* @param : 
* @defenc: 
* @return: 
* @author: liwenlang
* @date  : [26/12/2013]
*/
template <class T>
class CPMTMirrorPtr : public CPMMirrorPtr
{
public:
	CPMTMirrorPtr(T * pCtrlPtr) : m_pCtrlPtr(pCtrlPtr)
	{
		if(m_pCtrlPtr)
		{
			m_pCtrlPtr->AddNotifyPtr(this);
		}
	}

	~CPMTMirrorPtr(void)
	{
		UnRegMirrorPtr();
	}

	//注册控制指针
	void RegMirrorPtr(T * pPtr)
	{
		if(m_pCtrlPtr == pPtr)
			return;
		UnRegMirrorPtr();
		m_pCtrlPtr = pPtr;
		if(m_pCtrlPtr)
		{
			m_pCtrlPtr->AddNotifyPtr(this);
		}
	}

	//卸载控制指针
	void UnRegMirrorPtr()
	{
		//退出控制
		if(m_pCtrlPtr)
		{
			CPMTMirrorPtr * pTemPtr = this;
			m_pCtrlPtr->RemoveNotifyPtr(pTemPtr);
		}
		m_pCtrlPtr = NULL;
	}

	/**
	* @brief 去除 镜像指针关联  
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/12/2013]
	*/
	virtual void DoClearMirrorPtr()
	{
		m_pCtrlPtr = NULL;
	}

	/**
	* @brief 判断镜像指针 是否有效 
	*
	*          
	* @note  : 
	* @param : 
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/12/2013]
	*/
	BOOL CheckMirrorPtr_IsOK()
	{
		if(m_pCtrlPtr)
			return TRUE;
		return FALSE;
	}

	// Allow usage as a pointer
	T* operator->() {return m_pCtrlPtr;}

	// Allow usage as a pointer
	operator T*() {return m_pCtrlPtr;}


protected:

	T * m_pCtrlPtr;//模版对象
};
