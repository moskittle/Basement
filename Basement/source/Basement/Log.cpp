#include "bmpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Basement {
	
	std::shared_ptr<spdlog::logger> Log::m_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::m_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		m_CoreLogger = spdlog::stdout_color_mt("BASEMENT");
		m_CoreLogger->set_level(spdlog::level::trace);

		m_ClientLogger = spdlog::stdout_color_mt("APPLICATION");
		m_ClientLogger->set_level(spdlog::level::trace);
	}

}