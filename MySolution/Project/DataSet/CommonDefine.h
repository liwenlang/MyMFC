
#pragma once


#ifdef  UNICODE                    
#ifndef _PM_T_DEFINED
/// 		
typedef	std::wstring	PMTSTRING, PmTString;
typedef std::wostringstream PMTSTRINGSTREAM, PmTStringStream;
typedef std::wostream	PMOSTREAM, PmOStream;
#define _PM_T_DEFINED
#endif
#else
#ifndef _PM_T_DEFINED
typedef	std::string		PMTSTRING, PmTString;
typedef std::ostringstream PMTSTRINGSTREAM, PmTStringStream;
typedef std::ostream	PMOSTREAM, PmOStream;
#define _PM_T_DEFINED
#endif /* !_TCHAR_DEFINED */
#endif /*UNICODE*/

#ifdef _TESTMEMORY
#define PMCheckSpecMemoryLive(TYPE,ShowMsgBox)		CPMMemoryManager::Instance()->CheckSpecMemoryLine(TYPE,ShowMsgBox)
#define PMAddSpecMemory(TYPE)			CPMMemoryManager::Instance()->AddSpecMemory(TYPE)
#define PMNewCArray(TYPE)				(CArray<TYPE,TYPE&> *)(CPMMemoryManager::Instance()->AddMemoryInfo(new CArray<TYPE,TYPE&>,__FILE__,__LINE__))
#define PMNewAcArray(TYPE)				(AcArray<TYPE> *)(CPMMemoryManager::Instance()->AddMemoryInfo(new AcArray<TYPE>,__FILE__,__LINE__))
#define PMNew(TYPE)						(TYPE*)(CPMMemoryManager::Instance()->AddMemoryInfo(new TYPE,__FILE__,__LINE__))
#define PMNew_Parm(TYPE,Parm)			(TYPE*)(CPMMemoryManager::Instance()->AddMemoryInfo(new TYPE Parm,__FILE__,__LINE__))
#define PMRegMemory(TYPE)				CPMMemoryManager::Instance()->AddMemoryInfo(TYPE,__FILE__,__LINE__)
#define PMUnregMemory(TYPE)				CPMMemoryManager::Instance()->RemoveMemoryInfo(TYPE,__FILE__,__LINE__)
#define PMDelete(TYPE)					{delete TYPE;CPMMemoryManager::Instance()->RemoveMemoryInfo(TYPE,__FILE__,__LINE__);TYPE=NULL;}
#define PMDeleteVoid(TYPE,Value)		{delete (TYPE)Value;CPMMemoryManager::Instance()->RemoveMemoryInfo(Value,__FILE__,__LINE__);Value=NULL;}
#define PMDeletePBYTE(Value)			{delete [](BYTE*)Value;CPMMemoryManager::Instance()->RemoveMemoryInfo(Value,__FILE__,__LINE__);Value=NULL;}
#define PMShowLeaks						CPMMemoryManager::Instance()->ShowMemoryLeak()
#define PMFreeLeaks						CPMMemoryManager::Instance()->ClearMemoryInfo()
#else
#define PMCheckSpecMemoryLive(TYPE,ShowMsgBox)		TRUE	
#define PMAddSpecMemory(TYPE)			TRUE
#define PMNewCArray(TYPE)				new CArray<TYPE,TYPE&>
#define PMNewAcArray(TYPE)				new AcArray<TYPE>
#define PMNew(TYPE)						new TYPE
#define PMNew_Parm(TYPE,Parm)			new TYPE Parm
#define PMRegMemory(TYPE)
#define PMUnregMemory(TYPE)			
#define PMDelete(TYPE)					{delete TYPE;(TYPE)=NULL;}
#define PMDeleteVoid(TYPE,Value)		{delete (TYPE)Value;Value=NULL;}
#define PMDeletePBYTE(Value)			{delete [](BYTE*)Value;Value=NULL;}
#define PMShowLeaks		
#define PMFreeLeaks		
#endif

