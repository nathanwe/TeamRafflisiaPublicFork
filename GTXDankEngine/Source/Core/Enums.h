#ifndef ENUMS_H
#define ENUMS_H

#include <json/json.h>

#define REGISTER_STRING(x) {x,#x},

enum ComponentType
{
	ABSTRACT = 0,
	TRANSFORM = 1,
	LIGHT = 2,
	MODEL = 3,
	MATERIAL = 4,
	GAME_LOGIC = 5,
	STILL_BODY = 6,
	MOVING_BODY = 7,
	TOTAL
};

NLOHMANN_JSON_SERIALIZE_ENUM(ComponentType, {
	REGISTER_STRING(TRANSFORM)
	REGISTER_STRING(LIGHT)
	REGISTER_STRING(MODEL)
	REGISTER_STRING(MATERIAL)
	REGISTER_STRING(GAME_LOGIC)
	//
	REGISTER_STRING(STILL_BODY)
	REGISTER_STRING(MOVING_BODY)
	//
	});

static std::unordered_map<std::string, int> ComponentNameToType
{
	{"TRANSFORM", 1},
	{"LIGHT", 2},
	{"MODEL", 3},
	{"MATERIAL", 4},
	{"GAME_LOGIC", 5},
	//
	{"GAME_LOGIC", 6},
	{"GAME_LOGIC", 7},
	//
};

enum class ErrorEnum
{
	SUCCESS = 0,
	FAIL_BECAUSE_COMPONENT_ALREADY_EXISTS = 1,
	FAIL_BECAUSE_NO_MORE_MEMORY = 2
};

//only things that have a lua file
enum GameLogicCategories
{
	POKEBALL = 1,
	LION = 2,
	POINT_LIGHT_SOURCE = 3,
	MAX_CATEGORIES = 4
};

NLOHMANN_JSON_SERIALIZE_ENUM(GameLogicCategories, {
	REGISTER_STRING(POKEBALL)
	//REGISTER_STRING(VASE)
	REGISTER_STRING(LION)
	REGISTER_STRING(POINT_LIGHT_SOURCE)
	//REGISTER_STRING(DIR_LIGHT_SOURCE)
	//REGISTER_STRING(PLANE)
	});
	
enum class EventType
{
	DEFAULT_EVENT = 0,
	TOGGLE_MENU = 1,
	MUTE_ALL = 2,
	UNMUTE_ALL = 3,
	EMIT_LION = 4,
	DESTROY_ENTITY = 5,
	DESTROY_ALL_ENTITIYS = 6,
	DESTROY_LIONS = 7,
	MOVE_POKEBALL = 8
};

///////////////////////////////////////////////////////
// Physics
///////////////////////////////////////////////////////

enum class Collision_Type
{
	STATIC,
	DYNAMIC,

	BODY_TYPE
};

///////////////////////////////////////////////////////


///////////////////////////////////////////////////////

enum class Shape
{
	SPHERE,

	SHAPE
};

///////////////////////////////////////////////////////

#endif // !ENUMS_H
