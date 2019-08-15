#pragma once

#include <memory>

#include "core.h"
#include "spdlog/spdlog.h"

namespace Basement {
	
	class BASEMENT_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macro
#define BM_CORE_ERROR(...)      ::Basement::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BM_CORE_WARN(...)       ::Basement::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BM_CORE_INFO(...)       ::Basement::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BM_CORE_TRACE(...)      ::Basement::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BM_CORE_CRITICAL(...)   ::Basement::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macro
#define BM_ERROR(...)           ::Basement::Log::GetClientLogger()->error(__VA_ARGS__)
#define BM_WARN(...)            ::Basement::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BM_INFO(...)            ::Basement::Log::GetClientLogger()->info(__VA_ARGS__)
#define BM_TRACE(...)           ::Basement::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BM_CRITICAL(...)        ::Basement::Log::GetClientLogger()->critical(__VA_ARGS__)