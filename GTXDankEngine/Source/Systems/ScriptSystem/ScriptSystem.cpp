#include "pch.h"
#include "ScriptSystem.h"
#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"


extern Engine engine;

bool CheckLua(lua_State* L, int r)
{
    {
        if (r != LUA_OK)
        {
            std::string errormsg = lua_tostring(L, -1);
            //std::cout << errormsg << std::endl;
            LOG_ERROR(errormsg);
            return false;
        }
        return true;
    }
}




bool ScriptSystem::Init()
{
    LOG_ERROR("script system needs a script");
    assert(false && "script system needs a script");
    return false;
}

bool ScriptSystem::Init(std::string filePath)
{

    fileHandle = ScriptResourceManager.GetResourceHandleNoThread(filePath);
    L = luaL_newstate();
    luaL_openlibs(L);

    //regester all the lua hook functions
    lua_register(L, "HostFunction", lua_HostFunction);
    lua_register(L, "GetExampleData", lua_GetExampleData);
    lua_register(L, "AddToVQS", lua_AddToVQS);
    lua_register(L, "GetEntiysByCategory", lua_GetEntiysByCategory);
    lua_register(L, "GetCategorysOfEntity", lua_GetCategorysOfEntity);
    lua_register(L, "UpdateAllEntitys", lua_UpdateAllEntitys);
    lua_register(L, "LoadScript", lua_LoadScript);
    lua_register(L, "GetKeyTriggered", lua_GetKeyTriggered);
    lua_register(L, "GetPosition", lua_GetPosition);
    lua_register(L, "BeginImgui", lua_BeginImgui);
    lua_register(L, "EndImgui", lua_EndImgui);
    lua_register(L, "ButtonImgui", lua_ButtonImgui);
    lua_register(L, "ControlMenu", lua_ControlMenu);
    lua_register(L, "ResetControl", lua_ResetControl);
    lua_register(L, "IntSliderImgui", lua_IntSliderImgui);
    lua_register(L, "SendAudioEvent", lua_SendAudioEvent);
    lua_register(L, "DeleteEntity", lua_DeleteEntity);
    lua_register(L, "CreateEntity", lua_CreateEntity);
    lua_register(L, "SetPosition", lua_SetPosition);
    lua_register(L, "GetRigidData", lua_GetRigidData);
    lua_register(L, "SetPhysicsVelocity", lua_SetPhysicsVelocity);
    lua_register(L, "GetSoundVolumes", lua_GetSoundVolumes);
    lua_register(L, "GetScale", lua_GetScale);
    lua_register(L, "SetScale", lua_SetScale);
    lua_register(L, "GetRotation", lua_GetRotation);
    lua_register(L, "SetRotation", lua_SetRotation);
    lua_register(L, "SetRotationEuler", lua_SetRotationEuler);
    lua_register(L, "SetRotationFromDirection", lua_SetRotationFromDirection);
    lua_register(L, "SetGamePath", lua_SetGamePath);
    lua_register(L, "SaveIntFloatTableAsJson", lua_SaveIntFloatTableAsJson);
    lua_register(L, "LoadIntFloatTableFromJson", lua_LoadIntFloatTableFromJson);
    lua_register(L, "GetGamePath", lua_GetGamePath);
    lua_register(L, "GetImguiControledFloat", lua_GetImguiControledFloat);
    lua_register(L, "ImguiControledFloat", lua_ImguiControledFloat);
    lua_register(L, "ImguiText", lua_ImguiText);
    lua_register(L, "UpdateCameraOld", lua_UpdateCameraOld);
    lua_register(L, "Set3rdPerson", lua_Set3rdPerson);
    lua_register(L, "Raycast", lua_Raycast);
    lua_register(L, "SendEvent", lua_SendEvent);
    lua_register(L, "GetCameraDirection", lua_GetCameraDirection);
    lua_register(L, "GetCameraPosition", lua_GetCameraPosition);
    lua_register(L, "AddPhysicsVelocity", lua_AddPhysicsVelocity);
    lua_register(L, "SetPhysicsAcceleration", lua_SetPhysicsAcceleration);
    lua_register(L, "AddPhysicsAcceleration", lua_AddPhysicsAcceleration);
    lua_register(L, "GetCameraOrientation", lua_GetCameraOrientation);
    lua_register(L, "Set3rdPersonDistance", lua_Set3rdPersonDistance);
    lua_register(L, "UpdatePhysicsCollider", lua_UpdatePhysicsCollider);
    lua_register(L, "SetCameraPitchYaw", lua_SetCameraPitchYaw);
    lua_register(L, "SetCameraOffest", lua_SetCameraOffest);
    lua_register(L, "SetCellShade", lua_SetCellShade);
	lua_register(L, "SetSunAngle", lua_SetSunAngle);
	lua_register(L, "LoadNextLevel", lua_LoadNextLevel);
    lua_register(L, "RestartGame", lua_RestartGame);
    lua_register(L, "GetLevelNumber", lua_GetLevelNumber);
    lua_register(L, "LOG_INFO", lua_LOG_INFO);
    lua_register(L, "LOG_ERROR", lua_LOG_ERROR);
    lua_register(L, "SetBGM", lua_SetBGM);
    lua_register(L, "SetSFX", lua_SetSFX);
    lua_register(L, "LoadAudioBank", lua_LoadAudioBank);
    lua_register(L, "UnloadAudioBank", lua_UnloadAudioBank);
    lua_register(L, "PlayAudioEvent", lua_PlayAudioEvent);
    lua_register(L, "GetAudioEventInstanceParameter", lua_GetEventInstanceParameter);
    lua_register(L, "SetAudioEventInstanceParameter", lua_SetEventInstanceParameter);
    lua_register(L, "IsAudioEventPlaying", lua_IsAudioEventPlaying);
    lua_register(L, "SetBusMuted", lua_SetBusMuted);
    lua_register(L, "SetBusVolume", lua_SetBusVolume);
    lua_register(L, "SetAllMuted", lua_MuteAll);
    lua_register(L, "SetCameraStatic", lua_SetCameraStatic);
    lua_register(L, "SetCameraStaticScene", lua_SetCameraStaticScene);
    lua_register(L, "SetAudioEventPosition", lua_SetAudioEventPosition);
    

    bool out = CheckLua(L, luaL_dostring(L, fileHandle->GetPointer()->data.c_str()));
    lua_getglobal(L, "Init");
    if (lua_isfunction(L, -1))
    {
        CheckLua(L, lua_pcall(L, 0, 0, 0));
    }
    else
    {
        LOG_ERROR("scriptsystem error, Init() of " + filePath + "not found")
    }
    return out;
}

