#include "Log.h"

namespace jlyn {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	// Initializes the logger
	void Log::Init() {
		spdlog::set_pattern("%^%d-%m-%C | %T - [%n]:\t%v%$");

		s_CoreLogger = spdlog::stdout_color_mt("Core");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("Client");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}