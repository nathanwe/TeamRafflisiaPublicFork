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

void profile_this(const char* name);
#define PROFILE_THIS(name) profile_this(name)


#endif // !TIMER_H
