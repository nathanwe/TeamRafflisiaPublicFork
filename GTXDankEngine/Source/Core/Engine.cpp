#include "pch.h"
#include "Engine.h"
#include "../utils/Log.h"

#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"

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
	
	

	if (!CommandSys.Init()) LOG_ERROR("Command System failed to init.");
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
	Entity noSuchEntity = 0; // 0 means no entity
	EntityList.push_back(noSuchEntity);
	// pokemon ball entity
	Entity pokemonBall = 1;
	EntityList.push_back(pokemonBall);

	// model component
	Model* pokemonBallModel = new Model("Assets/models/PokemonBall/model.obj");
	ModelComponentPool.Add(pokemonBall, (pokemonBallModel));

	// Transform component
	VQS* pokemonBallTransform = new VQS(glm::vec3(0.0), 0.01f);
	TransformComponentPool.Add(pokemonBall, (pokemonBallTransform));

	// Material component
	Texture* pokemonBallDiffuse = new Texture("Assets/models/PokemonBall/albedo.jpg");
	Texture* pokemonBallNormal = new Texture("Assets/models/PokemonBall/normal.jpg");
	Texture* pokemonBallMetallic = new Texture("Assets/models/PokemonBall/metallic.jpg");
	Texture* pokemonBallRoughness = new Texture("Assets/models/PokemonBall/roughness.jpg");

	Material* pokemonBallMat = new Material(
		pokemonBallDiffuse, pokemonBallMetallic, pokemonBallNormal, pokemonBallRoughness);

	MaterialComponentPool.Add(pokemonBall, (pokemonBallMat));

	//--------------------------------------------------------------------------------
	
	// lion entity
	Entity lion = 2;
	EntityList.push_back(lion);

	// model component
	Model* lionModel = new Model("Assets/models/Lion/model.obj");
	ModelComponentPool.Add(lion, (lionModel));

	// Transform component
	VQS* lionTransform = new VQS(glm::vec3(2.0, -2.0, 4.0), 2.0f);
	TransformComponentPool.Add(lion, (lionTransform));

	// Material component
	Texture* lionDiffuse = new Texture("Assets/models/Lion/albedo.jpg");
	Texture* lionNormal = new Texture("Assets/models/Lion/normal.jpg");
	Texture* lionMetallic = new Texture("Assets/models/Lion/metallic.jpg");
	Texture* lionRoughness = new Texture("Assets/models/Lion/roughness.jpg");

	Material* lionMat = new Material(lionDiffuse, lionMetallic, lionNormal, lionRoughness);
	MaterialComponentPool.Add(lion, (lionMat));
	
	//---------------------------------------------------------------------
	// Point light source
	Entity LightSource1 = 3;
	EntityList.push_back(LightSource1);

	// model component
	Model* LightSource1Model = new Model("Assets/models/Sphere/model.obj");
	ModelComponentPool.Add(LightSource1, (LightSource1Model));

	// Transform component
	VQS* LightSource1Transform = new VQS(glm::vec3(1.5f), 0.008f);
	TransformComponentPool.Add(LightSource1, (LightSource1Transform));

	// Light source component
	// white
	Light* Light1 = new Light(LightType::Point, glm::vec3(1.0f), glm::vec3(10.0f));
	LightComponentPool.Add(LightSource1, (Light1));

	//--------------------------------------------------------------------------
	// Point light source
	Entity LightSource2 = 4;
	EntityList.push_back(LightSource2);

	// model component
	ModelComponentPool.Add(LightSource2, (LightSource1Model));

	// Transform component
	VQS* LightSource2Transform = new VQS(glm::vec3(3.5, 0.0, 4.0), 0.005f);
	TransformComponentPool.Add(LightSource2, (LightSource2Transform));

	// Light source component
	// yellow
	Light* Light2 = new Light(LightType::Point, glm::vec3(1.0f, 0.8f, 0.0), glm::vec3(5.0f));
	LightComponentPool.Add(LightSource2, (Light2));

	//--------------------------------------------------------------------------



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
		CommandSys.Update();

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
	if (!CommandSys.Destroy()) LOG_ERROR("Command System failed to destory properly.");
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
