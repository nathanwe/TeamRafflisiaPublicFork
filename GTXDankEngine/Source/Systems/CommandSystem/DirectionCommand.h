#ifndef DIRECTIONCOMMAND_H
#define DIRECTIONCOMMAND_H

#include "pch.h"
#include "../utils/SerializationHelper.h"

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

	DirectionCommand(){ }

	void SetKeyboardToExecute(int keycode[4]);
	void SetGamepadToExecute(int keycode[2]);
	void SetActionToExecute(std::function<void(MoveDirection, float)> actionToExecute);
	void Execute();
	//more functionalities eg Redo, Undo can be implemented here

public:
	//UP, lEFT, DOWN, RIGHT
	int keyboardCode[4];

	//vertical, horizontal
	int gamepadCode[2];
private:
	std::function<void(MoveDirection, float)> actionToExecute;
	void triggerEvent(std::string commandName);

};

inline void to_json(ordered_json& j, const DirectionCommand& command) {
	j["keyboardCode"] = command.keyboardCode;
	j["gamepadCode"] = command.gamepadCode;
}

inline void from_json(const ordered_json& j, DirectionCommand& command) {
	from_json(j["keyboardCode"], command.keyboardCode);
	from_json(j["gamepadCode"], command.gamepadCode);
}

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
#endif // !DIRECTIONCOMMAND_H
