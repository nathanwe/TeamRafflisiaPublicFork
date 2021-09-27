#pragma once

#include "pch.h"
#include "../../Core/System.h"
#include "Command.h"
#include "DirectionCommand.h"

class CommandSystem : public System
{
public:
	virtual bool Init() override;
	virtual void Update(float timeStamp = 0) override;
	virtual bool Destroy() override;

public:
	DirectionCommand MoveCommand;
	Command SpaceCommand;
	Command ShiftCommand;
	Command UnShiftCommand;
	Command CtrlCommand;

	Command Attack1Command;
	Command Attack2Command;
	Command Attack3Command;
	
	Command Skill1Command;
	Command Skill2Command;
	Command Skill3Command;
	
	Command ToggleMenuCommand;
	Command MenuUpCommand;
	Command MenuDownCommand;
	
	Command DebugModeCommand;
	//Command InvincibleModeCommand;
	//
	//Command Debug1Command;
	//Command Debug2Command;
	//Command Debug3Command;
	//
	//Command DebugNextLevelCommand;
	//Command DebugPreviousLevelCommand;

private:
	void ExecuteGameplayCommands();
	void ExecuteUICommands();
	void ExecuteDebugCommands();

private:
	bool menuMode;
	bool debugMode;
};

