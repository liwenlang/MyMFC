#include "StdAfx.h"
#include "PMSharePtr.h"

CPMMirrorPtr::CPMMirrorPtr(void)
{
}

CPMMirrorPtr::~CPMMirrorPtr(void)
{
}

//////////////////////////////////////////////////////////////////////////
//CNotifyPtr
CNotifyPtr::CNotifyPtr()
{

}

CNotifyPtr::~CNotifyPtr()
{
	DoClearMirrorPtr();
}

bool CNotifyPtr::AddNotifyPtr(CPMMirrorPtr * pTemMirrorBase)
{
	for (int i = 0;i < m_MirrorPtrArr.GetSize();i++)
	{
		if(pTemMirrorBase == m_MirrorPtrArr.GetAt(i))
			return false;
	}
	m_MirrorPtrArr.Add(pTemMirrorBase);
	return true;
}

bool CNotifyPtr::RemoveNotifyPtr(const CPMMirrorPtr * pTemMirrorBase)
{
	for (int i = 0;i < m_MirrorPtrArr.GetSize();i++)
	{
		if(pTemMirrorBase == m_MirrorPtrArr.GetAt(i))
		{
			m_MirrorPtrArr.RemoveAt(i);
			return true;
		}
	}
	
	return false;
}

void CNotifyPtr::DoClearMirrorPtr()
{
	for(int i = 0;i < m_MirrorPtrArr.GetSize();i++)
	{
		CPMMirrorPtr * pTemMirrorPtr = m_MirrorPtrArr.GetAt(i);
		pTemMirrorPtr->DoClearMirrorPtr();
	}
	m_MirrorPtrArr.RemoveAll();
}
