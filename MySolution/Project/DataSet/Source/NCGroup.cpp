// NCGroup.cpp: implementation of the NCGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCGroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NCGroup::NCGroup()
{
   m_pItems = NULL;
   m_pChildGroup = NULL;
   m_pGroupField = NULL;
}

NCGroup::~NCGroup()
{
    if (m_pChildGroup != NULL) 
	{
		delete m_pChildGroup;
    }
	if (m_pItems != NULL) 
	{
		delete m_pItems;
	}
}

BOOL NCGroup::AddChildGroup(NCGroup *pGroup, int iLevel)
{
   NCGroup *pCildGroup;
   if (iLevel == 1)
   {
	   CheckChildGroup();
	   m_pChildGroup->Add(pGroup);
	   return TRUE;
   }
   if (m_pChildGroup != NULL&&m_pChildGroup->GetSize()>0) 
   {
	   pCildGroup = (NCGroup *)m_pChildGroup->GetAt(m_pChildGroup->GetSize() - 1);
	   return pCildGroup->AddChildGroup(pGroup, iLevel - 1);
   }
   return FALSE;
}

void NCGroup::CheckChildGroup()
{
  if (m_pChildGroup == NULL) 
  {
	  m_pChildGroup = new CPtrArray;
  }
}

void NCGroup::FreeGroup()
{
	
   if (m_pChildGroup != NULL) 
   {
	   NCGroup *pGroup;
      for(int i = 0 ; i < m_pChildGroup->GetSize(); i++)
	  {
		  pGroup = (NCGroup *)m_pChildGroup->GetAt(i);
		  pGroup->FreeGroup();
		  delete pGroup;
	  }
	  m_pChildGroup->RemoveAll();
      delete m_pChildGroup;
	  m_pChildGroup = NULL;
   }
}

void * NCGroup::GetRecordID()
{
	if (m_pChildGroup != NULL && m_pChildGroup->GetSize() > 0) 
	{
		NCGroup *pGroup;
		pGroup = (NCGroup *)m_pChildGroup->GetAt(0);
		return pGroup->GetRecordID();
	}
	else
	{
		if (m_pItems != NULL && m_pItems->GetSize() >0 ) 
		{
			return m_pItems->GetAt(0);
		}
		else
		{
			return NULL;
		}
	}
}
