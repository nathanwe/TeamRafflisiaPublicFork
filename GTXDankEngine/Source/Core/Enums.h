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
	MOVING_BODY = 6,
	STILL_BODY = 7,
	TAG = 8,
	SPECIAL_BODY = 9,
	COLLIDER = 10,
	TOTAL
};
//add new component above here

NLOHMANN_JSON_SERIALIZE_ENUM(ComponentType, {
	REGISTER_STRING(TRANSFORM)
	REGISTER_STRING(LIGHT)
	REGISTER_STRING(MODEL)
	REGISTER_STRING(MATERIAL)
	REGISTER_STRING(GAME_LOGIC)
	REGISTER_STRING(MOVING_BODY)
	REGISTER_STRING(STILL_BODY)
	REGISTER_STRING(TAG)
	REGISTER_STRING(SPECIAL_BODY)
	REGISTER_STRING(COLLIDER)
	});
//add new component above here

static std::unordered_map<std::string, int> ComponentNameToType
{
	{"TRANSFORM", 1},
	{"LIGHT", 2},
	{"MODEL", 3},
	{"MATERIAL", 4},
	{"GAME_LOGIC", 5},
	{"MOVING_BODY", 6},
	{"STILL_BODY", 7},
	{"TAG", 8},
	{"SPECIAL_BODY", 9},
	{"COLLIDER", 10},
};
//add new component above here

enum class ErrorEnum
{
	SUCCESS = 0,
	FAIL_BECAUSE_COMPONENT_ALREADY_EXISTS = 1,
	FAIL_BECAUSE_NO_MORE_MEMORY = 2
};

//only things that have a lua file
enum GameLogicCategories
{
	PLAYER = 1,
	ENEMY = 2,
	PLAYER_PROJECTILE = 3,
	GOAL_ZONE = 4,
	MOVING_PLATFORM = 5,
	MAX_CATEGORIES = 6
};

NLOHMANN_JSON_SERIALIZE_ENUM(GameLogicCategories, {
	REGISTER_STRING(PLAYER)
	REGISTER_STRING(ENEMY)
	REGISTER_STRING(PLAYER_PROJECTILE)
	REGISTER_STRING(GOAL_ZONE)
	REGISTER_STRING(MOVING_PLATFORM)
	});
	
enum class EventType
{
	DEFAULT_EVENT = 0,
	TOGGLE_MENU = 1,
	MUTE_BGM = 2,
	UNMUTE_BGM = 3,
	EMIT_LION = 4,
	DESTROY_ENTITY = 5,
	DESTROY_ALL_ENTITIYS = 6,
	DESTROY_LIONS = 7,
	MOVE_POKEBALL = 8,
	PLAY_SOUND = 9,
	MUTE_SFX = 10,
	UNMUTE_SFX = 11,
	PHYSICS_COLLISION = 12,
	SAVE_LUA =13,
	LOAD_LUA = 14,
	COMMAND_GIVEN = 15,
	SEND_DATA_TO_IMGUI = 16,
	LOAD_DATA_FROM_IMGUI = 17,
	SHOT = 18

};

///////////////////////////////////////////////////////
// Physics
///////////////////////////////////////////////////////

enum CollisionType
{
	STATIC,
	DYNAMIC,

	BODY_TYPE
};

///////////////////////////////////////////////////////


///////////////////////////////////////////////////////

enum Shape
{
	SPHERE,
	PLANE,
	AABB,

	SHAPE
};

///////////////////////////////////////////////////////

#endif // !ENUMS_H
