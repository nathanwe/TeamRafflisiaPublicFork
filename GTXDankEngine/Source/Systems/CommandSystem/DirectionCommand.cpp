#include "pch.h"
#include "DirectionCommand.h"

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

void DirectionCommand::SetActionToExecute(std::function<void(MoveDirection)> actionToExecute)
{
	this->actionToExecute = actionToExecute;
}

void DirectionCommand::Execute(MoveDirection direction)
{
	this->actionToExecute(direction);
}
