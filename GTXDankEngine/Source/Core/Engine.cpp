#include "pch.h"
#include "Engine.h"
#include "../utils/Log.h"

std::vector<Entity> EntityList;

bool Engine::Init()
{
	// Format of Engine Init
	// TODO: Profiler will record how long it takes to init all system
	

	/*
	if (!MemorySystem.Init()) LOG_ERROR("Memory System failed to init.");

	if (!ResourceSystem.Init()) LOG_ERROR("Resource System failed to init.");

	if (!EntitySystem.Init()) LOG_ERROR("Entity System failed to init");

	if (!FrameRateSystem.Init()) LOG_ERROR("FrameRate System failed to init.")

	......

	if (!PhysicsSystem.Init()) LOG_ERROR("Physics System failed to init.");

	*/

	if (!GraphicsSys.Init()) LOG_ERROR("Graphics System failed to init.");

	LOG_INFO("Engine init.");
	return true;
}

// Game loop
void Engine::Run()
{
	while (!glfwWindowShouldClose(GraphicsSys.pWindow))
	{
		// Game loop format
		// TODO: Profiler records time spent for each update()

		/*
		MemorySystem.Update();

		ResourceSystem.Update();

		EntitySystem.Update();

		FrameRateSystem.Update();

		float timeStamp = FrameRateSystem.GetTimeStamp();

			......

		PhysicsSystem.Update(timeStamp);

		*/

		// hard code timestamp to 0 for now
		GraphicsSys.Update(0);
	}
}


void Engine::Destroy()
{
	// Destroy all systems in reverse order

	if (!GraphicsSys.Destroy()) LOG_ERROR("Graphics System failed to destory properly.");

	
	/*
	
	if (!PhysicsSystem.Destroy()) LOG_ERROR("Physics System failed to destory properly.");

	if (!FrameRateSystem.Destroy) LOG_ERROR("FrameRate System failed to destory properly.")

	......

	if (!EntitySystem.Destroy) LOG_ERROR("Entity system failed to desotyr properly");

	if (!ResourceSystem.Desotry()) LOG_ERROR("Resource System failed to destory properly.");

	if (!MemorySystem.Destroy()) LOG_ERROR("Memory System failed to destory properly.");

	*/

	Log::ShutDown();

}