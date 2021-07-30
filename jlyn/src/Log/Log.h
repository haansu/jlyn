#pragma once

#include <JLynCore.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace jlyn {

	class Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}


#define CORE_FATAL(...)		::jlyn::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CORE_ERROR(...)		::jlyn::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_WARN(...)		::jlyn::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_INFO(...)		::jlyn::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_TRACE(...)		::jlyn::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define LOG_FATAL(...)		::jlyn::Log::GetClientLogger()->critical(__VA_ARGS__)
#define LOG_ERROR(...)		::jlyn::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)		::jlyn::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...)		::jlyn::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)		::jlyn::Log::GetClientLogger()->trace(__VA_ARGS__)