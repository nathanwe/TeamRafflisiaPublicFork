#ifndef COMMAND_H
#define COMMAND_H

#include "pch.h"
#include "../utils/SerializationHelper.h"

enum KeyPressType
{
	Press,
	Trigger,
	Release
};

class Command
{
public:
	Command() : keyPressType(KeyPressType::Press) {}

	void SetActionToExecute(std::function<void()> actionToExecute);
	void Execute();
	//more functionalities eg Redo, Undo can be implemented here

	int keyboardcode;
	int gamepadCode;
	KeyPressType keyPressType;
private:
 	std::function<void()> actionToExecute;

};

inline void to_json(ordered_json& j, const Command& command) {
	to_json(j["keyboardcode"], command.keyboardcode);
	to_json(j["gamepadCode"], command.gamepadCode);
	to_json(j["keyPressType"], command.keyPressType);
}

inline void from_json(const ordered_json& j, Command& command) {
	from_json(j["keyboardcode"], command.keyboardcode);
	from_json(j["gamepadCode"], command.gamepadCode);
	command.keyPressType = static_cast<KeyPressType>(j["keyPressType"]);
}

#endif
