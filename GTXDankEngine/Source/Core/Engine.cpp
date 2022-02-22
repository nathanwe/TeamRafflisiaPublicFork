#include "pch.h"
#include "Engine.h"
#include "../utils/Log.h"

#include "ResourceManager.h"

#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"
#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"
#include "../utils/common.h"

std::string GAME_PATH;
ResourceManager<JsonFile> SerializationResourceManager;
ResourceManager<Model> ModelResourceManager;
ResourceManager<LuaFile> ScriptResourceManager;
ResourceManager<Texture> TextureResourceManger;


bool Engine::Init()
{
	// Format of Engine Init
	// TODO: Profiler will record how long it takes to init all system
	ScriptResourceManager.Init("Assets/Defaults/DefaultScript.lua");
	if (!FindGameSys.Init("../WhereTheGameAt.lua")) LOG_ERROR("Game not Found");
	//preload resouces
	ScriptResourceManager.GetResourceHandle("Assets/Scripts/Menu.lua");

	SerializationResourceManager.Init(GAME_PATH + std::string("Assets/Levels/GameObjects.json"));


	if (!CommandSys.Init()) LOG_ERROR("Command System failed to init.");
	if (!GraphicsSys.Init()) LOG_ERROR("Graphics System failed to init.");

	ModelResourceManager.Init(std::string("Assets/Defaults/Sphere.obj"));
	TextureResourceManger.Init(std::string("Assets/Defaults/BlackPurpleChecker.png"));
	
	if (!EntitySys.Init()) LOG_ERROR("Entity System failed to init.");
	if (!AudioSys.Init()) LOG_ERROR("Audio System failed to init.");

	if (!GameObjectFac.Init()) LOG_ERROR("Game Object Fac failed to init.");
	
	if (!PhysicsSys.Init()) LOG_ERROR("Physics System failed to init.");
	

	UISys.GrabWindow(GraphicsSys.pWindow);
	if (!UISys.Init()) LOG_ERROR("UI System failed to init.");
	if (!InputSys.Init(GraphicsSys.pWindow)) LOG_ERROR("Input System failed to init.");
	if (!LevelEditorSys.Init()) LOG_ERROR("LevelEditorSys failed to init.");

	Framerate = std::make_shared<FramerateController>();
	Framerate->Init(60);

	//-----------------------------------------------------------------------
	if (!DoGameLogicScriptSys.Init(std::string("Assets/Scripts/DoEverything.lua"))) LOG_ERROR("Game Logic Script System failed to init.");
	if (!MenuSys.Init("Assets/Scripts/Menu.lua")) LOG_ERROR("Menu System failed to init.");
	if (!CameraControlSys.Init(GAME_PATH + std::string("Assets/Scripts/Camera.lua"))) LOG_ERROR("Camera Control System failed to init.");


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
		//---------------------------------------
		// imGUI new frame create
		// DO NOT CHANGE THIS CODE
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if(!(menuMode || debugMode || editMode || pauseMenuMode))
			ImGui::SetMouseCursor(ImGuiMouseCursor_None);
		//----------------------------------------

		Framerate->StartFrame();

		
		
		InputSys.Update();
		CommandSys.Update();
		
		if (!pause)
		{
			accumulatedFrameDt += DeltaTime();
			float dt = Framerate->GetTargetFrameTime();
			while (accumulatedFrameDt > 0)
			{
				accumulatedFrameDt -= dt;

				EntitySys.Update(dt);
				SceneSys.Update(dt);
				PhysicsSys.Update(dt);
				DoGameLogicScriptSys.Update(dt);
				//update physics after lua changes stuff
				PhysicsSys.UpdatePosition();
				PhysicsSys.UpdateColliders();

				CameraControlSys.Update(dt);
			}
		}
		
		// hard code timestamp to 0 for now
		//ScriptResourceManager.Update(0);
		ModelResourceManager.Update(0);
		TextureResourceManger.Update(0);
		
		AudioSys.Update(0);
		
		GraphicsSys.Update(DeltaTime());
		UISys.Update(DeltaTime());

		if (editMode)
		{
			LevelEditorSys.Update(DeltaTime());
		}

		//---------------------------------------------------------
		// imGUI UI render
		// renders at the end of game loop
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//---------------------------------------------------------

		//occurs after IMGUI so it can get data set by imgui
		Event ev = Event(true);
		ev.type = EventType::LOAD_DATA_FROM_IMGUI;
		DoGameLogicScriptSys.HandleEvent(ev);

		// swap buffer
		glfwSwapBuffers(GraphicsSys.pWindow);
		glfwPollEvents();

		Framerate->EndFrame();
	}
}


void Engine::Destroy()
{
	// Destroy all systems in reverse order
	//
	if (!PhysicsSys.Destroy()) LOG_ERROR("Physics System failed to destroy properly.");
	//

	if (!CommandSys.Destroy()) LOG_ERROR("Command System failed to destroy properly.");
	if (!UISys.Destroy()) LOG_ERROR("Graphics System failed to destroy properly.");

	if (!GraphicsSys.Destroy()) LOG_ERROR("Graphics System failed to destroy properly.");
	if (!AudioSys.Destroy())
	{
		LOG_ERROR("Audio System failed to destroy properly.");
	}
	
	if (!DoGameLogicScriptSys.Destroy())
	{
		LOG_ERROR("GameLogic System failed to destroy properly.");
	}
	if (!MenuSys.Destroy())
	{
		LOG_ERROR("Menu System failed to destroy properly.");
	}
	if (!CameraControlSys.Destroy())
	{
		LOG_ERROR("Camera System failed to destroy properly.");
	}
	if (!LevelEditorSys.Destroy())
	{
		LOG_ERROR("Level Editor System failed to destroy properly.");
	}

	ScriptResourceManager.Destroy();
	TextureResourceManger.Destroy();
	ModelResourceManager.Destroy();
	SerializationResourceManager.Destroy();

	
	/*
	
	if (!PhysicsSystem.Destroy()) LOG_ERROR("Physics System failed to destroy properly.");

	if (!FrameRateSystem.Destroy) LOG_ERROR("FrameRate System failed to destroy properly.")

	......

	if (!EntitySystem.Destroy) LOG_ERROR("Entity system failed to destroy properly");

	if (!ResourceSystem.Desotry()) LOG_ERROR("Resource System failed to destroy properly.");

	if (!MemorySystem.Destroy()) LOG_ERROR("Memory System failed to destroy properly.");

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
	pause = menuMode || editMode || pauseMenuMode;
	ToggleCursor();
}

bool Engine::getEditMode()
{
	return editMode;
}

void Engine::setEditMode(bool mode)
{
	editMode = mode;
	pause = menuMode || editMode || pauseMenuMode;
	ToggleCursor();
}

bool Engine::getPauseMenuMode()
{
	return pauseMenuMode;
}

void Engine::setPauseMenuMode(bool mode)
{
	pauseMenuMode = mode;
	pause = menuMode || editMode || pauseMenuMode;
	ToggleCursor();
}

bool Engine::getDebugMode()
{
	return debugMode;
}

void Engine::setDebugMode(bool mode)
{
	debugMode = mode;
	ToggleCursor();
}

void Engine::ToggleCursor()
{
	if (menuMode || debugMode || editMode || pauseMenuMode)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
}
