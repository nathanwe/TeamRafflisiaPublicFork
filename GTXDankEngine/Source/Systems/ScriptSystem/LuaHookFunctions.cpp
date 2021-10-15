#include "pch.h"
#include "../Systems/ScriptSystem/ScriptSystem.h"

#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Core/Engine.h"
#include "../Core/ResourceManager.h"


extern Engine engine;


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
    for (auto comp : GameLogicCategoryComponentPool.componentList)
    {
        auto gLCComponent = comp.second;
        if (gLCComponent != nullptr)
        {
            if (gLCComponent->categories.find(gLC) != gLCComponent->categories.end())
            {
                lua_getglobal(L, "AddEntityToList");
                if (lua_isfunction(L, -1))
                {
                    lua_pushnumber(L, comp.first);
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
    for (int i = 1; i < static_cast<int>(GameLogicCategories::MAX_CATEGORIES); ++i)
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
    for (auto comp : GameLogicCategoryComponentPool.componentList)
    {
        lua_getglobal(L, "UpdateEntity");
        if (lua_isfunction(L, -1))
        {
            lua_pushnumber(L, static_cast<int>(comp.first));
            CheckLua(L, lua_pcall(L, 1, 0, 0));
        }
        else
        {
            LOG_ERROR("scriptsystem error, UpdateEntity not found")
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

int lua_GetKeyTriggered(lua_State* L)
{
    unsigned int keyToCheck = static_cast<unsigned int>(lua_tonumber(L, 1));
    lua_pushboolean(L, engine.InputSys.IsKeyTriggered(keyToCheck));
    return 1;
}

int lua_MakeLionByHand(lua_State* L)
{
    float xcoord = static_cast<float>(lua_tonumber(L, 1));
    float ycoord = static_cast<float>(lua_tonumber(L, 2));
    float zcoord = static_cast<float>(lua_tonumber(L, 3));
    // lion entity
    Entity lion = engine.EntitySys.CreateEntity();
    EntityList.push_back(lion);

    // model component
    ResourceHandle<Model>* lionModel = ModelResourceManager.GetResourceHandleNoThread("Assets/models/Lion/model.obj");
    ModelComponentPool.Add(lion, (lionModel));

    // Transform component
    VQS* lionTransform = new VQS(glm::vec3(xcoord, ycoord, zcoord), 1.0f);
    TransformComponentPool.Add(lion, (lionTransform));

    // Material component
    ResourceHandle<Texture>* lionDiffuse = TextureResourceManger.GetResourceHandle("Assets/models/Lion/albedo.jpg");
    ResourceHandle<Texture>* lionNormal = TextureResourceManger.GetResourceHandle("Assets/models/Lion/normal.jpg");
    ResourceHandle<Texture>* lionMetallic = TextureResourceManger.GetResourceHandle("Assets/models/Lion/metallic.jpg");
    ResourceHandle<Texture>* lionRoughness = TextureResourceManger.GetResourceHandle("Assets/models/Lion/roughness.jpg");

    Material* lionMat = new Material(lionDiffuse, lionMetallic, lionNormal, lionRoughness);
    MaterialComponentPool.Add(lion, (lionMat));
    GameLogicCategoryComponentPool.Add(lion, (std::vector<GameLogicCategories>({ GameLogicCategories::LION })));
    return 1;
}

int lua_GetPosition(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tointeger(L, 1));
    TransformComponent* trans = TransformComponentPool.GetComponentByEntity(e);
    if (trans != nullptr)
    {
        lua_pushnumber(L, trans->transform->position.x);
        lua_pushnumber(L, trans->transform->position.y);
        lua_pushnumber(L, trans->transform->position.z);
    }
    else
    {
        lua_pushnumber(L, 0);
        lua_pushnumber(L,0);
        lua_pushnumber(L, 0);
        LOG_ERROR("Transform not found");
    }
    return 1;
}
