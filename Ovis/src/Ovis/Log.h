#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Ovis {

	class OVIS_API Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#define OV_CORE_TRACE(...) ::Ovis::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OV_CORE_INFO(...) ::Ovis::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OV_CORE_WARN(...) ::Ovis::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OV_CORE_ERROR(...) ::Ovis::Log::GetCoreLogger()->error(__VA_ARGS__)
// #define OV_CORE_FATAL(...) ::Ovis::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define OV_TRACE(...) ::Ovis::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OV_INFO(...) ::Ovis::Log::GetClientLogger()->info(__VA_ARGS__)
#define OV_WARN(...) ::Ovis::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OV_ERROR(...) ::Ovis::Log::GetClientLogger()->error(__VA_ARGS__)
// #define OV_CLIENT_FATAL(...) ::Ovis::Log::GetClientLogger()->fatal(__VA_ARGS__)