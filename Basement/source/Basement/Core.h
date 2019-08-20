#pragma once

#ifdef BM_PLATFORM_WINDOWS
	#ifdef BM_BUILD_DLL
		#define BASEMENT_API __declspec(dllexport)
	#else
		#define BASEMENT_API __declspec(dllimport)
	#endif // BM_BUILD_DLL
#else
	#error Basement Engine only supports Windows!
#endif	// BM_PLATFORM_WINDOWS

#ifdef BM_ENABLE_ASSERTS
	#define BM_ASSERT(x, ...) { if(!x) { BM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define BM_CORE_ASSERT(x, ...) { if(!x) { BM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define BM_ASSERT(x, ...)
	#define BM_CORE_ASSERT(x, ...)
#endif // BM_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define BM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)