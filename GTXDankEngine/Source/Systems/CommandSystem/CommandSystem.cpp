#include "pch.h"
#include "CommandSystem.h"
#include "../../Core/Engine.h"
#include "../../Core/Camera.h"
#include "../GraphicsSystem/GraphicsSystem.h"

extern Engine engine;

bool CommandSystem::Init()
{
	engine.CommandSys.MoveCommand.SetActionToExecute([&](auto dir)
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
				engine.GraphicsSys.camera.Position += engine.GraphicsSys.camera.speed * quatOrientation;
			}
			if (dir == MoveDirection::LEFT)
			{
				ev.stringData1 = "Left";
				engine.GraphicsSys.camera.Position += engine.GraphicsSys.camera.speed * -glm::normalize(glm::cross(quatOrientation, engine.GraphicsSys.camera.Up));
			}
			if (dir == MoveDirection::DOWN)
			{
				ev.stringData1 = "Down";
				engine.GraphicsSys.camera.Position += engine.GraphicsSys.camera.speed * -quatOrientation;
			}
			if (dir == MoveDirection::RIGHT)
			{
				ev.stringData1 = "Right";
				engine.GraphicsSys.camera.Position += engine.GraphicsSys.camera.speed * glm::normalize(glm::cross(quatOrientation, engine.GraphicsSys.camera.Up));
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
	return true;
}

void CommandSystem::Update(float timeStamp)
{
	//toggle menu mode
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_ESCAPE))
	{
		menuMode = !menuMode;
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
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_W))
	{
		MoveCommand.Execute(MoveDirection::UP);
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_A))
	{
		MoveCommand.Execute(MoveDirection::LEFT);
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_S))
	{
		MoveCommand.Execute(MoveDirection::DOWN);
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_D))
	{
		MoveCommand.Execute(MoveDirection::RIGHT);
	}

	if (engine.InputSys.IsKeyPressed(GLFW_KEY_SPACE))
	{
		SpaceCommand.Execute();
	}

	if (engine.InputSys.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		ShiftCommand.Execute();
	}
	else
	{
		UnShiftCommand.Execute();
	}

	if (engine.InputSys.IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
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
}

void CommandSystem::ExecuteDebugCommands()
{
}
