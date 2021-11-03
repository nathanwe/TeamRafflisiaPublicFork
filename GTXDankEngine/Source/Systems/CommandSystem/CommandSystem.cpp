#include "pch.h"
#include "CommandSystem.h"
#include "../../Core/Engine.h"
#include "../../Core/Camera.h"
#include "../GraphicsSystem/GraphicsSystem.h"
#include "../ProfileSystem/ProfileSystem.h"

extern Engine engine;

bool CommandSystem::Init()
{
	auto* handle = SerializationResourceManager.GetResourceHandleNoThread("Assets/Configs/commands.json");
	ordered_json commandJson = handle->GetPointer()->data;
	setting = commandJson;

	//special command: non-configurable
	toggleMenuCommand.keyboardcode = GLFW_KEY_ESCAPE;
	toggleMenuCommand.gamepadCode = 7;
	toggleMenuCommand.keyPressType = KeyPressType::Press;

		setting.directionCommand.SetActionToExecute([&](auto dir, float scale)
			{
				Event ev = Event();
				ev.type = EventType::MOVE_POKEBALL;
				ev.runPerEntity = true;
				ev.thingsToEffect.insert(GameLogicCategories::POKEBALL);
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
		assert("Command is not setup correcly.");
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
	std::ofstream outputStream("Assets/Configs/commands.json");
	outputStream << output.dump(2);
}

void CommandSystem::LoadDefaultCommands()
{
	if (ImGui::Button("Reset Controls", ImVec2(100, 25)))
	{
		pendingKeyUpdate = false;
		auto* handle = SerializationResourceManager.GetResourceHandleNoThread("Assets/Configs/default.json");
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


