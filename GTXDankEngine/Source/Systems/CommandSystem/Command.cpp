#include "pch.h"
#include "../../Core/Engine.h"
#include "Command.h"

extern Engine engine;

void Command::SetActionToExecute(std::function<void()> actionToExecute)
{
	this->actionToExecute = actionToExecute;
}

void Command::Execute()
{
	if (actionToExecute)
	{
		if (keyPressType == KeyPressType::Press && (engine.InputSys.IsKeyPressed(keyboardcode) || engine.InputSys.IsControllerPressed(0, gamepadCode)))
		{
			this->actionToExecute();
		}
		else if (keyPressType == KeyPressType::Trigger && (engine.InputSys.IsKeyTriggered(keyboardcode) || engine.InputSys.IsControllerTriggered(0, gamepadCode)))
		{
			this->actionToExecute();
		}
		else if (keyPressType == KeyPressType::Release && (engine.InputSys.IsKeyReleased(keyboardcode) || engine.InputSys.IsControllerReleased(0, gamepadCode)))
		{
			this->actionToExecute();
		}
	}
}

