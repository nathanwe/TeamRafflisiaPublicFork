#include "pch.h"
#include "Engine.h"
#include "../utils/Log.h"

#include "ResourceManager.h"

#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
//
#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"
//
//#include "../Components/RoutineComponent/RoutineComponent.h"
#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"
#include "../utils/common.h"

std::vector<Entity> EntityList;
std::string GAME_PATH;
ResourceManager<JsonFile> SerializationResourceManager;
ResourceManager<Model> ModelResourceManager;
ResourceManager<LuaFile> ScriptResourceManager;
ResourceManager<Texture> TextureResourceManger;

bool Engine::Init()
{
	// Format of Engine Init
	// TODO: Profiler will record how long it takes to init all system
	ScriptResourceManager.Init("Assets/Scripts/WhereTheGameAt.lua");
	if (!FindGameSys.Init("Assets/Scripts/WhereTheGameAt.lua")) LOG_ERROR("Game not Found");
	//preload resouces
	ScriptResourceManager.GetResourceHandle(GAME_PATH + std::string("Assets/Scripts/DoEverything.lua"));
	ScriptResourceManager.GetResourceHandle("Assets/Scripts/Menu.lua");
	ScriptResourceManager.GetResourceHandle(GAME_PATH + std::string("Assets/Scripts/DoLionThings.lua"));
	ScriptResourceManager.GetResourceHandle(GAME_PATH + std::string("Assets/Scripts/DoPokeballThings.lua"));
	ScriptResourceManager.GetResourceHandle(GAME_PATH + std::string("Assets/Scripts/DoLightThings.lua"));

	SerializationResourceManager.Init(GAME_PATH + std::string("Assets/Levels/GameObjects.json"));
	//preload resouces
	SerializationResourceManager.GetResourceHandle(GAME_PATH + std::string("Assets/Levels/GameObjects.json"));


	if (!GraphicsSys.Init()) LOG_ERROR("Graphics System failed to init.");

	ModelResourceManager.Init(GAME_PATH + std::string("Assets/models/PokemonBall/model.obj"));
	TextureResourceManger.Init(GAME_PATH + std::string("Assets/models/PokemonBall/albedo.jpg"));
	
	if (!EntitySys.Init()) LOG_ERROR("Entity System failed to init.");
	if (!AudioSys.Init()) LOG_ERROR("Audio System failed to init.");

	if (!CommandSys.Init()) LOG_ERROR("Command System failed to init.");

	
	
	if (!GameObjectFac.Init()) LOG_ERROR("Game Object Fac failed to init.");
	//
	if (!PhysicsSys.Init()) LOG_ERROR("Physics System failed to init.");
	//

	UISys.GrabWindow(GraphicsSys.pWindow);
	if (!UISys.Init()) LOG_ERROR("UI System failed to init.");
	if (!InputSys.Init(GraphicsSys.pWindow)) LOG_ERROR("Input System failed to init.");
	
	Framerate = std::make_shared<FramerateController>();
	Framerate->Init(60);

	

	

	//temp: for game object factory demo
	CommandSys.GetCommand("Attack3").SetActionToExecute([&]()
		{
			Entity newBall = GameObjectFac.CreateObject("POKEBALL");
			auto* trans = TransformComponentPool.GetComponentByEntity(newBall);
			trans->transform.position.x += rand() % 5;
			trans->transform.position.y += rand() % 5;
			trans->transform.position.z += rand() % 5;
		});

	//-----------------------------------------------------------------------
	if (!DoGameLogicScriptSys.Init(GAME_PATH + std::string("Assets/Scripts/DoEverything.lua"))) LOG_ERROR("Game Logic Script System failed to init.");
	if (!MenuSys.Init("Assets/Scripts/Menu.lua")) LOG_ERROR("Menu System failed to init.");


	if (!SceneSys.Init())  LOG_ERROR("Scene System failed to init.");

	
	

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

		PhysicsSys.Update(0);
		
		// Game loop format
		// TODO: Profiler records time spent for each update()

		//for (auto [entity, rComponent] : RoutineComponentPool.componentList)
		//{
		//	rComponent->list->Update(DeltaTime(), &InputSys);
		//}

		DoGameLogicScriptSys.Update(DeltaTime());
		//MenuSys.Update(DeltaTime());
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

		GraphicsSys.Update(DeltaTime());
		AudioSys.Update(0);
		
		EntitySys.Update(0);
		SceneSys.Update(0);

		Framerate->EndFrame();
	}
}


void Engine::Destroy()
{
	// Destroy all systems in reverse order
	//
	if (!PhysicsSys.Destroy()) LOG_ERROR("Physics System failed to destory properly.");
	//

	if (!CommandSys.Destroy()) LOG_ERROR("Command System failed to destory properly.");
	if (!UISys.Destroy()) LOG_ERROR("Graphics System failed to destory properly.");

	if (!GraphicsSys.Destroy()) LOG_ERROR("Graphics System failed to destory properly.");
	if (!AudioSys.Destroy())
	{
		LOG_ERROR("Audio System failed to destory properly.");
	}
	
	if (!DoGameLogicScriptSys.Destroy())
	{
		LOG_ERROR("GameLogic System failed to destory properly.");
	}
	if (!MenuSys.Destroy())
	{
		LOG_ERROR("Menu System failed to destory properly.");
	}

	ScriptResourceManager.Destroy();
	TextureResourceManger.Destroy();
	ModelResourceManager.Destroy();
	SerializationResourceManager.Destroy();
	
	
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

bool Engine::getMenuMode()
{
	return menuMode;
}

void Engine::setMenuMode(bool mode)
{
	menuMode = mode;
	if (menuMode || debugMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
}

bool Engine::getDebugMode()
{
	return debugMode;
}

void Engine::setDebugMode(bool mode)
{
	debugMode = mode;
	if (menuMode || debugMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
}
