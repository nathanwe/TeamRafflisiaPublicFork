#ifndef COMMANDSYSTEM_H
#define COMMANDSYSTEM_H

#include "pch.h"
#include "../../Core/System.h"
#include "Command.h"
#include "DirectionCommand.h"
#include "PlayerSettings.h"

class CommandSystem : public System
{
public:
	virtual bool Init() override;
	virtual void Update(float timeStamp = 0) override;
	virtual bool Destroy() override;

	Command& GetCommand(std::string name);
	DirectionCommand& GetDirectionCommand();

	void ShowCommandMenu();

	void SerializeCommands();
	void LoadDefaultCommands();

public:
	PlayerSettings setting;
	Command toggleMenuCommand;

	bool menuMode;
	bool debugMode;
	bool editMode;

private:
	void ExecuteGameplayCommands();
	void ExecuteUICommands();
	void ExecuteDebugCommands();

private:
	//DirectionCommand* moveCommand;
	//std::unordered_map<std::string, Command*> commands;
	std::string commandNamePendingKeyUpdate;
	bool pendingKeyUpdate = false;
	bool keyUpdateBuffer = false;
};

#endif // !COMMANDSYSTEM_H
