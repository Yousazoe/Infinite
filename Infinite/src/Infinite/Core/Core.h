#pragma once

#include <memory>

#ifdef IFN_PLATFORM_WINDOWS
#if IFN_DYNAMIC_LINK
	#ifdef IFN_BUILD_DLL
		#define INFINITE_API _declspec(dllexport)
	#else
		#define INFINITE_API _declspec(dllimport)
	#endif
#else
    #define INFINITE_API
#endif
#else
	#error Infinite only support Windows
#endif

#ifdef IFN_ENABLE_ASSERTS
    #define IFN_ASSERT(x, ...) { if(!(x)) { IFN_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define IFN_CORE_ASSERT(x, ...) { if(!(x)) { IFN_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define IFN_ASSERT(x, ...)
    #define IFN_CORE_ASSERT(x, ...)
#endif

#ifdef IFN_DEBUG
	#define IFN_ENABLE_ASSERTS
#endif

#define BIT(x) (1 << (x))

#define IFN_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Infinite {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}