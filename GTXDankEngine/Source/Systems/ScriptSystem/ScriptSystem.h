#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "pch.h"


#include "../Core/System.h"
#include "../Core/ResourceManager.h"
#include "../Core/Enums.h"


typedef uint32_t Entity;
//TODO: move events somewhere else
struct Event
{
	std::set<GameLogicCategories> thingsToEffect;
	bool runPerEntity;
	EventType type;
	Entity e1;
	Entity e2;
	int intData1;
	float floatData1, floatData2, floatData3, floatData4;
	std::string stringData1;

	Event(bool affectEverything = false) : runPerEntity(false), type(EventType::DEFAULT_EVENT), e1(0), e2(0), intData1(0),
		floatData1(0), floatData2(0), floatData3(0), floatData4(0), stringData1("")
	{
		if (affectEverything)
		{
			for (int i = 1; i < static_cast<int>(GameLogicCategories::MAX_CATEGORIES); ++i)
			{
				thingsToEffect.insert(static_cast<GameLogicCategories>(i));
			}
		}
	}
};

// Little error checking utility function
bool CheckLua(lua_State* L, int r);

//helper funcs
static void dumpstack(lua_State* L);
void PassEvent(lua_State* L, Event event);
Event ReceiveEvent(lua_State* L);

//c++ func to pass to lua for testing purpouses
//returns an the nuber of valuse it returns in lua, takes a lua_State, and starts with lua_
int lua_HostFunction(lua_State* L);
int lua_GetExampleData(lua_State* L);

//real c++ funcs
int lua_AddToVQS(lua_State* L);
int lua_GetEntiysByCategory(lua_State* L);
int lua_GetCategorysOfEntity(lua_State* L);
int lua_UpdateAllEntitys(lua_State* L);
int lua_LoadScript(lua_State* L);
int lua_GetKeyTriggered(lua_State* L);
int lua_GetPosition(lua_State* L);
int lua_BeginImgui(lua_State* L);
int lua_EndImgui(lua_State* L);
int lua_ButtonImgui(lua_State* L);
int lua_ControlMenu(lua_State* L);
int lua_ResetControl(lua_State* L);
int lua_IntSliderImgui(lua_State* L);
int lua_SendAudioEvent(lua_State* L);
int lua_DeleteEntity(lua_State* L);
int lua_CreateEntity(lua_State* L);
int lua_SetPosition(lua_State* L);
int lua_GetRigidData(lua_State* L);
int lua_SetPhysicsVelocity(lua_State* L);
int lua_GetSoundVolumes(lua_State* L);
int lua_GetScale(lua_State* L);
int lua_SetScale(lua_State* L);
int lua_GetRotation(lua_State* L);
int lua_SetRotation(lua_State* L);
int lua_SetRotationEuler(lua_State* L);
int lua_SetRotationFromDirection(lua_State* L);
int lua_SetGamePath(lua_State* L);
int lua_SaveIntFloatTableAsJson(lua_State* L);
int lua_LoadIntFloatTableFromJson(lua_State* L);
int lua_GetGamePath(lua_State* L);
int lua_ImguiControledFloat(lua_State* L);
int lua_GetImguiControledFloat(lua_State* L);
int lua_ImguiText(lua_State* L);
int lua_UpdateCameraOld(lua_State* L);	// camera
int lua_Set3rdPerson(lua_State* L);
int lua_TravelToLocationPathingly(lua_State* L);
int lua_SetCameraPosition(lua_State* L);
int lua_SetCameraAutoScroll(lua_State* L);
int lua_GetCameraDirection(lua_State* L);
int lua_GetCameraPosition(lua_State* L);
int lua_Raycast(lua_State* L);
int lua_SendEvent(lua_State* L);
int lua_AddPhysicsVelocity(lua_State* L);
int lua_SetPhysicsAcceleration(lua_State* L);
int lua_AddPhysicsAcceleration(lua_State* L);
int lua_GetCameraOrientation(lua_State* L);
int lua_Set3rdPersonDistance(lua_State* L);
int lua_UpdatePhysicsCollider(lua_State* L);
int lua_SetCameraPitchYaw(lua_State* L);
int lua_SetCameraOffest(lua_State* L);
int lua_SetCellShade(lua_State* L);
int lua_SetSunAngle(lua_State* L);
int lua_LoadNextLevel(lua_State* L);
int lua_RestartGame(lua_State* L);
int lua_GetLevelNumber(lua_State* L);
int lua_LOG_INFO(lua_State* L);
int lua_LOG_ERROR(lua_State* L);
int lua_SetBGM(lua_State* L);
int lua_SetSFX(lua_State* L);

int lua_LoadAudioBank(lua_State* L);
int lua_UnloadAudioBank(lua_State* L);
int lua_PlayAudioEvent(lua_State* L);
int lua_IsAudioEventPlaying(lua_State* L);
int lua_GetEventInstanceParameter(lua_State* L);
int lua_SetEventInstanceParameter(lua_State* L);

int lua_SetBusMuted(lua_State* L);
int lua_SetBusVolume(lua_State* L);
int lua_MuteAll(lua_State* L);


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


class ScriptSystem : public System
{
public:
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