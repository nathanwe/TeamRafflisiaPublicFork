#include "pch.h"
#include "Timer.h"
#include "../Systems/ProfileSystem/ProfileSystem.h"

extern ProfileSystem ProfileSys;


Timer::Timer(const char* name) :timerName(name), isTimerStopped(false)
{
	//Add a this on stack memory in a scope of any function
	//(simply add it as a local variable, but there's nothing to change in this)
	
	startTimePoint = std::chrono::high_resolution_clock::now();
}


Timer::~Timer()
{
	if (!isTimerStopped)
	{
		Stop();
	}
}


void Timer::Stop()
{
	auto endTimePoint = std::chrono::high_resolution_clock::now();

	long long start = std::chrono::time_point_cast<std::chrono::nanoseconds>(startTimePoint).time_since_epoch().count();
	long long end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimePoint).time_since_epoch().count();

	long long duration = end - start;
	double microsecond = duration * 0.001;

	ProfileSys.AddResult(timerName, microsecond);

	//std::cout << timerName << " uses " << microsecond << " microseconds\n";
}

void profile_this(const char* name) 
{
#ifdef _DEBUG
	Timer timer(name);
#endif // _DEBUG
}