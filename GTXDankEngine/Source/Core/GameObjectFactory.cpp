#include "pch.h"
#include "GameObjectFactory.h"
#include "Enums.h"

#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
#include "../Components/RoutineComponent/RoutineComponent.h"
#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"
#include "../utils/common.h"
#include "../utils/JsonFile.h"

extern Engine engine;


void DeserializeVQS(ordered_json j, Entity e)
{
    VQS* vqs = new VQS();
    from_json(j, *vqs);
    TransformComponentPool.Add(e, (vqs));
}

void DeserializeLight(ordered_json j, Entity e)
{
    Light* light = new Light();
    from_json(j, *light);
    LightComponentPool.Add(e, (light));
}

void DeserializeModel(ordered_json j, Entity e)
{
    std::string path = j["path"];
    ResourceHandle<Model>* model = ModelResourceManager.GetResourceHandle(path);
    ModelComponentPool.Add(e, (model));
}

void DeserializeMaterial(ordered_json j, Entity e)
{
    std::string AlbedoPath = j["AlbedoPath"];
    std::string MetallicPath = j["MetallicPath"];
    std::string NormalPath = j["NormalPath"];
    std::string RoughnessPath = j["RoughnessPath"];

    ResourceHandle<Texture>* diffuse = TextureResourceManger.GetResourceHandle(AlbedoPath);

    if (NormalPath.empty())
    {
        Material* material = new Material(diffuse);
        MaterialComponentPool.Add(e, (material));
        return;
    }
    else
    {
        ResourceHandle<Texture>* normal = TextureResourceManger.GetResourceHandle(NormalPath);
        ResourceHandle<Texture>* metallic = TextureResourceManger.GetResourceHandle(MetallicPath);
        ResourceHandle<Texture>* roughness = TextureResourceManger.GetResourceHandle(RoughnessPath);
        Material* material = new Material(diffuse, metallic, normal, roughness);
        MaterialComponentPool.Add(e, (material));
        return;
    }
}

void DeserializeGameLogic(ordered_json j, Entity e)
{
    auto args = j.get<std::vector<GameLogicCategories>>();
    GameLogicCategoryComponentPool.Add(e, args);
}

bool GameObjectFactory::Init()
{
    auto* handle = engine.serializationResourceManager.GetResourceHandleNoThread("Assets/Levels/gameObjects.json");
    ordered_json archetypeJson = handle->GetPointer()->data;

    for (auto itr = archetypeJson.begin(); itr != archetypeJson.end(); ++itr)
    {
        archetypes[itr.key()] = itr.value();
    }

    DeserializeFunctions[1] = DeserializeVQS;
    DeserializeFunctions[2] = DeserializeLight;
    DeserializeFunctions[3] = DeserializeModel;
    DeserializeFunctions[4] = DeserializeMaterial;
    DeserializeFunctions[5] = DeserializeGameLogic;

    return true;
}

Entity GameObjectFactory::CreateObject(std::string name)
{
    ordered_json entityJson = archetypes[name];
    Entity newEntity = engine.EntitySys.CreateEntity();
    EntityList.push_back(newEntity);
    
    for (auto itr = entityJson.begin(); itr != entityJson.end(); ++itr)
    {
        std::string componentName = itr.key();
        int componentType = ComponentNameToType[componentName];
        ordered_json componentJson = itr.value();
        DeserializeFunctions[componentType](componentJson, newEntity);
    }
    
    return newEntity;
}

//temp function, optimize when archetype system is done
void GameObjectFactory::SaveObject(std::string name, Entity entity)
{
    ordered_json objectJson;

    ModelComponent* modelCom = ModelComponentPool.GetComponentByEntity(entity);
    if (modelCom != nullptr)
    {
        std::string key = json(ComponentType::MODEL);
        objectJson[key] = *modelCom->model->GetPointer();
    }

    auto* transCom = TransformComponentPool.GetComponentByEntity(entity);
    if (transCom != nullptr)
    {
        std::string key = json(ComponentType::TRANSFORM);
        objectJson[key] = *transCom->transform;
    }

    auto* matCom = MaterialComponentPool.GetComponentByEntity(entity);
    if (matCom != nullptr)
    {
        std::string key = json(ComponentType::MATERIAL);
        objectJson[key] = *matCom->material;
    }

    auto* ligthCom = LightComponentPool.GetComponentByEntity(entity);
    if (ligthCom != nullptr)
    {
        std::string key = json(ComponentType::LIGHT);
        objectJson[key] = *ligthCom->LightSource;
    }

    auto* gameLogicCom = GameLogicCategoryComponentPool.GetComponentByEntity(entity);
    if (gameLogicCom != nullptr)
    {
        std::string key = json(ComponentType::GAME_LOGIC);
        objectJson[key] = gameLogicCom->categories;
    }

    archetypes[name] = objectJson;

    ordered_json output;
    for (const auto& [key, value] : archetypes)
    {
        output[key] = value;
    }
    std::ofstream outputStream("Assets/Levels/gameObjects.json");
    outputStream << output.dump(2);
}

/*
    temperary leave here for reference only, delete soon
    //ordered_json j;
    //std::string pokeBallCategory = json(GameLogicCategories::POKEBALL);
    //j[pokeBallCategory];
    //j[pokeBallCategory][(std::string)json(ComponentType::MODEL)] = *ModelComponentPool.GetComponentByEntity(pokemonBall)->model->GetPointer();
    //j[pokeBallCategory][(std::string)json(ComponentType::TRANSFORM)] = *pokemonBallTransform;
    //j[pokeBallCategory][(std::string)json(ComponentType::MATERIAL)] = *pokemonBallMat;
    //std::string vaseCategory = json(GameLogicCategories::VASE);
    //j[vaseCategory];
    //j[vaseCategory][(std::string)json(ComponentType::MODEL)] = *ModelComponentPool.GetComponentByEntity(vase)->model->GetPointer();
    //j[vaseCategory][(std::string)json(ComponentType::TRANSFORM)] = *vaseTransform;
    //j[vaseCategory][(std::string)json(ComponentType::MATERIAL)] = *vaseMat;
    //std::ofstream outputStream("Assets/Levels/gameObjects.json");
    //outputStream << j.dump(2);
*/
/*
todo save things in level editor:

    //GameObjectFac.SaveObject(GameLogicCategories::POKEBALL, pokemonBall);
    //GameObjectFac.SaveObject(GameLogicCategories::VASE, vase);
    //GameObjectFac.SaveObject(GameLogicCategories::POINTLIGHTSOURCE, LightSource1);
    //GameObjectFac.SaveObject(GameLogicCategories::POINTLIGHTSOURCE, LightSource2);
    //GameObjectFac.SaveObject(GameLogicCategories::LION, lion);


*/