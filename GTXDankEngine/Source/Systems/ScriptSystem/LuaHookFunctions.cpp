#include "pch.h"
#include "../utils/JsonFile.h"
#include "../Systems/ScriptSystem/ScriptSystem.h"

#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"

#include "../Core/Engine.h"
#include "../Core/ResourceManager.h"
#include "../Core/GameObjectFactory.h"


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
    //dumpstack(L);
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
        trans->transform.position.x += amountToAddx;
        trans->transform.position.y += amountToAddy;
        trans->transform.position.z += amountToAddz;
    }
    return 0;

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
    return 0;
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

    return 0;
}

int lua_GetKeyTriggered(lua_State* L)
{
    unsigned int keyToCheck = static_cast<unsigned int>(lua_tonumber(L, 1));
    lua_pushboolean(L, engine.InputSys.IsKeyTriggered(keyToCheck));
    return 1;
}

int lua_GetPosition(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tointeger(L, 1));
    TransformComponent* trans = TransformComponentPool.GetComponentByEntity(e);
    if (trans != nullptr)
    {
        lua_pushnumber(L, trans->transform.position.x);
        lua_pushnumber(L, trans->transform.position.y);
        lua_pushnumber(L, trans->transform.position.z);
    }
    else
    {
        lua_pushnumber(L, 0);
        lua_pushnumber(L,0);
        lua_pushnumber(L, 0);
        LOG_ERROR("Transform not found");
    }
    return 3;
}

int lua_BeginImgui(lua_State* L)
{
    const char* menuName = lua_tostring(L, 1);
    ImGui::Begin(menuName);
    return 0;
}

int lua_EndImgui(lua_State* L)
{
    ImGui::End();
    return 0;
}

int lua_ButtonImgui(lua_State* L)
{
    const char* ButtonName = lua_tostring(L, 1);
    float buttonWidth = static_cast<float>(lua_tonumber(L, 2));
    float buttonHight = static_cast<float>(lua_tonumber(L, 3));
    bool pressed = ImGui::Button(ButtonName, ImVec2(buttonWidth, buttonHight));
    lua_pushboolean(L, pressed);
    return 1;
}

int lua_ControlMenu(lua_State* L)
{
    engine.CommandSys.ShowCommandMenu();
    return 1;
}

int lua_ResetControl(lua_State* L)
{
    engine.CommandSys.LoadDefaultCommands();
    return 1;
}

int lua_IntSliderImgui(lua_State* L)
{
    const char* SliderName = lua_tostring(L, 1);
    void* sliderValueLocation = lua_touserdata(L, 2);
    int sliderMinValue = static_cast<int>(lua_tointeger(L, 3));
    int sliderMaxValue = static_cast<int>(lua_tointeger(L, 4));
    ImGui::SliderInt(SliderName, static_cast<int*>(sliderValueLocation), sliderMinValue, sliderMaxValue);
    return 0;
}

int lua_SendAudioEvent(lua_State* L)
{
    Event ev = ReceiveEvent(L);
    engine.AudioSys.HandleEvent(ev);
    return 0;
}

int lua_DeleteEntity(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tonumber(L, 1));
    Event ev = Event(true);
    ev.type = EventType::DESTROY_ENTITY;
    ev.e1 = e;
    engine.DoGameLogicScriptSys.HandleEvent(ev);
    engine.EntitySys.DestroyEntity(e);
    return 0;
}
int lua_CreateEntity(lua_State* L)
{
    std::string str = lua_tostring(L, 1);
    Entity e = engine.GameObjectFac.CreateObject(str);
    lua_pushinteger(L, static_cast<int>(e));
    return 1;
}

int lua_SetPosition(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tointeger(L, 1));
    float x = static_cast<float>(lua_tonumber(L, 2));
    float y = static_cast<float>(lua_tonumber(L, 3));
    float z = static_cast<float>(lua_tonumber(L, 4));
    TransformComponent* trans = TransformComponentPool.GetComponentByEntity(e);
    if (trans != nullptr)
    {
        trans->transform.position.x = x;
        trans->transform.position.y = y;
        trans->transform.position.z = z;
    }
    else
    {
        LOG_ERROR("Transform not found");
    }
    return 0;
}

