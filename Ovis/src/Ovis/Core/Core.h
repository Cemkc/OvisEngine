#pragma once

#include "Ovis/Debug/Instrumentor.h"

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
	#define OV_ASSERT(x, ...) {if(!(x)) { OV_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define OV_CORE_ASSERT(x, ...) {if(!(x)) { OV_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define OV_ASSERT(x, ...)
	#define OV_CORE_ASSERT(x, ...)
#endif


//#define OV_PROFILE 1
#ifdef OV_PROFILE
#define OV_PROFILE_BEGIN_SESSION(name, filepath) ::Ovis::Instrumentor::Get().BeginSession(name, filepath)
#define OV_PROFILE_END_SESSION() ::Ovis::Instrumentor::Get().EndSession()
#define OV_PROFILE_SCOPE(name) ::Ovis::InstrumentationTimer timer##__LINE__(name)
#define OV_PROFILE_FUNC() OV_PROFILE_SCOPE(__FUNCSIG__)
#else
#define OV_PROFILE_BEGIN_SESSION(name, filepath)
#define OV_PROFILE_END_SESSION()
#define OV_PROFILE_SCOPE(name)
#define OV_PROFILE_FUNC()
#endif

//#define OV_RENDER_PROFILE 1
#ifdef OV_RENDER_PROFILE
#define OV_RENDER_PROFILE_SCOPE(name) ::Ovis::InstrumentationTimer timer##__LINE__(name)
#define OV_RENDER_PROFILE_FUNC() OV_PROFILE_SCOPE(__FUNCSIG__)
#else
#define OV_RENDER_PROFILE_SCOPE(name)
#define OV_RENDER_PROFILE_FUNC()
#endif
