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
	if (keyPressType == KeyPressType::Press 
		&& (engine.InputSys.IsKeyPressed(keyboardcode) 
			|| engine.InputSys.IsControllerPressed(0, gamepadCode)
			|| mouseLeftClick && engine.InputSys.IsLeftMousePressed()
			|| mouseRightClick && engine.InputSys.IsRightMousePressed()))
	{
		if (actionToExecute)
			this->actionToExecute();
		triggerEvent();
	}
	else if (keyPressType == KeyPressType::Trigger 
		&& (engine.InputSys.IsKeyTriggered(keyboardcode) 
			|| engine.InputSys.IsControllerTriggered(0, gamepadCode)
			|| mouseLeftClick && engine.InputSys.IsLeftMouseTriggered()
			|| mouseRightClick && engine.InputSys.IsRightMouseTriggered()))
	{
		if (actionToExecute)
			this->actionToExecute();
		triggerEvent();
	}
	else if (keyPressType == KeyPressType::Release 
		&& (engine.InputSys.IsKeyReleased(keyboardcode)
			|| engine.InputSys.IsControllerReleased(0, gamepadCode)
			|| mouseLeftClick && engine.InputSys.IsLeftMouseReleased()
			|| mouseRightClick && engine.InputSys.IsRightMouseReleased()))
	{
		if (actionToExecute)
			this->actionToExecute();
		triggerEvent();
	}
}

//event that send to lua script
void Command::triggerEvent()
{
	Event ev = Event();
	ev.type = EventType::COMMAND;
	ev.thingsToEffect.insert(GameLogicCategories::PLAYER);
	ev.floatData1 = engine.DeltaTime();
	ev.stringData1 = commandName;
	engine.DoGameLogicScriptSys.HandleEvent(ev);

}

