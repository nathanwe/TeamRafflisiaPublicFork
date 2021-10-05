#pragma once

#include "Routine.h"
#include "LeftRight.h"


enum RoutineType
{
	TYPE_LEFTRIGHT,
	TYPE_UPDOWN
};


class RoutineList
{
public:
	RoutineList(uint32_t id) : id(id) {}

	std::vector<Routine*> routines;
	uint32_t id;

	void addRoutine(RoutineType type) {
		Routine* newRoutine;

		switch (type)
		{
		case TYPE_LEFTRIGHT:
			newRoutine = new LeftRight(id);
			break;
		default:
			break;
		}

		routines.push_back(newRoutine);
	}

	void Update(float deltaTime, InputManager *inputSys)
	{
		for (auto routine : routines) {
			routine->Update(deltaTime, inputSys);
		}
	}
};
