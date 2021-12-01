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

	std::string commandName;
	int keyboardcode;
	int gamepadCode;
	bool mouseLeftClick = false;
	bool mouseRightClick = false;
	KeyPressType keyPressType;
private:
 	std::function<void()> actionToExecute;
	void triggerEvent();

};

inline void to_json(ordered_json& j, const Command& command) {
	to_json(j["keyboardcode"], command.keyboardcode);
	to_json(j["gamepadCode"], command.gamepadCode);
	to_json(j["keyPressType"], command.keyPressType);
	to_json(j["mouseLeftClick"], command.mouseLeftClick);
	to_json(j["mouseRightClick"], command.mouseRightClick);
}

inline void from_json(const ordered_json& j, Command& command) {
	from_json(j["keyboardcode"], command.keyboardcode);
	from_json(j["gamepadCode"], command.gamepadCode);
	if (j.contains("mouseLeftClick"))
		from_json(j["mouseLeftClick"], command.mouseLeftClick);
	if (j.contains("mouseRightClick"))
		from_json(j["mouseRightClick"], command.mouseRightClick);
	command.keyPressType = static_cast<KeyPressType>(j["keyPressType"]);
}

#endif
