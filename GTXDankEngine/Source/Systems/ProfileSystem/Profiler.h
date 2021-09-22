#pragma once
#include "pch.h"

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





