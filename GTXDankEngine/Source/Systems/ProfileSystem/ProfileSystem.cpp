#include "pch.h"
#include "ProfileSystem.h"
#include "../Systems/GraphicsSystem/GraphicsSystem.h"
#include "../Core/FramerateControlSystem/RunningAverage.h"
#include "../Core/Engine.h"

extern Engine engine;
ProfileSystem ProfileSys;

bool ProfileSystem::Init()
{
	//This does nothing
	return true;
}

// used for changing Framerate UI
int targetFramerate = 60;

void ProfileSystem::Update(float timeStamp)
{
	//Defines the things to show in Imgui
	PROFILE_THIS("Profile Update");

	ImGui::Begin("ImGui Window");
	//every output on imgui is contained within this scope
	{

	ImGui::Text("average %2.2f FPS", engine.Framerate->CurrentFramerate());

	for (auto& result : profileResults)
	{
		char label[50];
		strcpy_s(label, result.name);
		strcat_s(label, " %4.3f microseconds");

		ImGui::Text(label, result.time);
	}

	ImGui::SliderInt("set target framerate", &targetFramerate, 10, 200);
	engine.Framerate->SetFramerate(targetFramerate);
	
	profileResults.clear();

	}
	ImGui::End();
}

bool ProfileSystem::Destroy()
{
	//This does nothing
	return false;
}

void ProfileSystem::AddResult(const char* resultName, double resultTime)
{
	//For everything that got tracked, create a struct of tracked time
	//and pushed into a vector of those struct

	ProfileResult newProRes;
	newProRes.name = resultName;
	newProRes.time = resultTime;

	profileResults.push_back(newProRes);
}





