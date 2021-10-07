#include "pch.h"
#include "ScriptSystem.h"

#include "..\Components\GameLogicCategoryComponent\GameLogicCategoryComponent.h"
#include "..\Components\TransformComponent\TransformComponent.h"

ResourceManager<LuaFile> ScriptResourceManager;

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

int lua_HostFunction(lua_State* L)
{
    float a = (float)lua_tonumber(L, 1);
    float b = (float)lua_tonumber(L, 2);
    std::cout << "[C++] Hostfunc a = " << a << ", b = " << b << std::endl;
    float c = a * b;
    lua_pushnumber(L, c);
    return 1;
}

int lua_GetExampleData(lua_State* L)
{
    int a = lua_tonumber(L, 1);
    int data[] = { 2,1,0 };
    lua_pushnumber(L, data[a]);
    return 1;
}

int lua_AddToVQS(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tonumber(L, 1));
    float amountToAddx = static_cast<float>(lua_tonumber(L, 2));
    float amountToAddy = static_cast<float>(lua_tonumber(L, 3));
    float amountToAddz = static_cast<float>(lua_tonumber(L, 4));
    TransformComponent* trans = TransformComponentPool.GetComponentByEntity(e);
    if (trans != nullptr)
    {
        trans->transform->position.x += amountToAddx;
        trans->transform->position.y += amountToAddy;
        trans->transform->position.z += amountToAddz;
    }
    return 1;

}

int lua_GetEntiysByCategory(lua_State* L)
{
    int index = 1;
    GameLogicCategories gLC = static_cast<GameLogicCategories>(lua_tonumber(L, 1));
    for (auto e : EntityList)
    {
        auto gLCComponent = GameLogicCategoryComponentPool.GetComponentByEntity(e);
        if (gLCComponent != nullptr)
        {
            if (gLCComponent->categories.find(gLC) != gLCComponent->categories.end())
            {
                lua_getglobal(L, "AddEntityToList");
                if (lua_isfunction(L, -1))
                {
                    lua_pushnumber(L, e);
                    CheckLua(L, lua_pcall(L, 1, 0, 0));
                }
                else
                {
                    LOG_ERROR("scriptsystem error, AddEntityToList not found")
                }
            }
        }
    }
    return 1;
}


ScriptSystem::ScriptSystem()
{
}

bool ScriptSystem::Init()
{
    LOG_ERROR("script system needs a script");
    assert(!"script system needs a script");
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
    return ScriptResourceManager.Destroy();;
}
