#ifndef COMMAND_H
#define COMMAND_H

class Command
{
public:

	void SetActionToExecute(std::function<void()> actionToExecute);
	void Execute();
	//more functionalities eg Redo, Undo can be implemented here

private:
	std::function<void()> actionToExecute;
};
#endif
