#include "pch.h"
#include "CommandSystem.h"
#include "../../Core/Engine.h"
#include "../../Core/Camera.h"
#include "../GraphicsSystem/GraphicsSystem.h"
#include "../ProfileSystem/ProfileSystem.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../PhysicsSystem/Raycasting/Raycasting.h"

extern Engine engine;


glm::vec3 mouse_click_callback(int width, int height, int mouse_x, int mouse_y)
{
	float x = (2.0f * mouse_x) / width - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / height;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);
	// NDC
	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	// back in view
	glm::vec4 ray_eye = glm::inverse(engine.GraphicsSys.camera.projectionMatrix) * ray_clip;
	// back to world
	glm::vec3 ray_wor = glm::vec3((glm::inverse(engine.GraphicsSys.camera.viewMatrix) * ray_eye));
	ray_wor = glm::normalize(ray_wor);
	return ray_wor;
}

void SendLionEmitEvent()
{
	Event ev;
	ev.runPerEntity = true;
	ev.e1 = 1;
	ev.type = EventType::EMIT_LION;
	ev.thingsToEffect.insert(GameLogicCategories::PLAYER);
	engine.DoGameLogicScriptSys.HandleEvent(ev);
}
void SendMenuToggleEvent()
{
	Event ev;
	ev.type = EventType::TOGGLE_MENU;
	engine.MenuSys.HandleEvent(ev);
}
void SendLionDeleteEvent()
{
	Event ev;
	ev.runPerEntity = true;
	ev.type = EventType::DESTROY_LIONS;
	ev.thingsToEffect.insert(GameLogicCategories::ENEMY);
	engine.DoGameLogicScriptSys.HandleEvent(ev);
}
bool CommandSystem::Init()
{
	auto* handle = SerializationResourceManager.GetResourceHandleNoThread(GAME_PATH + std::string("Assets/Configs/commands.json"));

	ordered_json commandJson = handle->GetPointer()->data;
	setting = commandJson;

	//special command: non-configurable
	toggleMenuCommand.keyboardcode = GLFW_KEY_ESCAPE;
	toggleMenuCommand.gamepadCode = 7;
	toggleMenuCommand.keyPressType = KeyPressType::Press;

	//all below control functions should be called at script side
		setting.directionCommand.SetActionToExecute([&](auto dir, float scale)
			{
				Event ev = Event();
				ev.type = EventType::MOVE_POKEBALL;
				ev.runPerEntity = true;
				ev.thingsToEffect.insert(GameLogicCategories::PLAYER);
				ev.floatData1 = engine.DeltaTime();
				glm::vec3 quatOrientation(sin(engine.GraphicsSys.camera.yaw) * cos(engine.GraphicsSys.camera.pitch), -sin(engine.GraphicsSys.camera.pitch), -cos(engine.GraphicsSys.camera.yaw) * cos(engine.GraphicsSys.camera.pitch));
		
				if (dir == MoveDirection::UP)
				{
					ev.stringData1 = "Up";
					engine.GraphicsSys.camera.Position += scale * engine.GraphicsSys.camera.speed * quatOrientation * engine.DeltaTime();
				}
				if (dir == MoveDirection::LEFT)
				{
					ev.stringData1 = "Left";
					engine.GraphicsSys.camera.Position += scale * engine.GraphicsSys.camera.speed * -glm::normalize(glm::cross(quatOrientation, engine.GraphicsSys.camera.Up)) * engine.DeltaTime();
				}
				if (dir == MoveDirection::DOWN)
				{
					ev.stringData1 = "Down";
					engine.GraphicsSys.camera.Position += scale * engine.GraphicsSys.camera.speed * -quatOrientation * engine.DeltaTime();
				}
				if (dir == MoveDirection::RIGHT)
				{
					ev.stringData1 = "Right";
					engine.GraphicsSys.camera.Position += scale * engine.GraphicsSys.camera.speed * glm::normalize(glm::cross(quatOrientation, engine.GraphicsSys.camera.Up)) * engine.DeltaTime();
				}
				engine.DoGameLogicScriptSys.HandleEvent(ev);
			});
		
		engine.CommandSys.GetCommand("Space").SetActionToExecute([&]()
			{
				Event ev = Event();
				ev.type = EventType::MOVE_POKEBALL;
				ev.thingsToEffect.insert(GameLogicCategories::PLAYER);
				ev.floatData1 = engine.DeltaTime();
				ev.stringData1 = "Space";
				engine.DoGameLogicScriptSys.HandleEvent(ev);
				engine.GraphicsSys.camera.Position += engine.GraphicsSys.camera.speed / 2.f * engine.GraphicsSys.camera.Up * engine.DeltaTime();
			});
		engine.CommandSys.GetCommand("Ctrl").SetActionToExecute([&]()
			{
				engine.GraphicsSys.camera.Position += engine.GraphicsSys.camera.speed / 2.f * -engine.GraphicsSys.camera.Up * engine.DeltaTime();
			});
		engine.CommandSys.GetCommand("Shift").SetActionToExecute([&]()
			{
				engine.GraphicsSys.camera.speed = 0.5f;
			});

		//engine.CommandSys.GetCommand("Skill1").SetActionToExecute(SendLionEmitEvent);
		engine.CommandSys.toggleMenuCommand.SetActionToExecute(SendMenuToggleEvent);
		//engine.CommandSys.GetCommand("Skill3").SetActionToExecute(SendLionDeleteEvent);

	return true;
}

