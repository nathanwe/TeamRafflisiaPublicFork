#include "pch.h"
#include "Command.h"

void Command::SetActionToExecute(std::function<void()> actionToExecute)
{
	this->actionToExecute = actionToExecute;
}

void Command::Execute()
{
	if (actionToExecute)
		this->actionToExecute();
}

