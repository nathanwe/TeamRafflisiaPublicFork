#include "pch.h"
#include "SceneSystem.h"
#include "../../Core/Engine.h"
#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
#include "../Components/RoutineComponent/RoutineComponent.h"
#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"

extern Engine engine;

bool SceneSystem::Init()
{
    auto* handle = engine.serializationResourceManager.GetResourceHandleNoThread("Assets/Levels/levels.json");
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
    GameLogicCategoryComponentPool.DeleteAll();
    ModelComponentPool.DeleteAll();
    MaterialComponentPool.DeleteAll();
    LightComponentPool.DeleteAll();
    RoutineComponentPool.DeleteAll();
    GameLogicCategoryComponentPool.DeleteAll();
    TransformComponentPool.DeleteAll();
    engine.EntitySys.DeleteAllEntities();
    Event ev = Event(true);
    ev.type = EventType::DESTROY_ALL_ENTITIYS;
    engine.DoGameLogicScriptSys.HandleEvent(ev);
    EntityList.clear();
}

void SceneSystem::LoadScene(int level)
{
    UnloadScene();

    currentLevel = level;
    ordered_json levelJson = levels[std::to_string(level)];

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
    }
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
