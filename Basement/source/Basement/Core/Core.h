#pragma once

#include <memory>

#ifdef BM_PLATFORM_WINDOWS
	#if BM_DYNAMIC_LINK	
		#ifdef BM_BUILD_DLL
			#define BASEMENT_API __declspec(dllexport)
		#else
			#define BASEMENT_API __declspec(dllimport)
		#endif // BM_BUILD_DLL
	#else
		#define BASEMENT_API
	#endif	// BM_DYNAMIC_LINK
#else
	#error Basement Engine only supports Windows!
#endif	// BM_PLATFORM_WINDOWS

#ifdef BM_DEBUG
	#define BM_ENABLE_ASSERTS
	#define BM_ENABLE_OPENGL_DEBUG
#endif	//BM_DEBUG

#ifdef BM_ENABLE_ASSERTS
	#define BM_ASSERT(x, ...) { if(!x) { BM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define BM_CORE_ASSERT(x, ...) { if(!(x)) { BM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define BM_ASSERT(x, ...)
	#define BM_CORE_ASSERT(x, ...)
#endif // BM_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define BM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

// Pointer Wrappers
namespace Basement {

	template<typename T>
	using SharedPtr = std::shared_ptr<T>;

	template<typename T>
	using UniquePtr = std::unique_ptr<T>;

	using byte = unsigned char;

	// signed integer type
	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;

	// unsigned integer type
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;

	// floating point
	using f32 = float_t;
	using f64 = double_t;

}
