#include "pch.h"
#include "SceneSystem.h"
#include "../../Core/Engine.h"
#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"
#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"

#include "../ProfileSystem/ProfileSystem.h"


extern Engine engine;

bool SceneSystem::Init()
{
    auto* handle = SerializationResourceManager.GetResourceHandleNoThread(GAME_PATH + std::string("Assets/Levels/level.json"));
    levels = handle->GetPointer()->data;

    LoadScene(0);

    engine.CommandSys.GetCommand("NextLevel").SetActionToExecute([&]()
        {
            LoadNextLevel();
        });
    engine.CommandSys.GetCommand("PreviousLevel").SetActionToExecute([&]()
        {
            LoadPreviousLevel();
        });

	return true;
}

void SceneSystem::UnloadScene()
{
    engine.EntitySys.DeleteAllEntities();
    Event ev = Event(true);
    ev.type = EventType::DESTROY_ALL_ENTITIYS;
    engine.DoGameLogicScriptSys.HandleEvent(ev);
}

void SceneSystem::LoadScene(int level)
{
    UnloadScene();
    shouldLoadLevel = true;
    levelToLoad = level;
    
}

void SceneSystem::LoadCurrentLevel()
{
    LoadScene(currentLevel);
}

void SceneSystem::LoadNextLevel()
{
    currentLevel++;
    LoadScene(currentLevel);
}

void SceneSystem::LoadPreviousLevel()
{
    currentLevel--;
    LoadScene(currentLevel);
}

void SceneSystem::SaveCurrentLevel()
{
    ordered_json levelJson;

    for (auto entity : engine.EntitySys.allocatedEntities)
    {
        levelJson[entity] = engine.GameObjectFac.SerializeObject(entity);
    }

    levels[std::to_string(currentLevel)] = levelJson;

    std::ofstream outputStream(GAME_PATH + std::string("Assets/Levels/level.json"));
    outputStream << levels.dump(2);

    Event ev = Event(true);
    ev.type = EventType::SAVE_LUA;
    ev.intData1 = currentLevel;
    engine.DoGameLogicScriptSys.HandleEvent(ev);
}

void SceneSystem::SaveAsNewLevel()
{
    ordered_json levelJson;

    for (auto entity : engine.EntitySys.allocatedEntities)
    {
        levelJson[entity] = engine.GameObjectFac.SerializeObject(entity);
    }

    int newLevel = static_cast<int>(levels.size());
    levels[std::to_string(levels.size())] = levelJson;

    std::ofstream outputStream(GAME_PATH + std::string("Assets/Levels/level.json"));
    outputStream << levels.dump(2);

    Event ev = Event(true);
    ev.type = EventType::SAVE_LUA;
    ev.intData1 = newLevel;
    engine.DoGameLogicScriptSys.HandleEvent(ev);

    LoadScene(newLevel);
}

void SceneSystem::Update(float dt)
{
    Timer timer("Scene Update");
    
    if (shouldLoadLevel)
    {
        currentLevel = levelToLoad;
        ordered_json levelJson = levels[std::to_string(currentLevel)];

        for (auto itr = levelJson.begin(); itr != levelJson.end(); ++itr)
        {
            ordered_json j = itr.value();
            Entity entity = engine.GameObjectFac.CreateObjectFromJson(j);
        }

        Event ev = Event(true);
        ev.type = EventType::LOAD_LUA;
        ev.intData1 = currentLevel;
        engine.DoGameLogicScriptSys.HandleEvent(ev);

        // For Physics
        engine.PhysicsSys.UpdatePosition();
        engine.PhysicsSys.UpdateColliders();
    }
    shouldLoadLevel = false;
}

bool SceneSystem::Destroy()
{
    return false;
}

int SceneSystem::GetCurrentLevel()
{
    return currentLevel;
}
