#include "pch.h"
#include "ScriptSystem.h"
#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"

ResourceManager<LuaFile> ScriptResourceManager;
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

bool ScriptSystem::Init()
{
    LOG_ERROR("script system needs a script");
    assert(!"script system needs a script");
    return false;
}

bool ScriptSystem::Init(std::string filePath)
{
    engine.CommandSys.Skill1Command.SetActionToExecute(SendLionEmitEvent);

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
    lua_close(L);
    return ScriptResourceManager.Destroy();
}

void passEvent(lua_State* L, Event event){
    int index = 0;
    lua_newtable(L);
    int top = lua_gettop(L);
    for (GameLogicCategories gLC : event.thingsToEffect)
    {
        lua_pushnumber(L, index);
        lua_pushnumber(L, static_cast<int>(gLC));
        lua_settable(L, top);
        ++index;
    }
    lua_newtable(L);
    top = lua_gettop(L);

    lua_pushstring(L, "type");
    lua_pushnumber(L, static_cast<int>(event.type));
    lua_settable(L, top);

    lua_pushstring(L, "e1");
    lua_pushnumber(L, static_cast<int>(event.e1));
    lua_settable(L, top);

    lua_pushstring(L, "e2");
    lua_pushnumber(L, static_cast<int>(event.e2));
    lua_settable(L, top);

    lua_pushstring(L, "intData1");
    lua_pushnumber(L, event.intData1);
    lua_settable(L, top);

    lua_pushstring(L, "floatData1");
    lua_pushnumber(L, event.floatData1);
    lua_settable(L, top);

    lua_pushstring(L, "floatData2");
    lua_pushnumber(L, event.floatData2);
    lua_settable(L, top);

    lua_pushstring(L, "floatData3");
    lua_pushnumber(L, event.floatData3);
    lua_settable(L, top);

    lua_pushstring(L, "floatData4");
    lua_pushnumber(L, event.floatData4);
    lua_settable(L, top);

    lua_pushstring(L, "stringData1");
    lua_pushstring(L, event.stringData1.c_str());
    lua_settable(L, top);
}
void ScriptSystem::HandleEvent(Event event)
{
    if (event.runPerEntity)
    {
        for (Entity e : EntityList)
        {
            if (GameLogicCategoryComponentPool.GetComponentByEntity(e) != nullptr)
            {
                lua_getglobal(L, "HandleEventPerEntity");
                if (lua_isfunction(L, -1))
                {
                    lua_pushnumber(L, static_cast<int>(e));
                    passEvent(L, event);
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

            passEvent(L, event);

            CheckLua(L, lua_pcall(L, 2, 0, 0));
        }
        else
        {
            LOG_ERROR("scriptsystem error, HandleEvent not found")
        }
    }
    
}
