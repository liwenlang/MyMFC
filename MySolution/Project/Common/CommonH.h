
#pragma once

#ifdef Common_Module
	#define CommonAPI __declspec(dllexport)
	
#else
	#define CommonAPI __declspec(dllimport)
	#pragma comment(lib, "Common.lib")
#endif

#include "Source/Excel.h"