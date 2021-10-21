#include "pch.h"
#include "CommandSystem.h"
#include "../../Core/Engine.h"

extern Engine engine;


bool CommandSystem::Init()
{
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
