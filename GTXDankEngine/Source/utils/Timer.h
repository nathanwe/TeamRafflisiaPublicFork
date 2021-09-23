#ifndef TIMER_H
#define TIMER_H



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
#endif // !TIMER_H
