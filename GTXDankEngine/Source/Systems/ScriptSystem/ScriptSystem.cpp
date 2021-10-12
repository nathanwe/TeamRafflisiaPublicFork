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
    int a = static_cast<int>(lua_tonumber(L, 1));
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
    GameLogicCategories gLC = static_cast<GameLogicCategories>(lua_tointeger(L, 1));
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

int lua_GetCategorysOfEntity(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tonumber(L, 1));
    lua_newtable(L);
    int top = lua_gettop(L);
    int index = 0;
    auto GLCC = GameLogicCategoryComponentPool.GetComponentByEntity(e);
    for (int i = 1; i<static_cast<int>(GameLogicCategories::MAX_CATEGORIES);++i)
    {
        if (GLCC->categories.find(static_cast<GameLogicCategories>(i)) != GLCC->categories.end())
        {
            lua_pushnumber(L, index);
            lua_pushnumber(L, i);
            lua_settable(L, top);
        }
        
    }

    return 1;
}

int lua_UpdateAllEntitys(lua_State* L)
{
    for (Entity e : EntityList)
    {
        if (GameLogicCategoryComponentPool.GetComponentByEntity(e) != nullptr)
        {
            lua_getglobal(L, "UpdateEntity");
            if (lua_isfunction(L, -1))
            {
                lua_pushnumber(L, static_cast<int>(e));
                CheckLua(L, lua_pcall(L, 1, 0, 0));
            }
            else
            {
                LOG_ERROR("scriptsystem error, UpdateEntity not found")
            }
        }
    }
    return 1;
}

int lua_LoadScript(lua_State* L)
{
    std::string filePath = static_cast<std::string>(lua_tostring(L, 1));
    ResourceHandle<LuaFile>* newScript = ScriptResourceManager.GetResourceHandleNoThread(filePath);

    lua_getglobal(L, "DoStringWithErrorCheck");
    if (lua_isfunction(L, -1))
    {
        lua_pushstring(L, newScript->GetPointer()->data.c_str());
        CheckLua(L, lua_pcall(L, 1, 0, 0));
    }
    else
    {
        LOG_ERROR("scriptsystem error, DoStringWithErrorCheck not found")
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
    lua_register(L, "GetCategorysOfEntity", lua_GetCategorysOfEntity);
    lua_register(L, "UpdateAllEntitys", lua_UpdateAllEntitys);
    lua_register(L, "LoadScript", lua_LoadScript);

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
