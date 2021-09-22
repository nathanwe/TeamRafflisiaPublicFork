#include "pch.h"
#include "Profiler.h"

Profiler::Profiler() 
{
	frameRate = 60;
}

void Profiler::UpdateLog()
{
	//take FPS from frame rate controller
}
void Profiler::Pause()
{
	
}

void Profiler::PrintStuff() 
{
	frameRate = engine.Framerate->CurrentFramerate();
	ImGui::LogText("%d", frameRate);
}




