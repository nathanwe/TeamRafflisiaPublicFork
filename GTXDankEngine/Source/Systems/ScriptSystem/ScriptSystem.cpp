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

ScriptSystem::ScriptSystem()
{
}

void SendLionEmitEvent()
{
    Event ev;
    ev.runPerEntity = true;
    ev.e1 = 1;
    ev.type = EventType::EMIT_LION;
    ev.thingsToEffect.insert(GameLogicCategories::POKEBALL);
    engine.DoGameLogicScriptSys.HandleEvent(ev);
}
void SendMenuToggleEvent()
{
    Event ev;
    ev.type = EventType::TOGGLE_MENU;
    engine.MenuSys.HandleEvent(ev);
}
void SendLionDeleteEvent()
{
    Event ev;
    ev.runPerEntity = true;
    ev.type = EventType::DESTROY_LIONS;
    ev.thingsToEffect.insert(GameLogicCategories::LION);
    engine.DoGameLogicScriptSys.HandleEvent(ev);
}

bool ScriptSystem::Init()
{
    LOG_ERROR("script system needs a script");
    assert(false && "script system needs a script");
    return false;
}

bool ScriptSystem::Init(std::string filePath)
{
    engine.CommandSys.Skill1Command.SetActionToExecute(SendLionEmitEvent);
    engine.CommandSys.Skill2Command.SetActionToExecute(SendMenuToggleEvent);
    engine.CommandSys.Skill3Command.SetActionToExecute(SendLionDeleteEvent);

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
    lua_register(L, "MakeLionByHand", lua_MakeLionByHand);
    lua_register(L, "GetPosition", lua_GetPosition);
    lua_register(L, "BeginImgui", lua_BeginImgui);
    lua_register(L, "EndImgui", lua_EndImgui);
    lua_register(L, "ButtonImgui", lua_ButtonImgui);
    lua_register(L, "SendAudioEvent", lua_SendAudioEvent);
    lua_register(L, "DeleteEntity", lua_DeleteEntity);

    bool out = CheckLua(L, luaL_dostring(L, fileHandle->GetPointer()->data.c_str()));
    lua_getglobal(L, "Init");
    if (lua_isfunction(L, -1))
    {
        CheckLua(L, lua_pcall(L, 0, 0, 0));
    }
    else
    {
        LOG_ERROR("scriptsystem error, Init() not found")
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
