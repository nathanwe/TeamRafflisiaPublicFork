#ifndef PROFILER_H
#define PROFILER_H

class Profiler
{
public:
	
	Profiler();

	void UpdateLog();
	void Pause();
	void PrintStuff();

private:
	
	int frameRate;
	bool isActive;

};





#endif