void ScriptSystem::Update(float timeStamp)
{
    lua_getglobal(L, "Update");
    if (lua_isfunction(L, -1))
    {
        lua_pushnumber(L, timeStamp);
        CheckLua(L, lua_pcall(L, 1, 0, 0));
    }
    else
    {
        LOG_ERROR("scriptsystem error, Update(dt) not found")
    }
}

bool ScriptSystem::Destroy()
{
    lua_getglobal(L, "Destroy");
    if (lua_isfunction(L, -1))
    {
        CheckLua(L, lua_pcall(L, 0, 0, 0));
    }
    else
    {
        LOG_ERROR("scriptsystem error, Destroy not found")
    }
    lua_close(L);
    return ScriptResourceManager.Destroy();
}

void ScriptSystem::HandleEvent(Event event)
{
    if (event.runPerEntity)
    {
        for (auto comp :GameLogicCategoryComponentPool.componentList)
        {
            bool shouldPassEvent = false;
            for (GameLogicCategories glc : event.thingsToEffect)
            {
                if (comp.second->categories.find(glc) != comp.second->categories.end())
                {
                    shouldPassEvent = true;
                }
            }
            if (shouldPassEvent)
            {
                lua_getglobal(L, "HandleEventPerEntity");
                if (lua_isfunction(L, -1))
                {
                    lua_pushnumber(L, static_cast<int>(comp.first));
                    PassEvent(L, event);
                    CheckLua(L, lua_pcall(L, 3, 0, 0));
                }
                else
                {
                    LOG_ERROR("scriptsystem error, HandleEventPerEntity not found")
                }
            }
            
        }
    }
    else
    {
        lua_getglobal(L, "HandleEvent");
        if (lua_isfunction(L, -1))
        {

            PassEvent(L, event);

            CheckLua(L, lua_pcall(L, 2, 0, 0));
        }
        else
        {
            LOG_ERROR("scriptsystem error, HandleEvent not found")
        }
    }
    
}