int lua_GetRigidData(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tointeger(L, 1));
    lua_newtable(L);
    int top = lua_gettop(L);
    MovingBodyComponent* bod = MovingBodyComponentPool.GetComponentByEntity(e);
    if (bod != nullptr)
    {
        lua_pushstring(L, "elasticity");
        lua_pushnumber(L, bod->rigidBody.elasticity);
        lua_settable(L, top);

        lua_pushstring(L, "friction");
        lua_pushnumber(L, bod->rigidBody.friction);
        lua_settable(L, top);

        lua_pushstring(L, "isGravity");
        lua_pushboolean(L, bod->rigidBody.isGravity);
        lua_settable(L, top);

        lua_pushstring(L, "mass");
        lua_pushnumber(L, bod->rigidBody.mass);
        lua_settable(L, top);

        lua_pushstring(L, "collisionType");
        lua_pushinteger(L, static_cast<int>(bod->rigidBody.collisionType));
        lua_settable(L, top);

        lua_pushstring(L, "isColliding");
        lua_pushboolean(L, bod->rigidBody.isColliding);
        lua_settable(L, top);

        lua_pushstring(L, "mass");
        lua_pushnumber(L, bod->rigidBody.mass);
        lua_settable(L, top);

        lua_pushstring(L, "position");
        {
            lua_newtable(L);
            int top1 = lua_gettop(L);

            lua_pushstring(L, "x");
            lua_pushnumber(L, bod->rigidBody.position.x);
            lua_settable(L, top1);

            lua_pushstring(L, "y");
            lua_pushnumber(L, bod->rigidBody.position.y);
            lua_settable(L, top1);

            lua_pushstring(L, "x");
            lua_pushnumber(L, bod->rigidBody.position.z);
            lua_settable(L, top1);
        }
        lua_settable(L, top);

        lua_pushstring(L, "velocity");
        {
            lua_newtable(L);
            int top1 = lua_gettop(L);

            lua_pushstring(L, "x");
            lua_pushnumber(L, bod->rigidBody.velocity.x);
            lua_settable(L, top1);

            lua_pushstring(L, "y");
            lua_pushnumber(L, bod->rigidBody.velocity.y);
            lua_settable(L, top1);

            lua_pushstring(L, "x");
            lua_pushnumber(L, bod->rigidBody.velocity.z);
            lua_settable(L, top1);
        }
        lua_settable(L, top);

        lua_pushstring(L, "acceleration");
        {
            lua_newtable(L);
            int top1 = lua_gettop(L);

            lua_pushstring(L, "x");
            lua_pushnumber(L, bod->rigidBody.acceleration.x);
            lua_settable(L, top1);

            lua_pushstring(L, "y");
            lua_pushnumber(L, bod->rigidBody.acceleration.y);
            lua_settable(L, top1);

            lua_pushstring(L, "x");
            lua_pushnumber(L, bod->rigidBody.acceleration.z);
            lua_settable(L, top1);
        }
        lua_settable(L, top);

    }
    return 1;
}

int lua_SetPhysicsVelocity(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tointeger(L, 1));
    float x = static_cast<float>(lua_tonumber(L, 2));
    float y = static_cast<float>(lua_tonumber(L, 3));
    float z = static_cast<float>(lua_tonumber(L, 4));
    MovingBodyComponent* bod = MovingBodyComponentPool.GetComponentByEntity(e);
    if (bod != nullptr)
    {
        bod->rigidBody.velocity.x = x;
        bod->rigidBody.velocity.y = y;
        bod->rigidBody.velocity.z = z;
    }
    else
    {
        LOG_ERROR("Transform not found");
    }
    return 0;
}

int lua_GetSoundVolumes(lua_State* L)
{
    lua_pushlightuserdata(L,static_cast<void*>(&engine.AudioSys.BGMVolume));
    lua_pushlightuserdata(L,static_cast<void*>(&engine.AudioSys.SFXVolume));
    return 2;
}

int lua_GetScale(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tointeger(L, 1));
    TransformComponent* trans = TransformComponentPool.GetComponentByEntity(e);
    if (trans != nullptr)
    {
        lua_pushnumber(L, trans->transform.scale);
    }
    else
    {
        lua_pushnumber(L, 0);
        LOG_ERROR("Transform not found");
    }
    return 1;
}

int lua_SetScale(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tointeger(L, 1));
    float x = static_cast<float>(lua_tonumber(L, 2));
    TransformComponent* trans = TransformComponentPool.GetComponentByEntity(e);
    if (trans != nullptr)
    {
        trans->transform.scale = x;
    }
    else
    {
        LOG_ERROR("Transform not found");
    }
    return 0;
}

