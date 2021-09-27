#include "pch.h"
#include "Engine.h"
#include "../utils/Log.h"

#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"

std::vector<Entity> EntityList{100};

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
	UISys.GrabWindow(GraphicsSys.pWindow);

	if (!UISys.Init()) LOG_ERROR("UI System failed to init.");

	if (!InputSys.Init(GraphicsSys.pWindow)) LOG_ERROR("Input System failed to init.");

	Framerate = std::make_shared<FramerateController>();
	Framerate->Init(60);



	//--------------------------------------------------------------------
	// These datas are not suppose to be inside Engine.cpp
	// It will moved once scene system is done
	// 
	// pokemon ball entity
	Entity pokemonBall = 0;
	EntityList.push_back(pokemonBall);

	// model component
	Model* pokemonBallModel = new Model("Assets/models/PokemonBall/model.obj");
	ModelComponentPool.Add(pokemonBall, (pokemonBallModel));

	// Transform component
	VQS* pokemonBallTransform = new VQS(glm::vec3(0.0), 0.01f);
	TransformComponentPool.Add(pokemonBall, (pokemonBallTransform));

	// Material component
	Texture* pokemonBallDiffuse = new Texture("Assets/models/PokemonBall/albedo.jpg");
	Material* pokemonBallMat = new Material(pokemonBallDiffuse);
	MaterialComponentPool.Add(pokemonBall, (pokemonBallMat));

	//--------------------------------------------------------------------------------

	// lion entity
	Entity lion = 1;
	EntityList.push_back(lion);

	// model component
	Model* lionModel = new Model("Assets/models/Lion/model.obj");
	ModelComponentPool.Add(lion, (lionModel));

	// Transform component
	VQS* lionTransform = new VQS(glm::vec3(3.0, -2.0, 1.0), 2.0f);
	TransformComponentPool.Add(lion, (lionTransform));

	// Material component
	Texture* lionDiffuse = new Texture("Assets/models/Lion/albedo.jpg");
	Material* lionMat = new Material(lionDiffuse);
	MaterialComponentPool.Add(lion, (lionMat));

	//---------------------------------------------------------------------


	LOG_INFO("Engine init.");
	return true;
}

// Game loop
void Engine::Run()
{
	Framerate->BeginTotal();
	
	while (!glfwWindowShouldClose(GraphicsSys.pWindow))
	{
		Framerate->StartFrame();
		InputSys.Update();

		// Game loop format
		// TODO: Profiler records time spent for each update()

		/*
		MemorySystem.Update();

		ResourceSystem.Update();

		EntitySystem.Update();

		float timeStamp = FrameRateSystem.GetTimeStamp();

			......

		PhysicsSystem.Update(timeStamp);

		*/


		// hard code timestamp to 0 for now
		GraphicsSys.Update(0);
		
		
		Framerate->EndFrame();
	}
}


void Engine::Destroy()
{
	// Destroy all systems in reverse order
	if (!UISys.Destroy()) LOG_ERROR("Graphics System failed to destory properly.");

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

inline float Engine::DeltaTime()
{
	return this->Framerate->DeltaSeconds();
}
