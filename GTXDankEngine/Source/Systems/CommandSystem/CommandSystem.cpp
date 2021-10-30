#include "pch.h"
#include "CommandSystem.h"
#include "../../Core/Engine.h"
#include "../../Core/Camera.h"
#include "../GraphicsSystem/GraphicsSystem.h"

extern Engine engine;

bool CommandSystem::Init()
{
	if (!menuMode)
	{
		engine.CommandSys.MoveCommand.SetActionToExecute([&](auto dir)
			{
				Event ev = Event();
				ev.type = EventType::MOVE_POKEBALL;
				ev.runPerEntity = true;
				ev.thingsToEffect.insert(GameLogicCategories::POKEBALL);
				ev.floatData1 = engine.DeltaTime();
				glm::vec3 quatOrientation(sin(engine.GraphicsSys.camera.yaw) * cos(engine.GraphicsSys.camera.pitch), -sin(engine.GraphicsSys.camera.pitch), -cos(engine.GraphicsSys.camera.yaw) * cos(engine.GraphicsSys.camera.pitch));

				float scale = 1.f;
				if (dir == MoveDirection::UP)
				{
					ev.stringData1 = "Up";

					if (engine.InputSys.IsControllerActive(0)) {
						scale = -engine.InputSys.GetControllerAxis(0, 1);
					}

					engine.GraphicsSys.camera.Position += scale * engine.GraphicsSys.camera.speed * quatOrientation;
				}
				if (dir == MoveDirection::LEFT)
				{
					ev.stringData1 = "Left";

					if (engine.InputSys.IsControllerActive(0)) {
						scale = -engine.InputSys.GetControllerAxis(0, 0);
					}

					engine.GraphicsSys.camera.Position += scale * engine.GraphicsSys.camera.speed * -glm::normalize(glm::cross(quatOrientation, engine.GraphicsSys.camera.Up));
				}
				if (dir == MoveDirection::DOWN)
				{
					ev.stringData1 = "Down";

					if (engine.InputSys.IsControllerActive(0)) {
						scale = engine.InputSys.GetControllerAxis(0, 1);
					}

					engine.GraphicsSys.camera.Position += scale * engine.GraphicsSys.camera.speed * -quatOrientation;
				}
				if (dir == MoveDirection::RIGHT)
				{
					ev.stringData1 = "Right";

					if (engine.InputSys.IsControllerActive(0)) {
						scale = engine.InputSys.GetControllerAxis(0, 0);
					}

					engine.GraphicsSys.camera.Position += scale * engine.GraphicsSys.camera.speed * glm::normalize(glm::cross(quatOrientation, engine.GraphicsSys.camera.Up));
				}
				engine.DoGameLogicScriptSys.HandleEvent(ev);
			});

		engine.CommandSys.SpaceCommand.SetActionToExecute([&]()
			{
				engine.GraphicsSys.camera.Position += engine.GraphicsSys.camera.speed / 2.f * engine.GraphicsSys.camera.Up;
			});
		engine.CommandSys.CtrlCommand.SetActionToExecute([&]()
			{
				engine.GraphicsSys.camera.Position += engine.GraphicsSys.camera.speed / 2.f * -engine.GraphicsSys.camera.Up;
			});
		engine.CommandSys.ShiftCommand.SetActionToExecute([&]()
			{
				engine.GraphicsSys.camera.speed = 0.5f;
			});
		engine.CommandSys.UnShiftCommand.SetActionToExecute([&]()
			{
				engine.GraphicsSys.camera.speed = 0.1f;
			});
	}
	return true;
}

void CommandSystem::Update(float timeStamp)
{
	//toggle menu mode
	if (engine.InputSys.IsKeyTriggered(GLFW_KEY_ESCAPE) || engine.InputSys.IsControllerTriggered(0, 7))
	{
		menuMode = !menuMode;
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags &= ImGuiConfigFlags_NavEnableGamepad)
		{
			if (!menuMode)
			{
				io.ConfigFlags -= ImGuiConfigFlags_NavEnableGamepad;
				io.ConfigFlags -= ImGuiConfigFlags_NavEnableKeyboard;
			}
		}
		else
		{
			if (menuMode)
			{
				io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
				io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
				ImGui::SetWindowFocus("Camera Configs");
			}
		}
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ToggleMenuCommand.Execute();
	}

	//toggle debug mode
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_T))
	{
		debugMode = !debugMode;
		DebugModeCommand.Execute();
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

void CommandSystem::ExecuteGameplayCommands()
{
	//directional movements
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_W) || engine.InputSys.GetControllerAxis(0, 1) < -engine.InputSys.deadzone)
	{
		MoveCommand.Execute(MoveDirection::UP);
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_A) || engine.InputSys.GetControllerAxis(0, 0) < -engine.InputSys.deadzone)
	{
		MoveCommand.Execute(MoveDirection::LEFT);
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_S) || engine.InputSys.GetControllerAxis(0, 1) > engine.InputSys.deadzone)
	{
		MoveCommand.Execute(MoveDirection::DOWN);
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_D) || engine.InputSys.GetControllerAxis(0, 0) > engine.InputSys.deadzone)
	{
		MoveCommand.Execute(MoveDirection::RIGHT);
	}

	if (engine.InputSys.IsKeyPressed(GLFW_KEY_SPACE) || engine.InputSys.IsControllerPressed(0, 0))
	{
		SpaceCommand.Execute();
	}

	if (engine.InputSys.IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || engine.InputSys.GetControllerAxis(0, 5) > -1.f + engine.InputSys.deadzone)
	{
		ShiftCommand.Execute();
	}
	else
	{
		UnShiftCommand.Execute();
	}

	if (engine.InputSys.IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || engine.InputSys.IsControllerPressed(0, 1))
	{
		CtrlCommand.Execute();
	}

	//skill
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_1))
	{
		Skill1Command.Execute();
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_2))
	{
		Skill2Command.Execute();
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_3))
	{
		Skill3Command.Execute();
	}

	//attack
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_J))
	{
		Attack1Command.Execute();
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_K))
	{
		Attack2Command.Execute();
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_L))
	{
		Attack3Command.Execute();
	}

	if (engine.InputSys.IsKeyTriggered(GLFW_KEY_RIGHT))
	{
		NextLevelCommand.Execute();
	}
	if (engine.InputSys.IsKeyTriggered(GLFW_KEY_LEFT))
	{
		PreviousLevelCommand.Execute();
	}
}

void CommandSystem::ExecuteUICommands()
{
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
