#ifndef LOG_H
#define LOG_H

#include "pch.h"


// How to use spdlog:
// 
// LOG_ERROR("Some error message with arg: {}", 1);
// LOG_WARN("Easy padding in numbers like {:08d}", 12)
// LOG_INFO("Support for floats {:03.2f}", 1.23456);
// LOG_TRACE("Some trace message with param {}", 42);
// 
// This github page below has more usage examples
// https://github.com/gabime/spdlog

class Log
{
public:
	static std::shared_ptr<spdlog::logger>& GetLogger();
	static void Init(const std::string& name = "GTXDank Engine", const std::string& format = "%^[%T] %n: %v%$");
	static void ShutDown();

private:
	static std::shared_ptr<spdlog::logger> mLogger;
};




#define LOG_ERROR(...)	Log::GetLogger()->error(__VA_ARGS__);
#define LOG_WARN(...)	Log::GetLogger()->warn(__VA_ARGS__);
#define LOG_INFO(...)	Log::GetLogger()->info(__VA_ARGS__);
#define LOG_TRACE(...)	Log::GetLogger()->trace(__VA_ARGS__);

#endif

