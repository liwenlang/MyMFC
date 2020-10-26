// NCGroup.h: interface for the NCGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NCGROUP_H__C04724A8_44F4_4A7B_9485_61D2F0A6F77A__INCLUDED_)
#define AFX_NCGROUP_H__C04724A8_44F4_4A7B_9485_61D2F0A6F77A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class NCGroup  
{
public:
	void * GetRecordID();//取得本分组下面的第一条数据项的内存句柄
	void FreeGroup();
	//将分组加到当前的第iLevel级次上
	BOOL AddChildGroup(NCGroup *pGroup,int iLevel);
	NCGroup();
	virtual ~NCGroup();
	void AddChildGroup(NCGroup *pGroup)
	{
		m_pChildGroup->Add(pGroup);
	}
	void AddItem(void *pitem)
	{
		CheckItems();
		m_pItems->Add(pitem);
	}

	inline void SetGroupField(LPVOID pGroupField) {m_pGroupField = pGroupField;}
	inline LPVOID GetGroupField() {return m_pGroupField;}

protected:
	void CheckChildGroup();
	void CheckItems()
	{
		if (m_pItems == NULL) 
		{
			m_pItems = new CPtrArray;
		}
	}

	CPtrArray *m_pItems;
	CPtrArray *m_pChildGroup;
	void*      m_pGroupField;
 
};

#endif // !defined(AFX_NCGROUP_H__C04724A8_44F4_4A7B_9485_61D2F0A6F77A__INCLUDED_)
