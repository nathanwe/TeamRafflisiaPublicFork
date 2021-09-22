#ifndef FRAMERATECONTROLLER_H
#define FRAMERATECONTROLLER_H

#include "pch.h"
#include "RunningAverage.h"
#include "../utils/Log.h"

using precision = std::chrono::nanoseconds;

class FramerateController
{
public:
	FramerateController(std::uint32_t targetFramerate);
	void BeginTotal();
	void StartFrame();
	void EndFrame();
	void SetFramerate(std::uint32_t targetFramerate);

	std::chrono::nanoseconds DeltaTime() const;
	std::chrono::nanoseconds SmoothedDeltaTime() const;
	float DeltaSeconds() const;
	float SmoothedDeltaSeconds() const;
	float CurrentFramerate() const;
	float SmoothedCurrentFramerate() const;
	std::chrono::nanoseconds CurrentFrameTime() const;
	float CurrentFrameSeconds() const;
	std::string FrameInfo() const;
	float TotalSeconds() const;

private:
	std::uint32_t targetFramerate;
	std::chrono::nanoseconds targetFrameTime;

	RunningAverage<std::chrono::nanoseconds> frameAverage;
	std::chrono::high_resolution_clock timer;
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end, beginTotal;

	std::chrono::nanoseconds deltaTime;
	std::chrono::nanoseconds smoothedDeltaTime;

	void Wait();
};

#endif