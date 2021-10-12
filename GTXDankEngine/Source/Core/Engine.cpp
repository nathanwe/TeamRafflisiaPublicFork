#include "pch.h"
#include "Engine.h"
#include "../utils/Log.h"

#include "ResourceManager.h"

#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
//#include "../Components/RoutineComponent/RoutineComponent.h"
#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"

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
	
	
	ScriptResourceManager.Init("Assets/Scripts/FirstScript.lua");
	
	if (!AudioSys.Init()) LOG_ERROR("Audio System failed to init.");

	if (!CommandSys.Init()) LOG_ERROR("Command System failed to init.");
	if (!GraphicsSys.Init()) LOG_ERROR("Graphics System failed to init.");
	UISys.GrabWindow(GraphicsSys.pWindow);

	if (!UISys.Init()) LOG_ERROR("UI System failed to init.");

	if (!InputSys.Init(GraphicsSys.pWindow)) LOG_ERROR("Input System failed to init.");
	

	Framerate = std::make_shared<FramerateController>();
	Framerate->Init(60);

	ModelResourceManager.Init("Assets/models/PokemonBall/model.obj");
	TextureResourceManger.Init("Assets/models/PokemonBall/albedo.jpg");


	//--------------------------------------------------------------------
	// These datas are not suppose to be inside Engine.cpp
	// It will moved once scene system is done


	Entity noSuchEntity = 0; // 0 means no entity
	EntityList.push_back(noSuchEntity);
	// pokemon ball entity
	Entity pokemonBall = 1;
	EntityList.push_back(pokemonBall);

	// model component

	ResourceHandle<Model>* pokemonBallModelHandle = ModelResourceManager.GetResourceHandleNoThread("Assets/models/PokemonBall/model.obj");
	ModelComponentPool.Add(pokemonBall, (pokemonBallModelHandle));

	// Transform component
	VQS* pokemonBallTransform = new VQS(glm::vec3(0.0), 0.01f);
	TransformComponentPool.Add(pokemonBall, (pokemonBallTransform));

	// Material component
	ResourceHandle<Texture>* pokemonBallDiffuse = TextureResourceManger.GetResourceHandle("Assets/models/PokemonBall/albedo.jpg");
	ResourceHandle<Texture>* pokemonBallNormal = TextureResourceManger.GetResourceHandle("Assets/models/PokemonBall/normal.jpg");
	ResourceHandle<Texture>* pokemonBallMetallic = TextureResourceManger.GetResourceHandle("Assets/models/PokemonBall/metallic.jpg");
	ResourceHandle<Texture>* pokemonBallRoughness = TextureResourceManger.GetResourceHandle("Assets/models/PokemonBall/roughness.jpg");

	Material* pokemonBallMat = new Material(
		pokemonBallDiffuse, pokemonBallMetallic, pokemonBallNormal, pokemonBallRoughness);

	MaterialComponentPool.Add(pokemonBall, (pokemonBallMat));

	//RoutineList* pokemonBallRoutines = new RoutineList(pokemonBall);
	//pokemonBallRoutines->addRoutine(TYPE_LEFTRIGHT);
	//RoutineComponentPool.Add(pokemonBall, (pokemonBallRoutines));
	GameLogicCategoryComponentPool.Add(pokemonBall, (std::vector<GameLogicCategories>({ GameLogicCategories::POKEBALL })));


	//--------------------------------------------------------------------------------
	
	// vase entity
	Entity vase = 2;
	EntityList.push_back(vase);

	// model component
	ResourceHandle<Model>* vaseModel = ModelResourceManager.GetResourceHandleNoThread("Assets/models/Vase/model.obj");
	ModelComponentPool.Add(vase, (vaseModel));

	// Transform component
	VQS* vaseTransform = new VQS(glm::vec3(2.0, -1.0, 4.0), 0.5f);
	TransformComponentPool.Add(vase, (vaseTransform));

	ResourceHandle<Texture>* vaseDiffuse = TextureResourceManger.GetResourceHandle("Assets/models/Vase/albedo.jpg");
	ResourceHandle<Texture>* vaseNormal = TextureResourceManger.GetResourceHandle("Assets/models/Vase/normal.jpg");
	ResourceHandle<Texture>* vaseMetallic = TextureResourceManger.GetResourceHandle("Assets/models/Vase/metallic.jpg");
	ResourceHandle<Texture>* vaseRoughness = TextureResourceManger.GetResourceHandle("Assets/models/Vase/roughness.jpg");


	Material* vaseMat = new Material(vaseDiffuse, vaseMetallic, vaseNormal, vaseRoughness);
	MaterialComponentPool.Add(vase, (vaseMat));
	GameLogicCategoryComponentPool.Add(vase, (std::vector<GameLogicCategories>({ GameLogicCategories::VASE })));
	
	//---------------------------------------------------------------------
	// Point light source
	Entity LightSource1 = 3;
	EntityList.push_back(LightSource1);

	// model component
	ResourceHandle<Model>* LightSource1Model = ModelResourceManager.GetResourceHandleNoThread("Assets/models/Sphere/model.obj");
	ModelComponentPool.Add(LightSource1, (LightSource1Model));

	// Transform component
	VQS* LightSource1Transform = new VQS(glm::vec3(1.5f), 0.008f);
	TransformComponentPool.Add(LightSource1, (LightSource1Transform));
	
	// Light source component
	// white
	Light* Light1 = new Light(LightType::Point, glm::vec3(1.0f), glm::vec3(10.0f));
	LightComponentPool.Add(LightSource1, (Light1));
	GameLogicCategoryComponentPool.Add(LightSource1, (std::vector<GameLogicCategories>({ GameLogicCategories::POINT_LIGHT_SOURCE })));


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
	GameLogicCategoryComponentPool.Add(LightSource2, (std::vector<GameLogicCategories>({ GameLogicCategories::POINT_LIGHT_SOURCE })));

	//--------------------------------------------------------------------------

	// lion entity
	Entity lion = 5;
	EntityList.push_back(lion);

	// model component
	ResourceHandle<Model>* lionModel = ModelResourceManager.GetResourceHandleNoThread("Assets/models/Lion/model.obj");
	ModelComponentPool.Add(lion, (lionModel));

	// Transform component
	VQS* lionTransform = new VQS(glm::vec3(-1.0, -1.0, 3.0), 1.0f);
	TransformComponentPool.Add(lion, (lionTransform));

	// Material component
	ResourceHandle<Texture>* lionDiffuse = TextureResourceManger.GetResourceHandle("Assets/models/Lion/albedo.jpg");
	ResourceHandle<Texture>* lionNormal = TextureResourceManger.GetResourceHandle("Assets/models/Lion/normal.jpg");
	ResourceHandle<Texture>* lionMetallic = TextureResourceManger.GetResourceHandle("Assets/models/Lion/metallic.jpg");
	ResourceHandle<Texture>* lionRoughness = TextureResourceManger.GetResourceHandle("Assets/models/Lion/roughness.jpg");

	Material* lionMat = new Material(lionDiffuse, lionMetallic, lionNormal, lionRoughness);
	MaterialComponentPool.Add(lion, (lionMat));
	GameLogicCategoryComponentPool.Add(lion, (std::vector<GameLogicCategories>({ GameLogicCategories::LION })));

	//-----------------------------------------------------------------------
	//Directional light source

	Entity DirLightSource = 6;
	EntityList.push_back(DirLightSource);

	// model component
	ModelComponentPool.Add(DirLightSource, (LightSource1Model));

	// Transform component
	VQS* DirLightSourceTransform = new VQS(glm::vec3(-20, 20, 20), 0.005f);
	TransformComponentPool.Add(DirLightSource, (DirLightSourceTransform));

	// Light source component
	// Red light
	// 4th argument is the target of directional light source
	// direciton: target - position
	Light* DirLight = new Light(LightType::Directional, glm::vec3(1.0f, 0.6f, 0.6), glm::vec3(5.0f), glm::vec3(0.0, -1.0, 0.0));
	LightComponentPool.Add(DirLightSource, (DirLight));
	GameLogicCategoryComponentPool.Add(DirLightSource, (std::vector<GameLogicCategories>({ GameLogicCategories::POINT_LIGHT_SOURCE })));

