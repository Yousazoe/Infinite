#pragma once

#include "Infinite/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Infinite {
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros
#define IFN_CORE_CRITICAL(...)	::Infinite::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define IFN_CORE_ERROR(...)		::Infinite::Log::GetCoreLogger()->error(__VA_ARGS__)
#define IFN_CORE_WARN(...)		::Infinite::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define IFN_CORE_INFO(...)			::Infinite::Log::GetCoreLogger()->info(__VA_ARGS__)
#define IFN_CORE_TRACE(...)		::Infinite::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client log macros
#define IFN_CRITICAL(...)				::Infinite::Log::GetClientLogger()->critical(__VA_ARGS__)
#define IFN_ERROR(...)					::Infinite::Log::GetClientLogger()->error(__VA_ARGS__)
#define IFN_WARN(...)					::Infinite::Log::GetClientLogger()->warn(__VA_ARGS__)
#define IFN_INFO(...)					::Infinite::Log::GetClientLogger()->info(__VA_ARGS__)
#define IFN_TRACE(...)					::Infinite::Log::GetClientLogger()->trace(__VA_ARGS__)