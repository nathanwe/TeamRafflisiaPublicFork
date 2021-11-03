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
		this->actionToExecute(MoveDirection::UP, 1.0f);
	if(axisValue2 < -engine.InputSys.deadzone)
		this->actionToExecute(MoveDirection::UP, abs(axisValue2));

	if (engine.InputSys.IsKeyPressed(keyboardCode[1]))
		this->actionToExecute(MoveDirection::LEFT, 1.0f);
	if (axisValue < -engine.InputSys.deadzone)
		this->actionToExecute(MoveDirection::LEFT, abs(axisValue));

	if (engine.InputSys.IsKeyPressed(keyboardCode[2]))
		this->actionToExecute(MoveDirection::DOWN, 1.0f);
	if (axisValue2 > engine.InputSys.deadzone)
		this->actionToExecute(MoveDirection::DOWN, axisValue2);

	if (engine.InputSys.IsKeyPressed(keyboardCode[3]))
		this->actionToExecute(MoveDirection::RIGHT, 1.0f);
	if (axisValue > engine.InputSys.deadzone)
		this->actionToExecute(MoveDirection::RIGHT, axisValue);

}
