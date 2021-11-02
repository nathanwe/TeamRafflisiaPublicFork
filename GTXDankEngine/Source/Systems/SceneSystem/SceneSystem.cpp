#include "pch.h"
#include "SceneSystem.h"
#include "../../Core/Engine.h"
#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
//
#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"
//
#include "../Components/RoutineComponent/RoutineComponent.h"
#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"

//#include "../Core/Engine.h"

extern Engine engine;
//extern PhysicsSystem PhysicsSys;

bool SceneSystem::Init()
{
    auto* handle = SerializationResourceManager.GetResourceHandleNoThread("Assets/Levels/levels.json");
    levels = handle->GetPointer()->data;

    LoadScene(0);

    engine.CommandSys.NextLevelCommand.SetActionToExecute([&]()
        {
            LoadNextLevel();
        });
    engine.CommandSys.PreviousLevelCommand.SetActionToExecute([&]()
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
    EntityList.clear();
}

void SceneSystem::LoadScene(int level)
{
    UnloadScene();
    shouldLoadLevel = true;
    levelToLoad = level;
    
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

void SceneSystem::Update(float dt)
{
    if (shouldLoadLevel)
    {
        currentLevel = levelToLoad;
        ordered_json levelJson = levels[std::to_string(currentLevel)];

        for (auto itr = levelJson.begin(); itr != levelJson.end(); ++itr)
        {
            ordered_json j = itr.value();
            std::string objectName = j["name"];
            Entity entity = engine.GameObjectFac.CreateObject(objectName);

            //assume we only need position and rotation at this moment
            auto* transCom = TransformComponentPool.GetComponentByEntity(entity);
            transCom->transform->position.x = j["px"];
            transCom->transform->position.y = j["py"];
            transCom->transform->position.z = j["pz"];
            transCom->transform->rotation.x = j["rx"];
            transCom->transform->rotation.y = j["ry"];
            transCom->transform->rotation.z = j["rz"];
            transCom->transform->rotation.w = j["rw"];

            // For Physics
            engine.PhysicsSys.UpdatePosition();
            //
        }
    }
    shouldLoadLevel = false;
}

bool SceneSystem::Destroy()
{
    return false;
}
