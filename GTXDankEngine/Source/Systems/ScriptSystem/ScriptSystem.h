#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "pch.h"

#include "../../Libraries/include/lua/lua.hpp"

#include "../Core/System.h"
#include "../Core/ResourceManager.h"
#include "../Core/Enums.h"


typedef uint32_t Entity;
//TODO: move events somewhere else
struct Event
{
	std::set<GameLogicCategories> thingsToEffect;
	EventType type;
	Entity e1;
	Entity e2;
	int intData1;
	float floatData1, floatData2, floatData3, floatData4;
	std::string stringData1;

};

// Little error checking utility function
bool CheckLua(lua_State* L, int r);


//c++ func to pass to lua for testing purpouses
//returns an int, takes a lua_State, and starts with lua_
int lua_HostFunction(lua_State* L);
int lua_GetExampleData(lua_State* L);

//real c++ funcs
int lua_AddToVQS(lua_State* L);
int lua_GetEntiysByCategory(lua_State* L);
int lua_GetCategorysOfEntity(lua_State* L);
int lua_UpdateAllEntitys(lua_State* L);
int lua_LoadScript(lua_State* L);
int lua_GetKeyTriggered(lua_State* L);
int lua_MakeLionByHand(lua_State* L);
int lua_GetPosition(lua_State* L);

class LuaFile
{
public:
	LuaFile(std::string filePath)
	{
		std::ifstream f(filePath); //taking file as inputstream
		if (f)
		{
			std::ostringstream ss;
			ss << f.rdbuf(); // reading data
			data = ss.str();
		}
	}
	~LuaFile()
	{
		
	}
	std::string data;
	void OnLoad() {};
};


extern ResourceManager<LuaFile> ScriptResourceManager;

class ScriptSystem : public System
{
public:
	ScriptSystem();
	~ScriptSystem() {};

	bool Init();
	bool Init(std::string filePath);
	void Update(float timeStamp = 0);
	bool Destroy();

	void HandleEvent(Event event);
private:

	ResourceHandle<LuaFile>* fileHandle;
	lua_State* L;

};


#endif // !SCRIPTSYSTEM_H