#include "pch.h"
#include "DirectionCommand.h"
#include "../../Core/Engine.h"

//void DirectionCommand::SetActionToExecute(MovementFunctor actionToExecute)
////void DirectionCommand::SetActionToExecute(void (*function)(MoveDirection))
//{
//}

//void DirectionCommand::Execute(MoveDirection direction)
//{
//	if(actionToExecute != nullptr)
//		actionToExecute(direction);
//
//	//SetActionToExecute([](auto dir)
//	//		{
//	//			if (dir == MoveDirection::UP)
//	//			{
//	//				Position += 10 * speed * Orientation;
//	//			}
//	//		});
//
//	std::vector<int> nums;
//	std::for_each(nums.begin(), nums.end(), DirectionCommand::Execute);
//
//	//auto test = [](DirectionCommand) {};
//	//SetActionToExecute(Execute);
//}

extern Engine engine;

void DirectionCommand::SetKeyboardToExecute(int keycode[4])
{
	keyboardCode[0] = keycode[0];
	keyboardCode[1] = keycode[1];
	keyboardCode[2] = keycode[2];
	keyboardCode[3] = keycode[3];
}

void DirectionCommand::SetGamepadToExecute(int keycode[2])
{
	gamepadCode[0] = keycode[0];
	gamepadCode[1] = keycode[1];
}

void DirectionCommand::SetActionToExecute(std::function<void(MoveDirection, float)> actionToExecute)
{
	this->actionToExecute = actionToExecute;
}

void DirectionCommand::Execute()
{
	float axisValue = engine.InputSys.GetControllerAxis(0, gamepadCode[0]);
	float axisValue2 = engine.InputSys.GetControllerAxis(0, gamepadCode[1]);

	if (engine.InputSys.IsKeyPressed(keyboardCode[0]))
	{
		if (actionToExecute)
			this->actionToExecute(MoveDirection::UP, 1.0f);
		triggerEvent("Up");
	}
	if (axisValue2 < -engine.InputSys.deadzone)
	{
		if (actionToExecute)
			this->actionToExecute(MoveDirection::UP, abs(axisValue2));
		triggerEvent("Up");
	}

	if (engine.InputSys.IsKeyPressed(keyboardCode[1]))
	{
		if (actionToExecute)
			this->actionToExecute(MoveDirection::LEFT, 1.0f);
		triggerEvent("Left");
	}
	if (axisValue < -engine.InputSys.deadzone)
	{
		if (actionToExecute)
			this->actionToExecute(MoveDirection::LEFT, abs(axisValue));
		triggerEvent("Left");
	}

	if (engine.InputSys.IsKeyPressed(keyboardCode[2]))
	{
		if (actionToExecute)
			this->actionToExecute(MoveDirection::DOWN, 1.0f);
		triggerEvent("Down");
	}
	if (axisValue2 > engine.InputSys.deadzone)
	{
		if (actionToExecute)
			this->actionToExecute(MoveDirection::DOWN, axisValue2);
		triggerEvent("Down");
	}

	if (engine.InputSys.IsKeyPressed(keyboardCode[3]))
	{
		if (actionToExecute)
			this->actionToExecute(MoveDirection::RIGHT, 1.0f);
		triggerEvent("Right");
	}
	if (axisValue > engine.InputSys.deadzone)
	{
		if (actionToExecute)
			this->actionToExecute(MoveDirection::RIGHT, axisValue);
		triggerEvent("Right");
	}

}

//event that send to lua script
void DirectionCommand::triggerEvent(std::string commandName)
{
	Event ev = Event();
	ev.type = EventType::COMMAND;
	ev.thingsToEffect.insert(GameLogicCategories::PLAYER);
	ev.floatData1 = engine.DeltaTime();
	ev.stringData1 = commandName;
	engine.DoGameLogicScriptSys.HandleEvent(ev);

}

