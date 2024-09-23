#pragma once

#ifdef OV_PLATFORM_WINDOWS
	#ifdef OV_BUILD_DLL
		#define OVIS_API __declspec(dllexport)
	#else
		#define  OVIS_API __declspec(dllimport)
	#endif // OV_BUILD_DLL
#else
	#error Ovis only supports windows
#endif // OV_PLATFORM_WINDOWS


#ifdef OV_ENABLE_ASSERTS
	#define OV_ASSERT(x, ...) {if(!x) { OV_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define OV_CORE_ASSERT(x, ...) {if(!x) { OV_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define OV_ASSERT(x, ...)
	#define OV_CORE_ASSERT(x, ...)
#endif
