#ifndef PLAYER_SETTINGS_H
#define PLAYER_SETTINGS_H

#include "pch.h"
#include "../utils/SerializationHelper.h"
#include "../CommandSystem/DirectionCommand.h"

class PlayerSettings
{
public:
	bool mouseInvertedX, mouseInvertedY, gamepadInvertedX, gamepadInvertedY;
	DirectionCommand directionCommand;
	std::unordered_map<std::string, Command> commands;
};

inline void to_json(ordered_json& j, const PlayerSettings& settings) {
	to_json(j["mouseInvertedX"], settings.mouseInvertedX);
	to_json(j["mouseInvertedY"], settings.mouseInvertedY);
	to_json(j["gamepadInvertedX"], settings.gamepadInvertedX);
	to_json(j["gamepadInvertedY"], settings.gamepadInvertedY);
	to_json(j["directionCommand"], settings.directionCommand);
	j["commands"] = settings.commands;
}

inline void from_json(const ordered_json& j, PlayerSettings& settings) {
	from_json(j["mouseInvertedX"], settings.mouseInvertedX);
	from_json(j["mouseInvertedY"], settings.mouseInvertedY);
	from_json(j["gamepadInvertedX"], settings.gamepadInvertedX);
	from_json(j["gamepadInvertedY"], settings.gamepadInvertedY);
	from_json(j["directionCommand"], settings.directionCommand);
	settings.commands = j["commands"].get<std::unordered_map<std::string, Command>>();
}

#endif