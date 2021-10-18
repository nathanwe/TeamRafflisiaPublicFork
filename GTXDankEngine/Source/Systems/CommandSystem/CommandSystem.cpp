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
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_W) || engine.InputSys.GetControllerAxis(0, 1) <= -deadzone ) 
	{
		MoveCommand.Execute(MoveDirection::UP);
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_A) || engine.InputSys.GetControllerAxis(0, 0) <= -deadzone )
	{
		MoveCommand.Execute(MoveDirection::LEFT);
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_S) || engine.InputSys.GetControllerAxis(0, 1) >= deadzone )
	{
		MoveCommand.Execute(MoveDirection::DOWN);
	}
	if (engine.InputSys.IsKeyPressed(GLFW_KEY_D) || engine.InputSys.GetControllerAxis(0, 0) >= deadzone )
	{
		MoveCommand.Execute(MoveDirection::RIGHT);
	}

	if (engine.InputSys.IsKeyPressed(GLFW_KEY_SPACE) || engine.InputSys.IsControllerPressed(0, 0) )
	{
		SpaceCommand.Execute();
	}

	if (engine.InputSys.IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || engine.InputSys.GetControllerAxis(0, 5) >= deadzone )
	{
		ShiftCommand.Execute();
	}
	else
	{
		UnShiftCommand.Execute();
	}

	if (engine.InputSys.IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || engine.InputSys.IsControllerPressed(0, 1) )
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


}

void CommandSystem::ExecuteUICommands()
{
}

void CommandSystem::ExecuteDebugCommands()
{
}
