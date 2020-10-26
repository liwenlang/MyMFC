#pragma once

/**
* @brief ����ָ�� ����  
*
*          
* @note  : ֻΪ��CNotifyPtr�࣬�п���ʵ�ʲ�����
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
* @brief ����ӿ�  
*
*          
* @note  : ����ָ�� ���ƽӿ�
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
	* @brief ��� ���� ָ֪ͨ��  
	*
	*          
	* @note  : 
	* @param : CPMMirrorPtr * pTemMirrorBase:����ʵ����
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/12/2013]
	*/
	bool AddNotifyPtr(CPMMirrorPtr * pTemMirrorBase);

	/**
	* @brief �Ƴ����� ָ֪ͨ��  
	*
	*          
	* @note  : 
	* @param :  CPMMirrorPtr * pTemMirrorBase:����ʵ����
	* @defenc: 
	* @return: 
	* @author: liwenlang
	* @date  : [26/12/2013]
	*/
	bool RemoveNotifyPtr(const CPMMirrorPtr * pTemMirrorBase);

	/**
	* @brief ��� ���� ����ָ�����  
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
* @brief ����ָ�� ģ����  
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

	//ע�����ָ��
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

	//ж�ؿ���ָ��
	void UnRegMirrorPtr()
	{
		//�˳�����
		if(m_pCtrlPtr)
		{
			CPMTMirrorPtr * pTemPtr = this;
			m_pCtrlPtr->RemoveNotifyPtr(pTemPtr);
		}
		m_pCtrlPtr = NULL;
	}

	/**
	* @brief ȥ�� ����ָ�����  
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
	* @brief �жϾ���ָ�� �Ƿ���Ч 
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

	T * m_pCtrlPtr;//ģ�����
};