void CommandSystem::Update(float timeStamp)
{
	Timer timer("Command Update");

	//toggle menu mode
	if (engine.InputSys.IsKeyTriggered(GLFW_KEY_ESCAPE) || engine.InputSys.IsControllerTriggered(0, 7))
	{
		menuMode = !menuMode;
		engine.setMenuMode(menuMode);

		if (!menuMode)
		{
			pendingKeyUpdate = false;
		}

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags &= ImGuiConfigFlags_NavEnableGamepad)
		{
			if (!menuMode)
			{
				io.ConfigFlags -= ImGuiConfigFlags_NavEnableGamepad;
				// io.ConfigFlags -= ImGuiConfigFlags_NavEnableKeyboard;
			}
		}
		else
		{
			if (menuMode)
			{
				io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
				// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
				ImGui::SetWindowFocus("Camera Configs");
			}
		}
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		toggleMenuCommand.Execute();
	}

	//waiting for control key update
	if (pendingKeyUpdate)
	{
		if (keyUpdateBuffer) 
		{
			keyUpdateBuffer = false;
		}
		else
		{
			if (engine.InputSys.IsLeftMouseTriggered())
			{
				pendingKeyUpdate = false;
				GetCommand(commandNamePendingKeyUpdate).mouseLeftClick = true;
				SerializeCommands();
			}
			else if (engine.InputSys.IsRightMouseTriggered())
			{
				pendingKeyUpdate = false;
				GetCommand(commandNamePendingKeyUpdate).mouseRightClick = true;
				SerializeCommands();
			}
			else
			{
				int key = engine.InputSys.IsAnyKeyTriggered();
				if (key != -1)
				{
					pendingKeyUpdate = false;
					GetCommand(commandNamePendingKeyUpdate).keyboardcode = key;
					SerializeCommands();
				}
				else
				{
					int key2 = engine.InputSys.IsAnyControllerTriggered();
					if (key2 != -1)
					{
						pendingKeyUpdate = false;
						GetCommand(commandNamePendingKeyUpdate).gamepadCode = key2;
						SerializeCommands();
					}
				}
			}
		}
	}

	if (engine.InputSys.IsKeyTriggered(GLFW_KEY_F1))
	{
		editMode = !editMode;
		engine.setEditMode(editMode);
	}
	if (editMode)
	{
		return;
	}

	//toggle debug mode
	if (engine.InputSys.IsKeyTriggered(GLFW_KEY_T))
	{
		debugMode = !debugMode;
		engine.setDebugMode(debugMode);
		GetCommand("DebugMode").Execute();
	}

	if (menuMode)
	{
		ExecuteUICommands();
		return;
	}

	ExecuteGameplayCommands();

	if (debugMode)
	{
		ExecuteDebugCommands();
	}
}

