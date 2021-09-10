#include "Log.h"

std::shared_ptr<spdlog::logger> Log::mLogger;

std::shared_ptr<spdlog::logger>& Log::getLogger()
{
	return mLogger;
}


void Log::init(const std::string& name, const std::string& format)
{
	spdlog::set_pattern(format);
	mLogger = spdlog::stdout_color_mt(name);
	mLogger->set_level(spdlog::level::trace);
}



void Log::shutdown()
{
	mLogger.reset();
}