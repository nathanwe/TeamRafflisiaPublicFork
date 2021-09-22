#include "pch.h"
#include "FramerateController.h"

using float_seconds = std::chrono::duration<float>;

FramerateController::FramerateController() :
	frameAverage(10, precision(0)),
	smoothedDeltaTime(precision(0)),
	deltaTime(precision(0))
{
}

void FramerateController::Init(std::uint32_t targetFramerate)
{
	this->targetFramerate = targetFramerate;
	auto frameTime = std::chrono::duration<double>(1) / targetFramerate;
	targetFrameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(frameTime);
}

void FramerateController::BeginTotal()
{
	beginTotal = timer.now();
}

void FramerateController::StartFrame()
{
	start = timer.now();
}

void FramerateController::EndFrame()
{
	Wait();

	end = timer.now();
	deltaTime = end - start;
	smoothedDeltaTime = frameAverage.Update(deltaTime);

	//LOG_INFO(FrameInfo());
}


void FramerateController::SetFramerate(std::uint32_t targetFramerate)
{
	this->targetFramerate = targetFramerate;
	auto frameTime = std::chrono::duration<double>(1) / targetFramerate;
	this->targetFrameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(frameTime);
}


std::chrono::nanoseconds FramerateController::DeltaTime() const
{
	return deltaTime;
}

std::chrono::nanoseconds FramerateController::SmoothedDeltaTime() const
{
	return smoothedDeltaTime;
}

float FramerateController::DeltaSeconds() const
{
	return std::chrono::duration_cast<float_seconds>(deltaTime).count();
}

float FramerateController::SmoothedDeltaSeconds() const
{
	return std::chrono::duration_cast<float_seconds>(smoothedDeltaTime).count();
}

float FramerateController::CurrentFramerate() const
{
	return 1 / DeltaSeconds();
}

float FramerateController::SmoothedCurrentFramerate() const
{
	return 1 / SmoothedDeltaSeconds();
}

std::chrono::nanoseconds FramerateController::CurrentFrameTime() const
{
	return std::chrono::high_resolution_clock::now() - start;
}

float FramerateController::CurrentFrameSeconds() const
{

	return std::chrono::duration_cast<float_seconds>(CurrentFrameTime()).count();
}

std::string FramerateController::FrameInfo() const
{
	return "delta: " + std::to_string(DeltaTime().count()) + "ns\n" +
		"smoothed framerate: " + std::to_string(SmoothedCurrentFramerate()) + "\n" +
		"current framerate: " + std::to_string(CurrentFramerate()) + "\n";
}

float FramerateController::TotalSeconds() const
{
	auto diff = start - beginTotal;
	return std::chrono::duration_cast<float_seconds>(diff).count();
}

void FramerateController::Wait()
{
	auto waitTime = targetFrameTime - CurrentFrameTime();

	auto startWait = std::chrono::high_resolution_clock::now();
	while (true)
	{
		auto now = std::chrono::high_resolution_clock::now();
		if (now - startWait > waitTime) 
			break;
	}
}
