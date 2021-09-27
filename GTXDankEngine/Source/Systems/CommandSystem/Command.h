#pragma once

class Command
{
public:

	void SetActionToExecute(std::function<void()> actionToExecute);
	void Execute();
	//more functionalities eg Redo, Undo can be implemented here

private:
	std::function<void()> actionToExecute;
};