bool CommandSystem::Destroy()
{
	return true;
}

Command& CommandSystem::GetCommand(std::string name)
{
	auto command = setting.commands.find(name);
	if (command == setting.commands.end())
	{
		Command newCommand;
		setting.commands[name] = newCommand;
		return setting.commands[name];
		//assert("Command is not setup correcly.");
	}
	return command->second;
}

DirectionCommand& CommandSystem::GetDirectionCommand()
{
	return setting.directionCommand;
}

void CommandSystem::ShowCommandMenu()
{
	if (ImGui::Checkbox("Invert Mouse X", &(setting.mouseInvertedX)))
	{
		SerializeCommands();
	}
	if (ImGui::Checkbox("Invert Mouse Y", &(setting.mouseInvertedY)))
	{
		SerializeCommands();
	}
	if (ImGui::Checkbox("Invert Gamepad X", &(setting.gamepadInvertedX)))
	{
		SerializeCommands();
	}
	if (ImGui::Checkbox("Invert Gamepad Y", &(setting.gamepadInvertedY)))
	{
		SerializeCommands();
	}
	
	for (auto& [key, command] : setting.commands)
	{
		if (ImGui::Button(key.c_str(), ImVec2(100, 25)))
		{
			commandNamePendingKeyUpdate = key;
			pendingKeyUpdate = true;
			keyUpdateBuffer = true;
		}
	}

	if (pendingKeyUpdate)
	{
		ImGui::Begin("Please press new key");
		ImGui::End();
	}
}

void CommandSystem::SerializeCommands()
{
	ordered_json output = setting;
	std::ofstream outputStream(GAME_PATH + std::string("Assets/Configs/commands.json"));
	outputStream << output.dump(2);
}

void CommandSystem::LoadDefaultCommands()
{
	if (ImGui::Button("Reset Controls", ImVec2(100, 25)))
	{
		pendingKeyUpdate = false;
		auto* handle = SerializationResourceManager.GetResourceHandleNoThread(GAME_PATH + std::string("Assets/Configs/default.json"));
		ordered_json commandJson = handle->GetPointer()->data;
		PlayerSettings defaultSetting = commandJson;
		for (auto& [key, command] : setting.commands)
		{
			command.gamepadCode = defaultSetting.commands[key].gamepadCode;
			command.keyboardcode = defaultSetting.commands[key].keyboardcode;
			command.keyPressType = defaultSetting.commands[key].keyPressType;
		}
		SerializeCommands();
	}


}

void CommandSystem::ExecuteGameplayCommands()
{
	//command pattern applied here for every gameplay commands
	setting.directionCommand.Execute();
	for (auto& [key, command] : setting.commands)
	{
		command.Execute();
	}
}

void CommandSystem::ExecuteUICommands()
{
	/*
	ImGuiIO& io = ImGui::GetIO();

	if (menuMode && engine.InputSys.IsControllerTriggered(0, 1)) {
		menuMode = false;
		if (io.ConfigFlags &= ImGuiConfigFlags_NavEnableGamepad)
		{
			io.ConfigFlags -= ImGuiConfigFlags_NavEnableGamepad;
		}

		if (io.ConfigFlags &= ImGuiConfigFlags_NavEnableKeyboard)
		{
			io.ConfigFlags -= ImGuiConfigFlags_NavEnableKeyboard;
		}
	}
	*/

	/*
	if (engine.InputSys.IsControllerActive(0)) {
		if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad == false) {
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
			if (io.ConfigFlags &= ImGuiConfigFlags_NavEnableKeyboard) {
				io.ConfigFlags -= ImGuiConfigFlags_NavEnableKeyboard;
			}
		}
	}
	else {
		if (io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard == false) {
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			if (io.ConfigFlags &= ImGuiConfigFlags_NavEnableGamepad) {
				io.ConfigFlags -= ImGuiConfigFlags_NavEnableGamepad;
			}
		}
	}
	*/
}

void CommandSystem::ExecuteDebugCommands()
{
}


