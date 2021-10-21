#include "pch.h"
#include "../Systems/ScriptSystem/ScriptSystem.h"

#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
#include "../Core/Engine.h"
#include "../Core/ResourceManager.h"


extern Engine engine;
static void dumpstack(lua_State* L)
{
    printf("Begin stack dump\n");
    int top = lua_gettop(L);
    for (int i = 1; i <= top; i++)
    {
        printf("%d\t%s\t", i, luaL_typename(L, i));
        switch (lua_type(L, i))
        {
        case LUA_TNUMBER:
            printf("%g\n", lua_tonumber(L, i));
            break;
        case LUA_TSTRING:
            printf("%s\n", lua_tostring(L, i));
            break;
        case LUA_TBOOLEAN:
            printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
            break;
        case LUA_TNIL:
            printf("%s\n", "nil");
            break;
        default:
            printf("%p\n", lua_topointer(L, i));
            break;
        }
    }
    printf("End stack dump\n");
}
void PassEvent(lua_State* L, Event event)
{
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
Event ReceiveEvent(lua_State* L)
{
    //std::cout << "reciving event" << std::endl;
    Event retvalue = Event();
    lua_settop(L, 1);
    luaL_checktype(L, 1, LUA_TTABLE);
    dumpstack(L);
    int rettype = lua_getfield(L, -1, "type");
    if (rettype != LUA_TNIL)
    {
        retvalue.type = static_cast<EventType>(lua_tointeger(L, -1));
    }
    else
    {
        retvalue.type = EventType::DEFAULT_EVENT;
    }
    lua_pop(L,1);
    //dumpstack(L);
    rettype = lua_getfield(L, -1, "e1");
    if (rettype != LUA_TNIL)
    {
        retvalue.e1 = static_cast<Entity>(lua_tointeger(L, -1));
    }
    else
    {
        retvalue.e1 = 0;
    }
    lua_pop(L, 1);
    //dumpstack(L);
    rettype = lua_getfield(L, -1, "e2");
    if (rettype != LUA_TNIL)
    {
        retvalue.e2 = static_cast<Entity>(lua_tointeger(L, -1));
    }
    else
    {
        retvalue.e2 = 0;
    }
    lua_pop(L, 1);
    //dumpstack(L);
    rettype = lua_getfield(L, -1, "intData1");
    if (rettype != LUA_TNIL)
    {
        retvalue.intData1 = static_cast<int>(lua_tointeger(L, -1));
    }
    else
    {
        retvalue.intData1 = 0;
    }
    lua_pop(L, 1);
    rettype = lua_getfield(L, -1, "floatData1");
    if (rettype != LUA_TNIL)
    {
        retvalue.floatData1 = static_cast<float>(lua_tonumber(L, -1));
    }
    else
    {
        retvalue.floatData1 = 0;
    }
    lua_pop(L, 1);
    rettype = lua_getfield(L, -1, "floatData2");
    if (rettype != LUA_TNIL)
    {
        retvalue.floatData2 = static_cast<float>(lua_tonumber(L, -1));
    }
    else
    {
        retvalue.floatData2 = 0;
    }
    lua_pop(L, 1);
    rettype = lua_getfield(L, -1, "floatData3");
    if (rettype != LUA_TNIL)
    {
        retvalue.floatData3 = static_cast<float>(lua_tonumber(L, -1));
    }
    else
    {
        retvalue.floatData3 = 0;
    }
    lua_pop(L, 1);
    rettype = lua_getfield(L, -1, "floatData4");
    if (rettype != LUA_TNIL)
    {
        retvalue.floatData4 = static_cast<float>(lua_tonumber(L, -1));
    }
    else
    {
        retvalue.floatData4 = 0;
    }
    lua_pop(L, 1);
    rettype = lua_getfield(L, -1, "stringData1");
    if (rettype != LUA_TNIL)
    {
        retvalue.stringData1 = lua_tostring(L, -1);
    }
    else
    {
        retvalue.stringData1 = "noStringProvided";
    }
    lua_pop(L, 1);

    return retvalue;
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

int lua_BeginImgui(lua_State* L)
{
    const char* menuName = lua_tostring(L, 1);
    ImGui::Begin(menuName);
    return 1;
}

int lua_EndImgui(lua_State* L)
{
    ImGui::End();
    return 1;
}

int lua_ButtonImgui(lua_State* L)
{
    
    const char* ButtonName = lua_tostring(L, 1);
    bool pressed = ImGui::Button(ButtonName, ImVec2(50,25));
    lua_pushboolean(L, pressed);
    return 1;
}

int lua_SendAudioEvent(lua_State* L)
{
    Event ev = ReceiveEvent(L);
    engine.AudioSys.HandleEvent(ev);
    return 1;
}

int lua_DeleteEntity(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tonumber(L, 1));
    Event ev = Event(true);
    ev.type = EventType::DESTROY_ENTITY;
    ev.e1 = e;
    engine.DoGameLogicScriptSys.HandleEvent(ev);

    TransformComponentPool.Delete(e);
    GameLogicCategoryComponentPool.Delete(e);
    MaterialComponentPool.Delete(e);
    ModelComponentPool.Delete(e);
    LightComponentPool.Delete(e);

    engine.EntitySys.DestroyEntity(e);
    return 1;
}