int lua_GetRotation(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tointeger(L, 1));
    TransformComponent* trans = TransformComponentPool.GetComponentByEntity(e);
    if (trans != nullptr)
    {
        lua_pushnumber(L, trans->transform.rotation.w);
        lua_pushnumber(L, trans->transform.rotation.x);
        lua_pushnumber(L, trans->transform.rotation.y);
        lua_pushnumber(L, trans->transform.rotation.z);
    }
    else
    {
        lua_pushnumber(L, 0);
        lua_pushnumber(L, 0);
        lua_pushnumber(L, 0);
        lua_pushnumber(L, 0);
        LOG_ERROR("Transform not found");
    }
    return 4;
}

int lua_SetRotation(lua_State* L)
{
    Entity e = static_cast<Entity>(lua_tointeger(L, 1));
    float w = static_cast<float>(lua_tonumber(L, 2));
    float x = static_cast<float>(lua_tonumber(L, 3));
    float y = static_cast<float>(lua_tonumber(L, 4));
    float z = static_cast<float>(lua_tonumber(L, 5));
    TransformComponent* trans = TransformComponentPool.GetComponentByEntity(e);
    if (trans != nullptr)
    {
        trans->transform.rotation.w = w;
        trans->transform.rotation.x = x;
        trans->transform.rotation.y = y;
        trans->transform.rotation.z = z;
    }
    else
    {
        LOG_ERROR("Transform not found");
    }
    return 0;
}

int lua_SetGamePath(lua_State* L)
{
    GAME_PATH = lua_tostring(L, 1);
    return 0;
}

int lua_SaveIntFloatTableAsJson(lua_State* L)
{
    
    nlohmann::ordered_json objectJson;
    int t = 1;
    // table is in the stack at index 't' 
    lua_pushnil(L);  // first key 
    while (lua_next(L, t) != 0)
    {
        int keynum = static_cast<int>(lua_tointeger(L, -2));
        // uses 'key' (at index -2) and 'value' (at index -1) 
        std::string key = json(std::to_string(keynum));
        objectJson[key] = lua_tonumber(L, -1);
        

        // removes 'value'; keeps 'key' for next iteration 
        lua_pop(L, 1);
    }
    std::string location = lua_tostring(L, 2);
    std::ofstream outputStream(GAME_PATH + location);
    outputStream << objectJson.dump(4);
    return 0;
}

int lua_LoadIntFloatTableFromJson(lua_State* L)
{
    std::string location = lua_tostring(L, 1);
    auto* handle = SerializationResourceManager.GetResourceHandleNoThread(GAME_PATH + location);
    ordered_json table = handle->GetPointer()->data;

    lua_newtable(L);
    int top = lua_gettop(L);
    for (auto itr = table.begin(); itr != table.end(); ++itr)
    {
        std::string key = itr.key();
        lua_pushnumber(L, std::stoi(key));
        lua_pushnumber(L, table[key]);
        lua_settable(L, top);
    }
    
    return 1;
}

int lua_GetGamePath(lua_State* L)
{
    lua_pushstring(L, GAME_PATH.c_str());
    return 1;
}

float imguifloats[3] = { 0.0f, 0.0f, 0.0f };

int lua_ImguiControledFloat(lua_State* L)
{
    int index = static_cast<int>(lua_tointeger(L, 1));
    std::string title = std::string(lua_tostring(L, 2));
    float value = static_cast<float>(lua_tonumber(L, 3));
    imguifloats[index] = value;
    ImGui::InputFloat(title.c_str(), &imguifloats[index]);
    return 0;
}

int lua_GetImguiControledFloat(lua_State* L)
{
    int index = static_cast<int>(lua_tonumber(L, 1));
    lua_pushnumber(L, imguifloats[index]);
    return 1;
}

int lua_ImguiText(lua_State* L)
{
    std::string text = std::string(lua_tostring(L, 1));
    ImGui::Text(text.c_str());
    return 0;
}

int lua_UpdateCameraOld(lua_State* L)
{
    engine.GraphicsSys.camera.Inputs(engine.GraphicsSys.pWindow);
    engine.GraphicsSys.camera.UpdateMatrix(45.0f, 0.1f, 100.0f);
    return 0;
}

int lua_Set3rdPerson(lua_State* L)
{
    bool third = lua_toboolean(L, 1);
    engine.GraphicsSys.camera.thirdPerson = third;
    engine.GraphicsSys.camera.objectTrack = third;
    return 0;
}
