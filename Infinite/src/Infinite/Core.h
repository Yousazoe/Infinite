#pragma once

#ifdef IFN_PLATFORM_WINDOWS
	#ifdef IFN_BUILD_DLL
		#define INFINITE_API _declspec(dllexport)
	#else
		#define INFINITE_API _declspec(dllimport)
	#endif
#else
	#error Infinite only support Windows
#endif
