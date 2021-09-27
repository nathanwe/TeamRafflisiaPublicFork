#pragma once

enum class MoveDirection
{
	NONE,
	UP,
	RIGHT,
	DOWN,
	LEFT,
};


class DirectionCommand
{
public:

	DirectionCommand()
	{
		//actionToExecute = [&](auto dir)
		//	{
		//		if (dir == MoveDirection::UP)
		//		{
		//		}
		//	};
	}

	void SetActionToExecute(std::function<void(MoveDirection)> actionToExecute);
	void Execute(MoveDirection direction);
	//more functionalities eg Redo, Undo can be implemented here

private:
	std::function<void(MoveDirection)> actionToExecute;
};

/*
#pragma once
#include "CommandReceiver.h"

enum class MoveDirection
{
	NONE,
	UP,
	RIGHT,
	DOWN,
	LEFT,
};

template<typename MovementFunctor>
class DirectionCommand
{
private:
	MovementFunctor actionToExecute;
public:

	void SetActionToExecute(MovementFunctor actionToExecute);
	{
		actionToExecute = actionToExecute;
	}
	void Execute(MoveDirection direction)
	{
		if (actionToExecute != nullptr)
			actionToExecute(direction);
	}
	//more functionalities eg Redo, Undo can be implemented here

};

//template<class MovementFunctor>
//inline void SetActionToExecute(MovementFunctor actionToExecute)
//{
//	actionToExecute = actionToExecute;
//}

*/