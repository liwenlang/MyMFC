// ==========================================================================
// 			Specification : header file for MFC DLL extension builds
// ==========================================================================

// Product Version: 7.51

//// This software along with its related components, documentation and files ("The Libraries")
// is ?1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#if !defined(PMBASECLASSOPRAPI) || !defined(OX_API_DECL) && !defined(OX_DATA_DECL)

#ifdef _BUILD_UTB_INTO_EXTDLL
#if !defined(WIN32) || !defined(_AFXEXT)
#pragma error("Wrong settings for UTB Extension DLL build")
#endif
#endif

#ifdef _LINK_TO_UTB_IN_EXTDLL
#if !defined(WIN32) || !defined(_AFXDLL)
#pragma error("Wrong settings for project that uses UTB Extension DLL")
#endif
#endif

#if defined(_LINK_TO_UTB_IN_EXTDLL) && defined(_BUILD_UTB_INTO_EXTDLL)
#pragma error("Error: both _BUILD_UTB_INTO_EXTDLL and _LINK_TO_UTB_IN_EXTDLL options has been specified")
#endif

// When including UTB classes into a MFC Extension DLL
#ifdef _BUILD_UTB_INTO_EXTDLL
	#ifndef PMBASECLASSOPRAPI
		#define PMBASECLASSOPRAPI		AFX_CLASS_EXPORT
	#endif
	#ifndef OX_API_DECL
		#define OX_API_DECL			AFX_API_EXPORT
	#endif
	#ifndef OX_DATA_DECL
		#define OX_DATA_DECL		AFX_DATA_EXPORT
	#endif
#elif defined(_LINK_TO_UTB_IN_EXTDLL)
// When linking to extension DLL that includes UTB classes 
	#ifndef PMBASECLASSOPRAPI
		#define PMBASECLASSOPRAPI		AFX_CLASS_IMPORT
	#endif
	#ifndef OX_API_DECL
		#define OX_API_DECL			AFX_API_IMPORT
	#endif
	#ifndef OX_DATA_DECL
		#define OX_DATA_DECL		AFX_DATA_IMPORT
	#endif
#else
	#ifndef PMBASECLASSOPRAPI
		#define PMBASECLASSOPRAPI	
	#endif
	#ifndef OX_API_DECL
		#define OX_API_DECL    
	#endif
	#ifndef OX_DATA_DECL
		#define OX_DATA_DECL    
	#endif
#endif

#endif	//	!defined(PMBASECLASSOPRAPI) || !defined(OX_API_DECL) && !defined(OX_DATA_DECL)