//-------------------------------------------------------------------------------

	if (!DoGameLogicScriptSys.Init("Assets/Scripts/DoEverything.lua")) LOG_ERROR("Game Logic Script System failed to init.");
	

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
		AudioSys.Update(0);
		// Game loop format
		// TODO: Profiler records time spent for each update()

		//for (auto [entity, rComponent] : RoutineComponentPool.componentList)
		//{
		//	rComponent->list->Update(DeltaTime(), &InputSys);
		//}

		DoGameLogicScriptSys.Update(DeltaTime());
		/*
		MemorySystem.Update();

		ResourceSystem.Update();

		EntitySystem.Update();

		float timeStamp = FrameRateSystem.GetTimeStamp();

			......

		PhysicsSystem.Update(timeStamp);

		*/
		
		
		// hard code timestamp to 0 for now
		//ScriptResourceManager.Update(0);
		ModelResourceManager.Update(0);
		TextureResourceManger.Update(0);

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
	if (!AudioSys.Destroy())
	{
		LOG_ERROR("Audio System failed to destory properly.");
	}
	
	if (!DoGameLogicScriptSys.Destroy())
	{
		LOG_ERROR("Test Script System failed to destory properly.");
	}

	ScriptResourceManager.Destroy();
	TextureResourceManger.Destroy();
	ModelResourceManager.Destroy();
	
	
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
