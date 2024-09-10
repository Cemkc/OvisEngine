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
