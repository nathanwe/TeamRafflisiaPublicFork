#pragma once

class Timer 
{
public:

	Timer(const char* name);
	~Timer();
	void Stop();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
	const char* timerName;
	bool isTimerStopped;

};