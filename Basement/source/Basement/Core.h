#pragma once

#ifdef BM_PLATFORM_WINDOWS
	#ifdef BM_BUILD_DLL
		#define BASEMENT_API __declspec(dllexport)
	#else
		#define BASEMENT_API __declspec(dllimport)
	#endif // BM_BUILD_DLL
#else
	#error Basement Engine only supports Windows!
#endif